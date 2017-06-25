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
\brief Definitions for math::MapSizeArea
*/


#include "libmath/MapSizeArea.h"

#include "libdat/info.h"
#include "libmath/interp.h"

#include <sstream>


namespace math
{

// explicit
MapSizeArea :: MapSizeArea
	( dat::Extents const & hwSize
	, dat::Area<double> const & xyArea
	)
	: theAreaHW
		{ dat::Range<double>(0., double(hwSize.high()))
		, dat::Range<double>(0., double(hwSize.wide()))
		}
	, theAreaXY{ xyArea }
{
}

bool
MapSizeArea :: isValid
	() const
{
	return
		(  theAreaHW.isValid()
		&& theAreaXY.isValid()
		);
}

dat::Spot
MapSizeArea :: hwSizeSpotForNoCheck
	( dat::Spot const & xyAreaSpot
	) const
{
	return math::interp::linear(theAreaXY, xyAreaSpot, theAreaHW);
}

dat::Spot
MapSizeArea :: xyAreaSpotForNoCheck
	( dat::Spot const & hwSizeSpot
	) const
{
	return math::interp::linear(theAreaHW, hwSizeSpot, theAreaXY);
}

dat::Spot
MapSizeArea :: hwSizeSpotFor
	( dat::Spot const & xyAreaSpot
	) const
{
	dat::Spot sizeSpot(dat::nullValue<dat::Spot>());
	if (isValid() && theAreaXY.contains(xyAreaSpot))
	{
		sizeSpot = hwSizeSpotForNoCheck(xyAreaSpot);
	//	sizeSpot = math::interp::linear(theAreaXY, xyAreaSpot, theAreaHW);
	}
	return sizeSpot;
}

dat::Spot
MapSizeArea :: xyAreaSpotFor
	( dat::Spot const & hwSizeSpot
	) const
{
	dat::Spot areaSpot(dat::nullValue<dat::Spot>());
	if (isValid() && theAreaHW.contains(hwSizeSpot))
	{
		areaSpot = xyAreaSpotForNoCheck(hwSizeSpot);
	//	areaSpot = math::interp::linear(theAreaHW, hwSizeSpot, theAreaXY);
	}
	return areaSpot;
}

std::string
MapSizeArea :: infoString
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
		oss << dat::infoString(theAreaHW, "theAreaHW");
		oss << std::endl;
		oss << dat::infoString(theAreaXY, "theAreaXY");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}


} // math

