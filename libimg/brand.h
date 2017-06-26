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

#ifndef img_brand_INCL_
#define img_brand_INCL_

/*! \file
\brief Declarations for img::brand
*/


#include "libdat/grid.h"

#include <array>
#include <cstddef>


namespace img
{

//! functions to insert/retrive information into image
namespace brand
{
	//! Magic number for us
	constexpr size_t sMagicSize{ 8u };
	struct Config
	{
		//! Magic number to detect authenticity
		std::array<uint8_t, sMagicSize> theMagic{{}};

		//! Raster data column in which to start the brand
		size_t theCol0{ 0u };

		//! Configure brand with specific magic number
		explicit
		Config
			( std::array<uint8_t, sMagicSize> const & magic
			, size_t const & col0 = 0u
			)
			: theMagic(magic)
			, theCol0( col0 )
		{ }
	};

	//! Write string into column(s) of an image
	void
	writeToColumns
		( dat::grid<uint8_t> * const & ptImage
		, std::string const & text
		, Config const & bConfig
		);

	//! Read string from column(s) of an image
	std::string
	readFromColumns
		( dat::grid<uint8_t> const & image
		, Config const & bConfig
		);
}

}

// Inline definitions
// #include "libimg/brand.inl"

#endif // img_brand_INCL_

