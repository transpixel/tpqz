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
\brief  This file contains unit test for dat::Area
*/


#include "libdat/Area.h"

#include "libdat/info.h"
#include "libdat/ops.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_Area_test0
	()
{
	std::ostringstream oss;
	dat::Area<int> const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check basic operations
std::string
dat_Area_test1
	()
{
	std::ostringstream oss;

	// get corners
	dat::Range<double> const xRange( -3.,  7.);
	dat::Range<double> const yRange( 23., 29.);
	dat::Area<double> const area{ xRange, yRange };

	std::array<double, 2u> const mins(area.minimums());
	std::array<double, 2u> const maxs(area.maximums());

	dat::Spot const expCenter
		{{ .5*(mins[0] + maxs[0]), .5*(mins[1] + maxs[1]) }};
	dat::Spot const gotCenter(area.center());
	if (! dat::nearlyEquals(gotCenter, expCenter))
	{
		oss << "Failure of center test" << std::endl;
		oss << dat::infoString(expCenter, "expCenter") << std::endl;
		oss << dat::infoString(gotCenter, "gotCenter") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for dat::Area
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_Area_test0();
	oss << dat_Area_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
