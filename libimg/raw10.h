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

#ifndef img_raw10_INCL_
#define img_raw10_INCL_

/*! \file
\brief Declarations for img::raw10 10-bit raw image format (from OmniVision?)
*/


#include "libdat/Extents.h"

#include <array>
#include <cstddef>


namespace img
{

/*! \brief Functions for decoding 'raw10' image file format.

\par Example
\dontinclude testimg/uraw10.cpp
\skip ExampleStart
\until ExampleEnd
*/

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

	template <typename PixType>
	std::array<PixType, 4u>
	pixelValues
		( FourPix const & quad
		);

	//! Header used by raspberry pi 'picamera' and 'raspistill' libraries
	struct HeadBRCM
	{
		static constexpr size_t theSize = { 32768u };
		std::array<uint8_t, theSize> theHdr{};

		//! Magic string expected at start of header
		inline
		static
		constexpr
		std::array<uint8_t, 4>
		magic
			()
		{
			return { 'B', 'R', 'C', 'M' };
		}
		
		//! True if 'valid' (e.g. magic number matches)
		bool
		isValid
			() const;
	};

	//! Quad data layout for RasPi with OV5647 imaging chip
	struct RasPiV1
	{
		// Sensor active pixel size
		static constexpr size_t const sExpPixHigh{ 1944u };
		static constexpr size_t const sExpPixWide{ 2592u };

		// File size - including unused data
		// constexpr size_t const sExpFileHigh{ 1952u };
		static constexpr size_t const sExpFileWide{ 3264u };
	};

	//! Quad data layout for RasPi with IMX219PQ imaging chip
	struct RasPiV2
	{
		// Sensor active pixel size
		static constexpr size_t const sExpPixHigh{ 2464u };
		static constexpr size_t const sExpPixWide{ 3280u };

		// File size - including unused data
		// constexpr size_t const sExpFileHigh{ 1952u };
		static constexpr size_t const sExpFileWide{ 4128u };
	};

	//! Byte sizes for 'raw10' file format (quad)data layout configuration
	class Sizes
	{
		size_t const theExpQuadHigh{};
		size_t const theExpQuadWide{};
		size_t const theExpRowUsed{};
		size_t const theExpRowJunk{};

	public:

		//! Construct sizes to match Raspberry Pi camera type
		template <typename RasPiType>
		explicit
		Sizes
			( RasPiType const & raspi
			)
			: theExpQuadHigh{ raspi.sExpPixHigh }
			, theExpQuadWide{ raspi.sExpPixWide / 4u }
			, theExpRowUsed{ (raspi.sExpPixWide * 10u) / 8u }
			, theExpRowJunk{ raspi.sExpFileWide - theExpRowUsed }
		{ }

		//! True if this instance has meaningful (non-zero) quad sizes
		bool
		isValid
			() const
		{
			return { hwSizeQuads().isValid() };
		}

		//! High/Wide in FourPix quad units (each quad spans 5 bytes)
		dat::Extents
		hwSizeQuads
			() const
		{
			return dat::Extents{ theExpQuadHigh, theExpQuadWide };
		}

		//! Number of bytes used for active quad data in a row
		size_t
		expRowQuadBytes
			() const
		{
			return { theExpQuadWide * sizeof(FourPix) };
		}

		//! Number of unused-bytes of data at end of valid row (quad)data
		size_t
		sizeRowJunk
			() const
		{
			return { theExpRowJunk };
		}

		//! Number of bytes of unused data at end of each line
		std::vector<uint8_t>
		junkBuffer
			() const
		{
			return std::vector<uint8_t>(sizeRowJunk());
		}

	};

} // raw10

} // img

// Inline definitions
#include "libimg/raw10.inl"

#endif // img_raw10_INCL_

