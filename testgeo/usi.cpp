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
\brief  This file contains unit test for geo::si
*/


#include "libgeo/si.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/sprintf.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
geo_si_test0
	()
{
	std::ostringstream oss;
	/*
	geo::si const aNull(dat::nullValue<geo::si>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

//! Check demonstrate Thales' type test
std::string
geo_si_test1
	()
{
	std::ostringstream oss;

	constexpr double staH{ 50. }; // station height
	constexpr double range{ 1000. }; // horizontal distance to target
	ga::Vector const base{ ga::zero };
	constexpr double sigRay{ 2. };
	constexpr double sigSea{ 1. };

	ga::Vector const raySta{ base + staH * ga::e3 };
	ga::Vector const expPnt{ base -range * ga::e1 };

	// measurement observations
	geo::Ray const ray{ geo::Ray::fromToward(raySta, expPnt) };
	geo::Plane const sea(base, ga::E12);

	/*
	io::out() << dat::infoString(base, "base") << std::endl;
	io::out() << dat::infoString(raySta, "raySta") << std::endl;
	io::out() << dat::infoString(expPnt, "expPnt") << std::endl;
	io::out() << dat::infoString(ray, "ray") << std::endl;
	io::out() << dat::infoString(sea, "sea") << std::endl;
	*/

	// collect observations of same classifications
	std::vector<geo::si::WRay> const wrays{ {(1./sigRay), ray} };
	std::vector<geo::si::WPlane> const wplanes{ {(1./sigSea), sea} };

	// create observation system
	geo::si::PointSystem system{};
	system.addWeightedRays(wrays);
	system.addWeightedPlanes(wplanes);

	// compute intersection of collections
	geo::si::PointSoln const pntSoln{ system.pointSolution() };

	// check point solution
	ga::Vector const & gotPnt = pntSoln.theLoc;
	bool okay{ true };
	double const tol{ staH * range * math::eps };
	if (! gotPnt.nearlyEquals(expPnt, tol))
	{
		ga::Vector const difPnt{ gotPnt - expPnt };
		double const difMag{ ga::magnitude(difPnt) };
		oss << "Failure of ray/sea pointSolution.theLoc test" << std::endl;
		oss << dat::infoString(expPnt, "expPnt") << std::endl;
		oss << dat::infoString(gotPnt, "gotPnt") << std::endl;
		oss << dat::infoString(difPnt, "difPnt")
			<< "  difMag: " << io::sprintf("%12.5e", difMag)
			<< std::endl;
		okay = false;
	}

	// check uncertainty
	// (no prediction possible; single plane and one ray has exact soln)

	constexpr bool show{ false };
	if ((! okay) || show)
	{
		io::out() << "==== test data ====>" << std::endl;
		io::out() << dat::infoString(sea, "sea") << std::endl;
		io::out() << dat::infoString(ray, "ray") << std::endl;
		io::out() << dat::infoString(pntSoln, "pntSoln") << std::endl;
		io::out() << "<=== test data" << std::endl;
	}

	return oss.str();
}

//! Check two ray intersection
std::string
geo_si_test2
	()
{
	std::ostringstream oss;

	ga::Vector const sta1{  1., -1.,  0. };
	ga::Vector const sta2{  1.,  1.,  0. };

	ga::Vector const expPnt{ 0., 0., 0. };
	double const sig0{ 8. };
	std::array<double, 3u> const expSigs{ sig0, sig0, .5*sig0 }; // big->sml

	// measurement observations
	geo::Ray const ray1{ geo::Ray::fromToward(sta1, expPnt) };
	geo::Ray const ray2{ geo::Ray::fromToward(sta2, expPnt) };
	double const sig1{ sig0 };
	double const sig2{ sig0 };

	// collect observations of same classifications
	std::vector<geo::si::WRay> const wrays
		{ { (1./sig1), ray1 }
		, { (1./sig2), ray2 }
		};

	// compute intersection of collections
	geo::si::PointSystem system{};
	system.addWeightedRays(wrays);

	// io::out() << "\n\n";
	// io::out() << dat::infoString(system, "system") << std::endl;

	geo::si::PointSoln const pntSoln{ system.pointSolution() };

	// io::out() << dat::infoString(pntSoln, "pntSoln") << std::endl;

	// check point location
	ga::Vector const & gotPnt = pntSoln.theLoc;
	bool okay{ true };
	if (! gotPnt.nearlyEquals(expPnt))
	{
		oss << "Failure of ray/ray pointSolution.theLoc test" << std::endl;
		oss << dat::infoString(expPnt, "expPnt") << std::endl;
		oss << dat::infoString(gotPnt, "gotPnt") << std::endl;
		okay = false;
	}

	// check uncertainty ellipsoid
	geo::si::SemiAxis const gotSig0{ pntSoln.kthLargestSemiAxis(0) };
	geo::si::SemiAxis const gotSig1{ pntSoln.kthLargestSemiAxis(1) };
	geo::si::SemiAxis const gotSig2{ pntSoln.kthLargestSemiAxis(2) };
	std::array<double, 3u> const gotSigs
		{ gotSig0.theMag, gotSig1.theMag, gotSig2.theMag };
	if (! dat::nearlyEquals(gotSigs, expSigs))
	{
		oss << "Failure of aposterior sigma estimate test" << std::endl;
		oss << dat::infoString(expSigs, "expSigs") << std::endl;
		oss << dat::infoString(gotSigs, "gotSigs") << std::endl;
	}

	constexpr bool show{ false };
	if ((! okay) || show)
	{
		io::out() << "==== test data ====>" << std::endl;
		io::out() << dat::infoString(ray1, "ray1") << std::endl;
		io::out() << dat::infoString(ray2, "ray2") << std::endl;
		io::out() << dat::infoString(pntSoln, "pntSoln") << std::endl;
		io::out() << "<=== test data" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for geo::si
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << geo_si_test0();
	oss << geo_si_test1();
	oss << geo_si_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
