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
\brief  This file contains unit test for ga::constructs
*/


#include "libga/constructs.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check basic operators
std::string
ga_constructs_test0
	()
{
	std::ostringstream oss;

	using namespace ga;
	// bivector/bivector dot product
	{
		BiVector const bivA( -3., 5., 7.);
		BiVector const bivB( 13., 17., -11.);

		Scalar const expDot{ .5*(bivA*bivB + bivB*bivA).theS };
		Scalar const gotDot{ dot(bivA, bivB) };
		if (! gotDot.nearlyEquals(expDot))
		{
			oss << "Failure of dot(biv,biv) test" << std::endl;
			oss << dat::infoString(expDot, "expDot") << std::endl;
			oss << dat::infoString(gotDot, "gotDot") << std::endl;
		}
	}

	// vector/bivector dot products
	{
		Vector const aa( -3., 5., 7.);
		BiVector const bb( 13., 17., -11.);

		Vector const expAB{ .5*(aa*bb - bb*aa).theV };
		Vector const expBA{ .5*(bb*aa - aa*bb).theV };
		Vector const gotAB{ dot(aa, bb) };
		Vector const gotBA{ dot(bb, aa) };

		if (! gotAB.nearlyEquals(expAB))
		{
			oss << "Failure of dot(vec,biv) test" << std::endl;
			oss << dat::infoString(expAB, "expAB") << std::endl;
			oss << dat::infoString(gotAB, "gotAB") << std::endl;
		}
		if (! gotBA.nearlyEquals(expBA))
		{
			oss << "Failure of dot(biv,vec) test" << std::endl;
			oss << dat::infoString(expBA, "expBA") << std::endl;
			oss << dat::infoString(gotBA, "gotBA") << std::endl;
		}
	}

	return oss.str();
}

//! Check TODO....
std::string
ga_constructs_test1
	()
{
	std::ostringstream oss;
//	oss << "Failure: implement this test!" << std::endl;
	return oss.str();
}


}

//! Unit test for ga::constructs
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ga_constructs_test0();
	oss << ga_constructs_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
