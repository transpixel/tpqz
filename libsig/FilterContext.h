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

#ifndef sig_FilterContext_INCL_
#define sig_FilterContext_INCL_

/*! \file
\brief Declarations for sig::FilterContext
*/


#include "libdat/Extents.h"
#include "libdat/Region.h"
#include "libdat/RowCol.h"
#include "libdat/SpotX.h"
#include "libdat/Spot.h"
#include "libdat/SubExtents.h"
#include "libsig/MoveConfig.h"

#include <sstream>


namespace sig
{

/*! \brief Moving filter context

\par Example
\dontinclude testsig/uFilterContext.cpp
\skip ExampleStart
\until ExampleEnd
*/

class FilterContext
{

// public : // data

	//! Location of (fixed) hunk in reference image
	dat::RowCol theRefUL;

	//! Location of move configuration in target image
	dat::RowCol theTgtUL;

	//! Specification of areas involved in matching
	sig::MoveConfig theMoveConfig;

	//! Quantization size (e.g. for color filter array alignments, etc)
	size_t theQuant;

public : // static methods

	//! Value construction
	static
	FilterContext
	fromCenters
		( dat::RowCol const & refCenterRC
		, dat::RowCol const & tgtCenterRC
		, dat::Extents const & hunkSize
		, dat::Extents const & moveSize
		, size_t const & quant = 1u
		);

	//! Area in target that overlaps with reference
	static
	dat::SubExtents
	commonCropIn1
		( dat::Extents const & size1
		, dat::SpotX const & x2w1
		, dat::Extents const & size2
		);

public : // methods

	//! Null instance
	FilterContext
		();

	//! Value construction
	explicit
	FilterContext
		( dat::RowCol const & refUL
		, dat::RowCol const & tgtUL
		, MoveConfig const & mcon
		, size_t const & quant = 1u
		);

	//! Value construction
	explicit
	FilterContext
		( dat::RowCol const & refUL
		, dat::RowCol const & tgtUL
		, dat::Extents const & hunkSize
		, dat::Extents const & moveSize
		, size_t const & quant = 1u
		);

	//! True if this instance is meaningful
	bool
	isValid
		() const;

	//! Working area in relation to sourceGrid
	dat::SubExtents
	referenceCrop
		() const;

	//! Working area in relation to sourceGridB - aligned to alignQuantize
	dat::SubExtents
	targetCrop
		() const;

	//! Extents of motion in target image for UL corner of hunk
	dat::Extents
	targetMoveSize
		() const;

	//! Location for *CENTER* of reference hunk
	dat::Spot
	hotSpotForReference
		() const;

	//! Location for *CENTER* of hunk when hunk UL corner is at pOrigInMove
	dat::Spot
	hotSpotForTargetAt
		( dat::Spot const & pOrigInMove
		) const;

	//! Specialization for half size downsample (e.g. bayer cfa bands)
	FilterContext
	halfContext
		() const;

	//! True if this and other are same (all exact integer compares)
	bool
	nearlyEquals
		( FilterContext const & other
		) const;

	//! Descriptive information about this instance
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

};

}

// Inline definitions
// #include "libsig/FilterContext.inl"

#endif // sig_FilterContext_INCL_

