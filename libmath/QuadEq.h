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

#ifndef math_QuadEq_INCL_
#define math_QuadEq_INCL_

/*! \file
\brief Declarations for math::QuadEq
*/

#include "libdat/validity.h"
#include "libmath/math.h"

#include <string>
#include <utility>


namespace math
{

/*! \brief Classic quadratic equation.

\par Example
\dontinclude testmath/uQuadEq.cpp
\skip ExampleStart
\until ExampleEnd
*/
class QuadEq
{

	double const theA{ dat::nullValue<double>() }; // x^2 coefficent
	double const theB{ dat::nullValue<double>() }; // x^1
	double const theC{ dat::nullValue<double>() }; // x^0

	// cached intermediary
	double const theDescrim{ dat::nullValue<double>() }; // descrimant
	double const theDelta{ dat::nullValue<double>() }; // root(descrim)

public: // methods

	QuadEq
		() = default;

	//! Value ctor
	explicit
	QuadEq
		( double const coA
		, double const coB
		, double const coC
		);

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! The more negative of the two roots
	double
	realRootMin
		() const;

	//! The more positive of the two roots
	double
	realRootMax
		() const;

	//! The smallest non-negative root (or null)
	double
	realRootMinPos
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
// #include "libmath/QuadEq.inl"

#endif // math_QuadEq_INCL_

