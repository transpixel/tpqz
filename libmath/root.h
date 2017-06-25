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

#ifndef math_root_INCL_
#define math_root_INCL_

/*! \file
\brief Declarations for math::root
*/


#include "libdat/validity.h"
#include "libmath/math.h"

#include <cassert>
#include <utility>


namespace math
{

/*! \brief Functions that support finding equation root.

\par Example
\dontinclude testmath/uroot.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace root
{

	//! Find root within bracket using Ridder's method
	template <typename Func>
	inline
	double
	viaRidder
		( Func const & func
		, std::pair<double, double> const & bracket
		, double const & xTol = math::eps // domain tolerance (delta x)
		);

} // root

} // math

// Inline definitions
#include "libmath/root.inl"

#endif // math_root_INCL_

