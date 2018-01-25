//
//
// MIT License
//
// Copyright (c) 2017 Stellacore Corporation.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject
// to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
// KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
// AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
// IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//


/*! \file
\brief Definitions for ro::sampcon
*/


#include "libro/sampcon.h"

#include "libdat/BestOf.h"
#include "libdat/info.h"
#include "libdat/random.h"
#include "libdat/validity.h"
#include "libgeo/intersect.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libro/Accord.h"
#include "libro/cast.h"
#include "libro/FitBaseZ.h"
#include "libro/model.h"
#include "libro/QuadForm.h"
#include "libro/ro.h"
#include "libro/SpinPQ.h"

#include <algorithm>
#include <array>
#include <array>
#include <mutex>
#include <random>
#include <set>
#include <sstream>


namespace ro
{
namespace sampcon
{

namespace rand
{
	//
	// TODO - factor random operations into thread-friendly pool
	//
	static std::mutex sRandMutex;
	static std::random_device sRandDev;
	static std::mt19937_64 sRandGen(sRandDev());
	static bool sRandSeeded{ false };

	//! Index values for use in {fitting | evaluation}
	struct Sampler
	{
		static constexpr size_t DimSamp{ 5u }; // not really specific to RO

		using IndexArray = std::array<size_t, DimSamp>;

		size_t theMaxNdx{ dat::nullValue<size_t>() };
		std::set<IndexArray> theSampHits;
		size_t theMaxRetries{ dat::nullValue<size_t>() };

		explicit
		Sampler
			( size_t const & fullSize
			, size_t const & maxTrys = 3u
			)
			: theMaxNdx{ fullSize }
			, theSampHits{}
			, theMaxRetries{ maxTrys }
		{
			// generate a sample of indices
			if (DimSamp < theMaxNdx) // else need bound checking below
			{
				{ std::lock_guard<std::mutex> guard(sRandMutex);
					if (! sRandSeeded)
					{
						sRandGen.seed(357u);
						sRandSeeded = true;
					}
				}
			}
			else
			{
				theMaxNdx = dat::nullValue<size_t>();
			}
		}

		//! Fill index quintuplet - true if successful
		bool
		setIndices
			( IndexArray * const & ptNdxs
			)
		{
			bool okay(false);
			IndexArray & useNdxs = *ptNdxs;

			//
			// TODO - need to factor random numbers into a pool or something
			//

			using dat::random::index_sample;

			// generate a sample of indices
			{ std::lock_guard<std::mutex> guard(sRandMutex);
				// note: returns values in sorted order
				useNdxs = index_sample<DimSamp>(theMaxNdx, sRandGen);
			}

			constexpr bool avoidDuplicates{ true };
			if (avoidDuplicates)
			{
				// track previous samples - to avoid duplicates
				// - probably good for "smaller" measurements sets 10's-100
				// - probably wasted overhead for larger sets
				size_t numHits{ theSampHits.count(useNdxs) };
				size_t nTrys{ 0u };
				while ((0u < numHits) && (nTrys < theMaxRetries))
				{
					{ std::lock_guard<std::mutex> guard(sRandMutex);
						useNdxs = index_sample<DimSamp>(theMaxNdx, sRandGen);
					}
					++nTrys;
					numHits = theSampHits.count(useNdxs);
				}

				// check for uniqueness
				if (nTrys < theMaxRetries)
				{
					// remember unique sample for next time
					theSampHits.insert(useNdxs);
					okay = true;
				}
				else
				{
					// problems - wipe out indices
					size_t const badNdx{ dat::nullValue<size_t>() };
					std::fill(useNdxs.begin(), useNdxs.end(), badNdx);
				}
			}

			return okay;
		}

		//! True if creation was valid
		bool
		isValid
			() const
		{
			return dat::isValid(theMaxNdx);
		}

		std::string
		infoString
			( std::string const & title = {}
			) const
		{
			std::ostringstream oss;

			if (! title.empty())
			{
				oss << title << std::endl;
			}

			if (isValid())
			{
				oss << dat::infoString(theMaxNdx, "theMaxNdx");
				oss << std::endl;
				oss << dat::infoString(theMaxRetries, "theMaxRetries");
			}
			else
			{
				oss << " <null>";
			}

			return oss.str();
		}

	}; // Sampler

}


namespace
{
	//! True if all members are valid
	bool
	areValidPairs
		( std::vector<ro::PairUV> const & uvPairs
		)
	{
		bool okay{ true }; // unless contradicted
		for (ro::PairUV const & uvPair : uvPairs)
		{
			if (! dat::isValid(uvPair))
			{
				okay = false;
				break;
			}
		}
		return okay;
	}

	//! True if all members are valid
	bool
	areValidPtrs
		( std::array<ro::PtrPairUV, 5u> const & uvPairPtrs
		)
	{
		bool okay{ true }; // unless contradicted
		for (ro::PtrPairUV const & uvPairPtr : uvPairPtrs)
		{
			if (! (uvPairPtr && dat::isValid(*uvPairPtr)))
			{
				okay = false;
				break;
			}
		}
		return okay;
	}

	//! Track which samples are active or inactive
	template <size_t SampSize>
	struct StateTracker
	{
		//! Status of each index being tracked
		std::vector<bool> theActives{};

		using Selection = std::array<size_t, SampSize>;

		StateTracker
			() = default;

		//! Construct to track specified number of indices
		StateTracker
			( size_t const & size
			)
			: theActives(size, false)
		{ }

		//! Set state of each index in config to state
		inline
		void
		updateState
			( Selection const & config
			, bool const & state
			)
		{
			for (size_t const & ndx : config)
			{
				theActives[ndx] = state;
			}
		}

		//! Activate each index contained in config
		inline
		void
		enactivate
			( Selection const & config
			)
		{
			updateState(config, true);
		}

		//! Deactivate each index contained in config
		inline
		void
		deactivate
			( Selection const & config
			)
		{
			updateState(config, false);
		}

		//! True if ndx-th index is currently active
		inline
		bool
		isActiveIndex
			( size_t const & ndx
			) const
		{
			return theActives[ndx];
		}

		//! Descriptive information about this instance
		std::string
		infoString
			( std::string const & title = {}
			) const
		{
			std::ostringstream oss;
			if (! title.empty())
			{
				oss << title << std::endl;
			}
			oss << dat::infoString(theActives.begin(), theActives.end());
			return oss.str();
		}
	};

	//! Combinatorial index generator
	struct Combo5
	{
		using NdxQuint = FiveOf<size_t>;

		size_t theNumElem;
		std::vector<NdxQuint> theQuints;

		static
		size_t
		sizeCombos
			( size_t const & numElem
			)
		{
			size_t const & max = numElem;
			return{ (max   ) * (max-1u) * (max-2u) * (max-3u) * (max-4u) };
		}

		//! Populate self with all combinations
		explicit
		Combo5
			( size_t const & numElem
			)
			: theNumElem{ numElem }
			, theQuints{}
		{
			assert(4u < theNumElem);
			theQuints.reserve(sizeCombos(theNumElem));

			for (size_t na{   0u} ; na < theNumElem ; ++na)
			for (size_t nb{na+1u} ; nb < theNumElem ; ++nb)
			for (size_t nc{nb+1u} ; nc < theNumElem ; ++nc)
			for (size_t nd{nc+1u} ; nd < theNumElem ; ++nd)
			for (size_t ne{nd+1u} ; ne < theNumElem ; ++ne)
			{
				theQuints.emplace_back( NdxQuint{{ na, nb, nc, nd, ne }} );
			}
		}

		//! return values associated with a combo
		template <typename DataType, typename FwdIter>
		std::array<DataType, 5u>
		valuesFor
			( NdxQuint const & ndxs
			, FwdIter const & valueIterBeg
			) const
		{
			return std::array<DataType, 5u>
				{{ *(valueIterBeg + ndxs[0])
				 , *(valueIterBeg + ndxs[1])
				 , *(valueIterBeg + ndxs[2])
				 , *(valueIterBeg + ndxs[3])
				 , *(valueIterBeg + ndxs[4])
				}};
		}
	};

}

namespace
{
	//! Pseudo-probability generating Functor
	struct PseudoProbGen
	{
		double theInvSigmaSq{ dat::nullValue<double>() };

		PseudoProbGen
			() = default;

		explicit
		PseudoProbGen
			( double const & estSigmaGap
			)
			: theInvSigmaSq{ 1. / math::sq(estSigmaGap) }
		{ }

		inline
		double
		operator()
			( double const & gapSq
			) const
		{
			assert(dat::isValid(theInvSigmaSq));
			return std::exp(-theInvSigmaSq * gapSq);
		}

	};
}


namespace
{
	using NdxQuint = FiveOf<size_t>;
	using PtrQuint = FiveOf<PtrPairUV>;

	//! A quintuple of pointers into the uvPairs struct
	PtrQuint
	ptrQuintInto
		( std::vector<PairUV> const * const & ptPairUVs
		, NdxQuint const & ndxs
		)
	{
		assert(dat::isValid(ndxs[0]) && (ndxs[0] < ptPairUVs->size()));
		assert(dat::isValid(ndxs[1]) && (ndxs[1] < ptPairUVs->size()));
		assert(dat::isValid(ndxs[2]) && (ndxs[2] < ptPairUVs->size()));
		assert(dat::isValid(ndxs[3]) && (ndxs[3] < ptPairUVs->size()));
		assert(dat::isValid(ndxs[4]) && (ndxs[4] < ptPairUVs->size()));

		return PtrQuint
			{{ & ((*ptPairUVs)[ ndxs[0] ])
			 , & ((*ptPairUVs)[ ndxs[1] ])
			 , & ((*ptPairUVs)[ ndxs[2] ])
			 , & ((*ptPairUVs)[ ndxs[3] ])
			 , & ((*ptPairUVs)[ ndxs[4] ])
			}};
	}

}


std::vector<QuintSoln>
allByCombo
	( std::vector<PairUV> const & uvPairs
	, OriPair const & roPairNom
	, FitConfig const & fitConfig
	)
{
	std::vector<QuintSoln> quintSolns;

	assert(areValidPairs(uvPairs));
	assert(5u < uvPairs.size()); // need at least one mea redundancy for rms

	ro::PairBaseZ const roNom(roPairNom);

	// try fitting all combinations
	Combo5 const combo(uvPairs.size());
	size_t const numQuints{ combo.theQuints.size() };
	for (size_t nq{0u} ; nq < numQuints ; ++nq)
	{
		// gain access to measurements for fitting
		Combo5::NdxQuint const & fitIndices = combo.theQuints[nq];
		PtrQuint const uvFitPtrs(ptrQuintInto(&uvPairs, fitIndices));
		assert(areValidPtrs(uvFitPtrs));

		// compute RO using fit partition
		FitBaseZ const fitter(uvFitPtrs);//.begin(), uvFitPtrs.end());
		Solution const roSoln{ fitter.roSolution(roNom, fitConfig) };
		if (dat::isValid(roSoln))
		{
			QuintSoln const quintSoln{ fitIndices, roSoln };
			quintSolns.emplace_back(quintSoln);
		}
	}

	return quintSolns;
}

std::vector<QuintSoln>
allBySample
	( std::vector<PairUV> const & uvPairs
	, OriPair const & roPairNom
	, size_t const & numDraws
	, FitConfig const & fitConfig
	, size_t const & maxTrys
	)
{
	std::vector<QuintSoln> quintSolns;

	assert(areValidPairs(uvPairs));
	assert(5u < uvPairs.size()); // need at least one mea redundancy for rms

	ro::PairBaseZ const roNom(roPairNom);

	Combo5::NdxQuint fitIndices{{}};
	rand::Sampler sampler(uvPairs.size(), maxTrys);

	for (size_t nDraw{0u} ; nDraw < numDraws ; ++nDraw)
	{
		// partition samples into fit and evaluation groups
		bool const goodSample{ sampler.setIndices(&fitIndices) };
		if (goodSample)
		{
			// access measurements to use for fitting
			PtrQuint const uvFitPtrs(ptrQuintInto(&uvPairs, fitIndices));
			assert(areValidPtrs(uvFitPtrs));

			// compute RO using fit partition
			FitBaseZ const fitter(uvFitPtrs);
			Solution const roSoln{ fitter.roSolution(roNom, fitConfig) };
			if (dat::isValid(roSoln))
			{
				QuintSoln const quintSoln{ fitIndices, roSoln };
				quintSolns.emplace_back(quintSoln);
			}
		}
		// else // ignore improper (e.g. duplicate) samples
		// io::out() << "WARNING: invalid partition" << std::endl;
	}

	return quintSolns;
}

namespace
{
	//! Compute probability associated with putative solution
	inline
	double
	probFor
		( QuintSoln const & quintSoln
		, std::vector<PairUV> const * const & ptPairUVs
		, PseudoProbGen const & probGen
		)
	{
		Accord const fit{ quintSoln.theSoln, ptPairUVs };
		double const gapSq{ fit.sumSqGapExcluding(quintSoln.theFitNdxs) };
		return probGen(gapSq);
	}
}

std::vector<QuintSoln>
bestOf
	( std::vector<QuintSoln> const & quintSolns
	, std::vector<PairUV> const & uvPairs
	, size_t const & numBest
	, double const & sigmaDirs
	)
{
	std::vector<QuintSoln> best;

	PseudoProbGen const probGen(sigmaDirs);
	using ProbNdxPair = std::pair<double, size_t>;
	dat::BestOf<ProbNdxPair> besty(numBest);

	size_t const numSolns{ quintSolns.size() };
	for (size_t ndx{0u} ; ndx < numSolns ; ++ndx)
	{
		QuintSoln const & quintSoln = quintSolns[ndx];
		double const prob{ probFor(quintSoln, &uvPairs, probGen) };
		ProbNdxPair const pnPair{ prob, ndx };
		besty.addSample(pnPair);
	}

	std::vector<ProbNdxPair> const pnBests{ besty.bestItems() };
	best.reserve(pnBests.size());
	for (ProbNdxPair const & pnPair : pnBests)
	{
		size_t const ndx{ pnPair.second };
		best.emplace_back(quintSolns[ndx]);
	}

	return best;
}

namespace
{
	QuintSoln
	bestFrom
		( std::vector<QuintSoln> const & allQuintSolns
		, std::vector<PairUV> const & uvPairs
		, double const & sigmaDirs
		)
	{
		QuintSoln bestQuintSoln{};
		constexpr size_t const numBest{ 1u };
		std::vector<QuintSoln> const bestQuintSolns
			{ bestOf(allQuintSolns, uvPairs, numBest, sigmaDirs) };
		if (! bestQuintSolns.empty())
		{
			bestQuintSoln = bestQuintSolns[0];
		}
		return bestQuintSoln;
	}
}

QuintSoln
byCombo
	( std::vector<PairUV> const & uvPairs
	, OriPair const & roPairNom
	, FitConfig const & fitConfig
	, double const & sigmaDirs
	)
{
	std::vector<QuintSoln> const allQuintSolns
		{ allByCombo(uvPairs, roPairNom, fitConfig) };
	return bestFrom(allQuintSolns, uvPairs, sigmaDirs);
}

QuintSoln
bySample
	( std::vector<PairUV> const & uvPairs
	, OriPair const & roPairNom
	, size_t const & numDraws
	, FitConfig const & fitConfig
	, double const & sigmaDirs
	, size_t const & maxTrys
	)
{
	std::vector<QuintSoln> const allQuintSolns
		{ allBySample(uvPairs, roPairNom, numDraws, fitConfig, maxTrys) };
	return bestFrom(allQuintSolns, uvPairs, sigmaDirs);
}


} // sampcon

} // ro

