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

#ifndef ro_Solution_INCL_
#define ro_Solution_INCL_

/*! \file
\brief Declarations for ro::Solution
*/

#include "libdat/validity.h"
#include "libro/Pair.h"

#include <memory>
#include <string>

namespace ro
{

/*! \brief Container for results associated with a RO solution

\par Example
\dontinclude testro/uSolution.cpp
\skip ExampleStart
\until ExampleEnd
*/

struct Solution
{
	std::shared_ptr<Pair> theRoPair;
	size_t theItCount{ dat::nullValue<size_t>() };
	size_t theCondNum{ dat::nullValue<size_t>() };
	double theConvergeGap{ dat::nullValue<double>() };

public: // methods

	//! default null constructor
	Solution
		() = default;

	//! Value ctor
	explicit
	Solution
		( std::shared_ptr<Pair> const & roPair
		, size_t const & itCount
		, size_t const & condNum
		, double const & convergeGap
		);

	//! True if instance is valid
	bool
	isValid
		() const;

	//! OriPair from theRoPair (if valid)
	std::pair<ga::Rigid, ga::Rigid>
	pair
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // Solution

} // ro

// Inline definitions
// #include "libro/Solution.inl"

#endif // ro_Solution_INCL_

