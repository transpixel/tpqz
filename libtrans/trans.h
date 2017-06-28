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

#ifndef trans_trans_INCL_
#define trans_trans_INCL_

/*! \file
\brief Declarations for trans::trans
*/


#include <functional>
#include <utility>


/*! \brief Declarations and Definitions for libtrans.

\par General Concept:

Support for generic transformation of data while respecting finite data ranges.

\par Special Notes:

+ TODO

\par Example
\dontinclude testtrans/utrans.cpp
\skip ExampleStart
\until ExampleEnd

*/
namespace trans
{
	//! Value interpolated through unit mapping function
	template <typename OutType>
	inline
	OutType
	mapped
		( std::pair<double, double> const & xRange
		, double const & xValue
		, std::pair<OutType, OutType> const & yRange
		, std::function<OutType(double const & arg)> const & unitMapping
		);
}

// Inline definitions
#include "libtrans/trans.inl"

#endif // trans_trans_INCL_

