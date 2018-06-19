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
\brief  This file contains unit test for img::iio
*/


extern "C"
{
#include "extiio/iio.h"
}

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include "libdat/grid.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check simple file write/read round trip
std::string
img_iio_test0
	()
{
	std::ostringstream oss;

	dat::grid<float> agrid(100u, 200u);
	std::fill(agrid.begin(), agrid.end(), 0.f);
	float * const data = agrid.begin();
	char cname[] = "iio_image.png";
	iio_write_image_float(cname, data, 200, 100);

	/*
	std::string const fname("iio_image.ppm");
	int wide{};
	int high{};
	float * buf{ iio_read_image_float(fname.c_str(), &wide, &high) };
	io::out() << dat::infoString(buf, "buf") << std::endl;
	*/

oss << "Failure: implement this test!" << std::endl;
	return oss.str();
}


}

//! Unit test for img::iio
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << img_iio_test0();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
