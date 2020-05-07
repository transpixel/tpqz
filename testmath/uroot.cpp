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
\brief  This file contains unit test for math::root
*/


#include "libmath/root.h"

#include "libdat/compare.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
math_root_test0
	()
{
	std::ostringstream oss;
	/*
	math::root const aNull(dat::nullValue<math::root>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

//! Check Ridder's method on simple function
std::string
math_root_test1
	()
{
	std::ostringstream oss;

	struct SomeFunc
	{
		double
		root
			() const
		{
			return 17.;
		}

		std::pair<double, double>
		bracket
			() const
		{
			return { 10., 20. };
		}

		double
		operator()
			( double const & xx
			) const
		{
			return (.001 * (xx - root()) * (xx + 3.) * (xx - 31.));
		}
	};

	SomeFunc const func;
	double const expRoot{ func.root() };

	// check 'typical' case
	double const gotRoot{ math::root::viaRidder(func, func.bracket()) };
	if (! dat::nearlyEquals(gotRoot, expRoot))
	{
		oss << "Failure of simple Ridder's test" << std::endl;
		oss << dat::infoString(expRoot, "expRoot") << std::endl;
		oss << dat::infoString(gotRoot, "gotRoot") << std::endl;
	}

	// root at edge of bracket
	std::pair<double, double> const braketL{ func.root(), func.root() + 1.};
	std::pair<double, double> const braketR{ func.root() - 1., func.root() };
	double const gotRootL{ math::root::viaRidder(func, braketL) };
	double const gotRootR{ math::root::viaRidder(func, braketR) };
	if ( (! dat::nearlyEquals(gotRootL, expRoot))
	  || (! dat::nearlyEquals(gotRootR, expRoot))
	   )
	{
		oss << "Failure of Ridder left/right edge test" << std::endl;
	}

	std::pair<double, double> const braketBad
		{ func.root() + 1. , func.root() + 2.};
	double const gotRootBad{ math::root::viaRidder(func, braketBad) };
	if (dat::isValid(gotRootBad))
	{
		oss << "Failure of Ridder bad bracket test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for math::root
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << math_root_test0();
	oss << math_root_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
