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

} // raw10

} // img

// Inline definitions
#include "libimg/raw10.inl"

#endif // img_raw10_INCL_

