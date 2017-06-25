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

#ifndef ga_comparisons_INCL_
#define ga_comparisons_INCL_

/*! \file
\brief Declarations for ga::comparisons
*/


#include "libga/constructs.h"
#include "libga/functions.h"
#include "libga/operators.h"
#include "libga/quantities.h"

#include "libmath/math.h"


namespace ga
{

	// Useful boolean functions using geometric algebra and algorithms.

	//
	// ==== Tests
	//

	//! True if two vectors are parallel
	inline
	bool
	areParallel // TODO generalize as/if needed
		( Vector const & v1
		, Vector const & v2
		, double const & tol = math::eps
		);

	//! True if two vectors are orthogonal
	inline
	bool
	areOrthogonal // TODO generalize as/if needed
		( Vector const & v1
		, Vector const & v2
		, double const & tol = math::eps
		);

}

// Inline definitions
#include "libga/comparisons.inl"

#endif // ga_comparisons_INCL_

