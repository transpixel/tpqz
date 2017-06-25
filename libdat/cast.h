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

#ifndef dat_cast_INCL_
#define dat_cast_INCL_

/*! \file
\brief Declarations for dat::cast
*/


#include "libdat/RowCol.h"
#include "libdat/Spot.h"
#include "libdat/validity.h"

#include <cmath>


namespace dat
{

/*! \brief Functions for casting between libdat entities
 */

/*
\par Example
\dontinclude testdat/ucast.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace cast
{
	//! Cast: RowCol associated with spot
	inline
	dat::RowCol
	RowCol
		( dat::Spot const & spot
		)
	{
		dat::RowCol rc(dat::nullValue<dat::RowCol>());
		if (dat::isValid(spot))
		{
			rc = dat::RowCol
				{{ static_cast<size_t>(std::floor(spot[0]))
				 , static_cast<size_t>(std::floor(spot[1]))
				}};
		}
		return rc;
	}

	//! Construct a (floating point) spot from a (integral) rowcol instance
	inline
	dat::Spot
	Spot
		( dat::RowCol const & rowcol
		)
	{
		dat::Spot spot(dat::nullValue<dat::Spot>());
		if (dat::isValid(rowcol))
		{
			spot = dat::Spot{{ double(rowcol[0]), double(rowcol[1]) }};
		}
		return spot;
	}

}

}

// Inline definitions
// #include "libdat/cast.inl"

#endif // dat_cast_INCL_

