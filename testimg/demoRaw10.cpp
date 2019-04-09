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

#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>


namespace tmpImg
{
	using PixType = float;
}


namespace raw10
{
	using WorkType = unsigned int; // probably more 'natural' for compiler
	// using WorkType = uint16_t;

	//! Data record
	struct FourPix
	{
		std::array<uint8_t, 4u> theHiBytes;
		uint8_t theLoBits;

	private: // en/de-coding functions

		//! Decode hiByte as largest component of full value
		static
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
		loBits
			( uint8_t const & mask
			) const
		{
			return
				{ static_cast<WorkType>
					( static_cast<WorkType>(theLoBits)
					& static_cast<WorkType>(mask)
					)
				};
		} 

		//! Decode member bit patterns into full values in working units
		inline
		std::array<WorkType, 4u>
		workValues
			() const
		{
			return std::array<WorkType, 4u>
				{ static_cast<WorkType>
					( hiValue(theHiBytes[0])
					+ static_cast<WorkType>(loBits(0xC0) >> WorkType(6u))
					)
				, static_cast<WorkType>
					( hiValue(theHiBytes[1])
					+ static_cast<WorkType>(loBits(0x30) >> WorkType(4u))
					)
				, static_cast<WorkType>
					( hiValue(theHiBytes[2])
					+ static_cast<WorkType>(loBits(0x0C) >> WorkType(2u))
					)
				, static_cast<WorkType>
					( hiValue(theHiBytes[3])
					+ static_cast<WorkType>(loBits(0x03)                )
					)
				};
		}

	public:

		//! Full values converted to image pixel type
		inline
		std::array<tmpImg::PixType, 4u>
		pixelValues
			() const
		{
			std::array<WorkType, 4u> const workVals{ workValues() };
			using tmpImg::PixType;
			return std::array<PixType, 4u>
				{ static_cast<PixType>(workVals[0])
				, static_cast<PixType>(workVals[1])
				, static_cast<PixType>(workVals[2])
				, static_cast<PixType>(workVals[3])
				};
		}

	};

	// File size - including unused data
	// constexpr size_t const sExpNumRecs{ 1952u };
	// constexpr size_t const sExpRowWide{ 3264u };

	// Sensor active pixel size
	constexpr size_t const sExpPixHigh{ 1944u };
	constexpr size_t const sExpPixWide{ 2592u };

	constexpr size_t const sExpQuadHigh{ sExpPixHigh };
	constexpr size_t const sExpQuadWide{ sExpPixWide / 4u };

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
			grid = dat::grid<FourPix>(sExpQuadHigh, sExpQuadWide);

			// load line by line
			for (size_t row{0u} ; row < sExpQuadHigh ; ++row)
			{
				constexpr size_t expBytes{ sExpQuadWide * sizeof(FourPix) };
				ifs.read((char * const)grid.beginRow(row), expBytes);
				size_t const gotBytes(ifs.gcount());
				if (! (gotBytes == expBytes))
				{
					// discard partial data
					grid = dat::grid<FourPix>{};
					break;
				}
			}
		}
		return grid;
	}

}

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

	dat::grid<raw10::FourPix> const quadGrid{ raw10::loadFourPixGrid(pathraw) };
io::out() << dat::infoString(quadGrid, "quadGrid") << std::endl;

	return 0;
}
