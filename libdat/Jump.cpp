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
\brief Definitions for dat::Jump
*/

#include "libdat/Jump.h"

#include "libdat/compare.h"

#include <iomanip>
#include <sstream>


namespace dat
{
//======================================================================

// explicit
Jump :: Jump
	( size_t const & ndx
	, double const & lo
	, double const & hi
	)
	: theNdx(ndx)
	, theLo(lo)
	, theHi(hi)
{ }

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

bool
Jump :: nearlyEquals
	( Jump const & other
	) const
{
	bool same(theNdx == other.theNdx);
	if (same)
	{
		same &= dat::nearlyEquals(theLo, other.theLo);
		same &= dat::nearlyEquals(theHi, other.theHi);
	}
	return same;
}

std::string
Jump :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(3u);
	oss
		<< title
		<< " " << std::setw(4u) << theNdx
		<< " " << std::setw(9u) << theLo
		<< " " << std::setw(9u) << theHi
		;
	return oss.str();
}

//======================================================================
}

