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
\brief  This file contains unit test for ext::stb
*/


#include "teststb/testfunc.h"

#include "extstb/stb_image.h"
#include "extstb/stb_image_write.h"

#include "libdat/array.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"
#include "libmath/math.h"

#include "libdat/grid.h"

#include <array>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check JPEG
std::string
img_stb_test0
	()
{
	std::ostringstream oss;

	using PixRGB = stb::testfunc::PixRGB;
	constexpr size_t const tolPixDif{ 1u }; // jpeg compression loss

	std::string const fpath{ "ustb_foo.jpg" };

	// simulate image
	constexpr int const expHigh{ 73 };
	constexpr int const expWide{ 35 };
	constexpr int const expDeep{ 3 };
	dat::grid<PixRGB> const expGrid{ stb::testfunc::simRGB(expHigh, expWide) };
	dat::grid<PixRGB> gotGrid;

	// write to file
	constexpr int const expQual{ 100 };
	int const wstat
		{ stbi_write_jpg
			(fpath.c_str(), expWide, expHigh, expDeep, expGrid.begin(), expQual)
		};

	// check write status
	bool const okayWrite{ (0 < wstat) };
	if (! okayWrite)
	{
		oss << "Failure of stbi_write_jpg test" << std::endl;
		oss << "wstat = " << wstat << std::endl;
	}

	else // if (okayWrite); have test image in file
	{
		// read jpeg image
		int gotWide{}, gotHigh{}, gotDeep{};
		unsigned char * imgdat
			{ stbi_load(fpath.c_str(), &gotWide, &gotHigh, &gotDeep, expDeep) };
		gotGrid = stb::testfunc::checkedGridFrom
			(oss, gotHigh, gotWide, gotDeep, imgdat, "JPG reload", expGrid);
		stbi_image_free(imgdat);
	}

	// check contents of loaded data
	bool const okayReadBack{ gotGrid.isValid() };
	if (okayWrite && okayReadBack)
	{
		assert(expGrid.size() == gotGrid.size()); // else above code is bad
		stb::testfunc::checkGrids
			(oss, expGrid, gotGrid, "jpg max pix dif", tolPixDif);
	}

	// cleanup test data
	std::remove(fpath.c_str());

	return oss.str();
}

//! Check PNG image i/o
std::string
img_stb_test1
	()
{
	std::ostringstream oss;

	using PixRGB = stb::testfunc::PixRGB;
	constexpr size_t const tolPixDif{ 0u }; // png lossless encoding

	std::string const fpath{ "ustb_foo.png" };

	// simulate image
	constexpr int const expHigh{ 73 };
	constexpr int const expWide{ 35 };
	constexpr int const expDeep{ 3 };
	dat::grid<PixRGB> const expGrid{ stb::testfunc::simRGB(expHigh, expWide) };
	dat::grid<PixRGB> gotGrid;

	// write to file
	stbi_write_png_compression_level = 9;
	int const rowBytes{ expWide * expDeep * sizeof(uint8_t) };
	void const * const ptData{ expGrid.begin() };
	int const wstat
		{ stbi_write_png
			(fpath.c_str(), expWide, expHigh, expDeep, ptData, rowBytes)
		};

	// check write status
	bool const okayWrite{ (0 < wstat) };
	if (! okayWrite)
	{
		oss << "Failure of stbi_write_png test" << std::endl;
		oss << "wstat = " << wstat << std::endl;
	}

	else // if (okayWrite); have test image in file
	{
		// read jpeg image
		int gotWide{}, gotHigh{}, gotDeep{};
		unsigned char * imgdat
			{ stbi_load(fpath.c_str(), &gotWide, &gotHigh, &gotDeep, expDeep) };
		gotGrid = stb::testfunc::checkedGridFrom
			(oss, gotHigh, gotWide, gotDeep, imgdat, "PNG reload", expGrid);
		stbi_image_free(imgdat);
	}

	// check contents of loaded data
	bool const okayReadBack{ gotGrid.isValid() };
	if (okayWrite && okayReadBack)
	{
		assert(expGrid.size() == gotGrid.size()); // else above code is bad
		stb::testfunc::checkGrids
			(oss, expGrid, gotGrid, "png max pix dif", tolPixDif);
	}

	// cleanup test data
	std::remove(fpath.c_str());

	return oss.str();
}


}

//! Unit test for ext::stb
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << img_stb_test0();
	oss << img_stb_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
