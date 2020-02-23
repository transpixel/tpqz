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
\brief Declarations for geo::Plane
*/


#include "libgeo/Plane.h"

#include <string>
#include <sstream>


namespace geo
{

// explicit
Plane :: Plane
	( ga::Vector const & anyPIP
	, ga::BiVector const & dirBiv
	)
	: theDir(ga::unit(dirBiv))
	, theNormVec(-ga::E123*theDir)
	, theNormMag(ga::dot(anyPIP, theNormVec).theValue)
{
}

bool
Plane :: isValid
	() const
{
	return
		(  theDir.isValid()
		&& theNormVec.isValid()
		&& dat::isValid(theNormMag)
		);
}

ga::Vector
Plane :: origin
	() const
{
	return (theNormMag * theNormVec);
}

ga::Vector
Plane :: unitNormal
	() const
{
	return theNormVec;
}

ga::Vector
Plane :: projectionOf
	( ga::Vector const & anypnt
	) const
{
	ga::Vector proj{};
	if (isValid() && anypnt.isValid())
	{
		proj = anypnt - offsetTo(anypnt) * theNormVec;
	}
	return proj;
}

ga::Vector
Plane :: rejectionTo
	( ga::Vector const & anypnt
	) const
{
	ga::Vector rej{};
	if (isValid() && anypnt.isValid())
	{
		rej = offsetTo(anypnt) * theNormVec;
	}
	return rej;
}

double
Plane :: offsetTo
	( ga::Vector const & anypnt
	) const
{
	double const offFromZero{ ga::dot(anypnt, theNormVec).theValue };
	return (offFromZero - theNormMag);
}

std::string
Plane :: infoString
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
		oss << dat::infoString(origin(), "pointOn");
		oss << std::endl;
		oss << dat::infoString(theNormVec, "normVec");
		oss << std::endl;
		oss << dat::infoString(theNormMag, "origDist");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

}

