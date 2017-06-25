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

#ifndef math_PolyFunc_INCL_
#define math_PolyFunc_INCL_

/*! \file
\brief Declarations for math::PolyFunc
*/


#include "libdat/types.h"
#include "libdat/Range.h"

#include <functional>
#include <string>
#include <utility>
#include <vector>


namespace math
{

/*! \brief Polyline representation of a function.

\par Example
\dontinclude testmath/uPolyFunc.cpp
\skip ExampleStart
\until ExampleEnd
*/

class PolyFunc
{
	dat::Range<double> theRangeX;
	std::vector<dat::Spot> theVerts;

	// cached values
	std::vector<double> theValuesX;
	std::vector<double> theValuesY;

public: // static methods

	//! Ordering test for vertex 'x'-component
	inline
	bool
	static
	xIsLess
		( dat::Spot const & vA
		, dat::Spot const & vB
		);

public: // methods

	//! default null constructor
	PolyFunc
		() = default;

	//! Construct an instance from values with ranges at min/max values
	template <typename FwdIter>
	inline
	explicit
	PolyFunc
		( FwdIter const & vertsBeg
		, FwdIter const & vertsEnd
		);

	//! Convenience ctor
	explicit
	PolyFunc
		( std::vector<dat::Spot> const & verts
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Function value for x-loc
	double
	operator()
		( double const & xarg
		) const;

	//! Range of x-values
	dat::Range<double> const &
	range
		() const;

	//! Points on polyline with uniform 'x'spacing
	std::vector<dat::Spot> const &
	xyVertices
		() const;

	//! "Reversed" vertices on inverse polyline
	std::vector<dat::Spot>
	yxVertices
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

	//! Descriptive information including internal data
	std::string
	infoStringContents
		( std::string const & title = std::string()
		) const;

};

}

// Inline definitions
#include "libmath/PolyFunc.inl"

#endif // math_PolyFunc_INCL_

