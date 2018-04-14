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

#ifndef img_dilate_INCL_
#define img_dilate_INCL_

/*! \file
\brief Declarations for img::dilate
*/


#include "libdat/algorithm.h"
#include "libdat/grid.h"
#include "libdat/Extents.h"
#include "libdat/ExtentsIterator.h"
#include "libdat/IndexIterator.h"
#include "libmath/Extreme.h"
#include "libmath/math.h"

#include <memory>

#include "libdat/info.h"
#include "libio/stream.h"


namespace img
{

/*! \brief Functions associated with image dilation operations.

\par Example
\dontinclude testimg/udilate.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace dilate
{

	//! Grid with all *NULL* elements replaced with nearby(est?) valid element
	template <typename ElemType>
	inline
	dat::grid<ElemType>
	floodFilled
		( dat::grid<ElemType> const & srcGrid
		);

} // dilate

} // img

// Inline definitions
#include "libimg/dilate.inl"

#endif // img_dilate_INCL_

