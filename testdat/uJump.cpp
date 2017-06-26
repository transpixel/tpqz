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
\brief  This file contains unit test for dat::Jump
*/


#include "libdat/Jump.h"

#include "libdat/compare.h"
#include "libdat/info.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_Jump_test0
	()
{
	std::ostringstream oss;
	dat::Jump const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check basic operations
std::string
dat_Jump_test1
	()
{
	std::ostringstream oss;

	// configure simple case
	//                              ^   -    v    ^   ^   -   ^
	//                              0   1    2    3   4   5   6
	std::vector<double> const data{ 2., 2., -5., -7., 8., 8., 9. };
	std::vector<dat::Jump> const expJs
		{ dat::Jump(1,  2., -5.)
		, dat::Jump(2, -5., -7.)
		, dat::Jump(3, -7.,  8.)
		, dat::Jump(5,  8.,  9.)
		};

	// check values
	std::vector<dat::Jump> const gotJs
		(dat::Jump::fromData<double>(data.begin(), data.end()));
	if (! (gotJs.size() == expJs.size()))
	{
		oss << "Failure of jump size test" << std::endl;
		oss << "exp: " << expJs.size() << std::endl;
		oss << "got: " << gotJs.size() << std::endl;
	}
	else
	{
		size_t const numJs(expJs.size());
		for (size_t nn(0u) ; nn < numJs ; ++nn)
		{
			dat::Jump const & expJ = expJs[nn];
			dat::Jump const & gotJ = gotJs[nn];
			if (! gotJ.nearlyEquals(expJ))
			{
				oss << "Failure of jump value test" << std::endl;
				oss << dat::infoString(expJ, "expJ") << std::endl;
				oss << dat::infoString(gotJ, "gotJ") << std::endl;
			}
		}
	}

	return oss.str();
}


}

//! Unit test for dat::Jump
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_Jump_test0();
	oss << dat_Jump_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
