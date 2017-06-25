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

#ifndef dat_algorithm_INCL_
#define dat_algorithm_INCL_

/*! \file
\brief Declarations for dat::algorithm
*/


#include "libdat/validity.h"

#include <algorithm>
#include <iterator>
#include <utility>


namespace dat
{

// Generic algorithms (e.g. extension/variation on STL)

	//! Indices such that (data[first] <= findVal < data[second])
	template <typename FwdIter>
	std::pair<size_t, size_t>
	indexBounds
		( double const & findVal
		, FwdIter const & beg
		, FwdIter const & end
		);

	//! Value guarenteed to be within *IN*clusive range (or null)
	template <typename Type>
	inline
	Type
	clamped
		( Type const & candidate
		, std::pair<Type, Type> const & range
		);

}

// Inline definitions
#include "libdat/algorithm.inl"

#endif // dat_algorithm_INCL_

