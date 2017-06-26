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
\brief  This file contains unit test for cloud::io
*/


#include "libcloud/io.h"
#include "libdat/io.h"

#include "libdat/Extents.h"
#include "libdat/grid.h"
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
cloud_io_test0
	()
{
	std::ostringstream oss;
	/*
	cloud::io const aNull(dat::nullValue<cloud::io>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

//! Check uint32 i/o operations
std::string
cloud_io_test1
	()
{
	std::ostringstream oss;

	// create artificial data
	dat::Extents const hwSize(5u, 7u);
	dat::grid<cloud::NdxType> expGrid(hwSize);
	std::iota(expGrid.begin(), expGrid.end(), 100u);

	// save to stream
	std::ostringstream obuf;
	using OutIter = dat::grid<cloud::NdxType>::const_iterator;
	dat::io::saveBinary<OutIter>(obuf, expGrid.begin(), expGrid.end());

	// copy stream contents
	std::istringstream ibuf(obuf.str());

	// read from stream
	using InIter = dat::grid<cloud::NdxType>::iterator;
	dat::grid<cloud::NdxType> gotGrid(hwSize);
	dat::io::loadBinary<InIter>(ibuf, gotGrid.begin(), gotGrid.end());

	// check round trip
	if (! dat::nearlyEquals<cloud::NdxType>
		( gotGrid.begin(), gotGrid.end()
		, expGrid.begin(), expGrid.end()
		)
	   )
	{
		oss << "Failure of grid save/load test" << std::endl;
		oss << dat::infoString(expGrid, "expGrid") << std::endl;
		oss << dat::infoString(gotGrid, "gotGrid") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for cloud::io
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << cloud_io_test0();
	oss << cloud_io_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
