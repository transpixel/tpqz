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
\brief  This file contains unit test for ga::spin
*/


#include "libga/spin.h"

#include "libdat/compare.h"
#include "libio/stream.h"
#include "libga/ga.h"
#include "libga/Pose.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
/*
std::string
ga_spin_test0
	()
{
	std::ostringstream oss;
	ga::spin const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}
*/

//! check basic functions
std::string
ga_spin_test2
	()
{
	std::ostringstream oss;

	// check that result is unit magnitude
	{
		using namespace ga;

		Vector const vecA{3., 5., 7.};
		Vector const vecB{17., -11., 13.};
		Spinor const gotSpin(spin::between(vecA, vecB));
		double const gotMag(magnitude(gotSpin));
		double const expMag(1.);
		if (! dat::nearlyEquals(gotMag, expMag))
		{
			oss << "Failure of unit magnitude test" << std::endl;
			oss << "exp: " << expMag << std::endl;
			oss << "got: " << gotMag << std::endl;
		}
	}

	// check spinor creation - simple case
	{
		using namespace ga;

		Vector const vecA(e1);
		Vector const expB(e2);
		Spinor const expSpin(sqrt(e2*e1));
		Spinor const gotSpin(spin::between(vecA, expB));
		if (! nearlyEquals(gotSpin, expSpin))
		{
			oss << "Failure of simple direction test" << std::endl;
		}
		Vector const gotB((gotSpin * vecA * gotSpin.reverse()).theV);
		if (! nearlyEquals(gotB, expB))
		{
			oss << "Failure of spin/phys magnitude test" << std::endl;
			oss << expSpin.infoString("expSpin") << std::endl;
			oss << gotSpin.infoString("gotSpin") << std::endl;
		}
	}

	// check spinor creation - special case - identity
	{
		using namespace ga;

		Spinor const expSpin(one);
		Vector const vecA(-7., 5., 11.);
		Spinor const gotSpin(spin::between(vecA, vecA));
		if (! nearlyEquals(gotSpin, expSpin))
		{
			oss << "Failure of simple identity test" << std::endl;
		}
	}

	// check spinor creation - special case - pi
	{
		using namespace ga;

		BiVector const plane(unit(BiVector{-5., 7., 2.}));
		BiVector const pAngle(math::pi * plane);
		Spinor const expSpin(exp(.5 * pAngle));
		Vector const tmpA{3., 5., 7.};
		// project tmpA into the plane
		Vector const vecA((.5 * (tmpA * plane - plane * tmpA)).theV);
		Vector const expB((expSpin * vecA * expSpin.reverse()).theV);

		// check that result is undefined w/o additional information
		Spinor const gotSpin1(spin::between(vecA, expB));
		if (gotSpin1.isValid())
		{
			Spinor const unitAB(unit(vecA * expB));
			oss << "Failure of UNresolved pi-rotation test" << std::endl;
			oss << expSpin.infoString("expSpin") << std::endl;
			oss << gotSpin1.infoString("gotSpin1") << std::endl;
			oss << vecA.infoString("vecA") << std::endl;
			oss << expB.infoString("expB") << std::endl;
			oss << unitAB.infoString("unitAB") << std::endl;
		}

		// check disambiguation with conditional plane
		Spinor const gotSpin2(spin::between(vecA, expB, plane));
		if (! gotSpin2.isValid())
		{
			oss << "Failure of resolved pi-rotation valid test" << std::endl;
		}
		else
		{
			// confirm pi rotation effect
			Vector const gotB((gotSpin2 * vecA * gotSpin2.reverse()).theV);
			if (! nearlyEquals(gotB, expB))
			{
				oss << "Failure of resolved pi-rotation test" << std::endl;
				oss << expB.infoString("expB") << std::endl;
				oss << gotB.infoString("gotB") << std::endl;
			}
		}
	}

	return oss.str();
}

//! check rotation of projected vectors
std::string
ga_spin_test3
	()
{
	std::ostringstream oss;

	// check special cases
	{
		using namespace ga;

		// generate vector perpendicular to plane
		BiVector const plane(BiVector{5., -7., 2.});
		Vector const norm(E123 * plane);

		// generate two vectors in the plane
		Vector const tan1(3.*anyPerpendicularDir(norm));
		Vector const tan2((.25*tan1 * unit(plane)).theV);

		// no solution (one vector perpendicular to plane
		Spinor const gotZero1(spin::betweenProjectionsOnto(norm, tan1, plane));
		Spinor const gotZero2(spin::betweenProjectionsOnto(tan2, norm, plane));
		if ((gotZero1.isValid()) || (gotZero2.isValid()))
		{
			oss << "Failure of zero projection test" << std::endl;
			oss << gotZero1.infoString("gotZero1") << std::endl;
			oss << gotZero2.infoString("gotZero2") << std::endl;
		}

		Spinor const got12(spin::betweenProjectionsOnto(tan1, tan2, plane));
		Spinor const exp12(sqrt(unit(tan2 * tan1)));
		if (! nearlyEquals(got12, exp12))
		{
			oss << "Failure of tangent projection test" << std::endl;
			oss << exp12.infoString("exp12") << std::endl;
			oss << got12.infoString("got12") << std::endl;
		}
	}

	return oss.str();
}

//! check estimation of rotation matrices
std::string
ga_spin_test4
	()
{
	std::ostringstream oss;

	{
		using namespace ga;

		// utilize orthonormal triad at arbitrary rotation
		Vector const vec_xInRef(e1);
		Vector const vec_yInRef(e2);
		Vector const vec_zInRef(e3);

		double const aMag1(.625 * math::pi);
		double const aMag2(.250 * math::pi);

		// rotate by first angle
		BiVector const expAngle1(aMag1 * unit(BiVector{.5, -.7, .3}));

		Pose const poseAwR(expAngle1);
		Vector const vec_xInA(poseAwR(vec_xInRef));
		Vector const vec_yInA(poseAwR(vec_yInRef));
		Vector const vec_zInA(poseAwR(vec_zInRef));

		// rotate by second angle
		BiVector const plane2(E123 * vec_xInA);
		BiVector const expAngle2(aMag2 * plane2);

		Pose const poseBwA(expAngle2);
		Vector const vec_xInB(poseBwA(vec_xInA));
		Vector const vec_yInB(poseBwA(vec_yInA));
		Vector const vec_zInB(poseBwA(vec_zInA));

		Pose const poseBwR(poseBwA * poseAwR);
		Spinor const expSpinNet(exp(.5 * poseBwR.physicalAngle()));

		// verify test setup code
		{
			Vector const chk_xInB(poseBwR(vec_xInRef));
			Vector const chk_yInB(poseBwR(vec_yInRef));
			Vector const chk_zInB(poseBwR(vec_zInRef));
			if ( (! nearlyEquals(chk_xInB, vec_xInB))
			  || (! nearlyEquals(chk_yInB, vec_yInB))
			  || (! nearlyEquals(chk_zInB, vec_zInB))
			   )
			{
				oss << "Test setup failure" << std::endl;
				io::err() << "Test setup failure" << std::endl;
				assert(! "bad test code");
			}
		}

		// retrieve fit spinor
//Spinor const gotSpinNet(

bool const debug(false);
if (debug)
{
io::out() << std::endl;
io::out() << "vec:.." << std::endl;
io::out() << vec_xInRef.infoString("xInRef") << std::endl;
io::out() << vec_yInRef.infoString("yInRef") << std::endl;
io::out() << vec_zInRef.infoString("zInRef") << std::endl;
io::out() << "vec:.." << std::endl;
io::out() << vec_xInA.infoString("xInA") << std::endl;
io::out() << vec_yInA.infoString("yInA") << std::endl;
io::out() << vec_zInA.infoString("zInA") << std::endl;
io::out() << "vec:.." << std::endl;
io::out() << vec_xInB.infoString("xInB") << std::endl;
io::out() << vec_yInB.infoString("yInB") << std::endl;
io::out() << vec_zInB.infoString("zInB") << std::endl;
io::out() << std::endl;
}

		// Frames:
		//  Ref: Starting frame
		//    A: first rotation
		//    B: second rotation

		// check recovery of first rotation
		Spinor const gotSpin1(spin::between(vec_xInRef, vec_xInA));
		Spinor const & s1 = gotSpin1;
		Spinor const s1r = s1.reverse();
		Vector const got_xInA((s1 * vec_xInRef * s1r).theV);
		Vector const got_yInA((s1 * vec_yInRef * s1r).theV);
		Vector const got_zInA((s1 * vec_zInRef * s1r).theV);
		if (! nearlyEquals(got_xInA, vec_xInA))
		{
			oss << "Failure of step 1 spinor recovery test" << std::endl;
			oss << vec_xInA.infoString("vec_xInA") << std::endl;
			oss << got_xInA.infoString("got_xInA") << std::endl;
		}

		// check recover of second rotation
		BiVector const plane1(E123 * vec_xInA);
		Spinor const gotSpin2
			(spin::betweenProjectionsOnto(got_yInA, vec_yInB, plane1));
		Spinor const & s2 = gotSpin2;
		Spinor const s2r = s2.reverse();
		Vector const got_xInB((s2 * got_xInA * s2r).theV);
		Vector const got_yInB((s2 * got_yInA * s2r).theV);
		Vector const got_zInB((s2 * got_zInA * s2r).theV);
		bool const same_xInB(nearlyEquals(got_xInB, vec_xInB));
		bool const same_yInB(nearlyEquals(got_yInB, vec_yInB));
		bool const same_zInB(nearlyEquals(got_zInB, vec_zInB));
		if (! (same_xInB && same_yInB && same_zInB))
		{
			oss << "Failure of step 2 spinor recovery test" << std::endl;
			oss << std::boolalpha
				<< "same_xInB: " << same_xInB << '\n'
				<< "same_yInB: " << same_yInB << '\n'
				<< "same_zInB: " << same_zInB << '\n'
				;
		}

if (debug)
{
BiVector const gotAngle1(ga::spin::argAngleFrom(gotSpin1));

io::out() << gotSpin1.infoString("gotSpin1") << std::endl;
io::out() << gotAngle1.infoString("gotAngle1") << std::endl;
io::out() << "got:.." << std::endl;
io::out() << got_xInA.infoString("xInA") << std::endl;
io::out() << got_yInA.infoString("yInA") << std::endl;
io::out() << got_zInA.infoString("zInA") << std::endl;
io::out() << "got:.." << std::endl;
io::out() << got_xInB.infoString("xInB") << std::endl;
io::out() << got_yInB.infoString("yInB") << std::endl;
io::out() << got_zInB.infoString("zInB") << std::endl;
}

		Spinor const gotSpinNetTmp(gotSpin2 * gotSpin1);
		double const tol(2. * math::eps); // ?
		if (! nearlyEquals(gotSpinNetTmp, expSpinNet, tol))
		{
			Spinor const diff(gotSpinNetTmp * expSpinNet.reverse());
			oss << "Failure of 2-step spinor recovery test" << std::endl;
			oss << expSpinNet.infoString("expSpinNet") << std::endl;
			oss << gotSpinNetTmp.infoString("gotSpinNetTmp") << std::endl;
			oss << "diff: " << '\n'
				<< std::fixed << std::setprecision(18)
				<< " " << diff.theS.theValue << '\n'
				<< " " << diff.theB[0] << '\n'
				<< " " << diff.theB[1] << '\n'
				<< " " << diff.theB[2] << '\n'
				<< std::endl;
		}


	}

	return oss.str();
}

//! check spinor convention and interpretation
std::string
ga_spin_test5
	()
{
	std::ostringstream oss;

	ga::Vector const aFrom{ ga::e1 };
	ga::Vector const bInto{ ga::e2 };

	// create spinor via (root of) geometric product
	ga::Spinor const spinSq(aFrom * bInto);
	ga::Spinor const spinor(ga::sqrt(spinSq));

	// check active interpretation
	ga::Vector const bGot{ (spinor.reverse() * aFrom * spinor).theV };
	if (! bGot.nearlyEquals(bInto))
	{
		oss << "Failure of spinor rotate test" << std::endl;
		oss << dat::infoString(bInto, "expInto") << std::endl;
		oss << dat::infoString(bGot, "gotInto") << std::endl;
	}

	ga::BiVector const expPhys{ math::qtrTurn * ga::E12 };
	ga::BiVector const gotPhys{ ga::spin::physicalAngleFrom(spinor) };
	if (! gotPhys.nearlyEquals(expPhys))
	{
		oss << "Failure of physicalAngleFrom test" << std::endl;
		oss << dat::infoString(expPhys, "expPhys") << std::endl;
		oss << dat::infoString(gotPhys, "gotPhys") << std::endl;
	}

	ga::BiVector const expSpin{ .5 * expPhys };
	ga::BiVector const gotSpin{ ga::spin::argAngleFrom(spinor) };
	if (! gotSpin.nearlyEquals(expSpin))
	{
		oss << "Failure of argAngleFrom test" << std::endl;
		oss << dat::infoString(expSpin, "expSpin") << std::endl;
		oss << dat::infoString(gotSpin, "gotSpin") << std::endl;
	}

	ga::Spinor const gotSpinor{ ga::exp(expSpin) };
	if (! gotSpinor.nearlyEquals(spinor))
	{
		oss << "Failure of spinor reconstruction test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for ga::spin
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
//	oss << ga_spin_test0();
	oss << ga_spin_test2();
	oss << ga_spin_test3();
	oss << ga_spin_test4();
	oss << ga_spin_test5();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
