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
\brief Definitions for cam::Camera
*/


#include "libcam/Camera.h"


namespace
{
	dat::Range<double>
	symmetricRange
		( size_t const & fullSpan
		)
	{
		return dat::Range<double>
			( -double(fullSpan) / 2.
		 	,  double(fullSpan) / 2.
			);
	}
}


namespace cam
{

// explicit
Camera :: Camera
	( double const & pd
	, dat::Extents const & detSize
	)
	: theOptics(pd)
	, theDetSize(detSize)
	, theAreaInImg
		{ symmetricRange(theDetSize.high())
		, symmetricRange(theDetSize.wide())
		}
	, theAreaInDet
		{ dat::Range<double>(0., theDetSize.high())
		, dat::Range<double>(0., theDetSize.wide())
		}
{ }

bool
Camera :: isValid
	() const
{
	return
		(  theOptics.isValid()
		&& theDetSize.isValid()
		&& theAreaInImg.isValid()
		);
}

std::array<ga::Vector, 4u>
Camera :: cornerDirections
	() const
{
	std::array<ga::Vector, 4u> dirs;
	dat::Range<double> const & rowRange = theAreaInImg[0];
	dat::Range<double> const & colRange = theAreaInImg[1];
	std::array<dat::Spot, 4u> const corners
		{ dat::Spot{ rowRange.min(), colRange.min() }
		, dat::Spot{ rowRange.min(), colRange.max() }
		, dat::Spot{ rowRange.max(), colRange.max() }
		, dat::Spot{ rowRange.max(), colRange.min() }
		};
	for (size_t nn{0} ; nn < corners.size() ; ++nn)
	{
		dirs[nn] = theOptics.directionOf(corners[nn]);
	}
	return dirs;
}

std::string
Camera :: infoString
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
		oss << dat::infoString(theOptics, "theOptics");
		oss << std::endl;
		oss << dat::infoString(theDetSize, "theDetSize");
		oss << std::endl;
		oss << dat::infoString(theAreaInImg, "theAreaInImg");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

}

