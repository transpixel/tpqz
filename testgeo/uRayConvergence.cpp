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
\brief  This file contains unit test for geo::RayConvergence
*/


#include "libgeo/RayConvergence.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
geo_RayConvergence_test0
	()
{
	std::ostringstream oss;
	geo::RayConvergence const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check basic intersection characteristics
std::string
geo_RayConvergence_test1
	()
{
	std::ostringstream oss;

//#define EASYDATA

	// simulation configuration
#	if defined (EASYDATA)
		constexpr double smlDist{  1. };
		constexpr double staDist{ 10. };
		constexpr double rejTol{ .5 };
		std::vector<ga::Vector> const aimPoints
			{  smlDist *         (ga::Vector(0., 0., 0.))
			,  smlDist *         (ga::Vector(0., 0., 0.))
			,  smlDist * ga::unit(ga::Vector(0., 1., 0.))
			};
		std::vector<ga::Vector> const starts
			{  staDist * ga::unit(ga::Vector( 1.,  0.,  1.))
			,  staDist * ga::unit(ga::Vector(-1.,  0.,  1.))
			,  staDist * ga::unit(ga::Vector( 0.,  0.,  1.))
			};
#	else
		constexpr double smlDist{  1. };
		constexpr double bigDist{  7. };
		constexpr double staDist{ 17. };
		constexpr double rejTol{ .5 * bigDist };

		static ga::Vector const diag1{ ga::unit( ga::e1 + ga::e2 + ga::e3) };
		static ga::Vector const diag2{ ga::unit(-ga::e1 + ga::e2 + ga::e3) };
		static ga::Vector const diag3{ ga::unit(-ga::e1 - ga::e2 + ga::e3) };
		static ga::Vector const diag4{ ga::unit( ga::e1 - ga::e2 + ga::e3) };

		static ga::Vector const diagA{ ga::unit(ga::Vector(-3., 4., -5.)) };
		static ga::Vector const diagB{ ga::unit(ga::Vector( 7., -5., 3.)) };
		static ga::Vector const diagC{ ga::unit(ga::Vector( 2., -3., 8.)) };

		std::vector<ga::Vector> const aimPoints
			{  smlDist * ga::vZero // expected
			// tight symmetric distribution
			,  smlDist * diag1 , -smlDist * diag1
			, -smlDist * diag1 ,  smlDist * diag1
			,  smlDist * diag2 , -smlDist * diag2
			, -smlDist * diag2 ,  smlDist * diag2
			,  smlDist * diag3 , -smlDist * diag3
			, -smlDist * diag3 ,  smlDist * diag3
			// outliers or bias
			,  bigDist * diag1
			,  bigDist * diag2
			,  bigDist * diag3
			,  bigDist * diag4
			};
		std::vector<ga::Vector> const starts
			{ staDist * ga::e3
			//
			,  staDist * ga::e1 , -staDist * ga::e1
			, -staDist * ga::e1 ,  staDist * ga::e1
			,  staDist * ga::e2 , -staDist * ga::e2
			, -staDist * ga::e2 ,  staDist * ga::e2
			,  staDist * ga::e3 , -staDist * ga::e3
			, -staDist * ga::e3 ,  staDist * ga::e3
			//
			, staDist * ga::e3
			, staDist * ga::e1
			, staDist * ga::e2
			, staDist * ga::e3 // same station to check for that
			};
#	endif

	ga::Vector const expRobust(ga::vZero);
	ga::Vector const expFit(ga::vZero);
	double const tolRobust{ smlDist };
	double const tolFit{ .5 * smlDist };

	// generate rays to aim points
	std::vector<geo::Ray> rays(aimPoints.size());
	std::transform
		( aimPoints.begin(), aimPoints.end()
		, starts.begin()
		, rays.begin()
		, []
			(ga::Vector const & aimPoint, ga::Vector const & start)
			{ return geo::Ray(start, ga::unit(aimPoint - start)); }
		);

	// exercise class
	geo::RayConvergence const bundle(rays);
	ga::Vector const gotRobust(bundle.robustPoint());
	ga::Vector const gotFit(bundle.meanNearTo(gotRobust, rejTol));

	/*
	io::out() << dat::infoString(gotRobust, "gotRobust") << std::endl;
	io::out() << dat::infoString(gotFit, "gotFit") << std::endl;
	io::out() << dat::infoString(rejTol, "rejTol") << std::endl;
	io::out() << dat::infoString(tolFit, "tolFit") << std::endl;
	*/

	// check results
	if (! gotRobust.nearlyEquals(expRobust, tolRobust))
	{
		oss << "Failure of robustPoint test" << std::endl;
		oss << dat::infoString(expRobust, "expRobust") << std::endl;
		oss << dat::infoString(gotRobust, "gotRobust") << std::endl;
	}
	if (! gotFit.nearlyEquals(expFit, tolFit))
	{
		oss << "Failure of fitPoint test" << std::endl;
		oss << dat::infoString(expFit, "expFit") << std::endl;
		oss << dat::infoString(gotFit, "gotFit") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for geo::RayConvergence
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << geo_RayConvergence_test0();
	oss << geo_RayConvergence_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
