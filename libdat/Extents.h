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

#ifndef dat_Extents_INCL_
#define dat_Extents_INCL_

/*! \file
\brief Declarations for dat::Extents
*/


#include "libdat/dat.h"
#include "libdat/RowCol.h"

#include <array>
#include <cassert>
#include <string>


namespace dat
{

//! Simple container for 2D area dimensions (high & wide)
class Extents
{
	std::array<size_t, 2u> theArea;

public: // methods

	//! default null constructor
	inline
	Extents
		();

	//! value construction
	constexpr
	inline
	explicit
	Extents
		( size_t const & high
		, size_t const & wide
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! True if one (or both) dimension is zero
	inline
	bool
	empty
		() const;

	//! Height
	inline
	size_t const &
	high
		() const;

	//! Width
	inline
	size_t const &
	wide
		() const;

	//! Size (high() * wide())
	inline
	size_t
	size
		() const;

	//! Index for (high, wide)
	inline
	size_t const &
	operator[]
		( size_t const & ndx
		) const;

	//! True if all dimensions are exactly equal
	inline
	bool
	operator==
		( Extents const & other
		) const;

	// TODO - involves "interpretation" - should move someplace else
	//! True if {{row,col}} is a member of current extents
	inline
	bool
	includes
		( std::array<size_t, 2u> const & rowcol
		) const;

	//! Half size in each dimension (NOTE! module 2 division)
	inline
	Extents
	halfSizeMod2
		() const;

	//! Center rowcol (NOTE! module 2 division)
	inline
	RowCol
	centerMod2
		() const;

	//! RowCol "just inside" the BR corner (high(),wide() cast to RowCol)
	RowCol
	insideCornerBR
		() const;

	//! True if this instance fits within fullSize (<= in each dimension)
	bool
	fitsWithin
		( Extents const & other
		) const;

	//! Included UL corner location for given center (via module /2u)
	RowCol
	cornerFromCenter
		( RowCol const & centerRC
		) const;

	//! Center location for given UL corner (via module /2u)
	RowCol
	centerFromCorner
		( RowCol const & cornerRC
		) const;

	//! True if these sizes are the same
	bool
	nearlyEquals
		( Extents const & other
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
#include "libdat/Extents.inl"

#endif // dat_Extents_INCL_

