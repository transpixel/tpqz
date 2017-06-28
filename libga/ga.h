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

#ifndef ga_ga_INCL_
#define ga_ga_INCL_

/*! \file
\brief Declarations for namespace ga
*/


#include "libdat/array.h"
#include "libga/comparisons.h"
#include "libga/constructs.h"
#include "libga/quantities.h"
#include "libga/operators.h"
#include "libga/functions.h"
#include "libmath/math.h"


/*! \brief Declarations and Definitions for libga.

\par General Concept:

A simple implementation of Geometric Algebra (GA) operations in 3D space.

\par Special Notes:

+ Implementation is simple, but relatively efficient in terms of operations.
+ Operations between various grade instances may need to be added as needed.
+ The Pose and Rigid classes support 3D pose/orientation modeling.

\par Example
\dontinclude testga/uga.cpp
\skip ExampleStart
\until ExampleEnd

*/
namespace ga
{

	//! Pass through to itemA.nearlyEquals(itemB)
	template <typename Type>
	inline
	bool
	nearlyEquals
		( Type const & itemA
		, Type const & itemB
		, double const & tol = math::eps
		);

}

// Inline definitions
#include "libga/ga.inl"

#endif // ga_ga_INCL_

