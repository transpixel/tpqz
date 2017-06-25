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

#ifndef sig_MatchConfig_INCL_
#define sig_MatchConfig_INCL_

/*! \file
\brief Declarations for sig::MatchConfig
*/


#include "libdat/RowCol.h"
#include "libdat/SpotX.h"
#include "libdat/SubExtents.h"

#include <string>


namespace sig
{

/*! \brief Basic configuration for grid-to-grid matching
*/

struct MatchConfig
{

public: // data

	dat::SpotX const theTgtWrtRef;
	dat::Extents const theFilterSize;
	dat::Extents const theSizeA;
	dat::Extents const theSizeB;

public: // static methods

	//! Transformed instance - Quantization works for matching (generalize?)
	static
	dat::RowCol
	xformed
		( dat::SpotX const & x2wrt1
		, dat::RowCol const & abIn1
		);

	//! Transformed instance - Quantization works for matching (generalize?)
	static
	dat::SubExtents
	xformed
		( dat::SpotX const & x2wrt1
		, dat::SubExtents const & cropIn1
		);

public: // methods

	/* -- use value initialization {..., ...}
	MatchConfig
		() = default;
	*/

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Spot transformed into reference (A) frame (from Tgt/B)
	dat::Spot
	inReference
		( dat::Spot const & spotInTgt
		) const;

	//! Location transformed into reference (A) frame (from Tgt/B)
	dat::Spot
	inReference
		( dat::RowCol const & rcInTgt
		) const;

	//! Crop transformed into reference (A) frame (from Tgt/B)
	dat::SubExtents
	inReference
		( dat::SubExtents const & cropInTgt
		) const;

	//! Crop associated with overlap between reference and target extents
	dat::SubExtents
	overCropInReference
		() const;

	//! Crop into which moving filter windows will fit
	dat::SubExtents
	workCropInReference
		() const;

	//! Transform: reference source origin w.r.t. working crop origin
	dat::SpotX
	xReferenceWrtWork
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = {}
		) const;

};

}

// Inline definitions
// #include "libsig/MatchConfig.inl"

#endif // sig_MatchConfig_INCL_

