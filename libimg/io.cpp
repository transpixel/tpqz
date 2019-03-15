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
\brief Definitions for img::io
*/


#include "libimg/io.h"

#include "libdat/algorithm.h"
#include "libimg/convert.h"
#include "libimg/img.h"
#include "libimg/stats.h"
#include "libio/sprintf.h"
#include "libio/string.h"

#include "extstb/stb.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>



namespace img
{
namespace io
{
//======================================================================

/* TODO - replace with stb_image call - or direct binary io (like Pgm8)
dat::grid<uint16_t>
loadFromPgm
	( std::string const & fpath
	)
{
	dat::grid<uint16_t> image16;
	cv::Mat const cvMat(cv::imread(fpath));
	if (! cvMat.empty())
	{
		// check for valid sizes
		int const cvHigh(cvMat.rows);
		int const cvWide(cvMat.cols);
		if ((0 < cvHigh) && (0 < cvWide))
		{
			// allocate space
			size_t const high((size_t)cvHigh);
			size_t const wide((size_t)cvWide);
			dat::grid<uint8_t> image8(high, wide);
			size_t const dSize((high * wide));

			// copy/convert contents
			image16 = dat::grid<uint16_t>(high, wide);
			std::transform
				( cvMat.data, cvMat.data+dSize
				, image16.begin()
				, [] (uint8_t const & pix8) { return (uint16_t)pix8; }
				);
		}
	}
	return image16;
}
*/

namespace
{
	static std::string const sHeaderString
		( std::string("P0123456789")
		+ std::string(::io::string::sWhiteSpace)
		);

	//! True if cc is expected in PGM header
	inline
	bool
	isInPgmHeader
		( char const & cc
		)
	{
		return ::io::string::contains(sHeaderString, cc);
	}


	//! parse first four 'ascii' fields
	std::vector<std::string>
	headerFields
		( std::istream & iss
		)
	{
		std::vector<std::string> fields;
		char cc('\0');
		std::string buffer;
		while (iss && (! iss.eof()))
		{
			// get next char
			iss.get(cc);
			if (isInPgmHeader(cc))
			{
				// add to buffer
				if (::io::string::isWhiteSpace(cc) && (! buffer.empty()))
				{
					fields.push_back(buffer);
					buffer.clear();
				}
				else
				{
					buffer += cc;
				}
			}
			else
			{
				// error: encounter non-header char before end of header
				fields.clear();
				break;
			}

			// stop when header has been read
			if (4u == fields.size())
			{
				break;
			}
		}

		return fields;
	}
}

dat::grid<uint8_t>
loadFromPgm8
	( std::string const & fpath
	)
{
	dat::grid<uint8_t> image8;
	std::ifstream ifs(fpath, std::ios::binary);
	if (ifs.good())
	{
		// read header info
		std::vector<std::string> const header(headerFields(ifs));
		if (4u == header.size())
		{
			assert("P5" == header[0]);
			std::string const & widestr = header[1];
			std::string const & highstr = header[2];
			assert("255" == header[3]);

			// allocate space
			size_t const high(::io::string::from<size_t>(highstr, 0u));
			size_t const wide(::io::string::from<size_t>(widestr, 0u));
			if ((0u < high) && (0u < wide))
			{
				// allocate space for read operation
				image8 = dat::grid<uint8_t>(high, wide);

				// read data
				size_t const expBytes(image8.byteSize());
				ifs.read((char * const)image8.begin(), expBytes);

				// if not enough to complete the image...
				size_t const gotBytes(ifs.gcount());
				if (gotBytes != expBytes)
				{
					// .. return null data
					image8 = dat::grid<uint8_t>{};
				}
			}
		}
	}
	return image8;
}

namespace
{
	//! Load 8-bit per channel RGB data using extstb
	dat::grid<std::array<uint8_t, 3> >
	loadRgb8_stb
		( std::string const & fpath
		)
	{
		dat::grid<std::array<uint8_t, 3> > grid;
		int high{}, wide{}, deep{};
		unsigned char * imgdat{ nullptr };
		{ // guarded
			std::lock_guard<std::mutex> lock(stb::gStbMutex);
			imgdat = stbi_load(fpath.c_str(), &wide, &high, &deep, 3);
		}
		if (imgdat && (3u == deep))
		{
			grid = convert::gridRgb8From(high, wide, imgdat);
		}
		if (imgdat)
		{
			stbi_image_free(imgdat);
		}
		return grid;
	}
}

dat::grid<std::array<uint8_t, 3> >
loadFromJpgRgb8
	( std::string const & fpath
	)
{
	return loadRgb8_stb(fpath);
}

dat::grid<std::array<uint8_t, 3> >
loadFromPngRgb8
	( std::string const & fpath
	)
{
	return loadRgb8_stb(fpath);
}

dat::grid<uint8_t>
loadFromPng8
	( std::string const & fpath
	)
{
	dat::grid<uint8_t> grid;
	if (! fpath.empty())
	{
		int wide{}, high{}, deep{};
		unsigned char * imgdat{ nullptr };
		{ // guarded
			std::lock_guard<std::mutex> lock(stb::gStbMutex);
			imgdat = stbi_load(fpath.c_str(), &wide, &high, &deep, 1);
		}
		if (imgdat && (1u == deep))
		{
			grid = dat::grid<uint8_t>(high, wide);
			size_t const byteSize{ high * wide * sizeof(unsigned char) };
			std::transform
				( imgdat, imgdat + byteSize
				, grid.begin()
				, [] (unsigned char const & pix)
					{ return static_cast<uint8_t>(pix); }
				);
		}
		if (imgdat)
		{
			stbi_image_free(imgdat);
		}
	}
	return grid;
}


namespace
{
	//! Container for image dimensions
	struct Format
	{
		size_t theHigh;
		size_t theWide;
	};

	//! Simple header for floating point image format
	struct FloatHeader
	{
		union
		{
			Format theFormat;
			char theHeader[sizeof(Format)];
		};
	};

	//! Return header for specific image size
	inline
	FloatHeader
	headerFor
		( dat::grid<float> const & grid
		)
	{
		FloatHeader header;
		header.theFormat.theHigh = grid.high();
		header.theFormat.theWide = grid.wide();
		return header;
	}

	//! Number of bytes header requires for storage
	inline
	size_t
	headerSize
		()
	{
		return (sizeof(FloatHeader));
	}

	//! Number of bytes image requires for storage
	inline
	size_t
	imageSize
		( FloatHeader const & header
		)
	{
		return
			( header.theFormat.theHigh
			* header.theFormat.theWide
			* sizeof(float)
			);
	}

}



dat::grid<float>
loadFromFloat
	( std::string const & fpath
	)
{
	dat::grid<float> grid;
	std::ifstream ifs(fpath, std::ios::binary);
	if (ifs.good())
	{
		// space for header
		FloatHeader header;
		// read header
		size_t const expNumHdr(headerSize());
		ifs.read(header.theHeader, (long)expNumHdr);
		size_t const gotNumHdr((size_t)ifs.gcount());
		if (gotNumHdr == expNumHdr)
		{
			// allocate image storage
			grid = dat::grid<float>
				(header.theFormat.theHigh, header.theFormat.theWide);
			// read image
			size_t const expNumImg(imageSize(header));
			ifs.read((char*)grid.begin(), (long)expNumImg);
			size_t const gotNumImg((size_t)ifs.gcount());
			if (! (gotNumImg == expNumImg))
			{
				// else return null
				grid = dat::grid<float>{};
			}
		}
	}
	return grid;
}

bool
saveToFloat
	( dat::grid<float> const & grid
	, std::string const & fpath
	)
{
	bool okay(false);
	if (grid.isValid())
	{
		// configure header
		FloatHeader const header(headerFor(grid));
		// open file
		std::ofstream ofs(fpath, std::ios::binary);
		// write header
		ofs.write(header.theHeader, sizeof(FloatHeader));
		// write image
		ofs.write((char*)grid.begin(), imageSize(header));
		// check status
		okay = (! ofs.fail());
	}
	return okay;
}

bool
saveToXyz
	( dat::grid<float> const & grid
	, std::string const & filename
	, size_t const & stride
	, std::string const & valueFmt
	)
{
	bool okay(false);

	size_t const strideMax(std::min(grid.high(), grid.wide()));
	std::pair<size_t, size_t> const strideMinMax(1u, strideMax);
	size_t const rcDelta(dat::clamped(stride, strideMinMax));

	std::ofstream ofs(filename);
	if (grid.isValid() && (! ofs.fail()))
	{
		size_t const high(grid.high());
		size_t const wide(grid.wide());

		// loop over input data and save to file
		ofs << "# high: " << high << std::endl;
		ofs << "# wide: " << wide << std::endl;
		for (size_t row(0u) ; row < high ; row += rcDelta)
		{
			ofs << std::endl;
			for (size_t col(0u) ; col < wide ; col += rcDelta)
			{
				float const & value = grid(row, col);
				ofs
					<< row
					<< " " << col
					<< " " << ::io::sprintf(valueFmt, value)
					<< std::endl;
			}
			ofs << std::endl;
		}

		okay = (! ofs.fail());
	}

	return okay;
}

bool
savePgm
	( dat::grid<uint8_t> const & grid
	, std::string const & filename
	)
{
	bool okay(false);

	// check for valid input
	if (grid.isValid())
	{
		// open file for output
		std::ofstream ofs(filename, std::ios::binary);
		if (ofs)
		{
			// write header
			dat::Extents const inSize(grid.hwSize());
			std::ostringstream oss;
			oss << "P5" << '\n'
				<< inSize.wide() << '\n' // col, row order for PGM
				<< inSize.high() << '\n'
				<< (int)std::numeric_limits<uint8_t>::max() << '\n'
				;
			std::string const header(oss.str());
			ofs.write(header.data(), header.size());

			// write data - for 8-bit, endian-ness doesn't matter
			size_t const numBytes(grid.byteSize());
			size_t const pos1(ofs.tellp());
			ofs.write((char const * const)grid.begin(), numBytes);
			size_t const pos2(ofs.tellp());
			size_t const gotBytes(pos2 - pos1);
			okay = ((! ofs.fail()) && (gotBytes == numBytes));
		}
	}

	return okay;
}

bool
savePpm
	( std::array<dat::grid<uint8_t>, 3u> const & srgbGrids
	, std::string const & filename
	)
{
	bool okay(false);
	std::ofstream ofs(filename, std::ios::binary);

	assert(srgbGrids[0].isValid());
	assert(srgbGrids[1].isValid());
	assert(srgbGrids[2].isValid());

	// write header
	dat::Extents const inSize(srgbGrids[0].hwSize());
	std::ostringstream oss;
	oss << "P6" << '\n'
		<< inSize.wide() << '\n' // col, row order for PPM
		<< inSize.high() << '\n'
		<< (int)std::numeric_limits<uint8_t>::max() << '\n'
		;
	std::string const header(oss.str());
	ofs.write(header.data(), header.size());

	// multiplex image channels into PPM pixels
	dat::grid<std::array<uint8_t, 3u> > const gridOfRgb
		(convert::multiplexed<uint8_t>(srgbGrids));

	// write data - for 8-bit, endian-ness doesn't matter
	size_t const numBytes(gridOfRgb.byteSize());
	size_t const pos1(ofs.tellp());
	ofs.write((char const * const)gridOfRgb.begin(), numBytes);
	size_t const pos2(ofs.tellp());
	size_t const gotBytes(pos2 - pos1);
	okay = ((! ofs.fail()) && (gotBytes == numBytes));

	return okay;
}

bool
savePgmAutoScale
	( dat::grid<float> const & fgrid
	, std::string const & filename
	)
{
	bool okay(false);

	// check for valid input
	if (fgrid.isValid())
	{
		dat::MinMax<float> const inMinMax
			(stats::activeMinMax<float>(fgrid.begin(), fgrid.end()));
		dat::grid<uint8_t> const ugrid
			(convert::downMappedLinear(fgrid, inMinMax));
		okay = savePgm(ugrid, filename);
	}

	return okay;
}

bool
saveJpg
	( dat::grid<std::array<uint8_t, 3u> > const & rgbGrid
	, std::string const & fpath
	, size_t const & qualPercent
	)
{
	bool okay{ false };
	if ((! fpath.empty()) && rgbGrid.isValid())
	{
		constexpr std::pair<size_t, size_t> const pctRange{ 0u, 100u };
		int const useQual{ (int)dat::clamped(qualPercent, pctRange) };
		int const high{ (int)rgbGrid.high() };
		int const wide{ (int)rgbGrid.wide() };
		{ // guarded
			std::lock_guard<std::mutex> lock(stb::gStbMutex);
			constexpr int deep{ 3 };
			int const wstat
				{ stbi_write_jpg
					(fpath.c_str(), wide, high, deep, rgbGrid.begin(), useQual)
				};
			bool const okayWrite{ (0 < wstat) };
			okay = okayWrite;
		}
	}
	return okay;
}

bool
savePng
	( dat::grid<uint8_t> const & ugrid
	, std::string const & fpath
	)
{
	bool okay{ false };
	if ((! fpath.empty()) && ugrid.isValid())
	{
		{ // guarded
			std::lock_guard<std::mutex> lock(stb::gStbMutex);
			stbi_write_png_compression_level = 9;
			size_t const high{ ugrid.high() };
			size_t const wide{ ugrid.wide() };
			constexpr size_t const deep{ 1u };
			size_t const rowBytes{ wide * deep * sizeof(uint8_t) };
			int const stride{ static_cast<int>(rowBytes) };
			void const * const ptData{ ugrid.begin() };
			int const wstat
				{ stbi_write_png
					(fpath.c_str(), wide, high, deep, ptData, stride)
				};
			okay = (0 < wstat);
		}
	}
	return okay;
}

bool
savePng
	( dat::grid<std::array<uint8_t, 3u> > const & rgbGrid
	, std::string const & fpath
	)
{
	bool okay{ false };
	if ((! fpath.empty()) && rgbGrid.isValid())
	{
		{ // guarded
			std::lock_guard<std::mutex> lock(stb::gStbMutex);
			stbi_write_png_compression_level = 9;
			size_t const high{ rgbGrid.high() };
			size_t const wide{ rgbGrid.wide() };
			constexpr size_t const deep{ 3u };
			size_t const rowBytes{ wide * deep * sizeof(uint8_t) };
			int const stride{ static_cast<int>(rowBytes) };
			void const * const ptData{ rgbGrid.begin() };
			int const wstat
				{ stbi_write_png
					(fpath.c_str(), wide, high, deep, ptData, stride)
				};
			okay = (0 < wstat);
		}
	}
	return okay;
}

//======================================================================
}
}

