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
\brief Inline definitions for prob::CdfForward
*/


#include "libdat/Jump.h"
#include "libmath/PolyFunc.h"
#include "libprob/histo.h"

#include <cassert>
#include <cstddef>
#include <iterator>


namespace prob
{
//======================================================================

template <typename FwdIter>
// static
bool
CdfForward :: isValidCdf
	( FwdIter const & cdfDataBeg
	, FwdIter const & cdfDataEnd
	)
{
	size_t const cdfSize(std::distance(cdfDataBeg, cdfDataEnd));
	bool okay(1u < cdfSize); // need at least the defined end values

	if (okay)
	{
		// check if end values satisfy required conditions
		double const v0(*(cdfDataBeg));
		double const v1(*(cdfDataBeg + (cdfSize - 1u)));
		okay &= (0. == v0); // exact for valid cdf
		okay &= (1. == v1); // exact for valid cdf

		if (okay)
		{
			// check if all elements
			FwdIter itPrev(cdfDataBeg);
			FwdIter itCurr(itPrev);
			++itCurr;
			for ( ; cdfDataEnd != itCurr ; ++itCurr)
			{
				double const currValue(*itCurr);
				double const prevValue(*itPrev);

				// check each element is valid
				okay &= dat::isValid(currValue);

				// check each element for legitimate probability value
				okay &= ((0. <= currValue) && (currValue <= 1.));

				// check adjacent elements for monotonic property
				okay &= (prevValue <= currValue);

				// no point to check the rest
				if (! okay)
				{
					break;
				}

				// update trailing iterator
				itPrev = itCurr;
			}
		}
	}

	return okay;
}

template <typename FwdIter>
inline
// static
std::vector<double>
CdfForward :: preCdfValues
	( FwdIter const & freqBeg
	, FwdIter const & freqEnd
	)
{
	std::vector<double> fracValues;

	/* NOTE: the input data are defined on a half open range. This
	 *       can be mapped to [0,1). To be valid, the CDF function
	 *       must satisfy the condition that cdf(0)==0. and cdf(1)==1.
	 *
	 *       Consider a histogram with one "bin". In this case, the CMF
	 *       must have a discontinuity (e.g. at 0 or 1). To avoid this,
	 *       assume a continuous interpolation such that the CDF rises
	 *       from 0 to 1 over the range [0,1). However, this is the same
	 *       as a valid CDF even if no input histogram is specified.
	 *
	 *       Thus, it is clear, that the histogram contains one extra
	 *       degree of freedom which must be removed. This freedom is
	 *       associated with normalization of the histogram counts (i.e.
	 *       such that they sum to unity.
	 *
	 *       Let the sum of histogram values be denoted as "H". Then
	 *       the probability of the i-th histogram bin is h[i]/H, where
	 *       H = sum-over-i(h[i]).
	 *
	 *       The CDF may be set such that cdf[0]==0 with subsequent values
	 *       set such that cdf[j]=sum{over i, up to j}(h[i]/H). This is
	 *       equivalent to e.g.:
	 *         :: cdf[0] == 0.
	 *         :: cdf[1] = h[0] / H
	 *         :: cdf[2] = (h[1] + h[0]) / H
	 *         :: cdf[j] = { sum-over-i from 0 to (j-1) } h[i] / H
	 *         :: ...
	 *         :: cdf[N+1] = h[N] / H == 1.
	 *
	 *       Note that this naturally produces the second required end
	 *       condition, cdf[N+1]==1, as well as directly enforcing the
	 *       first one, cdf[0]==0.
	 *
	 *       Note that the cdf must therefore have one more element than
	 *       the histogram (or PDF). This difference is associated with
	 *       size differences
	 *         :: 2 more: to satisfy two end conditions exactly
	 *         :: 1 less: to recognize constraint that PDF sums to 1.
	 *       Net difference is therefore 1 more element.
	 */
	ptrdiff_t const dataSize(std::distance(freqBeg, freqEnd));

	if (0u < dataSize)
	{
		// configure CDF data size
		ptrdiff_t const cdfSize(dataSize + 1);
		fracValues.resize(cdfSize);

		// integrate relative frequency values
		std::partial_sum
			( freqBeg, freqEnd
			, (fracValues.begin() + 1)
			);

		// normalize integral to probability interpretation
		double const & sum(fracValues.back());
		if (std::numeric_limits<double>::min() < sum)
		{
			// normalization value
			double const scale(1. / sum);

			// set CDF probability values
			std::transform
				( fracValues.begin(), fracValues.end()
				, fracValues.begin()
				, [& scale]
					(double const & hi)
					{ return (scale*hi); }
				);
		}
		else // zero sum
		{
			std::fill(fracValues.begin(), fracValues.end(), 0.);
		}

		// ensure ends have (exact) required values
		size_t const ndx0(0u);
		size_t const ndx1(fracValues.size() - 1u);
		fracValues[ndx0] = 0.;
		fracValues[ndx1] = 1.;
	}
	// else // leave as invalid

	// verify that CDF is properly formed
	constexpr bool verify(true);
	if (verify)
	{
		assert(isValidCdf(fracValues.begin(), fracValues.end()));
	}

	return fracValues;
}

template <typename FwdIter>
inline
// explicit
CdfForward :: CdfForward
	( FwdIter const & cdfBeg
	, FwdIter const & cdfEnd
	, dat::Range<double> const & dataRange
	)
	: theFracValues(cdfBeg, cdfEnd)
	, theDataPart(dataRange, (theFracValues.size() - 1u))
{
}

template <typename FwdIter>
inline
// static
CdfForward
CdfForward :: fromSamps
	( FwdIter const & sampBeg
	, FwdIter const & sampEnd
	, math::Partition const & dataPart
	)
{
	std::vector<size_t> const hist
		(prob::histo::countsFromSamps(sampBeg, sampEnd, dataPart));
	return CdfForward::fromFreqs(hist.begin(), hist.end(), dataPart.range());
}

template <typename FwdIter>
inline
// static
CdfForward
CdfForward :: fromFreqs
	( FwdIter const & freqBeg
	, FwdIter const & freqEnd
	, dat::Range<double> const & dataRange
	)
{
	CdfForward cdfFwd;
	std::vector<double> const preCdf(preCdfValues(freqBeg, freqEnd));

	// check for something significant to do
	size_t const cdfSize(std::distance(preCdf.begin(), preCdf.end()));
	if (0u < cdfSize)
	{
		// find input data discontinuities
		std::vector<dat::Jump> const jumps
			(dat::Jump::fromData<double>(preCdf.begin(), preCdf.end()));

		// assemble vertices that provide symmetric interpolation
		std::vector<dat::Spot> verts;
		verts.reserve(jumps.size() + 2u);
		double const xMin(0.);
		double const yMin(0.);
		verts.push_back(dat::Spot{ xMin, yMin });
		for (dat::Jump const & jump : jumps)
		{
			double const xarg(double(jump.theNdx) + .5);
			double const value(jump.mean());
			verts.push_back(dat::Spot{ xarg, value });
		}
		double const xMax(cdfSize - 1u);
		double const yMax(1.);
		verts.push_back(dat::Spot{ xMax, yMax });

		// generate regularly spaced lookup table into function of vertices
		dat::Range<double> const ndxRange(0., cdfSize);
		math::Partition const ndxPart(ndxRange, cdfSize);
		math::PolyFunc const cdfPoly(verts.begin(), verts.end());
		std::vector<double> cdf(cdfSize);
		for (size_t nn(0u) ; nn < cdfSize ; ++nn)
		{
			double const dataValue(ndxPart.interpValueFor(double(nn)));
			double const cdfValue(cdfPoly(dataValue));
			cdf[nn] = cdfValue;
		}
		cdfFwd = CdfForward(cdf.begin(), cdf.end(), dataRange);
	}

	return cdfFwd;
}

inline
dat::Range<double>
CdfForward :: dataRange
	() const
{
	return theDataPart.range();
}

inline
std::vector<double> const &
CdfForward :: probabilities
	() const
{
	return theFracValues;
}

inline
double
CdfForward :: operator()
	( const double & dataValue
	) const
{
	double const fndx(theDataPart.interpIndexFor(dataValue));
	return math::interp::linear<double>(fndx, theFracValues);
}

//======================================================================
}

