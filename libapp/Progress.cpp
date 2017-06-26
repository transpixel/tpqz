//
// TODO_HEADER_NOTICE
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

