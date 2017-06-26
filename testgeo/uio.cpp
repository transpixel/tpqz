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
\brief  This file contains unit test for geo::io
*/


#include "libgeo/io.h"

#include "libdat/dat.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
geo_io_test0
	()
{
	std::ostringstream oss;
	/*
	geo::io const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	*/
	return oss.str();
}

//! Check CSV data operations
std::string
geo_io_test1
	()
{
	std::ostringstream oss;

	ga::Vector const p0(-1., -2., 3.);
	ga::Vector const dp(.125, .250, -.750);
	std::vector<ga::Vector> const expPnts
		{ (p0 + 0.*dp)
		, (p0 + 1.*dp)
		, (p0 + 2.*dp)
		, (p0 + 3.*dp)
		, (p0 + 4.*dp)
		, (p0 + 5.*dp)
		};

	std::ostringstream ostm;
	bool const okSave(geo::io::saveToCSV(ostm, expPnts));
	if (! okSave)
	{
		oss << "Failure of point save test" << std::endl;
	}

	std::istringstream istm(ostm.str());
	std::vector<ga::Vector> const gotPnts(geo::io::loadFromCSV(istm));

	if (! dat::nearlyEquals(gotPnts, expPnts, dat::smallValue<double>()))
	{
		oss << "Failure of point load test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for geo::io
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << geo_io_test0();
	oss << geo_io_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
