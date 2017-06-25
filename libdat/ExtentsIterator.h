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

#ifndef dat_ExtentsIterator_INCL_
#define dat_ExtentsIterator_INCL_

/*! \file
\brief Declarations for dat::ExtentsIterator
*/


#include "libdat/Extents.h"

#include <string>


namespace dat
{

/*! \brief Simple iterator over row/col values within dat::Extents

\par Example
\dontinclude testdat/uExtentsIterator.cpp
\skip ExampleStart
\until ExampleEnd
*/

class ExtentsIterator
{

	dat::RowCol theUL{{}};
	dat::Extents theDomain{};
	bool theIsActive{ false };

public: // methods

	//! default null constructor
	ExtentsIterator
		() = default;

	//! Attach to domainSize and initialize to beginning
	inline
	explicit
	ExtentsIterator
		( dat::Extents const & domainSize
		);

	//! True if this instance is not null - but may be active or not
	inline
	bool
	isValid
		() const;

	//! True if dereference op*() is valid -- not (yet) at end of domain
	inline
	bool
	isActive
		() const;

	//! Like isActive() but w/o validity check
	inline
	explicit operator bool
		() const;

	//! RowCol expression of current iterator value
	inline
	dat::RowCol const &
	operator*
		() const;

	//! Move forward by one element of domain
	inline
	ExtentsIterator &
	operator++
		();

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

};

}

// Inline definitions
#include "libdat/ExtentsIterator.inl"

#endif // dat_ExtentsIterator_INCL_

