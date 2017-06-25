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
\brief Definitions for geo::Ray
*/


#include "libgeo/Ray.h"

#include "libdat/info.h"
#include "libga/constructs.h"

#include <sstream>


namespace geo
{
//======================================================================

// static
Ray
Ray :: null
	()
{
	return Ray{};
}

// static
Ray
Ray :: fromToward
	( ga::Vector const & rayStart
	, ga::Vector const & aimPoint
	)
{
	return Ray(rayStart, ga::unit(aimPoint - rayStart));
}

//======================================================================

// explicit
Ray :: Ray
	( ga::Vector const & start
	, ga::Vector const & dir
	)
	: theStart{ start }
	, theDir{ ga::unit(dir) }
{ }

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

std::string
Ray :: infoString
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
		oss << dat::infoString(theStart, "theStart");

		oss << std::endl;
		oss << dat::infoString(theDir, "theDir");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

//======================================================================
}

