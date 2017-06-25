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
\brief Definitions for math::PolyFunc
*/


#include "libmath/PolyFunc.h"

#include "libdat/algorithm.h"
#include "libdat/info.h"
#include "libmath/interp.h"

#include <algorithm>
#include <sstream>


namespace math
{
//======================================================================

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

// explicit
PolyFunc :: PolyFunc
	( std::vector<dat::Spot> const & verts
	)
	: PolyFunc(verts.begin(), verts.end())
{
}

bool
PolyFunc :: isValid
	() const
{
	return
		(  theRangeX.isValid()
		);
}

double
PolyFunc :: operator()
	( double const & xarg
	) const
{
	double value(dat::nullValue<double>());

	if (isValid())
	{
		// check for end cases
		if (theRangeX.min() == xarg)
		{
			value = theVerts.front()[1];
		}
		else
		if (theRangeX.max() == xarg)
		{
			value = theVerts.back()[1];
		}
		else
		if (theRangeX.contains(xarg))
		{
			// find indices which span the argument value -- O(ln(N))
			std::pair<size_t, size_t> const ndxLoHi
				(dat::indexBounds(xarg, theValuesX.begin(), theValuesX.end()));
			size_t const & ndxLo = ndxLoHi.first;
			size_t const & ndxHi = ndxLoHi.second;
			std::pair<double, double> const xRange
				(theValuesX[ndxLo], theValuesX[ndxHi]);
			std::pair<double, double> const yRange
				(theValuesY[ndxLo], theValuesY[ndxHi]);

			value = math::interp::linear(xRange, xarg, yRange);
		}
	}

	return value;
}

dat::Range<double> const &
PolyFunc :: range
	() const
{
	return theRangeX;
}

std::vector<dat::Spot> const &
PolyFunc :: xyVertices
	() const
{
	return theVerts;
}

std::vector<dat::Spot>
PolyFunc :: yxVertices
	() const
{
	std::vector<dat::Spot> revVerts;
	if (isValid())
	{
		std::vector<dat::Spot> const & fwdVerts = theVerts;
		revVerts.resize(fwdVerts.size());
		std::transform
			( fwdVerts.begin(), fwdVerts.end()
			, revVerts.begin()
			, [] (dat::Spot const & fwd) { return dat::Spot{ fwd[1], fwd[0] }; }
			);
	}
	return revVerts;
}

std::string
PolyFunc :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}

	if (isValid())
	{
		oss << dat::infoString(theRangeX, "theRangeX");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

std::string
PolyFunc :: infoStringContents
	( std::string const & title
	) const
{
	std::ostringstream oss;
	oss << infoString(title);
	if (isValid())
	{
		std::vector<dat::Spot> const verts(xyVertices());
		for (dat::Spot const & vert : verts)
		{
			oss << std::endl;
			oss << dat::infoString(vert);
		}
	}
	return oss.str();
}

//======================================================================
}

