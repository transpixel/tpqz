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
\brief  This file contains unit test for geo::intersect
*/


#include "libgeo/intersect.h"

#include "libio/sprintf.h"
#include "libio/stream.h"

#include "libga/ga.h"
#include "libga/Rigid.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
geo_intersect_test0
	()
{
	std::ostringstream oss;
	return oss.str();
}

//! Check Ray/Plane intersection
std::string
geo_intersect_test1
	()
{
	std::ostringstream oss;

//#define EasyData
#if defined (EasyData)
	ga::Vector const pSta( 0., 0., 0.);
	ga::BiVector const pDir(0., 0., 1.);
	ga::Vector const rSta( 0., 0., 3.);
	ga::Vector const anyPnt(-3., 0., 5.);
#else
	ga::Vector const pSta( 3., -2., 5.);
	ga::BiVector const pDir(.2, .4, .3);
	ga::Vector const rSta( 17., -9., 13.);
	ga::Vector const anyPnt(-7., 11., 5.);
#endif

	geo::Plane const aPlane(pSta, pDir);
	ga::Vector const expPnt(aPlane.projectionOf(anyPnt));
	ga::Vector const rDir{ expPnt - rSta };
	geo::Ray const aRay(rSta, rDir);

	ga::Vector const gotPnt{ geo::intersect::point(aRay, aPlane) };
	if (! gotPnt.nearlyEquals(expPnt))
	{
		oss << "Failure of intersection test" << std::endl;
		oss << dat::infoString(expPnt, "expPnt") << std::endl;
		oss << dat::infoString(gotPnt, "gotPnt") << std::endl;
	}

	return oss.str();
}


	ga::Vector
	dirFor
		( double const & xx
		, double const & yy
		, double const & zz
		)
	{
		return ga::unit(ga::Vector{ xx, yy, zz });
	}

//! Check Ray/Ray intersection
std::string
geo_intersect_test2
	()
{
	std::ostringstream oss;

	using PntPair = std::pair<ga::Vector, ga::Vector>;
	ga::Vector const pnt1{  1.,  1.,  1. };
	ga::Vector const pnt2{ -1., -1., -1. };

	// opposite directions
	{
		geo::Ray const rayA{ pnt1, dirFor(  1.,  1.,  1. ) };
		geo::Ray const rayB{ pnt2, dirFor( -1., -1., -1. ) };
		PntPair const gotPnts(geo::intersect::pointsFor(rayA, rayB));
		if ((gotPnts.first.isValid()) || (gotPnts.first.isValid()))
		{
			oss << "Failure of opposite direction test" << std::endl;
		}
	}
	// parallel directions
	{
		geo::Ray const rayA{ pnt1, dirFor(  1.,  1.,  1. ) };
		geo::Ray const rayB{ pnt2, dirFor(  1.,  1.,  1. ) };
		PntPair const gotPnts(geo::intersect::pointsFor(rayA, rayB));
		if ((gotPnts.first.isValid()) || (gotPnts.first.isValid()))
		{
			oss << "Failure of parallel direction test" << std::endl;
		}
	}
	// orthogonal
	{
		geo::Ray const rayA{ pnt1, dirFor( -1.,  1.,  0. ) };
		geo::Ray const rayB{ pnt2, dirFor(  1.,  1., -2. ) };
		PntPair const gotPnts(geo::intersect::pointsFor(rayA, rayB));
		if ( (! gotPnts.first.nearlyEquals(pnt1))
		  || (! gotPnts.second.nearlyEquals(pnt2))
		   )
		{
			oss << "Failure of perpendicular direction test" << std::endl;
		}
	}

	// easy case
	{
		ga::Vector const pntA{  0.,  1.,  0. };
		ga::Vector const pntB{  0., -1.,  0. };
		geo::Ray const rayA{ pntA, dirFor(  1., -1.,  0. ) };
		geo::Ray const rayB{ pntB, dirFor(  1.,  1.,  0. ) };
		ga::Vector const expPnt{  1.,  0.,  0. };

		PntPair const gotPnts(geo::intersect::pointsFor(rayA, rayB));
		ga::Vector const & gotPnt1 = gotPnts.first;
		ga::Vector const & gotPnt2 = gotPnts.second;
		if ( (! gotPnt1.nearlyEquals(expPnt))
		  || (! gotPnt2.nearlyEquals(expPnt))
		   )
		{
			ga::Vector const difPnt1{ gotPnt1 - expPnt };
			ga::Vector const difPnt2{ gotPnt2 - expPnt };
			oss << "Failure of easy case test" << std::endl;
			oss << dat::infoString(expPnt, "expPnt") << std::endl;
			oss << dat::infoString(gotPnt1, "gotPnt1") << std::endl;
			oss << dat::infoString(gotPnt2, "gotPnt2") << std::endl;
			oss << "difPnt1:"
				<< " " << io::sprintf("%12.5e", difPnt1[0])
				<< " " << io::sprintf("%12.5e", difPnt1[1])
				<< " " << io::sprintf("%12.5e", difPnt1[2])
				<< std::endl;
			oss << "difPnt2:"
				<< " " << io::sprintf("%12.5e", difPnt2[0])
				<< " " << io::sprintf("%12.5e", difPnt2[1])
				<< " " << io::sprintf("%12.5e", difPnt2[2])
				<< std::endl;
			oss << dat::infoString(difPnt2, "difPnt2") << std::endl;
		}
	}

	// test a few intersections
	std::vector<ga::Vector> const somePnts
		{ ga::Vector{ -1., -5., -3. }
		, ga::Vector{  3., -3.,  7. }
		, ga::Vector{ -5.,  1., -2. }
		, ga::Vector{  7., -9.,  7. }
		, ga::Vector{ -2., -7., -6. }
		};
	for (ga::Vector const & somePnt : somePnts)
	{
		// construct exact intersection situation
		ga::Vector const dirA{ ga::unit(somePnt - pnt1) };
		ga::Vector const dirB{ ga::unit(somePnt - pnt2) };
		geo::Ray const rayA{ pnt1, dirA };
		geo::Ray const rayB{ pnt2, dirB };

		PntPair const gotPnts(geo::intersect::pointsFor(rayA, rayB));

		ga::Vector const & gotPnt1 = gotPnts.first;
		ga::Vector const & gotPnt2 = gotPnts.second;
		ga::Vector const & expPnt = somePnt;
		if ( (! gotPnt1.nearlyEquals(expPnt))
		  || (! gotPnt2.nearlyEquals(expPnt))
		   )
		{
			oss << "Failure of general intersection test" << std::endl;
			oss << dat::infoString(expPnt, "expPnt") << std::endl;
			oss << dat::infoString(gotPnt1, "gotPnt1") << std::endl;
			oss << dat::infoString(gotPnt2, "gotPnt2") << std::endl;
			/*
			oss << dat::infoString(rayA.theStart, "rayA.theStart") << '\n';
			oss << dat::infoString(rayA.theDir, "rayA.theDir") << '\n';
			oss << dat::infoString(rayB.theStart, "rayB.theStart") << '\n';
			oss << dat::infoString(rayB.theDir, "rayB.theDir") << '\n';
			*/
		}
	}

	return oss.str();
}

	//! Compute/store useful test values
	struct Config
	{
		// tube
		ga::Vector const theP;
		ga::Vector const theA;

		// point location
		double const theRho;
		double const theMu;
		ga::Vector const theB;
		ga::Vector const theX;

		// ray
		ga::Vector const theS;
		ga::Vector const theD;

		Config
			( ga::Vector const & argP
			, ga::Vector const & argA
			, double const & argRho
			, double const & argMu
			, ga::Vector const & argB
			, ga::Vector const & argX
			, ga::Vector const & argS
			, ga::Vector const & argD
			)
			: theP{ argP }
			, theA{ argA }
			, theRho{ argRho }
			, theMu{ argMu }
			, theB{ argB }
			, theX{ argX }
			, theS{ argS }
			, theD{ argD }
		{ }

		explicit
		Config
			( ga::Vector const & pVec
			, ga::Vector const & aDir
			, double const & rho
			, double const & mu
			, ga::Vector const & bDir
			, ga::Vector const & sVec
			)
			: theP{ pVec }
			, theA{ aDir }
			, theRho{ rho }
			, theMu{ mu }
			, theB{ ga::unit(bDir) }
			, theX{ theP + theMu*theA + theRho*theB }
			, theS{ sVec }
			, theD{ ga::unit(theX - theS) }
		{
			assert(dat::isValid(theP));
			assert(dat::isValid(theA));
			assert(dat::isValid(theRho));
			assert(dat::isValid(theMu));
			assert(dat::isValid(theB));
			assert(dat::isValid(theX));
			assert(dat::isValid(theS));
			assert(dat::isValid(theD));
		}

		geo::Ray
		ray
			() const
		{
			return geo::Ray(theS, theD);
		}

		geo::Ray
		axis
			() const
		{
			return geo::Ray(theP, theA);
		}

		geo::Tube
		tube
			() const
		{
			return geo::Tube(axis(), theRho);
		}

		Config
		xformed
			( ga::Rigid const & oriOutWrtSelf
			) const
		{
			ga::Pose const pose(oriOutWrtSelf.pose());
			return Config
				( oriOutWrtSelf(theP) // loc tube-start
				, pose(theA) // dir axis
				, theRho // tube radius
				, theMu // longitudinal coord
				, pose(theB) // dir transverse
				, oriOutWrtSelf(theX) // loc pnt
				, oriOutWrtSelf(theS) // loc ray-start
				, pose(theD) // dir ray
				);
		}

		std::string
		infoString
			( std::string const & title = {}
			) const
		{
			std::ostringstream oss;
			if (! title.empty())
			{
				oss << title << std::endl;
			}
			oss << dat::infoString(theP, "theP") << '\n'
				<< dat::infoString(theA, "theA") << '\n'
				<< dat::infoString(theRho, "theRho") << '\n'
				<< dat::infoString(theMu, "theMu") << '\n'
				<< dat::infoString(theB, "theB") << '\n'
				<< dat::infoString(theX, "theX") << '\n'
				<< dat::infoString(theS, "theS") << '\n'
				<< dat::infoString(theD, "theD")
				;
			return oss.str();
		}
	};

	void
	checkPoint
		( std::ostream & oss
		, geo::Ray const & ray
		, geo::Tube const & tube
		, ga::Vector const & pnt
		, std::string const & tname
		)
	{
		if (! ray.isValid())
		{
			oss << "Failure of valid ray for test: " << tname << '\n';
		}
		if (! tube.isValid())
		{
			oss << "Failure of valid tube for test: " << tname << '\n';
		}
		ga::Vector const rejTube{ tube.theAxis.rejectionTo(pnt) };
		double const rejMagTube{ ga::magnitude(rejTube) };
		double const delTube{ std::abs(rejMagTube - tube.theRadius) };
		if (! (delTube < math::eps))
		{
			oss << "Failure of valid tube contains test: " << tname << '\n';
			oss << dat::infoString(pnt, "pnt") << std::endl;
			oss << dat::infoString(rejTube, "rejTube") << std::endl;
			oss << dat::infoString(rejMagTube, "rejMagTube") << std::endl;
			oss << dat::infoString(delTube, "delTube") << std::endl;
		}
		ga::Vector const rejRay{ ray.rejectionTo(pnt) };
		double const rejMagRay{ ga::magnitude(rejRay) };
		double const delRay{ std::abs(rejMagRay) };
		if (! (delRay < math::eps))
		{
			oss << "Failure of valid ray contains test: " << tname << '\n';
			oss << dat::infoString(pnt, "pnt") << std::endl;
			oss << dat::infoString(rejRay, "rejRay") << std::endl;
			oss << dat::infoString(delRay, "delRay") << std::endl;
		}
	}

//! Check Ray/Cylinder intersection
std::string
geo_intersect_test3
	()
{
	std::ostringstream oss;

	// define test configuration in easy frame
	ga::Vector const pVec(0., 0., 0.);
	ga::Vector const aDir(1., 0., 0.);
	ga::Vector const bVec(0., 1., 2.);
	double const rho{ 3. };
	ga::Vector const sVec(7., 2., 5.);
	double const mu{ 11. };
	Config const cfgEasy(pVec, aDir, rho, mu, bVec, sVec);

	// transform into arbitrary frame
//#define EasyData
#if defined (EasyData)
	ga::Rigid const oriArbWrtEasy{ ga::Rigid::identity() };
#else
	ga::Rigid const oriArbWrtEasy
		( ga::Vector(-137., -39., 57.)
		, ga::Pose(ga::BiVector(.5, .75, -.25))
		);
#endif
	Config const config{ cfgEasy.xformed(oriArbWrtEasy) };

	// generate test objects
	geo::Ray const ray{ config.ray() };
	geo::Tube const tube{ config.tube() };

	using PointPair = std::pair<ga::Vector, ga::Vector>;

	// check null tube intersection
	geo::Tube const nullTube{};
	PointPair const gotNullTube{ geo::intersect::pointPair(ray, nullTube) };
	if (dat::isValid(gotNullTube.first) || dat::isValid(gotNullTube.second))
	{
		oss << "Failure of ray intersect test with null tube" << std::endl;
	}

	// check null ray intersection
	geo::Ray const nullRay{};
	PointPair const gotNullRay{ geo::intersect::pointPair(nullRay, tube) };
	if (dat::isValid(gotNullRay.first) || dat::isValid(gotNullRay.second))
	{
		oss << "Failure of ray intersect test with null ray" << std::endl;
	}

	// check expected intersection
	PointPair const gotPnt{ geo::intersect::pointPair(ray, tube) };
	checkPoint(oss, ray, tube, gotPnt.first, "pnt.first");
	checkPoint(oss, ray, tube, gotPnt.second, "pnt.second");

	return oss.str();
}


}

//! Unit test for ga::intersect
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << geo_intersect_test0();
	oss << geo_intersect_test1();
	oss << geo_intersect_test2();
	oss << geo_intersect_test3();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
