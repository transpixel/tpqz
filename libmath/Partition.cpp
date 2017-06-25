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
\brief Definitions for math::Partition
*/


#include "libmath/Partition.h"

#include "libdat/compare.h"
#include "libdat/dat.h"
#include "libdat/info.h"

#include <limits>
#include <sstream>


namespace math
{
//======================================================================

namespace
{
	//! Size of interval within min,max
	inline
	double
	deltaFor
		( double const & min
		, double const & max
		, size_t const & size
		)
	{
		return ((max - min) / (double)size);
	}
}

Partition :: Partition
	()
	: theNumParts(dat::nullValue<size_t>())
	, theMin(dat::nullValue<double>())
	, theDelta(dat::nullValue<double>())
{
}

// explicit
Partition :: Partition
	( double const & min
	, double const & delta
	, size_t const & numParts
	)
	: theNumParts(numParts)
	, theMin(min)
	, theDelta(delta)
{
}

// explicit
Partition :: Partition
	( dat::Range<double> const & range
	, size_t const & numParts
	)
	: theNumParts(numParts)
	, theMin(range.min())
	, theDelta(deltaFor(range.min(), range.max(), theNumParts))
{
}

// explicit
Partition :: Partition
	( std::pair<double, double> const & pair
	, size_t const & numParts
	)
	: theNumParts(numParts)
	, theMin(pair.first)
	, theDelta(deltaFor(pair.first, pair.second, theNumParts))
{
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

bool
Partition :: nearlyEquals
	( Partition const & other
	, double const & tol
	) const
{
	bool same(theNumParts == other.theNumParts);
	if (same)
	{
		same = range().nearlyEquals(other.range(), tol);
	}
	return same;
}

std::string
Partition :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << " ";
	}
	oss
		<< "num,del,range:"
		<< " " << dat::infoString(theNumParts)
		<< " " << dat::infoString(theDelta)
		<< " (" << range().infoString() << ")"
		;
	return oss.str();
}

//======================================================================
}

