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
\brief  This file contains unit test for dat::ops
*/


#include "libdat/ops.h"

#include "libdat/info.h"
#include "libio/stream.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

/*
//! Check for common functions
std::string
dat_ops_test0
	()
{
	std::ostringstream oss;
	dat::ops const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}
*/

//! Check operations
std::string
dat_ops_test1
	()
{
	std::ostringstream oss;

	dat::grid<float> const grid(1u, 1u, 1.f);
	float const scalar(-3.f);
	dat::grid<float> const expGrid(1u, 1u, scalar);
	dat::grid<float> const gotGrid(dat::scaled(scalar, grid));
	bool const same
		(std::equal(gotGrid.begin(), gotGrid.end(), expGrid.begin()));
	if (! same)
	{
		oss << "Failure of scaled grid test" << std::endl;
	}

	return oss.str();
}

//! Check operations
std::string
dat_ops_test2
	()
{
	std::ostringstream oss;

	// check extraction of center from Extents
	dat::Extents const area(7, 2);
	dat::Spot const expCenter{{ 3.5, 1. }};
	dat::Spot const gotCenter(dat::centerOf(area));
	if (! dat::nearlyEquals(gotCenter, expCenter))
	{
		oss << "Failure of center test" << std::endl;
		oss << dat::infoString(expCenter, "exp") << std::endl;
		oss << dat::infoString(gotCenter, "got") << std::endl;
	}

	// check angle computations
	{
		using namespace dat;

		double const x0(19.);
		double const y0(5.);
		double const xx(-17.);
		double const yy(7.);
		double const dy(yy - y0);
		double const dx(xx - x0);
		double const expAngle(std::atan2(dy, dx));

		// configure test case
		Spot const absFrom{{ x0, y0 }};
		Spot const absInto{{ xx, yy }};

		// check dual argument result
		{
			double const gotAngle(dat::angleToward(absInto, absFrom));
			if (! nearlyEquals(gotAngle, expAngle))
			{
				oss << "Failure of abs angle test" << std::endl;
				oss << infoString(expAngle, "exp") << std::endl;
				oss << infoString(gotAngle, "got") << std::endl;
			}
		}

		// check default argument result
		{
			Spot const relFrom(absFrom - absInto);
			double const gotAngle(dat::angleToward(absInto, absFrom));
			if (! nearlyEquals(gotAngle, expAngle))
			{
				oss << "Failure of abs angle test" << std::endl;
			}
		}
	}

	return oss.str();
}


}

//! Unit test for dat::ops
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
//	oss << dat_ops_test0();
	oss << dat_ops_test1();
	oss << dat_ops_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
