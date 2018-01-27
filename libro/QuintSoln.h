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

#ifndef ro_QuintSoln_INCL_
#define ro_QuintSoln_INCL_

/*! \file
\brief Declarations for ro::QuintSoln
*/


#include "libdat/validity.h"
#include "libro/ro.h"
#include "libro/Solution.h"

#include <string>


namespace ro
{

/*! \brief Relative Orientation for 5 pair of corresponding rays.

\par Example
\dontinclude testro/uQuintSoln.cpp
\skip ExampleStart
\until ExampleEnd
*/

struct QuintSoln
{
	//! Indices into implicit external collection of UV direction pairs
	FiveOf<size_t> theFitNdxs{ dat::nullValue<size_t, 5u>() };

	//! Solution associated with the five indicated values
	Solution theSoln{};

public: // methods

	//! default null constructor
	QuintSoln
		() = default;

	//! Value ctor
	explicit
	QuintSoln
		( FiveOf<size_t> const & fitNdxs
		, Solution const & soln
		);

	//! True if instance is valid
	bool
	isValid
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // QuintSoln

} // ro

// Inline definitions
// #include "libro/QuintSoln.inl"

#endif // ro_QuintSoln_INCL_

