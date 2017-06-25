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
\brief Definitions for sig::MatchConfig
*/


#include "libsig/MatchConfig.h"

#include "libdat/cast.h"
#include "libdat/xform.h"
#include "libsig/match.h"

#include <sstream>


namespace sig
{

// static
dat::RowCol
MatchConfig :: xformed
	( dat::SpotX const & x2wrt1
	, dat::RowCol const & abIn1
	)
{
	dat::Spot const spotIn1(dat::cast::Spot(abIn1));
	dat::Spot const spotIn2(x2wrt1(spotIn1));
	return dat::cast::RowCol(spotIn2);
}

// static
dat::SubExtents
MatchConfig :: xformed
	( dat::SpotX const & x2wrt1
	, dat::SubExtents const & cropIn1
	)
{
	dat::Spot const ulSpotIn1(dat::cast::Spot(cropIn1.theUL));
	dat::Spot const ulSpotIn2(x2wrt1(ulSpotIn1));
	dat::SubExtents const cropIn2
		( dat::cast::RowCol(ulSpotIn2)
		, cropIn1.theSize
		);
	return cropIn2;
}

bool
MatchConfig :: isValid
	() const
{
	return
		(  theTgtWrtRef.isValid()
		&& theFilterSize.isValid()
		&& theSizeA.isValid()
		&& theSizeB.isValid()
		);
}

dat::Spot
MatchConfig :: inReference
	( dat::Spot const & spotInTgt
	) const
{
	return dat::xform::inverse(theTgtWrtRef)(spotInTgt);
}

dat::Spot
MatchConfig :: inReference
	( dat::RowCol const & rcInTgt
	) const
{
	dat::Spot const spotInTgt(dat::cast::Spot(rcInTgt));
	return inReference(spotInTgt);
}

dat::SubExtents
MatchConfig :: inReference
	( dat::SubExtents const & cropInTgt
	) const
{
	return xformed(dat::xform::inverse(theTgtWrtRef), cropInTgt);
}

dat::SubExtents
MatchConfig :: overCropInReference
	() const
{
	return sig::match::overlapCrop(theSizeA, theTgtWrtRef, theSizeB);
}

dat::SubExtents
MatchConfig :: workCropInReference
	() const
{
	return sig::match::activeCrop(overCropInReference(), theFilterSize);
}

dat::SpotX
MatchConfig :: xReferenceWrtWork
	() const
{
	dat::SpotX const xWorkWrtSrcRef
		(dat::cast::Spot(workCropInReference().theUL));
	dat::SpotX const xSrcRefWrtWork
		(dat::xform::inverse(xWorkWrtSrcRef));
	return xSrcRefWrtWork;
}

std::string
MatchConfig :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	if (isValid())
	{
		constexpr char nl{ '\n' };
		oss << dat::infoString(theTgtWrtRef, "theTgtWrtRef");
		oss << nl;
		oss << dat::infoString(theSizeA, "theSizeA");
		oss << nl;
		oss << dat::infoString(theSizeB, "theSizeB");
		oss << nl;
		oss << dat::infoString(theFilterSize, "theFilterSize");
		oss << nl;
		oss << dat::infoString(workCropInReference(), "workSize");
		oss << nl;
		oss << dat::infoString(overCropInReference(), "overInA");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

}

