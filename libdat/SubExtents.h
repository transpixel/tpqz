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

#ifndef dat_SubExtents_INCL_
#define dat_SubExtents_INCL_

/*! \file
\brief Declarations for dat::SubExtents
*/


#include "libdat/Extents.h"
#include "libdat/grid.h"
#include "libdat/NdxBegEnd.h"
#include "libdat/RowCol.h"
#include "libdat/Spot.h"

#include <string>


namespace dat
{

//! \brief An Extents relative to some other 'full' Extents
struct SubExtents
{

public: // data

	dat::RowCol theUL;
	dat::Extents theSize;

public: // methods

	//! A crop of cropSize and centered on midRowCol (args must fit!)
	static
	SubExtents
	centeredOn
		( dat::RowCol const & midRowCol
		, dat::Extents const & cropSize
		);

	//! Like centeredOn(), but results adjusted/cropped within fullSize
	static
	SubExtents
	croppedCenterSize
		( RowCol const & midRowCol
		, Extents const & reqCropSize
		, Extents const & fullSize
		);

	//! SubExtents adjusted to align with delta (adjusted 'upward' in sizes)
	static
	SubExtents
	quantized
		( SubExtents const & orig
		, size_t const & delta
		);

public: // methods

	//! Construct a null instance
	SubExtents
		();

	//! value ctor
	explicit
	SubExtents
		( dat::RowCol const & ulCorner
		, dat::Extents const & hwSize
		);

	//! value ctor
	explicit
	SubExtents
		( dat::NdxBegEnd const & rowBegEnd
		, dat::NdxBegEnd const & colBegEnd
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! True if this instance is valid
	bool
	isValid
		() const;

	//! Forward theSize.high()
	inline
	size_t const &
	high
		() const;

	//! Forward theSize.wide()
	inline
	size_t const &
	wide
		() const;

	//! RowCol "one-past" the bottom right corner (e.g end iterator offsets)
	dat::RowCol
	outsideCornerBR
		() const;

	//! RowCol "just inside" the bottom right corner
	dat::RowCol
	insideCornerBR
		() const;

	//! Location of center
	dat::Spot
	center
		() const;

	//! True if this instance fits entirely within fullSize
	bool
	fitsWithin
		( dat::Extents const & fullSize
		) const;

	//! Location w.r.t. the domain on which this instance is defined
	inline
	dat::RowCol
	subRowColFor
		( dat::RowCol const & fullRowCol
		) const;

	//! Location w.r.t. the domain on which this instance is defined
	inline
	dat::RowCol
	fullRowColFor
		( dat::RowCol const & subRowCol
		) const;

	//! True if fullRowCol is included within this subArea
	inline
	bool
	includes
		( dat::RowCol const & fullRowCol
		) const;

	//! Location w.r.t. the domain on which this instance is defined
	inline
	dat::Spot
	fullSpotFor
		( dat::Spot const & relSpot
		) const;

	//! Iterator pair for fullGrid associated with subRow of this
	template <typename PixType>
	inline
	std::pair
		< typename dat::grid<PixType>::const_iterator
		, typename dat::grid<PixType>::const_iterator
		>
	iterFullBegEnd
		( dat::grid<PixType> const & fullGrid
		, size_t const & subRow
		) const;

	//! True if this and other are the same
	bool
	nearlyEquals
		( SubExtents const & other
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

};

}

// Inline definitions
#include "libdat/SubExtents.inl"

#endif // dat_SubExtents_INCL_

