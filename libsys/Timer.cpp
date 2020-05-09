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
\brief Definitions for sys::Timer
*/


#include "libsys/Timer.h"

#include <iomanip>
#include <sstream>


namespace sys
{
//======================================================================

inline
std::string
intervalString
	( std::string const & iname
	, double const & ivalue
	)
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(9);
	oss << std::setw(25u) << iname << ": " << std::setw(14u) << ivalue;
	return oss.str();
}

std::string
Timer::Interval :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;

	if (! title.empty())
	{
		oss << title << "  ";
	}

	if (isDone())
	{
		// actual interval time
		oss << intervalString(theInfo, duration());
	}
	else
	{
		// in progress running time
		double const delta(sys::time::relativeNow() - theStart);
		oss << intervalString(theInfo, delta) << "  (active)";
	}

	return oss.str();
}

//
//
//

double
Timer :: total
	() const
{
	double sum(0.);
	for (size_t nn(0u) ; nn < theIntervals.size() ; ++nn)
	{
		Interval const & ival = theIntervals[nn];
		sum += ival.duration();
	}
	return sum;
}

std::string
Timer :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << '\n';
	}

	// include each interval
	double sum(0.);
	for (size_t nn(0u) ; nn < theIntervals.size() ; ++nn)
	{
		Interval const & ival = theIntervals[nn];

		if (0u < nn)
		{
			oss << '\n';
		}
		if (ival.isDone())
		{
			sum += ival.duration();
		}
		oss << ival.infoString();
	}

	// add total time
	if (1u < theIntervals.size())
	{
		oss << '\n';
		oss << intervalString("--- total ---", sum);
	}

	return oss.str();
}

//======================================================================
}

