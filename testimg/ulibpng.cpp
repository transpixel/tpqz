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
\brief  This file contains unit test for img::libpng
*/


#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include "libdat/Extents.h"
#include "libdat/ExtentsIterator.h"
#include "libdat/grid.h"

#include "extpng/png++-0.2.9/png.hpp"

#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <sstream>
#include <string>

#include <stdio.h>

// libpng:
// http://www.piko3d.net/tutorials/libpng-tutorial-loading-png-files-from-streams/#CustomRead
//

//! Check basic API use
std::string
img_libpng_test0
	()
{
	std::ostringstream oss;

	return oss.str();
}

/*
//! Check TODO....
std::string
img_libpng_test1
	()
{
	std::ostringstream oss;
	oss << "Failure: implement this test!" << std::endl;
	return oss.str();
}
*/


size_t
numChanForColorType
	( png::color_type const ctype
	)
{
	size_t numChan{ dat::nullValue<size_t>() };
	using ColorSize = std::pair<png::color_type, size_t>;
	static size_t const sNullSize{ dat::nullValue<size_t>() };
	static std::map<png::color_type, size_t> const sizeMap
		{ ColorSize{ png::color_type_none, sNullSize }
		, ColorSize{ png::color_type_gray, sNullSize }
		, ColorSize{ png::color_type_palette, sNullSize }
		, ColorSize{ png::color_type_rgb, 3u }
		, ColorSize{ png::color_type_rgb_alpha, sNullSize }
		, ColorSize{ png::color_type_gray_alpha, sNullSize }
		, ColorSize{ png::color_type_rgba, sNullSize }
		, ColorSize{ png::color_type_ga, sNullSize }
		};
	std::map<png::color_type, size_t>::const_iterator const itFind
		{ sizeMap.find(ctype) };
	if (sizeMap.end() != itFind)
	{
		numChan = itFind->second;
	}
	return numChan;
}

inline
size_t
numChannels
	( png::info const & info
	)
{
	return numChanForColorType(info.get_color_type());
}

inline
size_t
bytesPerChannel
	( png::info const & info
	)
{
	return { (size_t)info.get_bit_depth() / 8u };
}

inline
size_t
bytesPerPixel
	( png::info const & info
	)
{
	return bytesPerChannel(info) * numChannels(info);
}

inline
dat::Extents
hwSizeFor
	( png::info const & info
	)
{
	return dat::Extents
		{ (size_t)info.get_height(), (size_t)info.get_width() };
}

void
experiment
	( std::string const & fname
	)
{
	// get basic information
	std::ifstream ifs(fname);
	png::reader<std::ifstream> reader(ifs);
	reader.read_info();

	png::info const & info = reader.get_info();
	dat::Extents const hwSize{ hwSizeFor(info) };
	size_t const numChan{ numChannels(info) };
	size_t const bytesPerChan{ bytesPerChannel(info) };
	size_t const bytesPerPix{ bytesPerPixel(info) };
	size_t const bytesTotal{ bytesPerPix * hwSize.size() };

io::out() << dat::infoString(numChan, "numChan") << std::endl;
io::out() << dat::infoString(hwSize, "hwSize") << std::endl;
io::out() << dat::infoString(bytesPerChan, "bytesPerChan") << std::endl;
io::out() << dat::infoString(bytesPerPix, "bytesPerPix") << std::endl;
io::out() << dat::infoString(bytesTotal, "bytesTotal") << std::endl;

	// instantiate image of supported type
	if ( (png::color_type_rgb == info.get_color_type())
	  && (2u == bytesPerChan)
	   )
	{
		using rgb16_t = png::rgb_pixel_16;
		ifs.clear();
		ifs.seekg(0);
		png::image<rgb16_t> img(ifs);


		// as grid structure
		dat::grid<std::array<float, 3u> > fgrid(hwSize);

		// copy data
		//  - should be able to read directly into fgrid buffer
		//  - or at least do a buffer copy
		//  - or at least a row by row copy
		//  ... TODO figure out png++ syntax
		for (dat::ExtentsIterator iter{hwSize} ; iter ; ++iter)
		{
			dat::RowCol const & rowcol = *iter;
			rgb16_t const rgbPix{ img.get_pixel(rowcol[1], rowcol[0]) };
			std::array<float, 3u> & outPix = fgrid(rowcol);
			outPix[0] = rgbPix.red;
			outPix[1] = rgbPix.green;
			outPix[2] = rgbPix.blue;
		}

		img.write("out.png");
	}
	else
	{
		assert(png::color_type_rgb == info.get_color_type());
		assert(2u == bytesPerChan);
	}
}

//! Unit test for img::libpng
int
main
//	( int const /*argc*/
//	, char const * const * /*argv*/
//	)
( int const argc
, char const * const * argv
)
{
	std::ostringstream oss;

	assert(2u == argc);
	std::string const fname( argv[1] );
	io::out() << "arg:fname: " << fname << std::endl;

	experiment(fname);

	// run tests
//	oss << img_libpng_test0();
//	oss << img_libpng_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
