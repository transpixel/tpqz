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
\brief Definitions for app::Progress
*/


#include "libapp/Progress.h"

#include "libdat/info.h"

#include <sstream>


namespace app
{
//======================================================================

// explicit
Progress :: Progress
	( size_t const & maxValue
	, size_t const & startValue
	)
	: theMaxValue(maxValue)
	, theAtValue(startValue)
{
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

bool
Progress :: isValid
	() const
{
	return dat::isValid(theMaxValue);
}

Progress const &
Progress :: operator++
	()
{
	++theAtValue;
	if (theMaxValue < theAtValue)
	{
		theAtValue = theMaxValue;
	}
	return *this;
}

Progress const &
Progress :: advance
	( size_t const & numBump
	)
{
	size_t nn{ 0u };
	while ((nn++ < numBump) && (theAtValue < theMaxValue))
	{
		operator++();
	}
	return *this;
}

double
Progress :: fractionDone
	() const
{
	double frac{ dat::nullValue<double>() };
	if (isValid())
	{
		if (theAtValue < theMaxValue)
		{
			frac = double(theAtValue) / double(theMaxValue);
		}
		else
		{
			frac = 1.;
		}
	}
	return frac;
}

uint8_t
Progress :: percentDone
	() const
{
	uint8_t pct{ dat::nullValue<uint8_t>() };
	double const frac{ fractionDone() };
	if (dat::isValid(frac))
	{
		pct = uint8_t(std::floor(100.*frac + .5));
	}
	return pct;
}

std::string
Progress :: infoString
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
			<< "  at " << theAtValue
			<< "  of " << theMaxValue
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

//======================================================================
}

