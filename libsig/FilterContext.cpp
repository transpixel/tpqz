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


/*! \file
\brief Definitions for sig::FilterContext
*/


#include "libsig/FilterContext.h"

#include "libdat/discrete.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libdat/xform.h"

#include "libio/stream.h"

#include <sstream>


namespace sig
{

namespace
{
	//! UL corner given center and size
	dat::RowCol
	ulCornerFor
		( dat::RowCol const & centerRC
		, dat::Extents const & fullSize
		, size_t const & quant
		)
	{
		dat::RowCol ulRC(dat::nullValue<dat::RowCol>());

		dat::RowCol const halfRC(fullSize.centerMod2());
		if (dat::canSubtract(centerRC, halfRC))
		{
			using dat::operator-;
			ulRC = dat::discrete::quantized(centerRC - halfRC, quant);
		}

		return ulRC;
	}

}

// static
FilterContext
FilterContext :: fromCenters
	( dat::RowCol const & refCenterRC
	, dat::RowCol const & tgtCenterRC
	, dat::Extents const & hunkSize
	, dat::Extents const & moveSize
	, size_t const & quant
	)
{
	FilterContext fc;

	// determine reference UL (w.r.t. source)
	dat::Extents const & refSize = hunkSize;
	dat::RowCol const refUL(ulCornerFor(refCenterRC, refSize, quant));

	// determine target UL (w.r.t. source)
	MoveConfig const tgtConfig(hunkSize, moveSize);
	dat::Extents const tgtSize(tgtConfig.fullExtents());
	dat::RowCol const tgtUL(ulCornerFor(tgtCenterRC, tgtSize, quant));

	// if both are valid, then populate return value
	if (dat::isValid(refUL) && dat::isValid(tgtUL))
	{
		fc.theRefUL = refUL;
		fc.theTgtUL = tgtUL;
		fc.theMoveConfig = tgtConfig;
		fc.theQuant = quant;
		assert(fc.isValid());
	}

	return fc;
}

// static
dat::SubExtents
FilterContext :: commonCropIn1
	( dat::Extents const & size1
	, dat::SpotX const & x2w1
	, dat::Extents const & size2
	)
{
	dat::SubExtents bothIn1;

	if (size1.isValid() && size2.isValid() && x2w1.isValid())
	{
		dat::SubExtents & bothInRef = bothIn1;
		dat::Extents const & sizeRef = size1;
		dat::SpotX const & xTgtWrtRef = x2w1;
		dat::Extents const & sizeTgt = size2;
		static dat::Spot const zero{{ 0., 0. }};

		dat::SpotX const xRefWrtTgt(dat::xform::inverse(xTgtWrtRef));

		// corners of reference expressed in frame 1
		dat::Spot const & ul1inRef = zero;
		dat::Spot const br1inRef
			{{ double(sizeRef.high()), double(sizeRef.wide()) }};

		// corners of target expressed in frame 2
		dat::Spot const & ul2inTgt = zero;
		dat::Spot const br2inTgt
			{{ double(sizeTgt.high()), double(sizeTgt.wide()) }};

		// determine corners of target expressed in frame 1
		dat::Spot const ul2inRef(xRefWrtTgt(ul2inTgt));
		dat::Spot const br2inRef(xRefWrtTgt(br2inTgt));

		// find the innermost (max) UL corner values
		dat::Spot const ulBothInRef
			{{ std::max(ul1inRef[0], ul2inRef[0])
			 , std::max(ul1inRef[1], ul2inRef[1])
			}};
		// find the innermost (min) UL corner values
		dat::Spot const brBothInRef
			{{ std::min(br1inRef[0], br2inRef[0])
			 , std::min(br1inRef[1], br2inRef[1])
			}};

		// ensure positive values
		dat::RowCol const bothUL
			{{ (size_t)(std::max(0., ulBothInRef[0]))
			 , (size_t)(std::max(0., ulBothInRef[1]))
			}};
		dat::Extents const bothSize
			( (size_t)(std::max(0., brBothInRef[0] - ulBothInRef[0]))
			, (size_t)(std::max(0., brBothInRef[1] - ulBothInRef[1]))
			);

		bothInRef = dat::SubExtents(bothUL, bothSize);
	}

	return bothIn1;
}

FilterContext :: FilterContext
	()
	: theRefUL(dat::nullValue<dat::RowCol>())
	, theTgtUL(dat::nullValue<dat::RowCol>())
	, theMoveConfig{}
	, theQuant{ dat::nullValue<size_t>() }
{ }

// explicit
FilterContext :: FilterContext
	( dat::RowCol const & refUL
	, dat::RowCol const & tgtUL
	, MoveConfig const & mcon
	, size_t const & quant
	)
	: theRefUL(refUL)
	, theTgtUL(tgtUL)
	, theMoveConfig(mcon)
	, theQuant{ quant }
{
}

// explicit
FilterContext :: FilterContext
	( dat::RowCol const & refUL
	, dat::RowCol const & tgtUL
	, dat::Extents const & hunkSize
	, dat::Extents const & moveSize
	, size_t const & quant
	)
	: FilterContext(refUL, tgtUL, MoveConfig(hunkSize, moveSize), quant)
{
}

bool
FilterContext :: isValid
	() const
{
	return
		(  dat::isValid(theRefUL)
		&& dat::isValid(theTgtUL)
		&& theMoveConfig.isValid()
		);
}

/*
FilterContext
FilterContext :: quantized
	( size_t const & quant
	) const
{
	FilterContext fc;
	if (isValid())
	{
		// start with current limits
		dat::RowCol const & cRefUL = theRefUL;
		dat::RowCol const & cTgtUL = theTgtUL;
		dat::Extents const & cPatchSize = theMoveConfig.theHunkSize;
		dat::Extents const & cMoveSize = theMoveConfig.theMoveSize;

		// quantize corners and sizes
		using dat::discrete::quantized;
		dat::RowCol const qRefUL(quantized(cRefUL, quant));
		dat::RowCol const qTgtUL(quantized(cTgtUL, quant));
		dat::Extents const qPatchSize(quantized(cPatchSize, quant));
		dat::Extents const qMoveSize(quantized(cMoveSize, quant));

		// return quantized instance
		fc = FilterContext(qRefUL, qTgtUL, qPatchSize, qMoveSize);
	}
	return fc;
}
*/

dat::SubExtents
FilterContext :: referenceCrop
	() const
{
	dat::SubExtents sub;
	if (isValid())
	{
		dat::Extents const & refSize = theMoveConfig.theHunkSize;
		using dat::discrete::quantized;
		sub = dat::SubExtents(theRefUL, quantized(refSize, theQuant));
	}
	return sub;
}

dat::SubExtents
FilterContext :: targetCrop
	() const
{
	dat::SubExtents sub;
	if (isValid())
	{
		dat::Extents const tgtSize(theMoveConfig.fullExtents());
		using dat::discrete::quantized;
		sub = dat::SubExtents(theTgtUL, quantized(tgtSize, theQuant));
	}
	return sub;
}

dat::Extents
FilterContext :: targetMoveSize
	() const
{
	return theMoveConfig.theMoveSize;
}

namespace
{
	//! Location for *CENTER* of hunk when hunk UL corner is at pOrigInMove
	dat::Spot
	hotSpotInFull
		( dat::Spot const & pOrigInMove
		, dat::Extents const & hunkSize
		, dat::Extents const & moveSize
		)
	{
		dat::Spot centerInFull(dat::nullValue<dat::Spot>());
		if (dat::isValid(pOrigInMove))
		{
			// determine center of hunk wrt hunk orig
			MoveConfig const config(hunkSize, moveSize);
			if (config.isValid())
			{
				// consider hunk center to be the hotspot
				dat::Spot const pCenterSpot(config.hunkCenterSpot());

				// transform hunk center into full target frame
				using dat::operator+;
				centerInFull = pOrigInMove + pCenterSpot;
			}
		}
		return centerInFull;
	}
}

dat::Spot
FilterContext :: hotSpotForReference
	() const
{
	dat::Spot centerInFull(dat::nullValue<dat::Spot>());
	if (isValid())
	{
		static dat::Spot const pOrigInMove{{ 0u, 0u }};
		static dat::Extents const moveSize(1u, 1u);
		dat::Extents const & hunkSize = theMoveConfig.theHunkSize;
		dat::Spot const centerWrtMove
			(hotSpotInFull(pOrigInMove, hunkSize, moveSize));
		using dat::operator+;
		centerInFull = dat::cast::Spot(theRefUL) + centerWrtMove;
	}
	return centerInFull;
}

dat::Spot
FilterContext :: hotSpotForTargetAt
	( dat::Spot const & pOrigInMove
	) const
{
	dat::Spot centerInFull(dat::nullValue<dat::Spot>());
	if (isValid())
	{
		MoveConfig const & cfg = theMoveConfig;
		dat::Spot const centerWrtMove
			(hotSpotInFull(pOrigInMove, cfg.theHunkSize, cfg.theMoveSize));
		using dat::operator+;
		centerInFull = dat::cast::Spot(theTgtUL) + centerWrtMove;
	}
	return centerInFull;
}

namespace
{
	//! True if both high() and wide() are even
	inline
	bool
	isEven
		( dat::RowCol const & rowcol
		)
	{
		return
			(  dat::isEven(rowcol[0])
			&& dat::isEven(rowcol[1])
			);
	}

	//! True if both high() and wide() are even
	inline
	bool
	isEven
		( dat::Extents const & hwSize
		)
	{
		return
			(  dat::isEven(hwSize.high())
			&& dat::isEven(hwSize.wide())
			);
	}

	//! Each component (integer) divided by 2
	inline
	dat::RowCol
	halfMod2
		( dat::RowCol const & rowcol
		)
	{
		return {{ (rowcol[0] / 2u), (rowcol[1] / 2u) }};
	}
}

FilterContext
FilterContext :: halfContext
	() const
{
	FilterContext half;
	if (isValid())
	{
		// ensure all sizes are even, so that halving will be exact
		assert(isEven(theRefUL));
		assert(isEven(theTgtUL));
		assert(isEven(theMoveConfig.theHunkSize));
		assert(isEven(theMoveConfig.theMoveSize));

		// construct half size context
		half = FilterContext
			( halfMod2(theRefUL)
			, halfMod2(theTgtUL)
			, theMoveConfig.theHunkSize.halfSizeMod2()
			, theMoveConfig.theMoveSize.halfSizeMod2()
			, theQuant
			);
	}
	return half;
}

bool
FilterContext :: nearlyEquals
	( FilterContext const & other
	) const
{
	return
		(  (theRefUL == other.theRefUL)
		&& (theTgtUL == other.theTgtUL)
		&& (theMoveConfig.theHunkSize == other.theMoveConfig.theHunkSize)
		&& (theMoveConfig.theMoveSize == other.theMoveConfig.theMoveSize)
		);
}

std::string
FilterContext :: infoString
	( std::string const & title
	) const
{
	std::stringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	if (isValid())
	{
		oss << " theRefUL:"
			<< " " << dat::infoString(theRefUL);

		oss << std::endl;
		oss << " theTgtUL:"
			<< " " << dat::infoString(theTgtUL);

		oss << std::endl;
		oss << "   theHunkSize:"
			<< " " << dat::infoString(theMoveConfig.theHunkSize);

		oss << std::endl;
		oss << "    theMoveSize:"
			<< " " << dat::infoString(theMoveConfig.theMoveSize);

		oss << std::endl;
		oss << "  referenceCrop:"
			<< " " << dat::infoString(referenceCrop());

		oss << std::endl;
		oss << "     targetCrop:"
			<< " " << dat::infoString(targetCrop());
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

}

