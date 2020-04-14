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
\brief Definitions for cam::PinHole
*/


#include "libcam/PinHole.h"

#include "libio/sprintf.h"

#include <sstream>


namespace cam
{
//======================================================================

PinHole :: PinHole
	()
	: thePD(dat::nullValue<double>())
{
}

// explicit
PinHole :: PinHole
	( double const & pd
	)
	: thePD(pd)
{
}

bool
PinHole :: nearlyEquals
	( PinHole const & other
	, double const & tol
	) const
{
	return
		{  dat::nearlyEquals(thePD, other.thePD, tol)
		};
}

std::string
PinHole :: infoString
	( std::string const & title
	, std::string const & fmt
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << " ";
	}
	oss << "PD: " << io::sprintf(fmt, thePD);
	return oss.str();
}

//======================================================================
}


