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
\brief Definitions for model::SimHole
*/


#include "libmodel/SimHole.h"

#include "libmath/angle.h"
#include "libmath/interp.h"
#include "libmath/math.h"
#include "libmath/QuadEq.h"
#include "libmodel/traits.h"

#include <sstream>


namespace
{
	//! Direction cosines of vector in the "x-y" plane
	inline
	std::pair<double, double>
	xyCosSin
		( ga::Vector const & vec // any non-zero mag
		)
	{
		double const hMag{ math::hypot(vec[0], vec[1]) };
		double const cosAzim{ vec[0] / hMag };
		double const sinAzim{ vec[1] / hMag };
		return { cosAzim, sinAzim };
	}
}


namespace model
{

ga::Vector
SimHole :: locationFor
	( geo::Ray const & rayInModel
	) const
{
	ga::Vector locInModel;

	ga::Vector const & rayDir = rayInModel.theDir;
	ga::Vector const & rayStart = rayInModel.theStart;
	double const & startZ = rayStart[2];

	// intersect horizontal projection of ray with wall surface
	ga::Vector const hWallPoint(horizontalXYZ(rayInModel));

	// compute elevation difference
	double const hRadMag{ ga::magnitude(hWallPoint - rayStart) };
	double const hDirMag{ math::hypot(rayDir[0], rayDir[1]) };
	double const & vDirVal = rayDir[2];

	// adjust wall point by elevation diff
	double const deltaZ{ hRadMag * (vDirVal / hDirMag) };
	double const solnZ{ startZ + deltaZ };

	// check if intersection is within depth range
	if (theRangeZ.contains(solnZ))
	{
		ga::Vector const wallPoint(hWallPoint[0], hWallPoint[1], solnZ);
		locInModel = wallPoint;
	}

	return locInModel;
}

Part
SimHole :: partFor
	( ga::Vector const & locInModel
	) const
{
	// radiometric properties
	double const red{ locInModel[0] / theRadius };
	double const grn{ locInModel[1] / theRadius };
	double const blu
		{ 1. - 2.*math::interp::fractionAtValid(locInModel[2], theRangeZ) };
	atom::Color const color
		{{ float(red)
		 , float(grn)
		 , float(blu)
		}};

	// geometric properties
	ga::Vector const surfVecH(-locInModel[0], -locInModel[1], 0.);
	ga::Vector const normvec(ga::unit(surfVecH));

	// return component
	traits::Radiometry const radTraits{ color };
	traits::Geometry const geoTraits{ normvec };
	return Part{ locInModel, radTraits, geoTraits };
}

ga::Vector
SimHole :: centroid
	() const
{
	return ga::Vector(0., 0., theRangeZ.middle());
}

dat::Range<double>
SimHole :: zRange
	() const
{
	return theRangeZ;
}

double
SimHole :: maxRadius
	() const
{
	return math::hypot(.5*depthSpan(), theRadius+theDeltaRad);
}

bool
SimHole :: isValid
	() const
{
	return
		(  dat::isValid(theRangeZ)
		&& dat::isValid(theRadius)
		&& dat::isValid(theDeltaRad)
		);
}

double
SimHole :: depthSpan
	() const
{
	return theRangeZ.magnitude();
}

double
SimHole :: perimeterSpan
	() const
{
	return (math::twoPi * theRadius);
}

//! Nominal radius of hole
double
SimHole :: nominalRadius
	() const
{
	return theRadius;
}

std::string
SimHole :: infoString
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
		oss << dat::infoString(theRangeZ, "theRangeZ");

		oss << '\n';
		oss << dat::infoString(theRadius, "theRadius");

		oss << '\n';
		oss << dat::infoString(theDeltaRad, "theDeltaRad");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}


std::pair<double, double>
SimHole :: hRangeBrackets
	( geo::Ray const & hRay
	) const
{
	constexpr double pad{ 1./1024. }; // make life easy
	double const rhoMin{ theRadius - theDeltaRad - pad};
	double const rhoMax{ theRadius + theDeltaRad + pad};

	ga::Vector const & hvec = hRay.theStart;
	ga::Vector const & dhat = hRay.theDir;
	double const hdDot{ ga::dot(hvec, dhat).theValue };
	double const hSq{ ga::dot(hvec, hvec).theValue };

	math::QuadEq const eqMin( 1., 2.*hdDot, (hSq-math::sq(rhoMin)) );
	double const lambdaMin{ eqMin.realRootMax() };
	math::QuadEq const eqMax( 1., 2.*hdDot, (hSq-math::sq(rhoMax)) );
	double const lambdaMax{ eqMax.realRootMax() };

	// assert(0. < lambdaMin);
	// assert(0. < lambdaMax);
	// assert(lambdaMin < lambdaMax);

	return { lambdaMin, lambdaMax };
}

double
SimHole :: hRadiusToward
	( std::pair<double, double> const & azimCosSin
	) const
{
	double const & cosAzim1 = azimCosSin.first;
	double const & sinAzim1 = azimCosSin.second;

	// add undulation to make interesting
	double const cosAzim2{ math::cos2x(cosAzim1, sinAzim1) };
	double const sinAzim2{ math::sin2x(cosAzim1, sinAzim1) };
	double const cosAzim4{ math::cos2x(cosAzim2, sinAzim2) };
	double const hRadMag{ theRadius*(1. - theDeltaRad*cosAzim4) };

	return hRadMag;
}

std::pair<double, double>
SimHole :: hMeritNegPos
	( ga::Vector const & hPntMin
	, ga::Vector const & hPntMax
	) const
{
	std::pair<double, double> const csMin(xyCosSin(hPntMin));
	double const hRadSurfMin{ hRadiusToward(csMin) };
	double const meritPos{ hRadSurfMin - ga::magnitude(hPntMin) };

	std::pair<double, double> const csMax(xyCosSin(hPntMax));
	double const hRadSurfMax{ hRadiusToward(csMax) };
	double const meritNeg{ hRadSurfMax - ga::magnitude(hPntMax) };

	// assert(0. < meritPos);
	// assert(meritNeg < 0.);
	return { meritNeg, meritPos };
}


ga::Vector
SimHole :: horizontalXY
	( geo::Ray const & rayInModel
	) const
{
	ga::Vector hPnt;

	ga::Vector const & rayDir = rayInModel.theDir;
	ga::Vector const & rayStart = rayInModel.theStart;

	// Here: surface determined by root-finding distance along ray

	// for example here, reduce to problem in horizontal plane
	ga::Vector const hvec(rayStart[0], rayStart[1], 0.);
	ga::Vector const hVec(rayDir[0], rayDir[1], 0.);
	geo::Ray const hRay(hvec, ga::unit(hVec));

	// determine range values that bracket the root
	// lambda: dist along rayInModel to surface
	std::pair<double, double> const lambdaMinMax(hRangeBrackets(hRay));

	// points on surface in direction of root brackets
	ga::Vector const hPntMin(hRay.pointAt(lambdaMinMax.first));
	ga::Vector const hPntMax(hRay.pointAt(lambdaMinMax.second));

	// evaluate merit function at the root brackets
	std::pair<double, double> const meritNegPos
		(hMeritNegPos(hPntMin, hPntMax));

	// Perform "root finding" to get value of lambda
	// Here - just use quick linear approximation
	ga::Vector hPntRoot;
	{
		double const fracToRoot
			{ math::interp::fractionAtValid(0., meritNegPos) };
		hPntRoot = math::interp::weightedCombo
			(fracToRoot, hPntMin, hPntMax);
	}

	hPnt = hPntRoot;

	return hPnt;
}

ga::Vector
SimHole :: horizontalXYZ
	( geo::Ray const & rayInModel
	) const
{
	ga::Vector const & rayStart = rayInModel.theStart;

	// get horizontal delta from origin
	ga::Vector const hXY(horizontalXY(rayInModel));

	// compute location on surface
	// hPntRoot contains the horizontal solution while
	// the original input ray contains the vertical offset
	// (for this specific intersection model)
	return ga::Vector(hXY[0], hXY[1], rayStart[2]);
}

}

