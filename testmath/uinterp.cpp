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
\brief  This file contains unit test for math::interp
*/


#include "libmath/interp.h"

#include "libdat/dat.h"
#include "libio/stream.h"

#include <cassert>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check basic 'fraction' functions....
std::string
math_interp_test1a
	()
{
	std::ostringstream oss;

	// synthesize linear data
	std::pair<double, double> range(-3., 7.);
	double const expValue(4.);
	double const expFrac(7./10.);

	// evaluate functions
	double const gotFrac(math::interp::fractionAtValid(expValue, range));
	double const gotValue(math::interp::valueAtValid(expFrac, range));

	// check results
	double const dFrac(gotFrac - expFrac);
	if (! (std::abs(dFrac) < math::eps))
	{
		oss << "Failure of gotFrac test" << std::endl;
		oss << "expFrac: " << expFrac << std::endl;
		oss << "gotFrac: " << gotFrac << std::endl;
	}

	double const dX(gotValue - expValue);
	if (! (std::abs(dX) < math::eps))
	{
		oss << "Failure of gotValue test" << std::endl;
		oss << "expValue: " << expValue << std::endl;
		oss << "gotValue: " << gotValue << std::endl;
	}

	return oss.str();
}


//! Check linear interpolation
std::string
math_interp_test1b
	()
{
	std::ostringstream oss;

	// synthesize linear data
	std::pair<double, double> xRange(-3., 7.);
	std::pair<double, double> yRange(17., 11.);
	double const expX(4.);

	// two-point formula for a line as check
	std::function<double(double const &)> const line
		( [xRange, yRange]
			( double const & xval)
			{
				double const dy(yRange.second - yRange.first);
				double const dx(xRange.second - xRange.first);
				return ((dy/dx) * (xval - xRange.first) + yRange.first);
			}
		);

	// evaluate functions
	double const gotY(math::interp::linear(xRange, expX, yRange));

	// check results
	double const expY(line(expX));
	double const dY(gotY - expY);
	if (! (std::abs(dY) < math::eps))
	{
		oss << "Failure of gotY test" << std::endl;
		oss << "expY: " << expY << std::endl;
		oss << "gotY: " << gotY << std::endl;
	}

	return oss.str();
}

	//! evaluate interpolated array value
	template <typename Value>
	void
	testArray
		( std::ostream & oss
		, double const & subNdx
		, Value const & expValue
		, std::vector<Value> const & values
		, std::string const & tname
		, Value const & tolValue = dat::smallValue<Value>()
		)
	{
		Value const gotValue
			(math::interp::linear<Value>(subNdx, values));
		Value difValue(dat::nullValue<Value>());
		bool haveError(false);
		if (dat::isValid(expValue))
		{
			// case for expecting a valid expValue
			haveError= (! dat::nearlyEquals(gotValue, expValue, tolValue));
			difValue = (gotValue - expValue);
		}
		else
		{
			// case for expecting bad value
			haveError = dat::isValid(gotValue);
		}
		if (haveError)
		{
			oss << "Failure of " << tname << " array test" << std::endl;
			oss << "exp: " << expValue << std::endl;
			oss << "got: " << gotValue << std::endl;
			oss << "dif: " << difValue << std::endl;
			oss << "tol: " << tolValue << std::endl;
		}
	}

//! Check linear array interpolation
std::string
math_interp_test2
	()
{
	std::ostringstream oss;

	double const expNull(dat::nullValue<double>());

	// check 'empty' case
	{
		std::vector<double> const values;
		testArray(oss, 0., expNull, values, "empty");
	}

	// check degenerate case
	{
		double const datValue(-19.);
		std::vector<double> const values{ datValue };
		testArray(oss, 0., datValue, values, "at one value");
		testArray(oss, .5, expNull, values, "one value .5");
		testArray(oss, 1., expNull, values, "one value 1.");
		testArray(oss, 2., expNull, values, "one value 2.");
	}

	// check general case
	{
		std::vector<double> const values{{ 0., 100., 200., 300., 400. }};
		testArray(oss,  .00,   0., values, ".000");
		testArray(oss,  .25,  25., values, ".250");
		testArray(oss,  .50,  50., values, ".500");
		testArray(oss,  .75,  75., values, ".750");
		testArray(oss, 3.00, 300., values, "3.000");
		// check 'open' end condition
		double const ep(8.*std::numeric_limits<double>::epsilon());
		testArray(oss, 4.-ep, 400.-ep, values, "4.-ep", 400.*ep);
		testArray(oss, 4.000, expNull, values, "4.000");
		testArray(oss, 4.+ep, expNull, values, "4.+ep");
	}

	return oss.str();
}

//! Check bi-directional interpolation
std::string
math_interp_test3
	()
{
	std::ostringstream oss;

	std::pair<double, double> const rangeInc{ 1., 2. };
	std::pair<double, double> const rangeDec{ 2., 1. };
	constexpr double outMin{ 1000. };
	constexpr double outSpan{ 100. };
	std::pair<double, double> const outRange{ outMin, outMin + outSpan };

	constexpr double inVal{ 1.75 };
	constexpr double expInc{ .75*outSpan + outMin };
	constexpr double expDec{ .25*outSpan + outMin };

	double const gotInc{ math::interp::linear(rangeInc, inVal, outRange) };
	double const gotDec{ math::interp::linear(rangeDec, inVal, outRange) };

	if (! dat::nearlyEquals(gotInc, expInc))
	{
		oss << "Failure of increasing range test" << std::endl;
	}
	if (! dat::nearlyEquals(gotDec, expDec))
	{
		oss << "Failure of decreasing range test" << std::endl;
	}

	return oss.str();
}

//! Check 2D interpolations
std::string
math_interp_test4
	()
{
	std::ostringstream oss;

	dat::Range<double> const fromX(  10.,   15.);
	dat::Range<double> const intoX( 100.,  150.);
	dat::Range<double> const fromY(  20.,   25.);
	dat::Range<double> const intoY(-200.,    0.);

	dat::Area<double> const fromArea{ fromX, fromY };
	dat::Area<double> const intoArea{ intoX, intoY };

	dat::Spot const expFromSpot{{  11.,   22. }};
	dat::Spot const expIntoSpot{{ 110., -120. }};

	dat::Spot const gotIntoSpot
		(math::interp::linear(fromArea, expFromSpot, intoArea));
	dat::Spot const gotFromSpot
		(math::interp::linear(intoArea, expIntoSpot, fromArea));

	if (! dat::nearlyEquals(gotIntoSpot, expIntoSpot))
	{
		oss << "Failure of Into test" << std::endl;
		oss << dat::infoString(expIntoSpot, "expIntoSpot") << std::endl;
		oss << dat::infoString(gotIntoSpot, "gotIntoSpot") << std::endl;
	}
	if (! dat::nearlyEquals(gotFromSpot, expFromSpot))
	{
		oss << "Failure of From test" << std::endl;
		oss << dat::infoString(expFromSpot, "expFromSpot") << std::endl;
		oss << dat::infoString(gotFromSpot, "gotFromSpot") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for math::interp
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << math_interp_test1a();
	oss << math_interp_test1b();
	oss << math_interp_test2();
	oss << math_interp_test3();
	oss << math_interp_test4();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
