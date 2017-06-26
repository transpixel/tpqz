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
\brief  This file contains unit test for math::QuadEq
*/


#include "libmath/QuadEq.h"

#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
math_QuadEq_test0
	()
{
	std::ostringstream oss;
	math::QuadEq const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

	void
	checkRoots
		( std::ostream & oss
		, double const & gotMin
		, double const & gotMax
		, double const & expMin
		, double const & expMax
		, std::string const & tname
		)
	{
		if (! dat::nearlyEquals(gotMin, expMin))
		{
			oss << "Failure of realRootMin test: " << tname << std::endl;
			oss << dat::infoString(expMin, "expMin") << std::endl;
			oss << dat::infoString(gotMin, "gotMin") << std::endl;
		}
		if (! dat::nearlyEquals(gotMax, expMax))
		{
			oss << "Failure of realRootMax test: " << tname << std::endl;
			oss << dat::infoString(expMax, "expMax") << std::endl;
			oss << dat::infoString(gotMax, "gotMax") << std::endl;
		}
		if (! (gotMin < gotMax))
		{
			oss << "Failure of root ordering test: " << tname << std::endl;
			oss << dat::infoString(gotMin, "gotMin") << std::endl;
			oss << dat::infoString(gotMax, "gotMax") << std::endl;
		}
	}

//! Check basic root finding
std::string
math_QuadEq_test1
	()
{
	std::ostringstream oss;

	{
		// 2 * (x - 7) * (x + 3) = 2*(x*x + x*3 -7*x -7*3)
		// 2*(x*x -4*x -21)
		// (2)x*x +(-8)*x +(-42)
		double const expMin{ -3. };
		double const expMax{  7. };
		math::QuadEq const eqn( 2., -8., -42. );
		double const gotMin{ eqn.realRootMin() };
		double const gotMax{ eqn.realRootMax() };

		checkRoots(oss, gotMin, gotMax, expMin, expMax, "negB");

		double const expMinPos{ expMax };
		double const gotMinPos{ eqn.realRootMinPos() };
		if (! dat::nearlyEquals(gotMinPos, expMinPos))
		{
			oss << "Failure of minPos case (-3,7)" << std::endl;
		}
	}
	{
		// 2 * (x + 7) * (x - 3)
		// (2)x*x +(+8)*x +(-42)
		double const expMin{ -7. };
		double const expMax{  3. };
		math::QuadEq const eqn( 2.,  8., -42. );
		double const gotMin{ eqn.realRootMin() };
		double const gotMax{ eqn.realRootMax() };

		checkRoots(oss, gotMin, gotMax, expMin, expMax, "posB");
	}

	{
		// 2 * (x - 7) * (x - 3)
		// (2)x*x +(-20)*x +(42)
		double const expMin{  3. };
		double const expMax{  7. };
		math::QuadEq const eqn( 2.,  -20., 42. );
		double const gotMin{ eqn.realRootMin() };
		double const gotMax{ eqn.realRootMax() };

		checkRoots(oss, gotMin, gotMax, expMin, expMax, "dualPos");

		double const expMinPos{ expMin };
		double const gotMinPos{ eqn.realRootMinPos() };
		if (! dat::nearlyEquals(gotMinPos, expMinPos))
		{
			oss << "Failure of minPos case (-3,-7)" << std::endl;
		}
	}

	{
		// 2 * (x + 7) * (x + 3)
		// (2)x*x +(+20)*x +(42)
		double const expMin{ -7. };
		double const expMax{ -3. };
		math::QuadEq const eqn( 2., 20., 42. );
		double const gotMin{ eqn.realRootMin() };
		double const gotMax{ eqn.realRootMax() };

		checkRoots(oss, gotMin, gotMax, expMin, expMax, "dualNeg");

		double const gotMinPos{ eqn.realRootMinPos() };
		if (  dat::isValid(gotMinPos))
		{
			oss << "Failure of minPos case (-3,-7)" << std::endl;
		}
	}

	return oss.str();
}


}

//! Unit test for math::QuadEq
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << math_QuadEq_test0();
	oss << math_QuadEq_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
