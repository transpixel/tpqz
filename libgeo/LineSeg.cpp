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
\brief Definitions for geo::LineSeg
*/


#include "libgeo/LineSeg.h"
#include "libdat/info.h"

#include <sstream>


namespace geo
{
//======================================================================

// static
LineSeg
LineSeg :: between
	( ga::Vector const & begPoint
	, ga::Vector const & endPoint
	)
{
	LineSeg seg;
	ga::Vector const delta(endPoint - begPoint);
	ga::Vector const dir(ga::unit(delta));
	if (dir.isValid())
	{
		double const mag{ ga::magnitude(delta) };
		if (dat::isValid(mag))
		{
			Ray const ray(begPoint, dir);
			seg = LineSeg(ray, mag);
		}
	}
	return seg;
}


// explicit
LineSeg :: LineSeg
	( Ray const & ray
	, double const & length
	)
	: theRay{ ray }
	, theLength{ length }
{ }


// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

bool
LineSeg :: isValid
	() const
{
	return dat::isValid(theLength);
}

std::string
LineSeg :: infoString
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
		oss << dat::infoString(start(), "start");
		oss << std::endl;
		oss << dat::infoString(stop(), "stop");
		oss << std::endl;
		oss
			<< dat::infoString(theRay.theDir, "dir")
			<< "  length: "
			<< dat::infoString(theLength);
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

//======================================================================
}

