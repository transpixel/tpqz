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

#ifndef dat_NdxBegEnd_INCL_
#define dat_NdxBegEnd_INCL_

/*! \file
\brief Declarations for dat::NdxBegEnd
*/


#include "libdat/info.h"
#include "libdat/validity.h"

#include <string>
#include <utility>


namespace dat
{
/*! \brief Classic begin/end index pair - with various utility operations.

\par Example
\dontinclude testdat/uNdxBegEnd.cpp
\skip ExampleStart
\until ExampleEnd
*/

struct NdxBegEnd
{
	size_t theNdxBeg{ dat::nullValue<size_t>() };
	size_t theNdxEnd{ dat::nullValue<size_t>() };

public: // methods

	//! default null constructor
	NdxBegEnd
		() = default;

	//! Value construction
	explicit
	NdxBegEnd
		( size_t const & ndxBeg
		, size_t const & ndxEnd
		);

	//! Value construction
	explicit
	NdxBegEnd
		( std::pair<size_t, size_t> const & bePair
		);

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! First included index.
	size_t const &
	begin
		() const;

	//! One-past last included index
	size_t const &
	end
		() const;

	//! Number of included indices
	size_t
	size
		() const;

	//! Sub interval of size, centered modulo-2 (exact if size() is odd)
	NdxBegEnd
	centeredSubOfSize
		( size_t const & subSize
		) const;

	//! Sub interval centered (mod2) on midNdx with size - cropped to fit this
	NdxBegEnd
	croppedCenterSize
		( size_t const & subCenter
		, size_t const & subSize
		) const;

	//! True if ends are equal to those of other within tolerance
	bool
	nearlyEquals
		( NdxBegEnd const & other
		, size_t const & tol = { 0u }
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // NdxBegEnd

} // dat

// Inline definitions
// #include "libdat/NdxBegEnd.inl"

#endif // dat_NdxBegEnd_INCL_

