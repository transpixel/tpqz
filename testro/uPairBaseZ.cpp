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
\brief  This file contains unit test for ro::PairBaseZ
*/


#include "libro/PairBaseZ.h"

#include "libdat/array.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libga/spin.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libmath/NumDeriv.h"
#include "libro/PairRel.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
ro_PairBaseZ_test0
	()
{
	std::ostringstream oss;
	ro::PairBaseZ const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

	//! Test case configuration
	struct ModelParms
	{
		ga::Vector const theLoc1wFit;
		ga::BiVector const theBiv1wFit;
		ga::Vector const theLoc2wFit;
		ga::BiVector const theBiv2wFit;

		explicit
		ModelParms
			( double const & halfBase
			, double const & phi1
			, double const & phi2
			, double const & alpha3
			, double const & theta1
			, double const & theta2
			)
			: theLoc1wFit{ -halfBase * ga::e3 }
			, theBiv1wFit(phi1, phi2, -alpha3)
			, theLoc2wFit{  halfBase * ga::e3 }
			, theBiv2wFit(theta1, theta2,  alpha3)
		{ }

		ro::OriPair
		roInFit
			() const
		{
			ga::Rigid const ori1wFit{ theLoc1wFit, ga::Pose(theBiv1wFit) };
			ga::Rigid const ori2wFit{ theLoc2wFit, ga::Pose(theBiv2wFit) };
			return { ori1wFit, ori2wFit };
		}

		ro::OriPair
		roPair
			( double const & azimFitWrtTmp = 0.
			, ga::Vector const & locTmpInRef = ga::Vector( 100., 200., 300.)
			, ga::BiVector const & bivTmpInRef = ga::bZero
			) const
		{
			// transform test orientations to expression w.r.t. Ref frame
			ro::OriPair const ori12wFit{ roInFit() };
			ga::Rigid const & ori1wFit = ori12wFit.first;
			ga::Rigid const & ori2wFit = ori12wFit.second;

			// configure (Fit w.r.t. Tmp) and (Tmp w.r.t. Ref)
			ga::Vector const locFitInTmp{ ga::vZero };
			ga::BiVector const bivFitInTmp{ azimFitWrtTmp * ga::E12 };
			ga::Rigid const oriFitWrtTmp{ locFitInTmp, ga::Pose(bivFitInTmp) };
			ga::Rigid const oriTmpWrtRef{ locTmpInRef, ga::Pose(bivTmpInRef) };

			ga::Rigid const ori1wRef{ ori1wFit * oriFitWrtTmp * oriTmpWrtRef};
			ga::Rigid const ori2wRef{ ori2wFit * oriFitWrtTmp * oriTmpWrtRef};
			return { ori1wRef, ori2wRef };
		}
	};


void
checkPair
	( std::ostream & oss
	, ro::OriPair const & roInRef
	)
{
	// access and verify test case inputs
	ga::Rigid const & exp1wRef = roInRef.first;
	ga::Rigid const & exp2wRef = roInRef.second;
	assert(exp1wRef.isValid());
	assert(exp2wRef.isValid());
	assert(math::eps < ro::gapBetween(roInRef)); // verify simulation

	// construct a symmetric pair
	ro::PairBaseZ const gotRoInFit(exp1wRef, exp2wRef);

	// check if ctor worked (should always be possible)
	if (! gotRoInFit.isValid())
	{
		oss << "Failure of construction validity test" << std::endl;
	}
	else
	{
		// retrieve computed orientations
		ga::Rigid const got1wFit{ gotRoInFit.rigid1w0() };
		ga::Rigid const got2wFit{ gotRoInFit.rigid2w0() };
		ga::Rigid const gotFitWrtRef{ gotRoInFit.rigid0wRef() };

		// check if azimuth angles satisfy anti-symmetry condition
		ga::BiVector const biv1WrtFit{ got1wFit.pose().physicalAngle() };
		ga::BiVector const biv2WrtFit{ got2wFit.pose().physicalAngle() };
		double const & alpha1 = biv1WrtFit[2];
		double const & alpha2 = biv2WrtFit[2];
		double const alphaSum{ alpha2 + alpha1 };
		constexpr double tolAlpha{ math::eps };
		if (! dat::nearlyEquals(alphaSum, 0., tolAlpha))
		{
			oss << "Failure of antisymmetry test" << std::endl;
			oss << dat::infoString(alpha1, "alpha1") << std::endl;
			oss << dat::infoString(alpha2, "alpha2") << std::endl;
			oss << dat::infoString(alphaSum, "alphaSum") << std::endl;
		}

		// check agreement with source
		ga::Rigid const got1wRef{ got1wFit * gotFitWrtRef };
		ga::Rigid const got2wRef{ got2wFit * gotFitWrtRef };
		constexpr double tolSta{ 1.e6*math::eps };
		constexpr double tolAtt{ 1.e6*math::eps };
		if (! got1wRef.nearlyEquals(exp1wRef, tolSta, tolAtt))
		{
			oss << "Failure of ori1 test" << std::endl;
			oss << dat::infoString(exp1wRef, "exp1wRef") << std::endl;
			oss << dat::infoString(got1wRef, "got1wRef") << std::endl;
		}
		if (! got2wRef.nearlyEquals(exp2wRef, tolSta, tolAtt))
		{
			ga::Rigid const oriDiff{ exp2wRef * exp2wRef.inverse() };
			oss << "Failure of ori2 test" << std::endl;
			oss << dat::infoString(exp2wRef, "exp2wRef") << std::endl;
			oss << dat::infoString(got2wRef, "got2wRef") << std::endl;
			oss << dat::infoString(oriDiff, "oriDiff") << std::endl;
		}
	}
}

//! Check defining properties
std::string
ro_PairBaseZ_test1
	()
{
	std::ostringstream oss;

	// attempt to construct an invalid case
	ga::Vector const sameLoc( -5., 2., 3.);
	ga::Rigid const bad1wRef(sameLoc, ga::Pose(ga::BiVector(1., 2., 3.)));
	ga::Rigid const bad2wRef(sameLoc, ga::Pose(ga::BiVector(-3., 4., 2.)));
	ro::OriPair const badPair{ bad1wRef, bad2wRef };
	ro::PairBaseZ const gotBad(bad1wRef, bad2wRef);
	if ( gotBad.isValid())
	{
		oss << "Failure of zero baseline bad-ctor test" << std::endl;
	}

	// simulate an easy case
	{
		double const halfBase{ 1. };
		double const alpha3{ 0. };
		ModelParms const modParms(halfBase, 0., 0., alpha3, 0., 0.);
		checkPair(oss, modParms.roPair());
	}

	// simulate a general case
	{
		double const halfBase{ 4. };
		double const alpha3{ 2.7 };
		ModelParms const modParms(halfBase, .2, .3, alpha3, .5, .7);
		double const azimFwT{ 2.5 };
		checkPair(oss, modParms.roPair(azimFwT));
	}

	// simulate a stress case - ref baseline already aligned
	{
		double const halfBase{ 4. };
		double const alpha3{ 2.7 };
		ModelParms const modParms(halfBase, .2, .3, alpha3, .5, .7);
		double const azimFwT{ 2.5 };
		checkPair(oss, modParms.roPair(azimFwT,  ga::e3));
		checkPair(oss, modParms.roPair(azimFwT, -ga::e3));
	}

	// simulate a stress case - pi/2 rotations
	{
		double const halfBase{ 4. };
		double const alpha3{ 2.7 };
		constexpr double hp{ math::halfPi };
		{
			ModelParms const modParms(halfBase, hp, 0., alpha3, 0., 0.);
			checkPair(oss, modParms.roPair());
		}
		{
			ModelParms const modParms(halfBase, hp, 0., alpha3, hp, 0.);
			checkPair(oss, modParms.roPair());
		}
		{
			ModelParms const modParms(halfBase, 0., hp, alpha3, 0., hp);
			checkPair(oss, modParms.roPair());
		}
		{
			constexpr double rp{ hp/std::sqrt(2.) };
			ModelParms const modParms(halfBase, rp, rp, alpha3, rp, rp);
			checkPair(oss, modParms.roPair());
		}
	}

	return oss.str();
}


//! Check differentiation
std::string
ro_PairBaseZ_test2
	()
{
	std::ostringstream oss;

	// test case configuration
//#define EasyData
#	if defined(EasyData)
	ga::Vector const nomPnt(0., .5, 0.);
	double const xx{ .5*math::qtrTurn };
	std::array<double, 5u> const vals
		{{ 0., 0.   // phi1,2
		 , 0., 0.   // theta1,2
		 , xx       // alpha3
		}};
#	else
	ga::Vector const nomPnt(.25, .125, .55);
	std::array<double, 5u> const vals
		{{ 1.2, 1.3   // phi1,2
		 , 2.1, 2.2   // theta1,2
		 , .50        // alpha3
		}};
#	endif

	// construct object with nominal values
	ro::ArgsBaseZ const nomArgs(vals);
	ro::PairBaseZ const nomPair(nomArgs);
	std::pair<ga::Vector, ga::Vector> const uvNoms
		{ nomPair.uvDirectionsFor(nomPnt) };

	// get computed values
	std::array<double, 5u> const gotdfdpxs(nomPair.jacobianRow(uvNoms));

	// perturbation for numerical differentation
	constexpr double dp{ std::sqrt(math::eps) };
	constexpr double perRun{ 1. / (2. * dp) };
	constexpr double tol{ dp };

	// compute phi perturbations
	std::array<double, 5u> expdfdqs;
	for (size_t ndx{0u} ; ndx < 5u ; ++ndx)
	{
		std::array<double, 5u> del{{ 0., 0., 0., 0., 0. }};
		del[ndx] = dp;

		// evaluate args at perturbed values
		using dat::operator-;
		using dat::operator+;
		ro::ArgsBaseZ const nArgs(vals - del);
		ro::ArgsBaseZ const pArgs(vals + del);
		ro::PairBaseZ const nPair(nArgs);
		ro::PairBaseZ const pPair(pArgs);

		// expected value df/dp1
		double const nFunc{ nPair.tripleProductGap(uvNoms) };
		double const pFunc{ pPair.tripleProductGap(uvNoms) };
		double const expdfdpx{ perRun * (pFunc - nFunc) };
		double const & gotdfdpx = gotdfdpxs[ndx];
		expdfdqs[ndx] = expdfdpx;

		/*
		io::out()
			<< "ndx,{exp,got}dfdpx"
			<< " " << dat::infoString(ndx)
			<< " " << dat::infoString(expdfdpx)
			<< " " << dat::infoString(gotdfdpx)
			<< std::endl;
		*/

		if (! dat::nearlyEquals(gotdfdpx, expdfdpx, tol))
		{
			double const diff{ gotdfdpx - expdfdpx };
			oss << "Failure of partial test: ndx = " << ndx << std::endl;
			oss << dat::infoString(expdfdpx, "expdfdpx") << std::endl;
			oss << dat::infoString(gotdfdpx, "gotdfdpx") << std::endl;
			oss << "dif: " << io::sprintf("%15.6e", diff) << std::endl;
		}

	}

	/*
	{
	static std::string const fmt{ "%15.12f" };
	io::out() << std::endl;
	io::out() << dat::infoString(expdfdqs, "expdfdqs") << std::endl;
	io::out() << "expdP1: " << io::sprintf(fmt, expdfdqs[0]) << std::endl;
	io::out() << "expdP2: " << io::sprintf(fmt, expdfdqs[1]) << std::endl;
	io::out() << "expdA3: " << io::sprintf(fmt, expdfdqs[2]) << std::endl;
	io::out() << "expdP1: " << io::sprintf(fmt, expdfdqs[3]) << std::endl;
	io::out() << "expdP2: " << io::sprintf(fmt, expdfdqs[4]) << std::endl;
	io::out() << std::endl;
	}
	*/

	return oss.str();
}

	/*
	//! Generate a spectrum of corresponding direction pairs
	std::vector<ro::PairUV>
	simPairUVs
		( ro::PairBaseZ const & ro
		)
	{
		std::vector<ro::PairUV> uvPairs;
		uvPairs.reserve(3u * 3u * 3u);
		for (int n1{-1} ; n1 < 2 ; ++n1)
		for (int n2{-1} ; n2 < 2 ; ++n2)
		for (int n3{-1} ; n3 < 2 ; ++n3)
		{
			constexpr double dist{ 1.25 }; // stay away from stations
			ga::Vector const pnt
				{ dist*double(n1)*ga::e1
				+ dist*double(n2)*ga::e2
				+ dist*double(n3)*ga::e3
				};
			ro::PairUV const uvPair{ ro.uvDirectionsFor(pnt) };
			uvPairs.emplace_back(uvPair);
		}
		return uvPairs;
	}
	*/

	/*
	// True if RO agrees with all uvPairs
	bool
	checkTriple
		( ro::Pair const & ro
		, std::vector<ro::PairUV> const & uvPairs
		)
	{
		bool okay{ true };
		for (ro::PairUV const & uvPair : uvPairs)
		{
			double const gap{ ro.tripleProductGap(uvPair) };
			if (! dat::nearlyEquals(gap, 0.))
			{
				okay = false;
				break;
			}
		}
		return okay;
	}
	*/

//! Check multiple solutions
std::string
ro_PairBaseZ_test3
	()
{
	std::ostringstream oss;

	/*
	// create an arbitrary configuration
	std::array<double, 5u> const vals
		{{ 1.2, 1.3   // phi1,2
		 , 2.1, 2.2   // theta1,2
		 , .50        // alpha3
		}};

	// get ref angles
	ro::ArgsBaseZ const args0(vals);
	ga::BiVector const p0{ args0.phiBiv() };
	ga::BiVector const t0{ args0.thetaBiv() };

	// get reference directions
	ro::PairBaseZ const ro0(args0);
	std::vector<ro::PairUV> const uvPairs{ simPairUVs(ro0) };
	*/

	return oss.str();
}

}

//! Unit test for ro::PairBaseZ
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ro_PairBaseZ_test0();
	oss << ro_PairBaseZ_test1();
	oss << ro_PairBaseZ_test2();
	oss << ro_PairBaseZ_test3();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
