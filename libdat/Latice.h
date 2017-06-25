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

#ifndef dat_Latice_INCL_
#define dat_Latice_INCL_

/*! \file
\brief Declarations for dat::Latice
*/


#include "libdat/info.h"
#include "libio/stream.h"

#include <sstream>
#include <string>
#include <vector>


namespace dat
{

/*! \brief Regularly partitioned 3D structure (e.g. block of voxels)

\par Example
\dontinclude testdat/uLatice.cpp
\skip ExampleStart
\until ExampleEnd
*/

template <typename Type>
class Latice
{
	size_t theHigh;
	size_t theWide;
	size_t theDeep;
	std::vector<Type> theData;

	//! cache: layer size
	size_t theLaySize;

public: // typedef

	using value_type = typename std::vector<Type>::value_type;
	using iterator = typename std::vector<Type>::iterator;
	using const_iterator = typename std::vector<Type>::const_iterator;

public: // methods

	//! default null constructor
	Latice
		() = default;

	//! Construct to specified size
	explicit
	Latice
		( size_t const & high
		, size_t const & wide
		, size_t const & deep
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Total number of elements
	inline
	size_t
	size
		() const;

	//! Returns const reference to element
	inline
	Type const &
	operator()
		( size_t const & row
		, size_t const & col
		, size_t const & lay
		) const;

	//! Returns direct reference to element
	inline
	Type &
	operator()
		( size_t const & row
		, size_t const & col
		, size_t const & lay
		);

	//! use as const_iterator
	inline
	const_iterator
	begin
		() const;

	//! use as const_iterator
	inline
	const_iterator
	end
		() const;

	//! use as iterator
	inline
	iterator
	begin
		();

	//! use as iterator
	inline
	iterator
	end
		();

	//! Descriptive information about this instance.
	inline
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

private:

	//! Offset into data vector
	inline
	size_t
	offsetFor
		( size_t const & row
		, size_t const & col
		, size_t const & lay
		) const;

};

}

// Inline definitions
#include "libdat/Latice.inl"

#endif // dat_Latice_INCL_

