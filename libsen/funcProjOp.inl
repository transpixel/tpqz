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
\brief Inline definitions for sen::funcProjOp
*/



namespace sen
{

namespace cylinder
{

inline
geo::Ray
ProjOp :: operator()
	( dat::Spot const & mapSpot
	) const
{
	// determine ray start location from row value
	double const zValue(thePartZ.interpValueFor(mapSpot[0]));
	ga::Vector const start(0., 0., zValue);

	// determine azimuth from column value
	// NOTE: simple case here: in general would keep sub-pix precision
	size_t const col{ size_t(std::floor(mapSpot[1])) };
	double const & azimCos = theLutCos[col];
	double const & azimSin = theLutSin[col];

	// horizontal direction in cylindrical coordinates
	ga::Vector const dir(azimCos, azimSin, 0.);

	// return ray
	geo::Ray const ray{start, dir};
	return ray;
}

}

namespace central
{

inline
geo::Ray
ProjOp :: operator()
	( dat::Spot const & spotInDet
	) const
{
	// transform into "optics" frame
	dat::Spot const spotInOpt(theOptWrtDet(spotInDet));

	// center of perspective is at origin
	static ga::Vector const & rayStart = ga::vZero;

	// rectilinear forward projection
	ga::Vector const rayDir(theCam.directionOf(spotInOpt));

	return geo::Ray(rayStart, rayDir);
}

}

}

