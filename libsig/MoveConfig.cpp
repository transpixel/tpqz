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
\brief Definitions for sig::MoveConfig
*/


#include "libsig/MoveConfig.h"

#include "libdat/info.h"
#include "libdat/ops.h"
#include "libsig/filter.h"

#include <sstream>


namespace sig
{

// static
MoveConfig
MoveConfig :: fullConfig
	( dat::Extents const & fullSize
	, dat::Extents const & hunkSize
	)
{
	MoveConfig mcon;
	if (fullSize.isValid() && hunkSize.isValid())
	{
		dat::Extents const moveSize
			( filter::moveSizeFor(fullSize.high(), hunkSize.high())
			, filter::moveSizeFor(fullSize.wide(), hunkSize.wide())
			);
		if (dat::isValid(moveSize))
		{
			mcon = MoveConfig(hunkSize, moveSize);
		}
	}
	return mcon;
}

// explicit
MoveConfig :: MoveConfig
	( dat::Extents const & hunkSize
	, dat::Extents const & moveSize
	)
	: theHunkSize(hunkSize)
	, theMoveSize(moveSize)
{
}

bool
MoveConfig :: isValid
	() const
{
	return
		(  (0u < theHunkSize.size())
		&& (0u < theMoveSize.size())
		);
}

dat::Extents
MoveConfig :: fullExtents
	() const
{
	return filter::fullExtentsFor(theHunkSize, theMoveSize);
}

dat::Spot
MoveConfig :: hunkCenterSpot
	() const
{
	dat::Spot center(dat::nullValue<dat::Spot>());
	if (isValid())
	{
		center = dat::centerOf(theHunkSize);
	}
	return center;
}

std::string
MoveConfig :: infoString
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
		dat::Extents const fullSize
			(filter::fullExtentsFor(theHunkSize, theMoveSize));
		oss << "hunkSize: " << dat::infoString(theHunkSize);
		oss << std::endl;
		oss << "moveSize: " << dat::infoString(theMoveSize);
		oss << std::endl;
		oss << "fullSize: " << dat::infoString(fullSize);
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

}


