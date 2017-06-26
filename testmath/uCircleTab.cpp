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
\brief  This file contains unit test for math::CircleTab
*/


#include "libmath/CircleTab.h"

#include "libdat/dat.h"
#include "libdat/info.h"
#include "libio/stream.h"
#include "libmath/interp.h"

#include <algorithm>
#include <complex>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
math_CircleTab_test0
	()
{
	std::ostringstream oss;
	math::CircleTab<uint8_t> const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check index management
std::string
math_CircleTab_test1a
	()
{
	std::ostringstream oss;
	// add whatever
	// ...
	return oss.str();
}

	std::string
	afString
		( double const & angle
		, std::function<double(double const &)> const & func
		)
	{
		std::ostringstream oss;
//		oss << std::fixed << std::setprecision(3);
		oss
			<< " " << std::setw(10) << angle
			<< " " << std::setw(10) << func(angle)
			;
		return oss.str();
	}


//! Check management of angle phase wrap
std::string
math_CircleTab_test1b
	()
{
	std::ostringstream oss;

	// configure test data values

	//     0 +.14 +1.14 +2.14  +-pi  -2.14 -1.14 -.14   0  +.14 +1.14
	// ... ()  p-3  p-2  p-1   [+-pi]  -p+1 -p+2 -p+3   ()  p-3  p-2 ...
	//         ...  100  200    300     400  500  ...
	//         ...                        (500+) 1*dy  2*dy 3*dy (100)
	using math::pi;

	// define key points and values at those points
	double const x0(-pi + 2.);
	double const y0(500.);
	double const x1( pi - 2.);
	double const y1(100.);

	typedef std::pair<double, double> av;
	std::vector<std::pair<double, double> > const avPairs
		{ av{  x0,  y0 }
		, av{  x1,  y1 }
		};
	std::vector<std::pair<double, double> > const expPairs
		{ av{-pi   ,  300. }
		, av{-pi+1.,  400. }
		, av{-pi+2.,  500. }
		, av{-pi+3.,  324.8}
		, av{    0.,  300. }
		, av{ pi-3.,  275.2}
		, av{ pi-2.,  100. }
		, av{ pi-1.,  200. }
		, av{ pi   ,  300. }
		};
	double const expDataQuant(.1); // data rounding in exp

	// create interpolator - use dense table to preserve 'corners'
	math::CircleTab<double> const fitter(avPairs, 4096u);

	// check results
	for (size_t nn(0u) ; nn < expPairs.size() ; ++nn)
	{
		// interpolate value
		double const & angle = expPairs[nn].first;
		double const & expValue = expPairs[nn].second;

		// check results of nearest table fetch
		double const gotNear(fitter.nearestTableValue(angle));
		if (! dat::nearlyEquals(gotNear, expValue, expDataQuant))
		{
			oss << "Failure of wrap nearest test at nn = " << nn << std::endl;
			oss << dat::infoString(expValue, "expValue") << std::endl;
			oss << dat::infoString(gotNear, "gotNear") << std::endl;
			break;
		}

		/*
		// check results of interpolation
		double const gotInterp(fitter.valueAt(angle));
		if (! dat::nearlyEquals(gotInterp, expValue, expDataQuant))
		{
			oss << "Failure of wrap interp test at nn = " << nn << std::endl;
			oss << dat::infoString(expValue, "expValue") << std::endl;
			oss << dat::infoString(gotInterp, "gotInterp") << std::endl;
			break;
		}
		*/
	}

	return oss.str();
}

//! Check simple interpolation
std::string
math_CircleTab_test2
	()
{
	std::ostringstream oss;

	// ease notation
	typedef std::complex<double> CNum;
	using namespace math;

	std::function<CNum(double const &)> const valueFor
		( []
			(double const & angle)
			{ return CNum(std::cos(angle), std::sin(angle)); }
		);

	// create 'sloppy' data: some duplicates, out of order, etc.
	std::vector<double> const angles
		{ -1.5, -pi, -3., -2., -2., -1., 0., 1., 2., 1.8, 1.9, pi, 2.4};
	std::vector<std::pair<double, CNum> > avPairs;
	for (double const & angle : angles)
	{
		std::pair<double, CNum> const avPair(angle, valueFor(angle));
		avPairs.push_back(avPair);
	}

	// create interpolator
	size_t const numNodes(64u);
	math::CircleTab<CNum> const fan(avPairs, numNodes);

	// evaluate interpolated angle values
	double const da(twoPi / (double)numNodes);
	for (double aa(-pi) ; aa <= pi ; aa += da)
	{
		CNum const expValue(valueFor(aa));
		CNum const gotValue(fan.nearestTableValue(aa));
		CNum const delta(gotValue - expValue);
		double const diff(std::abs(delta));
		static double const tol(.2); // linear interp - test data fit error
		if (! dat::nearlyEquals(diff, 0., tol))
		{
			oss << "Failure of nearestTableValue test" << std::endl;
			oss << "expValue: " << expValue << std::endl;
			oss << "gotValue: " << gotValue << std::endl;
			oss << "delta: " << delta << std::endl;
			oss << "diff: " << diff << std::endl;
			break;
		}
	}

	// check values at critical (phase wrap) values
	CNum const gotHalfPos(fan.nearestTableValue( pi));
	CNum const gotHalfNeg(fan.nearestTableValue(-pi));
	if (! (gotHalfPos == gotHalfNeg))
	{
		oss << "Failure of pos/neg pi test" << std::endl;
		oss << "gotHalfPos: " << gotHalfPos << std::endl;
		oss << "gotHalfNeg: " << gotHalfNeg << std::endl;
	}

	return oss.str();
}

//! Check single sample
std::string
math_CircleTab_test3
	()
{
	std::ostringstream oss;

	float const expValue(29.f);

	// construct fan with one value
	std::vector<std::pair<double, float> > const avPairs
		{{ std::pair<double, float>(1., expValue) }};
	math::CircleTab<float> const fan(avPairs, 4u);

	// ensure it is valid
	if (! fan.isValid())
	{
		oss << "Failure of 1 sample fan validity test" << std::endl;
	}

	// check interpolation (expect constant value)
	float const gotValue(fan.nearestTableValue(-2.));
	if (! dat::nearlyEquals(gotValue, expValue))
	{
		oss << "Failure of 1 sample interp test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for math::CircleTab
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << math_CircleTab_test0();
	oss << math_CircleTab_test1a();
	oss << math_CircleTab_test1b();
	oss << math_CircleTab_test2();
	oss << math_CircleTab_test3();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
