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
\brief Definitions for img::brand
*/


#include "libimg/brand.h"

#include "libdat/Extents.h"
#include "libdat/info.h"
#include "libio/stream.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <functional>


namespace img
{
namespace brand
{
//======================================================================

namespace
{
	bool const sShowAction(false);

	//! Perform action for numElems in order down columns
	void
	action
		( size_t const & numElem
		, size_t const & row0
		, size_t const & col0
		, dat::Extents const & hwSize
		, std::function
			<void (size_t const &, size_t const &, size_t const &)>
			const & func
		)
	{
		//! write bytes from string
		size_t row(row0);
		size_t col(col0);

		for (size_t nn(0u) ; nn < numElem ; ++nn)
		{
			if (col < hwSize.wide())
			{
				if (row < hwSize.high())
				{
					func(row, col, nn);
					++row;
				}
				else
				{
					row = 0u;
					++col;
				}
			}
		}
	}

	//! Provide size en/de-coding into/from bytes
	struct Header
	{
		static constexpr size_t sNumMagic{ 8u };
		std::array<uint8_t, sNumMagic> theMagic{{}};
		union
		{
			uint32_t theWord;
			uint8_t theBytes[4u];
		} theMem{};

		static constexpr size_t
		size ()
			{ return (sNumMagic+sizeof(theMem)); }

		Header
			( std::array<uint8_t, sNumMagic> const & magicBytes
			)
			: theMagic(magicBytes)
		{ }

		Header
			( uint32_t const & word
			, std::array<uint8_t, sNumMagic> const & magicBytes
			)
			: theMagic(magicBytes)
		{
			theMem.theBytes[0] = ((0x000000FF & word) >>  0);
			theMem.theBytes[1] = ((0x0000FF00 & word) >>  8);
			theMem.theBytes[2] = ((0x00FF0000 & word) >> 16);
			theMem.theBytes[3] = ((0xFF000000 & word) >> 24);
		}

		Header
			( uint8_t * const & bytes
			, std::array<uint8_t, sNumMagic> const & magicBytes
			)
			: theMagic(magicBytes)
		{
			theMem.theWord =
				( (bytes[0] <<  0)
				+ (bytes[1] <<  8)
				+ (bytes[2] << 16)
				+ (bytes[3] << 24)
				);
		}

		bool
		isValid
			( std::array<uint8_t, sNumMagic> const & expectMagic
			) const
		{
			return std::equal
				(theMagic.begin(), theMagic.end(), expectMagic.begin());
		}

		std::string
		infoString
			( std::string const & title = std::string()
			) const
		{
			std::ostringstream oss;
			if (! title.empty())
			{
				oss << title << " ";
			}
			oss << std::endl;
			oss
				<< "           size"
				<< ": " << size()
				;
			oss << std::endl;
			oss
				<< "  theMagicValue"
				<< ": "
				;
			for (uint8_t const & byte : theMagic)
			{
				oss << " " << std::dec << std::setw(3u) << byte;
			}
			oss << std::endl;
			oss
				<< " theMem.theWord"
				<< std::dec
				<< ": " << theMem.theWord
				<< '\n'
				<< "            hex"
				<< std::hex
				<< ": " << theMem.theWord
				;
			oss << std::endl;
			oss
				<< "theMem.theBytes"
				<< std::dec
				<< ": " << static_cast<int>(theMem.theBytes[0])
				<< ", " << static_cast<int>(theMem.theBytes[1])
				<< ", " << static_cast<int>(theMem.theBytes[2])
				<< ", " << static_cast<int>(theMem.theBytes[3])
				<< '\n'
				<< std::hex
				<< "            hex"
				<< ": " << static_cast<int>(theMem.theBytes[0])
				<< ", " << static_cast<int>(theMem.theBytes[1])
				<< ", " << static_cast<int>(theMem.theBytes[2])
				<< ", " << static_cast<int>(theMem.theBytes[3])
				;
			return oss.str();
		}

	};

}

void
writeToColumns
	( dat::grid<uint8_t> * const & ptImage
	, std::string const & text
	, Config const & bConfig
	)
{
	if (ptImage && ptImage->isValid() && (Header::size() < ptImage->size()))
	{
		//! Functor to write text bytes into image
		struct SetImage
		{
			dat::grid<uint8_t> * const thePtImage;
			uint8_t const * const theBytes;

			void
			operator()
				( size_t const & row
				, size_t const & col
				, size_t const & nn
				)
			{
				dat::grid<uint8_t> & image = *thePtImage;
				uint8_t const & byte = theBytes[nn];
				image(row, col) = byte;

				if (sShowAction)
				{
					io::out()
						<< "-->>:x: " << std::setw(3u) << +byte
						<< " row,col,nn,byte:"
						<< " " << std::setw(4u) << row
						<< " " << std::setw(4u) << col
						<< " " << std::setw(4u) << nn
						<< " " << std::setw(4u) << +byte
						<< std::endl;
				}

			}
		};

		dat::Extents const & imgSize = ptImage->hwSize();
		size_t rowAt(0);
		size_t const & col0 = bConfig.theCol0;

		// write header into image
		Header const head(static_cast<uint32_t>(text.size()), bConfig.theMagic);
		assert(head.isValid(bConfig.theMagic));
		SetImage putHeader
			{ ptImage
			, reinterpret_cast<uint8_t const * const>(&head)
			};
		action(head.size(), rowAt, col0, imgSize, putHeader);

		rowAt += head.size();

		// write bytes into image pixels from string
		uint8_t const * const bytes
			= reinterpret_cast<uint8_t const * const>(text.data());
		SetImage putMessage{ptImage, bytes};
		action(text.size(), rowAt, col0, imgSize, putMessage);
	}
}

std::string
readFromColumns
	( dat::grid<uint8_t> const & image
	, Config const & bConfig
	)
{
	std::string text;
	if (image.isValid() && (Header::size() < image.size()))
	{
		//! Functor to read text from image
		struct SetText
		{
			uint8_t * const theBytes;
			dat::grid<uint8_t> const * const thePtImage;

			void
			operator()
				( size_t const & row
				, size_t const & col
				, size_t const & nn
				)
			{
				dat::grid<uint8_t> const & image = *thePtImage;
				uint8_t & byte = theBytes[nn];
				byte = image(row, col);

				if (sShowAction)
				{
					io::out()
						<< "<<--:x: " << std::setw(3u) << +byte
						<< " row,col,nn,byte:"
						<< " " << std::setw(4u) << row
						<< " " << std::setw(4u) << col
						<< " " << std::setw(4u) << nn
						<< " " << std::setw(4u) << +byte
						<< std::endl;
				}

			}
		};

		size_t rowAt(0u);
		size_t const & col0 = bConfig.theCol0;

		// read header size
		Header head(bConfig.theMagic);
		SetText putHeader
			{ reinterpret_cast<uint8_t * const>(&head)
			, &image
			};
		action(head.size(), rowAt, col0, image.hwSize(), putHeader);

		if (head.isValid(bConfig.theMagic))
		{
			size_t const numBytes(static_cast<size_t>(head.theMem.theWord));
			rowAt += head.size();

			// read bytes into string from image pixels
			uint8_t * const bytes = new uint8_t[numBytes];
			SetText putMessage{bytes, &image};
			action(numBytes, rowAt, col0, image.hwSize(), putMessage);
			text = std::string(bytes, bytes + numBytes);
			delete[] bytes;
		}
	}
	return text;
}

//======================================================================
}
}

