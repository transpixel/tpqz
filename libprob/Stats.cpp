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
\brief Definitions for prob::Stats
*/


#include "libprob/Stats.h"

#include "libdat/dat.h"
#include "libdat/info.h"
#include "libprob/mean.h"
#include "libprob/median.h"

#include <sstream>


namespace prob
{
//======================================================================

Stats :: Stats
	()
	: theSamples()
{
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

bool
Stats :: isValid
	() const
{
	return (! theSamples.empty());
}

void
Stats :: add
	( double const & sample
	)
{
	theMinMax = theMinMax.expandedWith(sample);
	theSamples.push_back(sample);
}

double
Stats :: mean
	() const
{
	return prob::mean::arithmetic(theSamples.begin(), theSamples.end());
}

double
Stats :: medianValue
	() const
{
	return median::valueFromConst(theSamples.begin(), theSamples.end());
}

std::string
Stats :: infoString
	( std::string const & title
	, std::string const & fmt
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}

	if (isValid())
	{
		oss << io::sprintf("%15s", "sampCount")
			<< io::sprintf(fmt, theSamples.size());

		oss << std::endl;
		oss << io::sprintf("%15s", "sampMin")
			<< io::sprintf(fmt, theMinMax.min());

		oss << std::endl;
		oss << io::sprintf("%15s", "sampMax")
			<< io::sprintf(fmt, theMinMax.max());

		oss << std::endl;
		oss << io::sprintf("%15s", "sampMean")
			<< io::sprintf(fmt, mean());

		oss << std::endl;
		oss << io::sprintf("%15s", "sampMedian")
			<< io::sprintf(fmt, medianValue());

	}
	else
	{
		oss << " <null>";
	}

	return oss.str();
}

//======================================================================
}

