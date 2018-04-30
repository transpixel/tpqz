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

#ifndef dat_iter_INCL_
#define dat_iter_INCL_

/*! \file
\brief Declarations for dat::iter
*/


#include "libdat/compare.h"
#include "libdat/layout.h"
#include "libdat/validity.h"


namespace dat
{

/*! \brief Simple iterator manipulation functions

\par Example
\dontinclude testdat/uiter.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace iter
{
	//! Last index for which useSize will fit into sourceSize
	size_t
	activeEndFor
		( size_t const & sourceSize
		, size_t const & useSize
		);

	//! Last possible begin iter for sourceSize allowing for patch/search sizes
	size_t
	activeEndFor
		( size_t const & sourceSize
		, size_t const & patchSize
		, size_t const & numLooks
		);

	//! Iterator pairs that partition totalSize into numGroups
	template <typename Iter>
	inline
	std::vector<std::pair<Iter, Iter> >
	groups
		( Iter const & beg
		, Iter const & end
		, size_t const & numGroups
		);
}

}

// Inline definitions
#include "libdat/iter.inl"

#endif // dat_iter_INCL_

