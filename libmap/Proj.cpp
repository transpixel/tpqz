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
\brief Definitions for map::Proj
*/


#include "libmap/Proj.h"

#include "libdat/info.h"
#include "libdat/validity.h"

#include <sstream>


namespace map
{

// explicit
Proj :: Proj
	( dat::Extents const & hwSize
	)
	: theGridSize{ hwSize }
{
}

dat::Extents const &
Proj :: hwSize
	() const
{
	return theGridSize;
}

// virtual
bool
Proj :: isValid
	() const
{
	return theGridSize.isValid();
}

// virtual
dat::RowCol
Proj :: gridRowColFor
	( ga::Vector const & // dir
	) const
{
	return dat::nullValue<dat::RowCol>();
}

// virtual
ga::Vector
Proj :: directionFor
	( dat::RowCol const & // mapRC
	) const
{
	return ga::Vector{};
}

// virtual
std::string
Proj :: infoString
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
		oss << dat::infoString(hwSize(), "hwSize");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

}

