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

#ifndef prob_mean_INCL_
#define prob_mean_INCL_

/*! \file
\brief Declarations for prob::mean
*/


#include "libdat/validity.h"

#include <algorithm>
#include <iterator>


namespace prob
{

/*! \brief Functions related to mean value.

\par Example
\dontinclude testprob/umean.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace mean
{
	//! (Arithemetic) Average value of collection
	template <typename FwdIter, typename DataType = double>
	inline
	DataType
	arithmetic
		( FwdIter const & beg //!< (*it) must be convertable to double
		, FwdIter const & end
		);

	// geometric
	// harmonic

} // mean

} // prob

// Inline definitions
#include "libprob/mean.inl"

#endif // prob_mean_INCL_

