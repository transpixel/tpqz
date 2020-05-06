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
\brief Inline definitions for prob::histo
*/


#include "libmath/interp.h"
#include "libprob/prob.h"

#include <algorithm>
#include <cassert>
#include <iterator>

#include <functional>
#include <iomanip>
#include <iostream>


namespace prob
{
namespace histo
{
//======================================================================
std::function<bool(size_t const &)> const nonZero
	( [] (size_t const & value) { return (0u != value); });


template <typename FwdIter>
inline
std::array<size_t, 256u>
countsForUint8
	( FwdIter const & begin
	, FwdIter const & end
	)
{
	std::array<size_t, 256u> hist;
	std::fill(hist.begin(), hist.end(), 0u);
	for (FwdIter iter(begin) ; iter != end ; ++iter)
	{
		double const value(static_cast<double>(*iter));
		if ((0. <= value) && (value < 256.))
		{
			size_t const bin(static_cast<size_t>(value));
			++hist[bin];
		}
	}
	return hist;
}

inline
size_t
minBinNotZero
	( std::array<size_t, 256u> const & hist
	)
{
	size_t bin(0u);
	std::array<size_t, 256u>::const_iterator const iter
		(std::find_if(hist.begin(), hist.end(), nonZero));
	if (hist.end() != iter)
	{
		bin = static_cast<size_t>(std::distance(hist.begin(), iter));
	}

	return bin;
}

inline
size_t
maxBinNotZero
	( std::array<size_t, 256u> const & hist
	)
{
	size_t bin(255u);
	std::array<size_t, 256u>::const_reverse_iterator const iter
		(std::find_if(hist.rbegin(), hist.rend(), nonZero));
	if (hist.rend() != iter)
	{
		size_t const deltaEnd(iter - hist.rbegin());
		assert(deltaEnd < hist.size()); // should be true if find_if okay
		size_t const loc(hist.size() - deltaEnd);
		if (0u < loc)
		{
			bin = (hist.size() - deltaEnd) - 1u;
		}
		else
		{
			bin = 0u;
		}
	}
	return bin;
}

template <typename FwdIter>
inline
std::vector<size_t>
countsFromSamps
	( FwdIter const & sampBeg
	, FwdIter const & sampEnd
	, math::Partition const & part
	)
{
	std::vector<size_t> hist(part.size());
	std::fill(hist.begin(), hist.end(), 0u);
	for (FwdIter iter(sampBeg) ; sampEnd != iter ; ++iter)
	{
		double const samp(static_cast<double>(*iter));
		size_t const ndx{part.binIndexFor(samp)};
		if (dat::isValid(ndx))
		{
			++hist[ndx];
		}
	}
	return hist;
}

template <typename Collection>
inline
std::vector<size_t>
countsFromSamps
	( Collection const & samps
	, math::Partition const & part
	)
{
	return countsFromSamps(samps.begin(), samps.end(), part);
}


//======================================================================
}
}


