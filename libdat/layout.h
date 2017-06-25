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

#ifndef dat_layout_INCL_
#define dat_layout_INCL_

/*! \file
\brief Declarations for dat::layout
*/


#include "libdat/Area.h"
#include "libdat/Extents.h"
#include "libdat/RowCol.h"
#include "libdat/Spot.h"

#include <cstddef>
#include <vector>


namespace dat
{

/*! \brief Functions for arranging values within defined confines

\par Example
\dontinclude testdat/ulayout.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace layout
{
	//! Uniformly spaced with margin around the edges
	namespace centered
	{
		//! Indices distributed nominally uniformly and centered in [0,max)
		std::vector<size_t>
		indices
			( size_t const & max
			, size_t const & numValues
			);

		//! applies indices() for each or row and column
		std::vector<dat::RowCol>
		rowcols
			( dat::Extents const & domain
			, dat::Extents const & hwSize
			);
	}

	//! Uniformly spaced with locations tight to the edges
	namespace expanded
	{
		//! Values uniformly distributed over range [0,max)
		std::vector<double>
		values
			( size_t const & max
			, size_t const & numValues
			);

		//! Applies values() to cover area area with hwSize rows and cols
		std::vector<dat::Spot>
		spots
			( dat::Area<double> const & area
			, dat::Extents const & hwSize
			);

		//! Indices distributed nominally uniformly and centered in [0,max)
		std::vector<size_t>
		indices
			( size_t const & max
			, size_t const & numValues
			);

		//! Applies indices() for each or row and column
		std::vector<dat::RowCol>
		rowcols
			( dat::Extents const & domain
			, dat::Extents const & hwSize
			);
	}
}

}

// Inline definitions
// #include "libdat/layout.inl"

#endif // dat_layout_INCL_

