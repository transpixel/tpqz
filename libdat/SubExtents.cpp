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
\brief Definitions for dat::SubExtents
*/


#include "libdat/SubExtents.h"

#include "libdat/discrete.h"
#include "libdat/info.h"
#include "libdat/validity.h"

#include <cassert>
#include <sstream>


namespace dat
{

// static
SubExtents
SubExtents :: centeredOn
	( dat::RowCol const & midRowCol
	, dat::Extents const & cropSize
	)
{
	dat::Extents const halfSize(cropSize[0]/2u, cropSize[1]/2u);
	assert(halfSize[0] <= midRowCol[0]);
	assert(halfSize[1] <= midRowCol[1]);
	dat::RowCol const ulCorner
		{{ (midRowCol[0] - halfSize[0])
		 , (midRowCol[1] - halfSize[1])
		}};
	return SubExtents(ulCorner, cropSize);
}

// static
SubExtents
SubExtents :: croppedCenterSize
	( RowCol const & midRowCol
	, Extents const & reqCropSize
	, Extents const & fullSize
	)
{
	SubExtents sub;

	std::array<NdxBegEnd, 2u> const fullBEs
		{{ NdxBegEnd{ 0u, fullSize[0] }
		 , NdxBegEnd{ 0u, fullSize[1] }
		}};
	std::array<NdxBegEnd, 2u> const cropBEs
		{{ fullBEs[0].croppedCenterSize(midRowCol[0], reqCropSize[0])
		 , fullBEs[1].croppedCenterSize(midRowCol[1], reqCropSize[1])
		}};

	if (cropBEs[0].isValid() && cropBEs[1].isValid())
	{
		RowCol const ulCorner
			{{ cropBEs[0].begin(), cropBEs[1].begin() }};
		Extents const useCropSize
			(cropBEs[0].size(), cropBEs[1].size());
		sub = SubExtents(ulCorner, useCropSize);
	}

	return sub;
}

// static
SubExtents
SubExtents :: quantized
	( SubExtents const & orig
	, size_t const & delta
	)
{
	SubExtents result;
	if (orig.isValid())
	{
		dat::RowCol const currBR(orig.outsideCornerBR());
		dat::RowCol const quantUL
			{{ dat::discrete::quantizedLo(orig.theUL[0], delta)
			 , dat::discrete::quantizedLo(orig.theUL[1], delta)
			}};
		dat::RowCol const quantBR
			{{ dat::discrete::quantizedHi(currBR[0], delta)
			 , dat::discrete::quantizedHi(currBR[1], delta)
			}};
		dat::Extents const quantSize
			( quantBR[0] - quantUL[0]
			, quantBR[1] - quantUL[1]
			);
		// TODO remove these - but revisit all logic first
		assert(dat::isValid(currBR));
		assert(dat::isValid(quantUL));
		assert(dat::isValid(quantBR));
		assert(quantSize.isValid());
		result = SubExtents(quantUL, quantSize);
	}
	return result;
}


//======================================================================


SubExtents :: SubExtents
	()
	: theUL(dat::nullValue<dat::RowCol>())
	, theSize{}
{}

// explicit
SubExtents :: SubExtents
	( dat::RowCol const & ulCorner
	, dat::Extents const & hwSize
	)
	: theUL(ulCorner)
	, theSize{ hwSize }
{
}

// explicit
SubExtents :: SubExtents
	( dat::NdxBegEnd const & rowBegEnd
	, dat::NdxBegEnd const & colBegEnd
	)
	: theUL{{ rowBegEnd.begin(), colBegEnd.begin() }}
	, theSize(rowBegEnd.size(), colBegEnd.size())
{
}

bool
SubExtents :: isValid
	() const
{
	return
		(  dat::isValid(theUL)
		&& theSize.isValid()
		);
}

dat::RowCol
SubExtents :: outsideCornerBR
	() const
{
	dat::RowCol corner(dat::nullValue<dat::RowCol>());
	if (isValid())
	{
		corner = dat::RowCol
			{{ theUL[0] + theSize[0]
			 , theUL[1] + theSize[1]
			}};
	}
	return corner;
}

dat::RowCol
SubExtents :: insideCornerBR
	() const
{
	dat::RowCol corner(dat::nullValue<dat::RowCol>());
	if (isValid())
	{
		dat::RowCol const endBR(outsideCornerBR());
		if (dat::isValid(endBR))
		{
			using dat::operator-;
			static constexpr dat::RowCol oneone{{ 1u, 1u }};
			corner = endBR - oneone;
		}
	}
	return corner;
}

dat::Spot
SubExtents :: center
	() const
{
	return
		{{ double(theUL[0]) + double(high())/2.
		 , double(theUL[1]) + double(wide())/2.
		}};
}

bool
SubExtents :: fitsWithin
	( dat::Extents const & fullSize
	) const
{
	bool okay{ false };
	if (isValid() && fullSize.isValid())
	{
		okay = fullSize.includes(insideCornerBR());
	}
	return okay;
}

bool
SubExtents :: nearlyEquals
	( SubExtents const & other
	) const
{
	return
		(  dat::nearlyEquals(theUL, other.theUL)
		&& (theSize == other.theSize)
		);
}

std::string
SubExtents :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << " ";
	}
	if (isValid())
	{
		oss
			<< "theUL:"
			<< " " << dat::infoString(theUL)
			<< "  "
			<< "theSize:"
			<< " " << dat::infoString(theSize)
			<< "  "
			<< "center:"
			<< " " << dat::infoString(center())
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}
}

