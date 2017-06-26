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
\brief Definitions for cam::ViewPyramid
*/


#include "libcam/ViewPyramid.h"

#include "libga/ga.h"
#include "libgeo/xform.h"

#include <sstream>
#include <array>


namespace cam
{

// explicit
ViewPyramid :: ViewPyramid
	( Camera const & camera
	, double const & edgeMag
	)
	: theCamera(camera)
	, theEdgeMag{ edgeMag }
{
}

bool
ViewPyramid :: isValid
	() const
{
	return
		(  dat::isValid(theCamera)
		&& dat::isValid(theEdgeMag)
		);
}

std::vector<geo::Triangle>
ViewPyramid :: trianglesInCam
	() const
{
	std::vector<geo::Triangle> sides;

	// get directions associated with camera detector corners
	std::array<ga::Vector, 4u> const dirs
		(theCamera.cornerDirections());

	// scale to requested edge length
	std::array<ga::Vector, 4u> const pnts
		{{ theEdgeMag*dirs[0]
		 , theEdgeMag*dirs[1]
		 , theEdgeMag*dirs[2]
		 , theEdgeMag*dirs[3]
		}};

	// form outward oriented triangular sizes
	ga::Vector const & apex = ga::vZero;
	sides = std::vector<geo::Triangle>
		{ geo::Triangle(apex, pnts[0], pnts[1])
		, geo::Triangle(apex, pnts[1], pnts[2])
		, geo::Triangle(apex, pnts[2], pnts[3])
		, geo::Triangle(apex, pnts[3], pnts[0])
		};

	return sides;
}

std::vector<geo::Triangle>
ViewPyramid :: trianglesInRef
	( ga::Rigid const & xRefWrtCam
	) const
{
	std::vector<geo::Triangle> const asides(trianglesInCam());
	return geo::xform::apply(xRefWrtCam, asides.begin(), asides.end());
}


std::string
ViewPyramid :: infoString
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
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

}

