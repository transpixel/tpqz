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

/*! \file
\brief  This file contains unit test for ga::Pose
*/


#include "libga/Pose.h"

#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
ga_Pose_test0
	()
{
	std::ostringstream oss;
	ga::Pose const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check basic operations
std::string
ga_Pose_test1
	()
{
	std::ostringstream oss;

	// check basic ctor conventions
	{
		using namespace ga;

		// setup simple rotation case
		Vector const & aa = e1;
		Vector const & bb = e2;
		Spinor const spin(sqrt(aa * bb));

		// NOTE: expectation based on PASSIVE (frame motion) convention
		Vector const invec(3.*aa - 2.*bb);
		Vector const expvec((spin * invec * spin.reverse()).theV);

		// check angle constructor
		{
			BiVector const pAngle(2. * spin::argAngleFrom(spin));
			Pose const gotPose(pAngle);
			Vector const gotvec(gotPose(invec));
			if (! nearlyEquals(gotvec, expvec))
			{
				oss << "Failure of angle ctor test" << std::endl;
				oss << expvec.infoString("expvec") << std::endl;
				oss << gotvec.infoString("gotvec") << std::endl;
				oss << pAngle.infoString("pAngle") << std::endl;
			}
		}

		// check spin constructor
		{
			Pose const gotPose(spin);
			Vector const gotvec(gotPose(invec));
			if (! nearlyEquals(gotvec, expvec))
			{
				oss << "Failure of angle ctor test" << std::endl;
				oss << expvec.infoString("expvec") << std::endl;
				oss << gotvec.infoString("gotvec") << std::endl;
			}
		}

	}
	return oss.str();
}

//! Check basic operations
std::string
ga_Pose_test2
	()
{
	std::ostringstream oss;

	// check constructors
	{
		using namespace ga;

		BiVector const pAngle(-123., 456., -352);
		Spinor const spin(exp(.5 * pAngle));
		Pose const p1(pAngle);
		Pose const p2(spin);
		if (! nearlyEquals(p1, p2))
		{
			oss << "Failure of 1,2 construction test" << std::endl;
		}
	}
	return oss.str();
}

//! Check basic operations
std::string
ga_Pose_test3
	()
{
	std::ostringstream oss;

	// check pose magnitude and direction
	{
		using namespace ga;

		Pose const qtrTurn(math::halfPi * E12);
		Vector const expdir(-e2);
		Vector const gotdir(qtrTurn(e1));
		if (! nearlyEquals(gotdir, expdir))
		{
			oss << "Failure of magnitude/direction test" << std::endl;
			oss << expdir.infoString("expdir") << std::endl;
			oss << gotdir.infoString("expdir") << std::endl;
		}
	}
	return oss.str();
}

//! Check basic operations
std::string
ga_Pose_test4
	()
{
	std::ostringstream oss;

	// simple geometric tests
	{
		using namespace ga;

		// setup test data
		Vector const expA(-5., 3., -7.);
		BiVector const expPhysAngle(-.125, 1.5, .125);

		// generate rotation using explicit algebra
		Spinor const spin(exp(.5 * expPhysAngle));
		Vector const expB( (spin * expA * spin.reverse()).theV );

		// compute forward transform and check results
		Pose const attBwA(expPhysAngle);
		Vector const gotB(attBwA(expA));
		if (! nearlyEquals(gotB, expB))
		{
			oss << "Failure of rotation test" << std::endl;
			oss << expB.infoString("expB") << std::endl;
			oss << gotB.infoString("gotB") << std::endl;
		}

		// compute inverse and check results
		Pose const attAwB(attBwA.inverse());
		Vector const gotA(attAwB(expB));
		if (! nearlyEquals(gotA, expA))
		{
			oss << "Failure of inverse test" << std::endl;
			oss << expA.infoString("expA") << std::endl;
			oss << gotA.infoString("gotA") << std::endl;
		}
	}
	return oss.str();
}

//! Check basic operations
std::string
ga_Pose_test5
	()
{
	std::ostringstream oss;

	// composition test
	{
		using namespace ga;

		Pose const attBwA(BiVector(-.2,  .8, -.5));
		Pose const attCwB(BiVector( .5,  .4, -.7));
		Pose const gotAttCwA(attCwB * attBwA);

		Vector const expA(-23., 19., 11.);
		Vector const tmpB(attBwA(expA));
		Vector const expC(attCwB(tmpB));
		Vector const gotC(gotAttCwA(expA));

		if (! nearlyEquals(gotC, expC))
		{
			oss << "Failure of composition test" << std::endl;
		}
	}

	return oss.str();
}

//! Check basic operations
std::string
ga_Pose_test6
	()
{
	std::ostringstream oss;

	// check bivector rotation
	{
		using namespace ga;

		Vector const veca1(2., -1.,  3.);
		Vector const vecb1(-5., 7., 11.);
		Pose const pose(BiVector{3., 5., 2.});
		Vector const veca2(pose(veca1));
		Vector const vecb2(pose(vecb1));

		BiVector const bivc1(wedge(veca1, vecb1));
		BiVector const expc2(wedge(veca2, vecb2));
		BiVector const gotc2(pose(bivc1));
		if (! nearlyEquals(gotc2, expc2))
		{
			oss << "Failure of bivector rotation test" << std::endl;
			oss << expc2.infoString("expc2") << std::endl;
			oss << gotc2.infoString("gotc2") << std::endl;
		}
	}

	// spinor components operation
	{
		using namespace ga;

		BiVector const expPhysBiv(-123., 456., -352);
		Spinor const expSpin(exp(.5 * expPhysBiv));

		Pose const pose(expPhysBiv);
		std::array<double, 4u> const comps(pose.spinComponents());

		Spinor const gotSpin(comps[0], BiVector(comps[1], comps[2], comps[3]));
		if (! gotSpin.nearlyEquals(expSpin))
		{
			oss << "Failure of spinComponents test" << std::endl;
			oss << dat::infoString(expSpin, "expSpin") << std::endl;
			oss << dat::infoString(gotSpin, "gotSpin") << std::endl;
		}
	}

	return oss.str();
}


//! Check basic angle recovery conditions
std::string
ga_Pose_test7
	()
{
	std::ostringstream oss;

	// values within all quadrants and slightly beyond
	std::vector<double> const compVals
		{ -8., -6., -4., -2., -1., 0., 1., 2., 4., 6., 8. };

	for (double const & comp1 : compVals)
	for (double const & comp2 : compVals)
	for (double const & comp3 : compVals)
	{
		// construct pose (including with vector outside of ranges
		ga::BiVector const biv(comp1, comp2, comp3);
		ga::Pose const pose(biv);

		// check that spinor is unitary
		double const spinMag{ ga::magnitude(pose.theSpin) };
		if (! dat::nearlyEquals(spinMag, 1.))
		{
			oss << "Failure of pose, spinMag test" << std::endl;
			goto EndTest;
		}

		ga::BiVector const gotPhysAngle{ pose.physicalAngle() };

		// check magnitude less than pi (direction change handles larger)
		double const gotMag{ ga::magnitude(gotPhysAngle) };
		if (! (gotMag <= math::pi))
		{
			oss << "Failure of physicalAngle mag test" << std::endl;
			oss << dat::infoString(gotPhysAngle, "gotPhysAngle") << std::endl;
			oss << dat::infoString(gotMag, "gotMag") << std::endl;
			goto EndTest;
		}
	}
EndTest:


	return oss.str();
}

}

//! Unit test for ga::Pose
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ga_Pose_test0();
	oss << ga_Pose_test1();
	oss << ga_Pose_test2();
	oss << ga_Pose_test3();
	oss << ga_Pose_test4();
	oss << ga_Pose_test5();
	oss << ga_Pose_test6();
	oss << ga_Pose_test7();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
