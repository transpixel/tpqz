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
\brief Inline definitions for cam::Camera
*/


#include "libmath/interp.h"


namespace cam
{

inline
dat::Spot
Camera :: imageSpotFor
	( dat::Spot const & detspot
	) const
{
	return math::interp::linear(theAreaInDet, detspot, theAreaInImg);
}

inline
dat::Spot
Camera :: detectorSpotFor
	( dat::Spot const & imgspot
	) const
{
	dat::Spot detSpot(dat::nullValue<dat::Spot>());
	using math::interp::linear;
	dat::Spot const tmpSpot(linear(theAreaInImg, imgspot, theAreaInDet));
	if (dat::isValid(tmpSpot) && theAreaInDet.contains(tmpSpot))
	{
		detSpot = tmpSpot;
	}
	return detSpot;
}

inline
dat::RowCol
Camera :: detectorRowColFor
	( dat::Spot const & imgspot
	) const
{
	return dat::cast::RowCol(detectorSpotFor(imgspot));
}

inline
dat::Spot
Camera :: imageSpotFor
	( ga::Vector const & objpnt
	) const
{
	dat::Spot spot(dat::nullValue<dat::Spot>());
	if (theOptics.isValid())
	{
    	spot = theOptics.imageSpotFor(objpnt);
	}
    return spot;
}

inline
dat::RowCol
Camera :: detectorRowColFor
	( ga::Vector const & objpnt
	) const
{
	return detectorRowColFor(imageSpotFor(objpnt));
}

inline
bool
Camera :: isVisible
	( dat::Spot const & imgspot
	) const
{
	return theAreaInImg.contains(imgspot);
}

inline
ga::Vector
Camera :: directionOf
	( dat::Spot const & imgspot
	) const
{
	return theOptics.directionOf(imgspot);
}

inline
ga::Vector
Camera :: lookDir
	() const
{
	return theOptics.directionOf(dat::Spot{{ 0., 0. }});
}


}

