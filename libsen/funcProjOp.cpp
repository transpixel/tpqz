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
\brief Definitions for sen::funcProjOp
*/


#include "libsen/funcProjOp.h"

#include "libdat/ops.h"


namespace sen
{

namespace cylinder
{

namespace
{
	std::vector<double>
	lutCos
		( size_t const & numElem
		, double const & phase = 0.
		)
	{
		std::vector<double> lut;
		lut.reserve(numElem);
		double const da{ math::twoPi / double(numElem) };
		for (size_t nn{0u} ; nn < numElem ; ++nn)
		{
			double const angle{ double(nn) * da + phase};
			lut.emplace_back(std::cos(angle));
		}
		return lut;
	}
}

// explicit
ProjOp :: ProjOp
	( dat::Extents const & hwSize
	, dat::Range<double> const & zRange
	)
	: thePartZ(zRange, hwSize.high())
	, theLutCos(lutCos(hwSize.wide(), -math::pi))
	, theLutSin(lutCos(hwSize.wide(), -math::pi-math::halfPi))
{ }

}


namespace central
{

// explicit
ProjOp :: ProjOp
	( dat::Extents const & detSize
	)
	: theCam(.400 * dat::diagonalMag(detSize)) // ~ wide angle
	, theOptWrtDet(dat::cast::Spot(detSize.centerMod2()))
{ }

}

}

