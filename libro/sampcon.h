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

#ifndef ro_sampcon_INCL_
#define ro_sampcon_INCL_

/*! \file
\brief Declarations for ro::sampcon
*/


#include "libprob/median.h"
#include "libro/ro.h"

#include <vector>


namespace ro
{

/*! \brief Robust solutions for relative orientation given many measurements.

\par Example
\dontinclude testro/uFitBaseZ.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace sampcon
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

	//! Consenus solution information
	struct BestSoln
	{
		PseudoProbGen theProbGen;

		OriPair theOriPair{};
		double theProbMin{ dat::nullValue<double>() };
		double theProbMax{ dat::nullValue<double>() };
		std::vector<double> theBestGapSqs;

		//! Construct a null instance
		BestSoln
			() = default;

		explicit
		BestSoln
			( size_t const & numPnts
			, double const & dirSigma = {  4. * 10./450. }
			)
			: theProbGen(dirSigma)
			, theOriPair{}
			, theProbMin{ std::numeric_limits<double>::max() }
			, theProbMax{ std::numeric_limits<double>::lowest() }
			, theBestGapSqs(numPnts, dat::nullValue<double>())
		{ }

		//! True if this instance is not null
		bool
		isValid
			() const
		{
			return dat::isValid(theOriPair);
		}

		//! True if solution is modified
		bool
		updatePseudoProb
			( OriPair const & roPair
			, double const & pProbFit
			, std::vector<double> const & gapSqs
			)
		{
			bool modSoln{ false };
			if (pProbFit < theProbMin)
			{
				theProbMin = pProbFit;
			}
			else
			if (theProbMax < pProbFit)
			{
				modSoln = true;
				theOriPair = roPair;
				theBestGapSqs = gapSqs;
				theProbMax = pProbFit;
			}
			return modSoln;
		}

		double
		voteFor
			( size_t const & pntNdx
			) const
		{
			double vote{ dat::nullValue<double>() };
			if (pntNdx < theBestGapSqs.size())
			{
				double const & gapSq = theBestGapSqs[pntNdx];

				double const pProbMea{ theProbGen(gapSq) };
				double const pProbPnt{ pProbMea * theProbMax };
				vote = pProbPnt;
			}
			return vote;
		}

		//! Estimated gap (rms adjusted for 5 dof)
		double
		rmseGap
			() const
		{
			double rmse{ dat::nullValue<double>() };
			if (isValid() && (5u < theBestGapSqs.size()))
			{
				std::vector<double> const & gaps = theBestGapSqs;
				double const dom{ double(theBestGapSqs.size() - 5u) };
				double const sumSqs
					{ std::accumulate(gaps.begin(), gaps.end(), 0.) };
				rmse = std::sqrt(sumSqs / dom);
			}
			return rmse;
		}

		//! Estimated gap (rms adjusted for 5 dof)
		double
		medianGap
			() const
		{
			double median{ dat::nullValue<double>() };
			if (isValid() && (5u < theBestGapSqs.size()))
			{
				// make copy for sort
				std::vector<double> gaps{ theBestGapSqs };
				using Iter = typename std::vector<double>::iterator;
				Iter const it0 = gaps.begin();
				Iter const itBeg = it0 + 5u;
				Iter const itEnd = gaps.end();
				std::nth_element(it0, itBeg, itEnd);
				median = prob::median::valueFromConst(itBeg, itEnd);
			}
			return median;
		}
	};

	//! Best solution after exhaustive evaluation of all combinations
	BestSoln
	byCombo
		( std::vector<PairUV> const & uvPairs
		, OriPair const & roPairNom
		);

	//! Best solution from random sampling
	BestSoln
	bySample
		( std::vector<PairUV> const & uvPairs
		, OriPair const & roPairNom
		, size_t const & numDraws = { 640u }
		, size_t const & maxTrys = { 10u }
		);

} // sampcon

} // ro

// Inline definitions
// #include "libro/sampcon.inl"

#endif // ro_sampcon_INCL_

