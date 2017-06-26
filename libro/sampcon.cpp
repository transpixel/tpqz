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

#include "libdat/info.h"
#include "libdat/random.h"
#include "libdat/validity.h"
#include "libgeo/intersect.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
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

	//! True if value is in array
	bool
	contains
		( size_t const & key
		, std::array<size_t, 5u> const & values
		)
	{
		bool found{ false };
		for (size_t const & value : values)
		{
			if (key == value)
			{
				found = true;
				break;
			}
		}
		return found;
	}

	//! RMS gap value computed using ALL measurements - true if ptSoln modified
	bool
	updateSolution
		( ro::OriPair const & roPair
		, std::vector<ro::PairUV> const & uvPairs
		, StateTracker<5u> const & sampleState
		, BestSoln * const & ptSoln
		)
	{
		bool modSoln{ false };
		assert(ptSoln);
		if (dat::isValid(roPair))
		{
			std::vector<std::pair<size_t, double> > pProbs
				(uvPairs.size(), { dat::nullValue<size_t>(), 0. });

			// use measurements EXCLUDED from the fitting process for:
			// 	: gapSq values (save for next computation)
			// 	: pseudo-prob of fit
			size_t const uvSize{ uvPairs.size() };
			std::vector<double> gapSqs(uvPairs.size());
			ro::QuadForm const quad{ roPair };
			double pProbSum{ 0. };
			size_t pProbCount{ 0u };
			for (size_t nn{0u} ; nn < uvSize ; ++nn)
			{
				ro::PairUV const & uvPair = uvPairs[nn];
				double const gapSq{ math::sq(quad.tripleProductGap(uvPair)) };
				gapSqs[nn] = gapSq;
				if (! sampleState.isActiveIndex(nn))
				{
					pProbSum += ptSoln->theProbGen(gapSq);
					++pProbCount;
				}
			}
			assert(0u < pProbCount);
			double const pProbFit{ pProbSum / double(pProbCount) };

			// update best solution tracking
			modSoln = ptSoln->updatePseudoProb(roPair, pProbFit, gapSqs);
		}
		return modSoln;
	}

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

	//! True if all uv dirs intersect in forward directions.
	size_t
	numForward
		( ro::OriPair const & oriPair
		, PtrQuint const & uvFitPtrs
		)
	{
		size_t fwdCount{ 0u };
		if (dat::isValid(oriPair))
		{
			for (PtrPairUV const & uvFitPtr : uvFitPtrs)
			{
				PairUV const & uvPair = *uvFitPtr;
				PntPair const pntPair{ model::pointPair(uvPair, oriPair) };
				if (dat::isValid(pntPair))
				{
					++fwdCount;
				}
			}
		}

		return fwdCount;
	}

	//! Return one of multiple solutions with all points in front (or null).
	ro::OriPair
	aForwardRO
		( ro::OriPair const & anyPair
		, PtrQuint const & uvFitPtrs
		)
	{
		OriPair qualPair;

		// try given initial solution
		{
			size_t const numFwd{ numForward(anyPair, uvFitPtrs) };
			bool const isForward{ (uvFitPtrs.size() == numFwd) };
			if (isForward)
			{
				// return first qualifying solution
				qualPair = anyPair;
			}
		}

		// if initial solution doesn't qualify, try mirror permutations
		if (! dat::isValid(qualPair))
		{
			// use the PQ spinor convention to explore mirror solutions
			ro::SpinPQ const anyPQ(ro::SpinPQ::from(anyPair));
			for (size_t nn{0u} ; nn < anyPQ.theNumPerms ; ++nn)
			{
				// check if each solution produces a forward intersection
				ro::OriPair const tmpPair{ anyPQ[nn].pair() };
				size_t const numFwd{ numForward(tmpPair, uvFitPtrs) };
				bool const isForward{ (uvFitPtrs.size() == numFwd) };
				if (isForward)
				{
					// return first qualifying solution
					qualPair = tmpPair;
					assert(dat::isValid(qualPair));
					break;
				}
			}
		}

		return qualPair;
	}
}

BestSoln
byCombo
	( std::vector<PairUV> const & uvPairs
	, OriPair const & roPairNom
	)
{
	BestSoln soln(uvPairs.size());

	assert(areValidPairs(uvPairs));
	assert(5u < uvPairs.size()); // need at least one mea redundancy for rms

	StateTracker<5u> sampleState(uvPairs.size());
	ro::PairBaseZ const roNom(roPairNom);

	// try fitting all combinations
	Combo5 const combo(uvPairs.size());
	size_t const numQuints{ combo.theQuints.size() };
	size_t nqBest{ dat::nullValue<size_t>() }; 
	for (size_t nq{0u} ; nq < numQuints ; ++nq)
	{
		// gain access to measurements for fitting
		Combo5::NdxQuint const & fitIndices = combo.theQuints[nq];
		PtrQuint const uvFitPtrs(ptrQuintInto(&uvPairs, fitIndices));
		assert(areValidPtrs(uvFitPtrs));

		// compute RO using fit partition
		FitBaseZ const fitter(uvFitPtrs);//.begin(), uvFitPtrs.end());
		// ro::PairBaseZ const roFit{ fitter.improvedNear(roNom) };
		ro::PairBaseZ const roFit{ fitter.solutionNear(roNom) };

		// evaluate RO quality using evaluation partition
		if (dat::isValid(roFit))
		{
			// check if there is a forward-qualified version of soln
			OriPair const roFwd{ aForwardRO(roFit.pair(), uvFitPtrs) };
			if (dat::isValid(roFwd))
			{
				sampleState.enactivate(fitIndices);
				if (updateSolution(roFwd, uvPairs, sampleState, &soln))
				{
					nqBest = nq;
				}
				sampleState.deactivate(fitIndices);
			}
		}
	}

	// search for "forward" solution
	if (soln.isValid() && dat::isValid(nqBest))
	{
		Combo5::NdxQuint const & bestIndices = combo.theQuints[nqBest];
		PtrQuint const uvFitPtrs(ptrQuintInto(&uvPairs, bestIndices));
		assert(areValidPtrs(uvFitPtrs));
		// evaluate mirror solutions - to find physically legit one
		soln.theOriPair = aForwardRO(soln.theOriPair, uvFitPtrs);
	}

	return soln;
}

BestSoln
bySample
	( std::vector<PairUV> const & uvPairs
	, OriPair const & roPairNom
	, size_t const & numDraws
	, size_t const & maxTrys
	)
{
	BestSoln soln(uvPairs.size());

	assert(areValidPairs(uvPairs));
	assert(5u < uvPairs.size()); // need at least one mea redundancy for rms

	StateTracker<5u> sampleState(uvPairs.size());
	ro::PairBaseZ const roNom(roPairNom);

	Combo5::NdxQuint fitIndices{{}};
	rand::Sampler sampler(uvPairs.size(), maxTrys);

	Combo5::NdxQuint bestIndices{{ dat::nullValue<size_t>() }};
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
			// ro::PairBaseZ const roFit{ fitter.improvedNear(roNom) };
			ro::PairBaseZ const roFit{ fitter.solutionNear(roNom) };

			// evaluate RO quality using evaluation partition
			if (dat::isValid(roFit))
			{
				OriPair const roFwd{ aForwardRO(roFit.pair(), uvFitPtrs) };
				sampleState.enactivate(fitIndices);
				if (updateSolution(roFwd, uvPairs, sampleState, &soln))
				{
					bestIndices = fitIndices;
				}
				sampleState.deactivate(fitIndices);
			}
		}
		// else // ignore improper (e.g. duplicate) samples
		// io::out() << "WARNING: invalid partition" << std::endl;
	}

	// search for "forward" solution
	if (soln.isValid() && dat::isValid(bestIndices[0]))
	{
		// access measurements to use for fitting
		PtrQuint const uvFitPtrs(ptrQuintInto(&uvPairs, bestIndices));
		assert(areValidPtrs(uvFitPtrs));

		// evaluate mirror solutions - to find physically legit one
		soln.theOriPair = aForwardRO(soln.theOriPair, uvFitPtrs);

		/*
		io::out() << "..Fwd:" << std::endl;
		for (PtrPairUV const & ptrPairUV : uvFitPtrs)
		{
			PairUV const & uvPair = *ptrPairUV;
			ga::Vector const & uDir = uvPair.first;
			ga::Vector const & vDir = uvPair.second;
			OriPair const oriPair{ soln.theOriPair };
			bool const isFwd{ dat::isValid(model::pointPair(uvPair, oriPair)) };
			io::out()
				<< "arFwd:"
				<< " " << dat::infoString(uDir, "uDir")
				<< " " << dat::infoString(vDir, "vDir")
				<< " " << dat::infoString(isFwd, "isFwd")
				<< std::endl;
		}
		*/
	}

	return soln;
}


} // sampcon

} // ro

