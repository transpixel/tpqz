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
\brief  This file contains unit test for dat::RowCol
*/


#include "libdat/array.h"
#include "libdat/dat.h"
#include "libdat/info.h"

#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>

namespace
{

//! Check for common functions
std::string
dat_RowCol_test0
	()
{
	std::ostringstream oss;
	dat::RowCol const aNull(dat::nullValue<dat::RowCol>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull, "aNull") << std::endl;
	}
	return oss.str();
}

//! Check basic operations
std::string
dat_RowCol_test1
	()
{
	std::ostringstream oss;
	dat::RowCol const rcA{{ 7u, 4u }};
	dat::RowCol const rcB{{ 3u, 9u }};

	dat::RowCol const expSum{{ 10u, 13u }};
	using dat::operator+;
	dat::RowCol const gotSum(rcA + rcB);
	if (! dat::nearlyEquals(gotSum, expSum))
	{
		oss << "Failure of simple addition test" << std::endl;
	}

	using dat::operator-;
	dat::RowCol const & expDif = rcA;
	dat::RowCol const gotDif(expSum - rcB);
	if (! dat::nearlyEquals(gotDif, expDif))
	{
		oss << "Failure of simple subtraction test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for dat::RowCol
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_RowCol_test0();
	oss << dat_RowCol_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
