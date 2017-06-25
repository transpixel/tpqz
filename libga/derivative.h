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

#ifndef ga_derivative_INCL_
#define ga_derivative_INCL_

/*! \file
\brief Declarations for ga::derivative
*/


#include "libga/constructs.h"
#include "libga/functions.h"
#include "libga/quantities.h"


namespace ga
{

/*! \brief Functions that differentiate quantities.

\par Example
\dontinclude testga/uderivative.cpp
\skip ExampleStart
\until ExampleEnd
*/

	//! Derivative of exp(biv)
	inline
	ga::Spinor
	dexp
		( ga::BiVector const & bVal
		, ga::BiVector const & bDot
		);

	//! Derivative of the rotated entity (here a vector)
	inline
	ga::Vector
	dRotatedSpinArgs
		( ga::BiVector const & argSpinVal //!< E.g. .5*physAngle
		, ga::BiVector const & argSpinDot
		, ga::Vector const & vec
		, ga::Spinor const & eNegArgVal //!< exp(-argSpinVal) - e.g. cached
		);

	//! Derivative of the rotated entity (here a vector)
	inline
	ga::Vector
	dRotatedPhysArgs
		( ga::BiVector const & physAngleVal
		, ga::BiVector const & physAngleDot
		, ga::Vector const & vec
		);

} // ga

// Inline definitions
#include "libga/derivative.inl"

#endif // ga_derivative_INCL_

