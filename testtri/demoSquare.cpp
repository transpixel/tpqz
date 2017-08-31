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
\brief  This file contains main application program demoSquare
*/


#include "libtri/IsoGeo.h"
//#include "libtri/IsoTille.h"

#include "libio/stream.h"

#include <cassert>
#include <iostream>


namespace
{
	struct Domain
	{
		dat::Range<double> xRange{ -1., 1. };
		dat::Range<double> yRange{ -1., 1. };
		dat::Area<double> theArea{ xRange, yRange };

		std::array<dat::Spot, 4u>
		corners
			() const
		{
			return theArea.extrema<dat::Spot>();
		}

		bool
		contains
			( dat::Spot const & loc
			) const
		{
			return theArea.contains(loc);
		}
	};
}

//! Demonstrate iso-tritille tiling of a square region
int
main
	( int const // argc
	, char const * const * const // argv
	)
{
	constexpr double deltaHigh{ 1./10. };
//	constexpr double deltaWide{ 1./20. };
constexpr double deltaWide{ 1./10. };
	constexpr dat::Spot aDir{{ 0., 1. }};
	tri::IsoGeo const trigeo(deltaHigh, deltaWide, aDir);

	Domain const square;
	dat::Area<double> const rngArea{ trigeo.areaMuNu(square.theArea) };

	io::out() << dat::infoString(trigeo, "trigeo") << std::endl;
	io::out() << dat::infoString(square.theArea, "square.theArea") << std::endl;
	io::out() << dat::infoString(rngArea, "rngArea") << std::endl;

	std::array<dat::Spot, 4u> const domCorners(square.corners());
	for (dat::Spot const & domCorner : domCorners)
	{
		dat::Spot const rngCorner(trigeo.locMuNu(domCorner));
		dat::QuantumFrac const muQF{ trigeo.muNdxFrac(domCorner) };
		dat::QuantumFrac const nuQF{ trigeo.nuNdxFrac(domCorner) };

		io::out() << std::endl;
		io::out() << dat::infoString(domCorner, "domCorner") << std::endl;
		io::out() << dat::infoString(rngCorner, "rngCorner") << std::endl;
		io::out() << dat::infoString(muQF, "muQF") << std::endl;
		io::out() << dat::infoString(nuQF, "nuQF") << std::endl;
	}

	return 0;
}
