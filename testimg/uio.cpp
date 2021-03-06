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
\brief  This file contains unit test for img::io
*/


#include "libimg/io.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include "teststb/testfunc.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check JPEG image i/o
std::string
img_io_test0
	()
{
	std::ostringstream oss;

	using PixRGB = stb::testfunc::PixRGB;
	constexpr size_t const tolPixDif{ 1u }; // jpeg compression loss

	std::string const fpath{ "uio_foo.jpg" };

	// simulate image
	constexpr int const expHigh{ 731 };
	constexpr int const expWide{ 355 };
	dat::grid<PixRGB> const expGrid{ stb::testfunc::simRGB(expHigh, expWide) };
	dat::grid<PixRGB> gotGrid;

	// write to file
	constexpr int const expQual{ 100 };
	bool const okaySave{ img::io::saveJpg(expGrid, fpath, expQual) };

	// check write status
	if (! okaySave)
	{
		oss << "Failure of JPG write test" << std::endl;
	}

	else // if (okaySave); have test image in file
	{
		// read image back from file
		gotGrid = img::io::loadFromJpgRgb8(fpath);
	}

	// check contents of loaded data
	bool const okayLoad{ gotGrid.isValid() };
	if (okaySave && okayLoad)
	{
		assert(expGrid.size() == gotGrid.size()); // else above code is bad
		stb::testfunc::checkGrids
			(oss, expGrid, gotGrid, "jpg max pix dif", tolPixDif);
	}

	// cleanup test data
	std::remove(fpath.c_str());

	return oss.str();
}

//! Check RGB-PNG image i/o
std::string
img_io_test1
	()
{
	std::ostringstream oss;

	using PixRGB = stb::testfunc::PixRGB;
	constexpr size_t const tolPixDif{ 0u };

	std::string const fpath{ "uio_foo.png" };

	// simulate image
	constexpr int const expHigh{ 731 };
	constexpr int const expWide{ 355 };
	dat::grid<PixRGB> const expGrid{ stb::testfunc::simRGB(expHigh, expWide) };
	dat::grid<PixRGB> gotGrid;

	// write to file
	bool const okaySave{ img::io::savePng(expGrid, fpath) };

	// check write status
	if (! okaySave)
	{
		oss << "Failure of PNG write test" << std::endl;
	}

	else // if (okaySave); have test image in file
	{
		// read image back from file
		gotGrid = img::io::loadFromPngRgb8(fpath);
	}

	// check contents of loaded data
	bool const okayLoad{ gotGrid.isValid() };
	if (okaySave && okayLoad)
	{
		assert(expGrid.size() == gotGrid.size()); // else above code is bad
		stb::testfunc::checkGrids
			(oss, expGrid, gotGrid, "png max pix dif", tolPixDif);
	}

	// cleanup test data
	std::remove(fpath.c_str());

	return oss.str();
}

namespace split
{
	using PixRGB = stb::testfunc::PixRGB;

	//! Grid with value from the chan-th rgb channel of rgbGrid
	dat::grid<uint8_t>
	channelFrom
		( dat::grid<PixRGB> const & rgbGrid
		, size_t const & chan
		)
	{
		dat::grid<uint8_t> ugrid(rgbGrid.hwSize());
		dat::grid<PixRGB>::const_iterator itIn{ rgbGrid.begin() };
		dat::grid<uint8_t>::iterator itOut{ ugrid.begin() };
		while (rgbGrid.end() != itIn)
		{
			PixRGB const & pix = *itIn;
			*itOut = pix[chan];
			++itIn;
			++itOut;
		}
		return ugrid;
	}
}

//! Check Gray-PNG image i/o
std::string
img_io_test2
	()
{
	std::ostringstream oss;

	using PixRGB = stb::testfunc::PixRGB;
	constexpr size_t const tolPixDif{ 0u };

	std::string const fpath{ "uio_foo.png" };

	// simulate image
	constexpr int const expHigh{ 731 };
	constexpr int const expWide{ 355 };
	dat::grid<PixRGB> const rgbGrid{ stb::testfunc::simRGB(expHigh, expWide) };
	dat::grid<uint8_t> const expGrid{ split::channelFrom(rgbGrid, 0u) };
	dat::grid<uint8_t> gotGrid;

	// write to file
	bool const okaySave{ img::io::savePng(expGrid, fpath) };

	// check write status
	if (! okaySave)
	{
		oss << "Failure of PNG write test" << std::endl;
	}

	else // if (okaySave); have test image in file
	{
		// read image back from file
		gotGrid = img::io::loadFromPng8(fpath);
	}

	// check contents of loaded data
	bool const okayLoad{ gotGrid.isValid() };
	if (okaySave && okayLoad)
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

//! Unit test for img::io
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << img_io_test0();
	oss << img_io_test1();
	oss << img_io_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
