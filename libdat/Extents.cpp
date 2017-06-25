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
\brief Definitions for dat::Extents
*/


#include "libdat/Extents.h"

#include <iomanip>
#include <sstream>

#include "libio/stream.h"
#include "libdat/info.h"

namespace dat
{
//======================================================================

dat::RowCol
Extents :: insideCornerBR
	() const
{

	RowCol br(dat::nullValue<RowCol>());
	if (isValid())
	{
		br = {{ high()-1u , wide()-1u }};
	}
	return br;
}

bool
Extents :: fitsWithin
	( dat::Extents const & other
	) const
{
	bool fits{ false };
	if (isValid() && other.isValid())
	{

		fits =
			(  (high() <= other.high())
			&& (wide() <= other.wide())
			);
	}
	return fits;
}

RowCol
Extents :: cornerFromCenter
	( RowCol const & centerRC
	) const
{
	RowCol corner(nullValue<RowCol>());
	if (isValid() && dat::isValid(centerRC))
	{
		RowCol const hDelta(centerMod2());
		if (dat::canSubtract(centerRC, hDelta))
		{
			using dat::operator-;
			corner = centerRC - hDelta;
		}
	}
	return corner;
}

RowCol
Extents :: centerFromCorner
	( RowCol const & cornerRC
	) const
{
	RowCol center(nullValue<RowCol>());
	if (isValid() && dat::isValid(cornerRC))
	{
		RowCol const hDelta(centerMod2());
		using dat::operator+;
		center = cornerRC + hDelta;
	}
	return center;
}

bool
Extents :: nearlyEquals
	( Extents const & other
	) const
{
	bool same{ false };
	if (isValid() && other.isValid())
	{
		same =
			(  (high() == other.high())
			&& (wide() == other.wide())
			);
	}
	return same;
}

std::string
Extents :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << "  ";
	}
	oss << std::setw(5) << high();
	oss << " " << std::setw(5) << wide();
	oss << " " << std::setw(5) << size();
	return oss.str();
}

//======================================================================
}

