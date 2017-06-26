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
\brief  This file contains unit test for math::angle
*/


#include "libmath/angle.h"

#include "libio/stream.h"
#include "libmath/math.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{
	static double const negpi(-math::pi);
	static double const pospi( math::pi);

	std::vector<double>
	someAngles
		( double const & da
		, double const amin = { -2.5 * math::pi }
		)
	{
		std::vector<double> angles;

		// add (numerically) exact critical quadrature angles
		angles.emplace_back(-math::pi);
		angles.emplace_back(-math::halfPi);
		angles.emplace_back(0.);
		angles.emplace_back(math::halfPi);
		angles.emplace_back(math::pi);

		// add regular samples
		double const amax{ -amin };
		for (double angle{amin} ; angle <= amax ; angle += da)
		{
			angles.emplace_back(angle);
		}

		return angles;
	}

//! Check for common functions
std::string
math_angle_test0
	()
{
	std::ostringstream oss;
	/*
	math::angle const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	*/

	//
	// Check sameRotation() test
	//

	constexpr double da{ math::pi / 17. };
	constexpr double tol{ da / 2. }; // large finite value
	constexpr double deltaDif{ (9./8.) * tol };
	constexpr double deltaSam{ (7./8.) * tol };
	std::vector<double> const angles{ someAngles(da) };
	for (double const & aCur0 : angles)
	{
		// check angle against itself
		bool const expCur0{ true };
		bool const gotCur0{ math::sameRotation(aCur0, aCur0, tol) };

		bool hitErr{ false };
		if (! dat::nearlyEquals(gotCur0, expCur0))
		{
			oss << "Failure of gotCur0 test" << std::endl;
			hitErr = true;
		}

		// check 'small' differences (inside of tolerance)
		double const aSamN{ aCur0 - deltaSam };
		double const aSamP{ aCur0 + deltaSam };

		bool const expSamN{ true };
		bool const expSamP{ true };
		bool const gotSamN{ math::sameRotation(aCur0, aSamN, tol) };
		bool const gotSamP{ math::sameRotation(aCur0, aSamP, tol) };

		if (! dat::nearlyEquals(gotSamN, expSamN))
		{
			oss << "Failure of gotSamN test" << std::endl;
			hitErr = true;
		}

		if (! dat::nearlyEquals(gotSamP, expSamP))
		{
			oss << "Failure of gotSamP test" << std::endl;
			hitErr = true;
		}

		// check 'large' differences (outside of tolerance)
		double const aDifN{ aCur0 - deltaDif };
		double const aDifP{ aCur0 + deltaDif };

		bool const expDifN{ false };
		bool const expDifP{ false };
		bool const gotDifN{ math::sameRotation(aCur0, aDifN, tol) };
		bool const gotDifP{ math::sameRotation(aCur0, aDifP, tol) };

		if (! dat::nearlyEquals(gotDifN, expDifN))
		{
			oss << "Failure of gotDifN test" << std::endl;
			hitErr = true;
		}

		if (! dat::nearlyEquals(gotDifP, expDifP))
		{
			oss << "Failure of gotDifP test" << std::endl;
			hitErr = true;
		}

		// in case of trouble, display test info
		if (hitErr)
		{
			oss
				<< "aDifN,aSamN,aCur0,aSamP,aDifP:"
				<< " " << dat::infoString(aDifN)
				<< " " << dat::infoString(aSamN)
				<< " " << dat::infoString(aCur0)
				<< " " << dat::infoString(aSamP)
				<< " " << dat::infoString(aDifP)
				<< " " << ":::"
				<< " " << dat::infoString(gotDifN)
				<< " " << dat::infoString(gotSamN)
				<< " " << dat::infoString(gotCur0)
				<< " " << dat::infoString(gotSamP)
				<< " " << dat::infoString(gotDifP)
				<< std::endl;
			oss
				<< std::endl;
//			break;
		}

	}

	return oss.str();
}

//! Check basic constants
std::string
math_angle_test1
	()
{
	std::ostringstream oss;
	double const expPi(2. * std::acos(0.));

	double const gotPiQ(4. * math::qtrPi);
	double const gotPiH(2. * math::halfPi);
	double const gotPi1(     math::pi);
	double const gotPi2(.5 * math::twoPi);
	if (! dat::nearlyEquals(gotPiQ, expPi))
	{
		oss << "failure of gotPiQ test" << std::endl;
	}
	if (! dat::nearlyEquals(gotPiH, expPi))
	{
		oss << "failure of gotPiH test" << std::endl;
	}
	if (! dat::nearlyEquals(gotPi1, expPi))
	{
		oss << "failure of gotPi1 test" << std::endl;
	}
	if (! dat::nearlyEquals(gotPi2, expPi))
	{
		oss << "failure of gotPi2 test" << std::endl;
	}
	return oss.str();
}

//! Check basic functions
std::string
math_angle_test2
	()
{
	std::ostringstream oss;

	// principal angle values
	{
		// general case
		double const expAngle(-2.);
		double const gotMore(math::principalAngle(expAngle + math::twoPi));
		double const gotLess(math::principalAngle(expAngle - math::twoPi));
		if (! dat::nearlyEquals(gotMore, expAngle))
		{
			oss << "Failure of principal angle More test" << std::endl;
		}
		if (! dat::nearlyEquals(gotLess, expAngle))
		{
			oss << "Failure of principal angle Less test" << std::endl;
		}
	}
	{
		// branch-cut case
		double const expAngle(-math::pi);
		double const gotPos(math::principalAngle( math::pi));
		double const gotNeg(math::principalAngle(-math::pi));
		if (! dat::nearlyEquals(gotPos, expAngle))
		{
			oss << "Failure of principal angle Pos test" << std::endl;
		}
		if (! dat::nearlyEquals(gotNeg, expAngle))
		{
			oss << "Failure of principal angle Neg test" << std::endl;
		}
	}

	return oss.str();
}

	//! Test equivalence of index result
	void
	checkIndex
		( std::ostream & oss
		, size_t const & gotNdx
		, size_t const & expNdx
		, std::string const & tname
		)
	{
		bool const okayIfBad(! dat::isValid(expNdx));
		bool const matchingBad(okayIfBad && (! dat::isValid(gotNdx)));
		if (! matchingBad) // then test matching good
		{
			if (! dat::nearlyEquals(gotNdx, expNdx))
			{
				oss << "Failure of " << tname << " case" << std::endl;
				oss << "exp: " << expNdx << std::endl;
				oss << "got: " << gotNdx << std::endl;
			}
		}
	}

	void
	checkAngle
		( std::ostream & oss
		, double const & gotAngle
		, double const & expAngle
		, std::string const & tname
		)
	{
		bool const okayIfBad(! dat::isValid(expAngle));
		bool const matchingBad(okayIfBad && (! dat::isValid(gotAngle)));
		if (! matchingBad) // then test matching good
		{
			if (! dat::nearlyEquals(gotAngle, expAngle))
			{
				oss << "Failure of " << tname << " case" << std::endl;
				oss << "exp: " << expAngle << std::endl;
				oss << "got: " << gotAngle << std::endl;
			}
		}
	}

//! Check angle to array operations
std::string
math_angle_test3a
	()
{
	std::ostringstream oss;

	using math::indexForAngle;

	// check trivial case
	{
		size_t const numDat(1u);
		size_t const expNdx(dat::nullValue<size_t>());
		double const expAngle(dat::nullValue<double>()); // no soln for 1 datum
		// for only one values, everything should interpolate to 0
		{
			size_t const gotNdx(indexForAngle(negpi, numDat));
			checkIndex(oss, gotNdx, expNdx, "trivial index (negpi)");
			double const gotAngle(math::angleForIndex(expNdx, numDat));
			checkAngle(oss, gotAngle, expAngle, "trivial angle (negpi)");
		}
		{
			size_t const gotNdx(indexForAngle(2., numDat));
			checkIndex(oss, gotNdx, expNdx, "trivial index (2.)");
			double const gotAngle(math::angleForIndex(expNdx, numDat));
			checkAngle(oss, gotAngle, expAngle, "trivial angle (2.)");
		}
		{
			size_t const gotNdx(indexForAngle(pospi, numDat));
			checkIndex(oss, gotNdx, expNdx, "trivial index (pospi)");
			double const gotAngle(math::angleForIndex(expNdx, numDat));
			checkAngle(oss, gotAngle, expAngle, "trivial angle (pospi)");
		}
	}

	return oss.str();
}

//! Check angle to array operations
std::string
math_angle_test3b
	()
{
	std::ostringstream oss;

	using math::indexForAngle;

	// check two item case
	{
		size_t const numDat(2u);
		//  in[                                            )out
		//  -pi  -3  -2  -pi/2  -1  ()  +1  +pi/2  +2  +3  +pi
		//   0                       1                          (2)
		{
			size_t const expNdx(0u);
			size_t const gotNdx(indexForAngle(negpi, numDat));
			checkIndex(oss, gotNdx, expNdx, "two-value(A) negpi");
			double const gotAngle(math::angleForIndex(expNdx, numDat));
			checkAngle(oss, gotAngle, negpi, "two-value(A) angle (negpi)");
		}
		{
			size_t const expNdx(0u);
			size_t const gotNdx(indexForAngle(  -2., numDat));
			checkIndex(oss, gotNdx, expNdx, "two-value(A) -1.");
			double const gotAngle(math::angleForIndex(expNdx, numDat));
			checkAngle(oss, gotAngle, negpi, "two-value(A) angle (-2.)");
		}
		{
			size_t const expNdx(1u);
			size_t const gotNdx(indexForAngle(   1., numDat));
			checkIndex(oss, gotNdx, expNdx, "two-value(A) 1.");
			double const gotAngle(math::angleForIndex(expNdx, numDat));
			checkAngle(oss, gotAngle, 0., "two-value(A) angle (1.)");
		}
		{
			size_t const expNdx(0u);
			size_t const gotNdx(indexForAngle(pospi, numDat));
			checkIndex(oss, gotNdx, expNdx, "two-value(A) pospi");
			double const gotAngle(math::angleForIndex(expNdx, numDat));
			checkAngle(oss, gotAngle, negpi, "two-value(A) angle (pospi)");
		}

		//  -pi  -3  -2  -pi/2  -1  ()  +1  +pi/2  +2  +3  +pi
		//                  0                  1                (2)
		double const a0(-math::halfPi);
		double const half(math::halfPi);
		{
			size_t const expNdx(1u);
			size_t const gotNdx(indexForAngle(negpi, numDat, a0));
			checkIndex(oss, gotNdx, expNdx, "two-value(B) negpi");
			double const gotAngle(math::angleForIndex(expNdx, numDat, a0));
			checkAngle(oss, gotAngle,  half, "two-value(B) angle (negpi)");
		}
		{
			size_t const expNdx(0u);
			size_t const gotNdx(indexForAngle(   1., numDat, a0));
			checkIndex(oss, gotNdx, expNdx, "two-value(B) 1.");
			double const gotAngle(math::angleForIndex(expNdx, numDat, a0));
			checkAngle(oss, gotAngle, -half, "two-value(B) angle (1.)");
		}
		{
			size_t const expNdx(1u);
			size_t const gotNdx(indexForAngle(pospi, numDat, a0));
			checkIndex(oss, gotNdx, expNdx, "two-value(B) pospi");
			double const gotAngle(math::angleForIndex(expNdx, numDat, a0));
			checkAngle(oss, gotAngle,  half, "two-value(B) angle (pospi)");
		}
	}

	return oss.str();
}

//! Check angle to array operations
std::string
math_angle_test3c
	()
{
	std::ostringstream oss;

	using math::indexForAngle;

	// check multiple item case
	{
		size_t const numDat(8u);
		//  in[                                            )out
		// -3.14  -2.36  -1.57 -.79  0  .79   1.57  2.36   3.14
		//        -3p/4        -p/4    p/4           3p/4
		//  -pi  -3  -2  -pi/2  -1  (0)  +1  +pi/2  +2  +3  +pi
		//   0             1         2          3             .  (4)
		//   0      1      2    3    4    5     6      7      .  (8)
		//   8      9     10   ....
		double const qtr1(   math::qtrPi);
		double const qtr2(2.*math::qtrPi);
		double const qtr3(3.*math::qtrPi);
		{
			size_t const expNdx(0u);
			size_t const gotNdx(indexForAngle(negpi, numDat));
			checkIndex(oss, gotNdx, expNdx, "8-value index negpi");
			double const gotAngle(math::angleForIndex(expNdx, numDat));
			checkAngle(oss, gotAngle, negpi, "8-value angle [0a]");
		}
		{
			size_t const expNdx(0u);
			size_t const gotNdx(indexForAngle(  -3., numDat));
			checkIndex(oss, gotNdx, expNdx, "8-value index -3.");
			double const gotAngle(math::angleForIndex(expNdx, numDat));
			checkAngle(oss, gotAngle, negpi, "8-value angle [0b]");
		}
		{
			size_t const expNdx(1u);
			size_t const gotNdx(indexForAngle(  -2., numDat));
			checkIndex(oss, gotNdx, expNdx, "8-value index -2.");
			double const gotAngle(math::angleForIndex(expNdx, numDat));
			checkAngle(oss, gotAngle, -qtr3, "8-value angle [1]");
		}
		{
			size_t const expNdx(2u);
			size_t const gotNdx(indexForAngle( -1.5, numDat));
			checkIndex(oss, gotNdx, expNdx, "8-value index -1.5");
			double const gotAngle(math::angleForIndex(expNdx, numDat));
			checkAngle(oss, gotAngle, -qtr2, "8-value angle [2a]");
		}
		{
			size_t const expNdx(3u);
			size_t const gotNdx(indexForAngle(  -.5, numDat));
			checkIndex(oss, gotNdx, expNdx, "8-value index -.5");
			double const gotAngle(math::angleForIndex(expNdx, numDat));
			checkAngle(oss, gotAngle, -qtr1, "8-value angle [3]");
		}
		{
			size_t const expNdx(0u);
			size_t const gotNdx(indexForAngle(pospi, numDat));
			checkIndex(oss, gotNdx, expNdx, "8-value index [0]");
			double const gotAngle(math::angleForIndex(expNdx, numDat));
			checkAngle(oss, gotAngle, negpi, "8-value angle [0c]");
		}
		{
			size_t const expNdx(8u); // == 0
			double const gotAngle(math::angleForIndex(expNdx, numDat));
			checkAngle(oss, gotAngle, negpi, "8-value angle [8]");
		}
		{
			size_t const expNdx(9u);
			double const gotAngle(math::angleForIndex(expNdx, numDat));
			checkAngle(oss, gotAngle, -qtr3, "8-value angle [9]");
		}
	}

	return oss.str();
}

//! Check interpolation functions
std::string
math_angle_test4
	()
{
	std::ostringstream oss;

	//! Structure to run test cases
	struct PiValues
	{
		double thePiValue;
		std::pair<double, double> thePiRangePair;
		double theExpFrac;

		PiValues () = default;

		//! Compute interpolated fraction and test against expected value
		void
		runTest
			( std::ostream & oss
			) const
		{
			double const angleAt(thePiValue * math::pi);
			double const angleBeg(thePiRangePair.first * math::pi);
			double const angleEnd(thePiRangePair.second * math::pi);
			double const & expFrac = theExpFrac;

			double const gotFrac
				(math::angleFraction(angleAt, { angleBeg, angleEnd }));
			bool okay(false);
			if (! dat::isValid(expFrac))
			{
				okay = (! dat::isValid(gotFrac));
			}
			else
			{
				okay = dat::nearlyEquals(gotFrac, expFrac);
			}

			if (! okay)
			{
				oss << "Failure of fraction test" << std::endl;
				oss
					<< "case:(beg,at,end)_pi:"
					<< " " << dat::infoString(thePiRangePair.first)
					<< " " << dat::infoString(thePiValue)
					<< " " << dat::infoString(thePiRangePair.second)
					<< std::endl;
				oss
					<< "case:(beg,at,end)rad:"
					<< " " << dat::infoString(angleBeg)
					<< " " << dat::infoString(angleAt)
					<< " " << dat::infoString(angleEnd)
					<< std::endl;
				oss << dat::infoString(expFrac, "expFrac") << std::endl;
				oss << dat::infoString(gotFrac, "gotFrac") << std::endl;
			}
		}
	};

	constexpr double nil{ dat::nullValue<double>() };
	PiValues{  -.025 , {   .000,   .500 }, nil }.runTest(oss);
	PiValues{   .025 , {  -.500,   .000 }, nil }.runTest(oss);
	PiValues{ -1.250 , { -1.000, -1.500 }, nil }.runTest(oss);

	PiValues{   .125 , {   .000,   .500 }, .25 }.runTest(oss);
	PiValues{   .000 , {  -.500,   .500 }, .50 }.runTest(oss);
	PiValues{ -1.000 , {   .500,  -.500 }, .50 }.runTest(oss);
	PiValues{  1.250 , { -1.000,  1.500 }, .50 }.runTest(oss);

	PiValues{  1.500 , {  0.000,  2.000 }, .75 }.runTest(oss);

	return oss.str();
}

//! Check extrapolation functions
std::string
math_angle_test5
	()
{
	std::ostringstream oss;

	constexpr double a0{ 0. * math::qtrPi };
	constexpr double a1{ 1. * math::qtrPi };
	constexpr double a2{ 2. * math::qtrPi };
	constexpr double a3{ 3. * math::qtrPi };
	constexpr double a4{ 4. * math::qtrPi };
	constexpr double a5{ 5. * math::qtrPi };
	constexpr double a6{ 6. * math::qtrPi };
	constexpr double a7{ 7. * math::qtrPi };
	using Trio = std::array<double, 3u>;
	std::vector<Trio> const trios
		{ {{  a0,  a1,  a2 }}
		, {{  a0,  a3,  a2 }}
		, {{  a0,  a7,  a2 }}
		  //
		, {{  a3,  a4,  a5 }}
		, {{  a3,  a6,  a5 }}
		, {{  a3,  a2,  a5 }}
		  //
		, {{  a6,  a7,  a0 }}
		, {{  a6,  a1,  a0 }}
		, {{  a6,  a5,  a0 }}
		  //
		, {{  a7,  a0,  a1 }}
		, {{  a7,  a2,  a1 }}
		, {{  a7,  a6,  a1 }}
		};
	double const p1{  .50 };
	double const p3{ 1.50 };
	double const m1{ -.50 };
	std::vector<double> const expFracs
		{ p1, p3, m1
		, p1, p3, m1
		, p1, p3, m1
		, p1, p3, m1
		};
	size_t const numTrials(trios.size());
	assert(numTrials == expFracs.size());
	for (size_t nn(0u) ; nn < numTrials ; ++nn)
	{
		Trio const & trio = trios[nn];
		double const & expFrac = expFracs[nn];

		double const & aLo = trio[0];
		double const & aAt = trio[1];
		double const & aHi = trio[2];
		std::pair<double, double> const aLoHi{ aLo, aHi };

		double const gotFrac(math::angleExFraction(aAt, aLoHi));

		if (! dat::nearlyEquals(gotFrac, expFrac))
		{
			oss << "Failure of angle extrapolation test" << std::endl;
			oss
				<< "nn:"
				<< " " << dat::infoString(nn)
				<< " " << "aLo,At,Hi(qtrs):"
				<< " " << dat::infoString(aLo/math::qtrPi)
				<< " " << dat::infoString(aAt/math::qtrPi)
				<< " " << dat::infoString(aHi/math::qtrPi)
				<< " " << "exp,got: "
				<< " " << dat::infoString(expFrac)
				<< " " << dat::infoString(gotFrac)
				<< std::endl;
			break;
		}
	}

	return oss.str();
}


}

//! Unit test for math::angle
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << math_angle_test0();
	oss << math_angle_test1();
	oss << math_angle_test2();
	oss << math_angle_test3a();
	oss << math_angle_test3b();
	oss << math_angle_test3c();
	oss << math_angle_test4();
	oss << math_angle_test5();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
