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
\brief Inline definitions for prob::CdfInverse
*/


#include "libdat/algorithm.h"
#include "libmath/interp.h"


namespace prob
{
//======================================================================

template <typename FwdIter>
// static
std::vector<double>
CdfInverse :: ndxLocsForUniformDelta
	( FwdIter const & cdfFwdBeg
	, FwdIter const & cdfFwdEnd
	, size_t const & lutSize
	)
{
	std::vector<double> ndxLocs;

	// check inputs
	assert(1u < lutSize);
	size_t const cdfSize(std::distance(cdfFwdBeg, cdfFwdEnd));
	assert(CdfForward::isValidCdf(cdfFwdBeg, cdfFwdEnd));

	// establish size values
	size_t const lastNdx(cdfSize - 1u);
	double const delta(1. / (double)(lutSize - 1u));

	// compute return values at uniform delta spacing
	ndxLocs.reserve(lutSize);
	for (size_t nn(0u) ; nn < lutSize ; ++nn)
	{
		// compute sample value
		double const value((double)nn * delta);

		// get bounding indices (specially adjusted for end effects)
		std::pair<size_t, size_t> const ndxLoHi
			(dat::indexBounds(value, cdfFwdBeg, cdfFwdEnd));
		size_t const & ndxLo = ndxLoHi.first;
		size_t const & ndxHi = ndxLoHi.second;

		// compute fractional index into containing interval
		double fndx(0);
		if ((ndxLo < lastNdx) && (0u < ndxHi))
		{
			// interpolate between bracketing values
			std::pair<double, double> const valueLoHi
				(*(cdfFwdBeg + ndxLoHi.first), *(cdfFwdBeg + ndxLoHi.second));
			fndx = math::interp::fractionAtValid(value, valueLoHi);
		}

		// return absolute fractional index
		double const index((double)ndxLo + fndx);
		ndxLocs.push_back(index);
	}

	return ndxLocs;
}

template <typename FwdIter>
inline
// static
CdfInverse
CdfInverse :: fromFrequencies
	( FwdIter const & freqBeg
	, FwdIter const & freqEnd
	, dat::Range<double> const & dataRange
	, size_t const & lutSize
	)
{
	CdfInverse cdfInv;
	CdfForward const cdfFwd(CdfForward::fromFreqs(freqBeg, freqEnd, dataRange));
	if (cdfFwd.isValid())
	{
		cdfInv = fromCdfForward(cdfFwd, lutSize);
	}
	return cdfInv;
}

template <typename FwdIter>
inline
// explicit
CdfInverse :: CdfInverse
	( FwdIter const & cdfFwdBeg
	, FwdIter const & cdfFwdEnd
	, dat::Range<double> const & dataRange
	, size_t const & lutSize
	)
	: theDataIndices(ndxLocsForUniformDelta(cdfFwdBeg, cdfFwdEnd, lutSize))
	, theFracPart(dat::Range<double>(0., 1.), (theDataIndices.size()-1u))
	, theDataPart(dataRange, (std::distance(cdfFwdBeg, cdfFwdEnd)-1u))
{
}

inline
dat::Range<double>
CdfInverse :: dataRange
	() const
{
	return theDataPart.range();
}

inline
double
CdfInverse :: operator()
	( double const & prob
	) const
{
	double dataValue(dat::nullValue<double>());
	if (prob <= 0.)
	{
		dataValue = theDataPart.min();
	}
	else
	if (1. <= prob)
	{
		dataValue = theDataPart.max();
	}
	else
	{
		// interpolation functions check/propagate validity
		double const fracIndex(theFracPart.interpIndexFor(prob));
		double const ndx
			(math::interp::linear<double>(fracIndex, theDataIndices));
		dataValue = theDataPart.interpValueFor(ndx);
	}
	return dataValue;
}

//======================================================================
}

