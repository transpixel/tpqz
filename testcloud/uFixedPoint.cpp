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
\brief  This file contains unit test for cloud::FixedPoint
*/


#include "libcloud/FixedPoint.h"

#include "libcloud/cast.h"
#include "libdat/array.h"
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
cloud_FixedPoint_test0
	()
{
	std::ostringstream oss;
	cloud::FixedPoint const aNull(dat::nullArray<cloud::FixedPoint>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check basic point en/de-coding operations
std::string
cloud_FixedPoint_test1
	()
{
	std::ostringstream oss;

	ga::Vector const expVec( 1., -2., 3.);
	cloud::FixedPoint const ppoint(cloud::cast::FixedPoint(expVec));
	ga::Vector const gotVec(cloud::cast::Vector(ppoint));

	if (! gotVec.nearlyEquals(expVec))
	{
		oss << "Failure of pack/unpack test" << std::endl;
		oss << dat::infoString(ppoint, "ppoint") << std::endl;
		oss << dat::infoString(gotVec, "gotVec") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for cloud::FixedPoint
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << cloud_FixedPoint_test0();
	oss << cloud_FixedPoint_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
