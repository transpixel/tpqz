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
\brief  This file contains main application program demoRaw10
*/


#include "libapp/Usage.h"

#include "build/version.h"
#include "libio/stream.h"

#include "libdat/grid.h"
#include "libdat/info.h"
#include "libimg/convert.h"
#include "libimg/io.h"
#include "libimg/stats.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>


//! Structs and functions associated with OV5647 sensor chip raw data files.
namespace raw10
{
	//! Data record associated with 4x,10-bit pixel values
	struct FourPix
	{
		//! High 8-bits for each of four consecutive pixels
		std::array<uint8_t, 4u> theHiBytes;

		//! Low 2-bits for each of the four pixels
		uint8_t theLoBits;
	};

	//! Functions for decoding data byte/bit patterns
	namespace bitwork
	{

		using WorkType = unsigned int; // probably more 'natural' for compiler
		// using WorkType = uint16_t;

		//! Decode hiByte as largest component of full value
		inline
		WorkType
		hiValue
			( uint8_t const & hiByte
			)
		{
			// multiply to leave room for low 2 bits
			return
				{ static_cast<WorkType>
					(static_cast<WorkType>(hiByte) << WorkType(2u))
				};
		}

		//! Active low bits associated with mask
		inline
		WorkType
		onLoBits
			( uint8_t const & loBits
			, uint8_t const & mask
			)
		{
			return
				{ static_cast<WorkType>
					( static_cast<WorkType>(loBits)
					& static_cast<WorkType>(mask)
					)
				};
		} 

		//! Value associated with (masked and shifted) loBit pattern
		inline
		WorkType
		loValue
			( uint8_t const & loBits
			, uint8_t const & mask
			, WorkType const & shift
			)
		{
			return { static_cast<WorkType>(onLoBits(loBits, mask) >> shift) };
		}

		//! Value associated with (masked and UNshifted) loBit pattern
		inline
		WorkType
		loValue
			( uint8_t const & loBits
			, uint8_t const & mask
			)
		{
			return { onLoBits(loBits, mask) };
		}

		//! Decode member bit patterns into full values in working units
		inline
		std::array<WorkType, 4u>
		workValues
			( FourPix const & quad
			)
		{
			std::array<uint8_t, 4u> const & hiBytes = quad.theHiBytes;
			uint8_t const & loBits = quad.theLoBits;
			return std::array<WorkType, 4u>
				{ static_cast<WorkType>
					( hiValue(hiBytes[0]) + loValue(loBits, 0xC0, 6u)
					)
				, static_cast<WorkType>
					( hiValue(hiBytes[1]) + loValue(loBits, 0x30, 4u)
					)
				, static_cast<WorkType>
					( hiValue(hiBytes[2]) + loValue(loBits, 0x0C, 2u)
					)
				, static_cast<WorkType>
					( hiValue(hiBytes[3]) + loValue(loBits, 0x0C)
					)
				};
		}

		//! Full values converted to image pixel type
		template <typename PixType>
		inline
		std::array<PixType, 4u>
		pixelValues
			( FourPix const & quad
			)
		{
			std::array<WorkType, 4u> const workVals{ workValues(quad) };
			return std::array<PixType, 4u>
				{ static_cast<PixType>(workVals[0])
				, static_cast<PixType>(workVals[1])
				, static_cast<PixType>(workVals[2])
				, static_cast<PixType>(workVals[3])
				};
		}

		//! Specialization for uint8_t pixel recovery (hi-bits only)
		template <>
		inline
		std::array<uint8_t, 4u>
		pixelValues<uint8_t>
			( FourPix const & quad
			)
		{
			return { quad.theHiBytes };
		}

	} // bitwork

	//! Values associated with raw10 binary file layout
	namespace size
	{
		constexpr size_t const sExpHeadSize{ 32768u };
		constexpr std::array<uint8_t, 4> const sMagic{ 'B', 'R', 'C', 'M' };

		// Sensor active pixel size
		constexpr size_t const sExpPixHigh{ 1944u };
		constexpr size_t const sExpPixWide{ 2592u };

		constexpr size_t const sExpQuadHigh{ sExpPixHigh };
		constexpr size_t const sExpQuadWide{ sExpPixWide / 4u };

		// File size - including unused data
		// constexpr size_t const sExpFileHigh{ 1952u };
		constexpr size_t const sExpFileWide{ 3264u };
		constexpr size_t const sExpRowUsed{ (sExpPixWide * 10u) / 8u };
		constexpr size_t const sExpRowJunk{ sExpFileWide - sExpRowUsed };
	}

	//! Extract contiguous data elements out of (padded) file content
	dat::grid<FourPix>
	loadFourPixGrid
		( std::string const & fpath
		)
	{
		dat::grid<FourPix> grid;

		std::ifstream ifs(fpath, std::ios::binary);
		if (ifs.good())
		{
			// allocate space
			using namespace size;
			grid = dat::grid<FourPix>(sExpQuadHigh, sExpQuadWide);
			char junk[sExpRowJunk];

			// fetch and verify header
			char hdr[sExpHeadSize];
			ifs.read(hdr, sExpHeadSize);
			size_t const gotHeadSize{ (size_t)ifs.gcount() };
			bool const okayHead{ (gotHeadSize == sExpHeadSize) };
			bool const okayMagic
				{ std::equal(sMagic.begin(), sMagic.end(), hdr) };
			if (okayHead && okayMagic)
			{
				// load line by line
				for (size_t
					row{0u} ; (row < sExpQuadHigh) && ifs.good() ; ++row)
				{
					// load active portion of file row
					constexpr size_t expBytes{ sExpQuadWide * sizeof(FourPix) };
					ifs.read((char * const)grid.beginRow(row), expBytes);
					size_t const gotBytes{ (size_t)ifs.gcount() };

					// skip unused portion of row
					ifs.read(junk, sExpRowJunk);
					size_t const gotJunk{ (size_t)ifs.gcount() };

					// check status so far
					bool const okayData{ (gotBytes == expBytes) };
					bool const okayJunk{ (gotJunk == sExpRowJunk) };
					if (! (okayData && okayJunk))
					{
						// discard partial data
						grid = dat::grid<FourPix>{};
						break;
					}
				}
			}
			// skip unused rows (by stopping read at last active row)
		}
		return grid;
	}

	//! Pixel values created by expanding (compacted) quad data
	template <typename PixType>
	inline
	dat::grid<PixType>
	pixelGridFor
		( dat::grid<FourPix> const & quad
		)
	{
		dat::grid<PixType> pixels{};
		if (dat::isValid(quad))
		{
			// allocate space for output
			pixels = dat::grid<PixType>(quad.high(), 4u*quad.wide());
			typename dat::grid<PixType>::iterator itPix{ pixels.begin() };
			// fill pixels by expanding all quads in this row
			for (dat::grid<FourPix>::const_iterator
				itQuad{quad.begin()} ; quad.end() != itQuad ; ++itQuad)
			{
				std::array<PixType, 4u> const fourPix
					{ bitwork::pixelValues<PixType>(*itQuad) };
				std::copy(fourPix.begin(), fourPix.end(), itPix);
				itPix += fourPix.size();
			}
		}
		return pixels;
	}

	//! Pixel decoded from raw file
	template <typename PixType>
	inline
	dat::grid<PixType>
	pixelGridFor
		( std::string const & fpath
		)
	{
		// load data from disk (in compacted form)
		dat::grid<FourPix> const quadGrid{ loadFourPixGrid(fpath) };

		// expand to full pixels and return
		return { pixelGridFor<PixType>(quadGrid) };
	}

} // raw10


//! Read a 'raw10' formated binary (e.g. from Raspberry Pi cameras)
int
main
	( int const argc
	, char const * const * const argv
	)
{
	// Don't mix stdio & streams (for performance)
	//std::ios::sync_with_stdio(false);

	// check args
	app::Usage usage;
	usage.setSummary
		( "Read (Raspberry Pi Camera) 'raw10' image format and save components"
		);
	usage.addArg("PathInput", "Input data file <raw10 format>");
	// ...
	if (usage.argStatus(argc, argv) != app::Usage::Valid)
	{
		std::string const fname(argv[0]);
		io::err()
			<< std::endl << build::version::buildInfo(argv[0]) << std::endl
			<< usage.infoString(fname) << std::endl;
		return 1;
	}

	// parse input argument
	int argnum(0);
	std::string const pathraw(argv[++argnum]);

io::out() << dat::infoString(pathraw, "pathraw") << std::endl;
io::out() << std::endl;

	// load data from disk and expand to requested pixel type
	using PixType = uint8_t;
	dat::grid<PixType> const pixGrid{ raw10::pixelGridFor<PixType>(pathraw) };

	// radiometrically scale image
	dat::MinMax<PixType> const pixMinMax
		{ img::stats::activeMinMax<PixType>(pixGrid.begin(), pixGrid.end()) };
	bool const okayPix{ img::io::savePng(pixGrid, "pixGrid.png") };

io::out() << dat::infoString(pixGrid, "pixGrid") << std::endl;
io::out() << dat::infoString(pixMinMax, "pixMinMax") << std::endl;
io::out() << dat::infoString(okayPix, "okayPix") << std::endl;
io::out() << std::endl;

	dat::grid<float> const fltGrid{ raw10::pixelGridFor<float>(pathraw) };
	dat::MinMax<float> const fltMinMax
		{ img::stats::activeMinMax<float>(fltGrid.begin(), fltGrid.end()) };

	bool const okayFlt{ img::io::savePgmAutoScale(fltGrid, "fltGrid.pgm") };

io::out() << dat::infoString(fltGrid, "fltGrid") << std::endl;
io::out() << dat::infoString(fltMinMax, "fltMinMax") << std::endl;
io::out() << dat::infoString(okayFlt, "okayFlt") << std::endl;
io::out() << std::endl;

	return 0;
}
