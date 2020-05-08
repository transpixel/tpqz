//
//
// MIT License
//
// Copyright (c) 2020 Stellacore Corporation.
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
\brief Definitions for geo::VertGangle
*/


#include "libgeo/VertGangle.h"

#include <sstream>


namespace geo
{

// static
ga::Spinor
VertGangle :: spinGangleFor
	( Wedge const & wedge
	)
{
	ga::Vector const inv1{ ga::inverse(wedge.edge1()) };
	return { ga::logG2(wedge.edge2() * inv1) };
}

// explicit
VertGangle :: VertGangle
	( ga::Vector const & vert
	, std::pair<ga::Vector, ga::Vector> const & locPair
	)
	: theGangle{ spinGangleFor(geo::Wedge(vert, locPair)) }
{
}

// explicit
VertGangle :: VertGangle
	( Wedge const & wedge
	)
	: theGangle{ spinGangleFor(wedge) }
{
}

std::string
VertGangle :: infoString
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
		oss
			<< dat::infoString(theGangle, "theGangle")
			<< " angleMag: " << dat::infoString(angleMag())
			<< " ratioMag: " << dat::infoString(ratioMag())
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

} // geo

