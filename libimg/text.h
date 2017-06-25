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

#ifndef img_text_INCL_
#define img_text_INCL_

/*! \file
\brief Declarations for img::text
*/


#include "libdat/grid.h"

#include <array>
#include <cstddef>
#include <string>
#include <vector>


namespace img
{

//! Functions for drawing text into imagery
namespace text
{
	//! Image with text written to upper left region
	void
	putInto
		( dat::grid<std::array<uint8_t, 3u> > * const & ptGrid
		, std::vector<std::string> const & textLines
		, size_t const & textHigh = { 16u }
		);

}

}

// Inline definitions
// #include "libimg/text.inl"

#endif // img_text_INCL_

