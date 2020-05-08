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
\brief  This file contains unit test for ga::functions
*/


#include "libga/ga.h"

#include "libio/stream.h"
#include "libmath/math.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check fundamental functions
std::string
ga_functions_test0
	()
{
	std::ostringstream oss;

	{
		using namespace ga;

		Vector const tmp(-7., 5., 11.);
		Vector const dir(unit(tmp));
		Scalar const expMag(2.);
		Vector const vec(expMag * dir);

		// check inner product
		Scalar const gotMag(dot(vec, dir));
		if (! gotMag.nearlyEquals(expMag))
		{
			oss << "Failure of dot product test" << std::endl;
			oss << expMag.infoString("exp") << std::endl;
			oss << gotMag.infoString("got") << std::endl;
		}

		// check outer product
		BiVector const exp23(1., 0., 0.);
		BiVector const exp31(0., 1., 0.);
		BiVector const exp12(0., 0., 1.);
		BiVector const got23(wedge(e2, e3));
		BiVector const got31(wedge(e3, e1));
		BiVector const got12(wedge(e1, e2));
		if ( (! got23.nearlyEquals(exp23))
		  || (! got31.nearlyEquals(exp31))
		  || (! got12.nearlyEquals(exp12))
		   )
		{
			oss << "Failure of wedge product test" << std::endl;
			oss << exp23.infoString("exp23") << std::endl;
			oss << exp31.infoString("exp31") << std::endl;
			oss << exp12.infoString("exp12") << std::endl;
			oss << got23.infoString("got23") << std::endl;
			oss << got31.infoString("got31") << std::endl;
			oss << got12.infoString("got12") << std::endl;
		}
	}

	return oss.str();
}


//! Check magnitude functions
std::string
ga_functions_test0a
	()
{
	std::ostringstream oss;

	ga::Scalar const expS(-7.);
	ga::Vector const expV(-1., 3., 5.);
	ga::BiVector const expB(7., 11., 13.);
	ga::TriVector const expT(-3.);

	// check type magnitudes
	{
		using namespace ga;

		double const gotMagS(magnitude(expS));
		double const gotMagV(magnitude(expV));
		double const gotMagB(magnitude(expB));
		double const gotMagT(magnitude(expT));
		double const expMagS(std::sqrt((expS * expS.reverse()).theValue));
		double const expMagV(std::sqrt((expV * expV.reverse()).theS.theValue));
		double const expMagB(std::sqrt((expB * expB.reverse()).theS.theValue));
		double const expMagT(std::sqrt((expT * expT.reverse()).theValue));
		if (! dat::nearlyEquals(gotMagS, expMagS))
		{
			oss << "Failure of scalar magnitude test" << std::endl;
		}
		if (! dat::nearlyEquals(gotMagV, expMagV))
		{
			oss << "Failure of vector magnitude test" << std::endl;
		}
		if (! dat::nearlyEquals(gotMagB, expMagB))
		{
			oss << "Failure of bivector magnitude test" << std::endl;
		}
		if (! dat::nearlyEquals(gotMagT, expMagT))
		{
			oss << "Failure of trivector magnitude test" << std::endl;
		}
	}

	// check unit values
	{
		using namespace ga;

		ga::Scalar const gotDirS(unit(expS));
		ga::Vector const gotDirV(unit(expV));
		ga::BiVector const gotDirB(unit(expB));
		ga::TriVector const gotDirT(unit(expT));
		if (! dat::nearlyEquals(magnitude(gotDirS), 1.))
		{
			oss << "Failure of scalar dir/mag test" << std::endl;
		}
		if (! dat::nearlyEquals(magnitude(gotDirV), 1.))
		{
			oss << "Failure of vector dir/mag test" << std::endl;
			oss << gotDirV.infoString("gotDirV") << std::endl;
		}
		if (! dat::nearlyEquals(magnitude(gotDirB), 1.))
		{
			oss << "Failure of bivector dir/mag test" << std::endl;
		}
		if (! dat::nearlyEquals(magnitude(gotDirT), 1.))
		{
			oss << "Failure of trivector dir/mag test" << std::endl;
		}
	}

	return oss.str();
}


//! Check simple vector operations
std::string
ga_functions_test1
	()
{
	std::ostringstream oss;

	// check projection/rejection
	{
		using namespace ga;

		Vector const expvec(17., 11., -3.);
		Vector const somedir(unit(Vector(-5., 7., 2.)));
		Vector const pro(projectionOnto(expvec, somedir));
		Vector const rej(rejectionFrom(expvec, somedir));

		// projection should be idempotent
		Vector const pro2(projectionOnto(pro, somedir));
		if (! pro2.nearlyEquals(pro))
		{
			oss << "Failure of projection idempotence test" << std::endl;
		}

		// check if projection is parallel
		if (! areParallel(pro, somedir))
		{
			oss << "Failure of parallel projection test" << std::endl;
		}

		// check if rejection is orthogonal
		if (! areOrthogonal(rej, somedir))
		{
			Vector const unitrej(unit(rej));
			oss << "Failure of orthogonal rejection test" << std::endl;
			oss << rej.infoString("rej") << std::endl;
			oss << somedir.infoString("somedir") << std::endl;
			oss << unitrej.infoString("unitrej") << std::endl;
		}

		// check reconstruction
		Vector const gotvec(pro + rej);
		if (! gotvec.nearlyEquals(expvec))
		{
			oss << "Failure of pro/rej reconstruction test" << std::endl;
		}
	}

	// check more general cases
	{
		using namespace ga;

		// check dual version - projection onto plane
		Vector const normDir(unit(Vector{7., -5., 2.}));
		BiVector const somePlane(E123 * normDir);
		Vector const orthoDir(anyPerpendicularDir(normDir));
		Vector const someVec(11., -19., 17.);
		Vector const expProj(cross(cross(normDir, someVec), normDir));
		Vector const gotProj(projectionOnto(someVec, somePlane));
		if (! nearlyEquals(gotProj, expProj))
		{
			oss << "Failure of projection onto plane" << std::endl;
			oss << expProj.infoString("expProj") << std::endl;
			oss << gotProj.infoString("gotProj") << std::endl;
		}

		// check if projection of normal vector is zero
		Vector const gotNorm(projectionOnto(normDir, somePlane));
		Vector const expNorm(vZero);
		if (! nearlyEquals(gotNorm, expNorm))
		{
			oss << "Failure of normal vector projection test" << std::endl;
		}
	}


	return oss.str();
}

//! check basic functions
std::string
ga_functions_test1a
	()
{
	std::ostringstream oss;

	// check null case test
	{
		using namespace ga;

		Vector const vecA{1., 3., -2.};
		Vector const gotVec(cross(vecA, -vecA));
		Vector const expVec(vZero);
		if (! nearlyEquals(gotVec, expVec))
		{
			oss << "Failure of cross to zero test" << std::endl;
		}
	}

	// check basic 'cross product' operations
	{
		using namespace ga;

		Vector const gotA(cross(e1, e2));
		Vector const gotB(cross(e2, e3));
		Vector const gotC(cross(e3, e1));
		Vector const expA(e3);
		Vector const expB(e1);
		Vector const expC(e2);
		if ( (! nearlyEquals(gotA, expA))
		  || (! nearlyEquals(gotB, expB))
		  || (! nearlyEquals(gotC, expC))
		   )
		{
			oss << "Failure of cross() test" << std::endl;
		}
	}

	return oss.str();
}

	ga::BiVector
	bvAngle
		( double const & fracPi
		, ga::BiVector const & bVec
		)
	{
		ga::BiVector const bvDir{ ga::unit(bVec) };
		ga::BiVector const bv{ fracPi * math::pi * bvDir };
		return bv;
	}

	void
	checkExpLog
		( std::ostream & oss
		, std::string const & tname
		, double const & sMag
		, double const & fracPi
		, ga::BiVector const & bVec
		)
	{
		ga::Scalar const sAngle{ std::log(sMag) };
		ga::G2 const expFullAngle{ sAngle, bvAngle(fracPi, bVec) };
		ga::G2 const fullSpin{ ga::expG2(expFullAngle) };
		ga::G2 const gotFullAngle{ ga::logG2(fullSpin) };

		if (! gotFullAngle.nearlyEquals(expFullAngle))
		{
			oss << "Failure of full angle spin/log test case:"
				<< " " << tname << std::endl;
			oss << dat::infoString(expFullAngle, "expFullAngle") << '\n';
			oss << dat::infoString(gotFullAngle, "gotFullAngle") << '\n';
			oss << fullSpin.infoString("fullSpin", false, true) << '\n';
		}
	}

//! check exp(), log()
std::string
ga_functions_test2
	()
{
	std::ostringstream oss;

	using namespace ga;
	checkExpLog(oss, "A1", 2., .125, BiVector{-5., -3., 2.});
	checkExpLog(oss, "A2", 2., .250, BiVector{-5., -3., 2.});
	checkExpLog(oss, "B", 2., .500, BiVector{-1.,  3., 2.});
	checkExpLog(oss, "C", 2., .750, BiVector{-1.,  3., 2.});

	// check pi rotation case
	{
		double const sMag{ 2.5 };
		Scalar const sArg(sMag);

		double const bMag{ math::pi };
		BiVector const bDir{ ga::unit(ga::BiVector(1., 2., 3.)) };
		BiVector const bArg{ bMag * bDir };

		G2 const expArg{ sArg, bArg };
		G2 const popper{ ga::expG2(expArg) };

		// should be invalid w/o plane direction to resolve ambiguity
		{
			G2 const gotLog{ ga::logG2(popper) };
			if (gotLog.isValid())
			{
				oss << "Failure of singular pi-rotation test" << std::endl;
				oss << gotLog.infoString("gotLog", false, true) << '\n';
			}
		}

		// bivector part is pi in recovery under pi rotation
		{
			G2 const expLog{ sArg, math::pi*bDir };
			G2 const gotLog{ ga::logG2(popper, bDir) };
			if (! gotLog.nearlyEquals(expLog))
			{
				oss << "Failure of resolved pi-rotation test" << std::endl;
				oss << popper.infoString("popper", false, true) << '\n';
				oss << expLog.infoString("expLog", false, true) << '\n';
				oss << gotLog.infoString("gotLog", false, true) << '\n';
			}
		}
	}

	return oss.str();
}

//! check inverse()
std::string
ga_functions_test3
	()
{
	std::ostringstream oss;

	// check fundamental blades

	ga::Scalar const sFwd(4.);
	ga::Vector const vFwd(4., 2., -8.);
	ga::BiVector const bFwd(-3., -2., 5.);
	ga::TriVector const tFwd(4.);

	ga::Scalar const sInv{ ga::inverse(sFwd) };
	ga::Vector const vInv{ ga::inverse(vFwd) };
	ga::BiVector const bInv{ ga::inverse(bFwd) };
	ga::TriVector const tInv{ ga::inverse(tFwd) };

	ga::Scalar const allExp(1.);
	ga::Scalar const sGot{ sInv * sFwd };
	ga::Scalar const vGot{ (vInv * vFwd).theS };
	ga::Scalar const bGot{ (bInv * bFwd).theS };
	ga::Scalar const tGot{ tInv * tFwd };

	if (! sGot.nearlyEquals(allExp))
	{
		oss << "Failure of scalar inversion test" << std::endl;
		oss << dat::infoString(sGot, "sGot") << std::endl;
	}
	if (! vGot.nearlyEquals(allExp))
	{
		oss << "Failure of vector inversion test" << std::endl;
		oss << dat::infoString(vGot, "vGot") << std::endl;
	}
	if (! bGot.nearlyEquals(allExp))
	{
		oss << "Failure of bivector inversion test" << std::endl;
		oss << dat::infoString(bGot, "bGot") << std::endl;
	}
	if (! tGot.nearlyEquals(allExp))
	{
		oss << "Failure of trivector inversion test" << std::endl;
		oss << dat::infoString(tGot, "tGot") << std::endl;
	}

	// check multivector entities
	ga::Spinor const spFwd(sFwd, bFwd);
	ga::ImSpinor const imFwd(vFwd, tFwd);
	ga::Spinor const spInv{ ga::inverse(spFwd) };
	ga::ImSpinor const imInv{ ga::inverse(imFwd) };
	ga::Scalar const spGot{ (spInv * spFwd).theS };
	ga::Scalar const imGot{ (imInv * imFwd).theS };

	if (! spGot.nearlyEquals(allExp))
	{
		oss << "Failure of spinor inversion test" << std::endl;
		oss << dat::infoString(spGot, "spGot") << std::endl;
	}

	if (! imGot.nearlyEquals(allExp))
	{
		oss << "Failure of im-spinor inversion test" << std::endl;
		oss << dat::infoString(imGot, "imGot") << std::endl;
	}

	return oss.str();
}

//! check angle extraction corner cases
std::string
ga_functions_test4
	()
{
	std::ostringstream oss;

	ga::Spinor const spin{ -ga::e1 * ga::e1 };
	ga::BiVector const piPlane{ (ga::E12 + .3*ga::E23 - .5*ga::E31) };
	ga::Spinor const gangle{ ga::logG2(spin, piPlane) };
	ga::BiVector const & gotAngle = gangle.theB;
	ga::BiVector const expAngle{ math::pi * ga::unit(piPlane) };
	if (! gotAngle.nearlyEquals(expAngle))
	{
		oss << "Failure of pi-rotation special test" << std::endl;
		oss << dat::infoString(expAngle, "expAngle") << std::endl;
		oss << dat::infoString(gotAngle, "gotAngle") << std::endl;
	}
	return oss.str();
}


}

//! Unit test for ga::functions
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ga_functions_test0();
	oss << ga_functions_test0a();
	oss << ga_functions_test1();
	oss << ga_functions_test1a();
	oss << ga_functions_test2();
	oss << ga_functions_test3();
	oss << ga_functions_test4();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
