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

#ifndef img_io_INCL_
#define img_io_INCL_

/*! \file
\brief Declarations for img::io
*/


#include "libimg/pixel.h"
#include "libdat/grid.h"

#include <array>


namespace img
{
//! \brief Functions and operations related to img object I/O.
namespace io
{
	/*
	//! Load Raw8 (bayer) image from PGM file - valid image if success
	dat::grid<uint16_t>
	loadFromPgm
		( std::string const & fpath
		);
	*/

	//! Specialization of loadFromPgm for 8-bit pgm / little endian
	dat::grid<uint8_t>
	loadFromPgm8
		( std::string const & fpath
		);

	//! Load 8-bit RGB JPEG image
	dat::grid<std::array<uint8_t, 3> >
	loadFromJpgRgb8
		( std::string const & fpath
		);

	//! Load 8-bit RGB PNG image
	dat::grid<std::array<uint8_t, 3> >
	loadFromPngRgb8
		( std::string const & fpath
		);

	//! Load 8-bit Gray PNG image
	dat::grid<uint8_t>
	loadFromPng8
		( std::string const & fpath
		);

	//! Load from floating point format - valid on success
	dat::grid<float>
	loadFromFloat
		( std::string const & fpath
		);

	//! Save to floating point format - true on success
	bool
	saveToFloat
		( dat::grid<float> const & grid
		, std::string const & fpath
		);

	//! Save grid to ascii file in <row col value>
	bool
	saveToXyz
		( dat::grid<float> const & grid
		, std::string const & filename
		, size_t const & stride = 1u
		, std::string const & valueFmt = std::string("%6.3f")
		);

	//! Save gray image as PGM
	bool
	savePgm
		( dat::grid<uint8_t> const & grid
		, std::string const & filename
		);

	//! Save color image as PPM
	bool
	savePpm
		( std::array<dat::grid<uint8_t>, 3u> const & srgbGrids
		, std::string const & filename
		);

	//! Save gray image as PGM
	bool
	savePgmAutoScale
		( dat::grid<float> const & fgrid
		, std::string const & filename
		);

	//! Save 3-channel data to JPEG format
	bool
	saveJpg // TODO - could templatize
		( dat::grid<std::array<uint8_t, 3u> > const & rgbGrid
		, std::string const & fpath
		, size_t const & qualPercent = { 80u }
		);

	//! Save one-channel data to PNG format
	bool
	savePng // TODO - could templatize
		( dat::grid<uint8_t> const & ugrid
		, std::string const & fpath
		);

	//! Save 3-channel data to PNG format
	bool
	savePng // TODO - could templatize
		( dat::grid<std::array<uint8_t, 3u> > const & rgbGrid
		, std::string const & fpath
		);

}

}

// Inline definitions
// #include "libimg/io.inl"

#endif // img_io_INCL_

