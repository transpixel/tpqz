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
\brief  This file contains unit test for geo::stats
*/


#include "libgeo/stats.h"

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
geo_stats_test0
	()
{
	std::ostringstream oss;
	/*
	geo::stats const aNull(dat::nullValue<geo::stats>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

//! Check centroid operations
std::string
geo_stats_test1
	()
{
	std::ostringstream oss;

	std::vector<ga::Vector> none{};
	ga::Vector const goNone{ geo::stats::centroidOf(none.begin(), none.end()) };
	if (  dat::isValid(goNone))
	{
		oss << "Failure of centroid empty test" << std::endl;
	}

	ga::Vector const expCen(17., 23., -19.);
	std::vector<ga::Vector> const sym
		{ expCen + ga::Vector( -5.,  3., -7. )
		, expCen + ga::Vector(  5., -3.,  7. )
		, expCen + ga::Vector( -2., -4.,  8. )
		, expCen + ga::Vector(  2.,  4., -8. )
		};
	ga::Vector const gotCen{ geo::stats::centroidOf(sym.begin(), sym.end()) };
	if (! gotCen.nearlyEquals(expCen))
	{
		oss << "Failure of centroid sym test" << std::endl;
		oss << dat::infoString(expCen, "expCen") << std::endl;
		oss << dat::infoString(gotCen, "gotCen") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for geo::stats
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << geo_stats_test0();
	oss << geo_stats_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
