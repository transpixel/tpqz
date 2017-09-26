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
\brief Inline definitions for prob::Stats
*/


#include "libdat/dat.h"

#include <cstddef>
#include <iterator>


namespace prob
{
// ======================================================================

template <typename FwdIter, typename DataType>
inline
// static
DataType
Stats :: meanValue
	( FwdIter const & beg //!< (*it) must be convertable to double
	, FwdIter const & end
	)
{
	DataType mean(dat::nullValue<DataType>());
	if (beg != end)
	{
		DataType const sum
			(std::accumulate(beg, end, static_cast<DataType>(0)));
		size_t const numSamps(std::distance(beg, end));
		mean = (1./(double)numSamps) * sum;
	}
	return mean;
}

template <typename FwdIter, typename DataType>
inline
// static
DataType
Stats :: nonConstMedianValue
	( FwdIter const & beg
	, FwdIter const & end
	)
{
	DataType median(dat::nullValue<DataType>());
	size_t const numSamps(std::distance(beg, end));
	if (1u == numSamps)
	{
		// deal with trival case
		median = *beg;
	}
	else
	if (1u < numSamps)
	{
		// Select middle value (exact center, or just less than center)
		ptrdiff_t const deltaMid((numSamps-1u) / 2u);
		typename std::vector<DataType>::iterator const itMid
			{ beg + deltaMid };
		std::nth_element(beg, itMid, end);
		if (1u == (numSamps % 2u)) // 3, 5, ...
		{
			// for odd number of elements use the middle
			median = *itMid;
		}
		else // 2, 4, ...
		{
			// for even number of elements, incorporate next value
			DataType const & med1 = *itMid;
			typename std::vector<DataType>::iterator const itNext
				{ itMid + 1 };
			typename std::vector<DataType>::const_iterator const itFind
				{ std::min_element(itNext, end) };
			DataType const med2(*itFind);
			median = .5 * (med1 + med2);
		}
	}
	return median;
}

template <typename FwdIter, typename DataType>
inline
// static
DataType
Stats :: medianValue
	( FwdIter const & beg //!< (*it) must be convertable to double
	, FwdIter const & end
	)
{
	DataType median(dat::nullValue<DataType>());
	size_t const numSamps(std::distance(beg, end));
	if (1u == numSamps)
	{
		// deal with trival case
		median = *beg;
	}
	else
	if (1u < numSamps)
	{
		// copy into working area (to let sort have its way)
		std::vector<DataType> tmp(numSamps);
		std::copy(beg, end, tmp.begin());

		// compute median by shuffling data around
		median = nonConstMedianValue(tmp.begin(), tmp.end());
	}
	return median;
}

template <typename FwdIter>
inline
Stats :: Stats
	( FwdIter const & beg
	, FwdIter const & end
	)
	: theSamples()
{
	FwdIter itIn(beg);
	while (end != itIn)
	{
		theSamples.push_back((double)*itIn++);

	}
}

// ======================================================================
}

