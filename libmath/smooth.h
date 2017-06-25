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

#ifndef math_smooth_INCL_
#define math_smooth_INCL_

/*! \file
\brief Declarations for math::smooth
*/


#include <cstddef>
#include <vector>


namespace math
{

/*! \brief Data smoothing functions.

\par Example
\dontinclude testmath/usmooth.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace smooth
{
	//! Smoth data using a triangle function
	template <typename Type>
	inline
	std::vector<Type> // TODO generisize
	triangleWrapped
		( std::vector<Type> const & data
		, size_t const & halfSize //!< Filter window *half* size
		);
}

}

// Inline definitions
#include "libmath/smooth.inl"

#endif // math_smooth_INCL_

