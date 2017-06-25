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
\brief Inline definitions for trans::RangeMap
*/


#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"
#include "libmath/interp.h"
#include "libtrans/trans.h"

#include <cassert>
#include <iostream>
#include <sstream>


namespace trans
{
//======================================================================


namespace priv
{
	static std::function<double(double const &)> const nullMapper
		( [] (double const &) { return dat::nullValue<double>(); } );
}

inline
RangeMap :: RangeMap
	()
	: theDomain()
	, theUnitMap(priv::nullMapper)
	, theRange()
{
}

inline
// explicit
RangeMap :: RangeMap
	( dat::Range<double> const & domain
	, std::function<double(double const &)> const & unitMapping
	, dat::Range<double> const & range
	)
	: theDomain(domain)
	, theUnitMap(unitMapping)
	, theRange(range)
{
	if (! theRange.isValid())
	{
		theRange = theDomain;
	}

	// verify that unit mapping function behaves at/near endpoints
	static double const eps(dat::smallValue<double>());
	double const gotY0(unitMapping(0.));
	double const gotY1(unitMapping(1. - eps));
	static double const expY0(0.);
	static double const expY1(1. - eps);
	if (  (! dat::nearlyEquals(gotY0, expY0))
	   || (! dat::nearlyEquals(gotY1, expY1))
	   )
	{
		io::err()
			<< "WARNING: RangeMap:"
			<< " unitMapping function does not satisfy end conditions!"
			<< '\n'
			<< dat::infoString(expY0, "expY0")
			<< " " << dat::infoString(gotY0, "gotY0")
			<< '\n'
			<< dat::infoString(expY1, "expY1")
			<< " " << dat::infoString(gotY1, "gotY1")
			<< std::endl;
		assert(! "Fix mapping function");
	}
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

inline
bool
RangeMap :: isValid
	() const
{
	return
		(  theDomain.isValid()
		&& theRange.isValid()
		);
}

inline
double
RangeMap :: operator()
	( double const & arg
	) const
{
	double result(dat::nullValue<double>());
	if (theDomain.contains(arg)) // NOTE: fails if instance is invalid
	{
		result = trans::mapped<double>(theDomain, arg, theRange, theUnitMap);
	}
	return result;
}

inline
std::string
RangeMap :: infoString
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
		oss << "theDomain: " << theDomain.infoString();

		oss << std::endl;
		oss << " theRange: " << theRange.infoString();
	}
	else
	{
		oss << " <null>";
	}

	return oss.str();
}

//======================================================================
}


