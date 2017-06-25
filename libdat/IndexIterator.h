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

#ifndef dat_IndexIterator_INCL_
#define dat_IndexIterator_INCL_

/*! \file
\brief Declarations for dat::IndexIterator
*/


#include "libdat/validity.h"

#include <string>
#include <utility>


namespace dat
{
	using IndexPair = std::pair<size_t, size_t>;

/*! \brief Iterator compatibility for a simple range of index values.

\par Example
\dontinclude testdat/uIndexIterator.cpp
\skip ExampleStart
\until ExampleEnd
*/

class IndexIterator
{
	IndexPair const theBegEnd;
	size_t theAt;

public: // methods

	//! Construct an invalid instance
	inline
	IndexIterator
		();

	//! Construct with valid range
	inline
	explicit
	IndexIterator
		( IndexPair const & indexBegEnd
		);

	//! True if iterator is active within the range
	inline
	explicit
	operator bool
		() const;

	//! Current index within half open range [beg, end)
	inline
	size_t const &
	operator*
		() const;

	//! Increment iterator to next element in the range
	inline
	IndexIterator &
	operator++
		();

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

};

}

// Inline definitions
#include "libdat/IndexIterator.inl"

#endif // dat_IndexIterator_INCL_

