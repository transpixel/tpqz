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
\brief  This file contains unit test for ga::derivative
*/


#include "libga/derivative.h"
#include "libga/ga.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libga/Pose.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libmath/NumDeriv.h"

#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>


namespace
{
	struct Exponator
	{
		ga::Spinor
		operator()
			( ga::BiVector const & biv
			) const
		{
			return ga::exp(biv);
		}
	};


//! Check spinor derivative
std::string
ga_derivative_test1
	()
{
	std::ostringstream oss;

	// Construct numeric differentiator for bivectors
	math::NumDeriv<ga::Spinor> const nDeriv;
	std::vector<ga::BiVector> const dirs
		{ ga::E12, ga::E23, ga::E31 };

	// some general arbitrary value
	ga::BiVector const bivVal( .3, .5, .7);

	// check differentiation against each basis
	for (ga::BiVector const & dir : dirs)
	{
		ga::BiVector const bivDelta{ nDeriv.deltaInDirOf(dir) };
		ga::Spinor const exp
			{ nDeriv.derivative(bivVal, bivDelta, Exponator()) };
		ga::Spinor const got{ ga::dexp(bivVal, dir) };

		if (! got.nearlyEquals(exp, nDeriv.tolerance()))
		{
			ga::Spinor const dif{ got - exp };
			oss << "Failure of dexp test" << std::endl;
			oss << dat::infoString(exp, "exp") << std::endl;
			oss << dat::infoString(got, "got") << std::endl;
			oss << dif.infoStringFmt("dif", "%12.5e") << std::endl;
		}
	}

	return oss.str();
}

//! Check spinor derivative
std::string
ga_derivative_test2
	()
{
	std::ostringstream oss;

	using namespace ga;
	struct Rotor
	{
		//! Construct with arbitrary test values
		Vector const theV0{ Vector(-5., 7., 3.) };
		BiVector const thePhysAngle{ BiVector(.6, .4, .7) };

		//! Result of rotating inVec
		Vector
		operator()
			( BiVector const & physAngle
			) const
		{
			Pose const pose(physAngle);
			return pose(theV0);
		}
	};

	// test several angle directions
	Rotor const rotr;
	BiVector const & phiVal = rotr.thePhysAngle;
	static std::vector<BiVector> const phiDots
		{ BiVector(1., 0., 0.)
		, BiVector(0., 1., 0.)
		, BiVector(0., 0., 1.)
		, BiVector(5., 2., 7.)
		};
	math::NumDeriv<Vector> const nDeriv;
	for (BiVector const & phiDot : phiDots)
	{
		// estimate derivative numerically
		BiVector const delPhi{ nDeriv.deltaInDirOf(phiDot) };
		Vector const expDeriv{ nDeriv.derivative(phiVal, delPhi, Rotor()) };

		// evaluate analytic expression
		Vector const gotDeriv
			{ ga::dRotatedPhysArgs(phiVal, phiDot, rotr.theV0) };

		// check agreement
		double const tol{ 1.e2 * nDeriv.tolerance() };
		if (! gotDeriv.nearlyEquals(expDeriv, tol))
		{
			Vector const difDeriv{ gotDeriv - expDeriv };
			oss << "Failure of dRotatedPhysArgs test" << std::endl;
			oss << dat::infoString(phiDot, "phiDot") << std::endl;
			oss << dat::infoString(expDeriv, "expDeriv") << std::endl;
			oss << dat::infoString(gotDeriv, "gotDeriv") << std::endl;
			oss << difDeriv.infoStringFmt("difDeriv", "%12.5e") << std::endl;
			oss << dat::infoString(tol, "tol") << std::endl;
		}
	}

	return oss.str();
}

}

//! Unit test for ga::derivative
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
//	oss << ga_derivative_test0();
	oss << ga_derivative_test1();
	oss << ga_derivative_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
