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

#ifndef dat_random_INCL_
#define dat_random_INCL_

/*! \file
\brief Declarations for dat::random
*/

#include "libdat/validity.h"

#include <array>
#include <cassert>
#include <random>


namespace dat
{

/*! \brief Functions associated with pseudo-random numbers/operations.

\par Example
\dontinclude testdat/urandom.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace random
{
	//
	// TODO - factor the distribution from inside
	//        replace with general (uniform)rand value function
	//

	//! Generate random sampling (w/o replacement) of values [0,sourceSize)
	template <size_t Dim, typename RandGen>
	void
	index_sample
		( std::array<size_t, Dim> * const & ptSampNdxs
		, size_t const & sourceSizeN
		, RandGen & rgen //!< Random generator e.g. std::19937_64 or any
		);

	//! Generate random sampling (w/o replacement) of values [0,sourceSize)
	template <size_t Dim, typename RandGen>
	std::array<size_t, Dim>
	index_sample
		( size_t const & sourceSizeN
		, RandGen & rgen //!< Random generator e.g. std::19937_64 or any
		);

} // random

} // dat

// Inline definitions
#include "libdat/random.inl"

#endif // dat_random_INCL_

