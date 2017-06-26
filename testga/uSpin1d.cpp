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
\brief  This file contains unit test for ga::Spin1d
*/


#include "libga/Spin1d.h"

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
ga_Spin1d_test0
	()
{
	std::ostringstream oss;
	ga::Spin1d const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check basic 1d solution family
std::string
ga_Spin1d_test1
	()
{
	std::ostringstream oss;

	using namespace ga;

	// Sample test directions
	std::vector<Vector> const someVecs
		{  e1,  e2,  e3
		, -e1, -e2, -e3
		, (e1+e2), (e1+e2+e3)
		, Vector( 3.,  4.,  5.)
		, Vector(-3., -4., -5.)
		, Vector( 5.,  3.,  4.)
		, Vector(-5., -3., -4.)
		};

	// test with all combinations
	for (Vector const & xVec : someVecs)
	for (Vector const & yVec : someVecs)
	{
		Vector const expIn{ ga::unit(xVec) };
		Vector const expOut{ ga::unit(yVec) };

		// construct functor for this rotation pair
		Spin1d const family(expIn, expOut);
		if (! family.isValid())
		{
			oss << "Failure of valid family test" << std::endl;
			oss << dat::infoString(expIn, "expIn") << std::endl;
			oss << dat::infoString(expOut, "expOut") << std::endl;
			goto EndTest;
		}

		// run the functor through a number of angles to verify matches
		constexpr double dw{ math::twoPi / 64. };
		for (double ww{0.} ; ww <= math::twoPi ; ww += dw)
		{
			// get angle at this value of the free parameter
			ga::BiVector const gotBiv{ family.argFor(ww) };

			// reconstitute spinor - and apply with passive convention
			ga::Spinor const gotR{ ga::exp(gotBiv) };
			ga::Vector const gotOut{ (gotR * expIn * gotR.reverse()).theV };

			// check if results match those expected
			if (! gotOut.nearlyEquals(expOut))
			{
				oss << "Failure of spinor consistency test" << std::endl;
				oss << dat::infoString(expIn, "expIn") << std::endl;
				oss << dat::infoString(ww, "ww") << std::endl;
				oss << dat::infoString(expOut, "expOut") << std::endl;
				oss << dat::infoString(gotOut, "gotOut") << std::endl;
				goto EndTest;
			}
		}
	}
	EndTest:

	return oss.str();
}


}

//! Unit test for ga::Spin1d
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ga_Spin1d_test0();
	oss << ga_Spin1d_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
