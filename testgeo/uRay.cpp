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
\brief  This file contains unit test for geo::Ray
*/


#include "libgeo/Ray.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include "libga/Pose.h"
#include "libga/spin.h"
#include "libio/sprintf.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
geo_Ray_test0
	()
{
	std::ostringstream oss;
	geo::Ray const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check basic projection operations
std::string
geo_Ray_test1
	()
{
	std::ostringstream oss;

//#define EasyTest

	// test configuration
#	if defined(EasyTest)
	ga::Vector const expSta(  0., 0., 0.);
	ga::Vector const expDir(ga::unit(ga::Vector(1., 0., 0.)));
	ga::Vector const expPnt( 1.,  1.,  1.);
#	else
	ga::Vector const expSta( -5., 3., 7.);
	ga::Vector const expDir(ga::unit(ga::Vector(5., -2., 3.)));
	ga::Vector const expPnt(17., -13., 19.);
#	endif

	// arbitrary ray
	geo::Ray const ray(expSta, expDir);

	// construct coherent test data
	double const expDist{ ga::dot((expPnt-expSta), expDir).theValue };
	ga::Vector const expPrj(ray.pointAt(expDist));
	ga::Vector const expRej(expPnt - expPrj);

	// evaluate ray methods
	ga::Vector const gotPrj(ray.projectionOf(expPnt));
	ga::Vector const gotRej(ray.rejectionTo(expPnt));
	double const gotDist{ ray.distanceAlong(expPnt) };

	if (! dat::nearlyEquals(gotDist, expDist))
	{
		oss << "Failure of distance along test" << std::endl;
		oss << dat::infoString(expDist, "expDist") << std::endl;
		oss << dat::infoString(gotDist, "gotDist") << std::endl;
	}
	if (! gotPrj.nearlyEquals(expPrj))
	{
		oss << "Failure of projection test" << std::endl;
		oss << dat::infoString(expPrj, "expPrj") << std::endl;
		oss << dat::infoString(gotPrj, "gotPrj") << std::endl;
	}
	if (! gotRej.nearlyEquals(expRej))
	{
		oss << "Failure of rejection test" << std::endl;
		oss << dat::infoString(expRej, "expRej") << std::endl;
		oss << dat::infoString(gotRej, "gotRej") << std::endl;
	}

	return oss.str();
}

//! Check angleTo()
std::string
geo_Ray_test2
	()
{
	std::ostringstream oss;

	using namespace ga;

//#define EasyData
#	if defined(EasyData)
	Vector const pntInRef( -1.,  -1.,   0.);
	Vector const staInRef(  0.,   0.,   0. );
	Vector const dirInRef{ unit(Vector(0., 1., 0.)) };
#	else
	Vector const pntInRef( 1.20, 2.30, 3.40);
	Vector const staInRef(-1.00, 2.00, 3.00 );
	Vector const dirInRef{ unit(Vector(3., 2., 1.)) };
#	endif
	geo::Ray const rayInRef(staInRef, dirInRef);

	// get angle of pnt w.r.t. ray
	BiVector const gotAngPntWrtRay{ rayInRef.angleTo(pntInRef) };
	Pose const attPntWrtRef(gotAngPntWrtRay);

	// transform direction of ray
	geo::Ray const rayToPnt(staInRef, attPntWrtRef(dirInRef));
	ga::Vector const gotRejVec{ rayToPnt.rejectionTo(pntInRef) };

	// and check if rotated ray hits point
	if (! gotRejVec.nearlyEquals(vZero))
	{
		double const gotRejMag{ magnitude(gotRejVec) };
		oss << "Failure of angleTo, rejection test" << std::endl;
		oss << dat::infoString(gotRejVec, "gotRejVec") << std::endl;
		oss << "gotRejMag: " << io::sprintf("%12.5e", gotRejMag) << std::endl;
		/*
		oss << dat::infoString(pntInRef, "pntInRef") << std::endl;
		oss << dat::infoString(rayInRef, "rayInRef") << std::endl;
		oss << dat::infoString(gotAngPntWrtRay, "gotAngPntWrtRay") << std::endl;
		oss << dat::infoString(dirInRef, "dirInRef") << std::endl;
		oss << dat::infoString(rayToPnt, "rayToPnt") << std::endl;
		*/
	}

	return oss.str();
}

}

//! Unit test for geo::Ray
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << geo_Ray_test0();
	oss << geo_Ray_test1();
	oss << geo_Ray_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
