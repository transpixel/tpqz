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

#ifndef dat_discrete_INCL_
#define dat_discrete_INCL_

/*! \file
\brief Declarations for dat::discrete
*/


#include "libdat/Extents.h"
#include "libdat/RowCol.h"
#include "libdat/validity.h"

#include <cstddef>


namespace dat
{

/*! \brief Functions pertaining to data quantization

\par Example
\dontinclude testdat/udiscrete.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace discrete
{
	//! Adjusted to integral boundary of delta
	inline
	size_t
	quantizedLo
		( size_t const & orig
		, size_t const & delta
		);

	//! Adjusted to integral boundary of delta
	inline
	size_t
	quantizedHi
		( size_t const & orig
		, size_t const & delta
		);

	//! Quantized row/col value
	inline
	dat::RowCol
	quantized
		( dat::RowCol const & orig
		, size_t const & quant
		);

	//! Quantized sizes value
	inline
	dat::Extents
	quantized
		( dat::Extents const & orig
		, size_t const & quant
		);
}

}

// Inline definitions
#include "libdat/discrete.inl"

#endif // dat_discrete_INCL_

