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
\brief Inline definitions for tri::IsoTille
*/


#include "libtri/IsoTille.h"

#include "libdat/MinMax.h"

#include <sstream>


namespace tri
{

// explicit
IsoTille :: IsoTille
	( tri::IsoGeo const & geometry
	)
	: theTileGeo{ geometry }
{
}

bool
IsoTille :: isValid
	() const
{
	return
		{  dat::isValid(theTileGeo)
		};
}

dat::Area<double>
IsoTille :: areaMuNu
	( dat::Area<double> const & areaXY
	) const
{
	dat::Area<double> mnArea;
	if (areaXY.isValid())
	{
		dat::MinMax<double> muMinMax;
		dat::MinMax<double> nuMinMax;
		std::array<Vec2D, 4u> const xyCorners(areaXY.extrema<Vec2D>());
		for (Vec2D const & xyCorner : xyCorners)
		{
			// expand the mu,nu dimensions (independently)
			double const mu{ theTileGeo.mu(xyCorner) };
			double const nu{ theTileGeo.nu(xyCorner) };
			muMinMax = muMinMax.expandedWith(mu);
			nuMinMax = nuMinMax.expandedWith(nu);
		}
		mnArea = dat::Area<double>{ muMinMax.pair(), nuMinMax.pair() };
	}
	return mnArea;
}

std::string
IsoTille :: infoString
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
		oss << dat::infoString(theTileGeo, "theTileGeo");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

} // tri

