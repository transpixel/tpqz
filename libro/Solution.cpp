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
\brief Definitions for ro::Solution
*/


#include "libro/Solution.h"

#include "libio/sprintf.h"

#include <sstream>


namespace ro
{

// explicit
Solution :: Solution
	( std::shared_ptr<Pair> const & roPair
	, size_t const & itCount
	, size_t const & condNum
	, double const & convergeGap
	)
	: theRoPair{ roPair }
	, theItCount{ itCount }
	, theCondNum{ condNum }
	, theConvergeGap{ convergeGap }
{ }

bool
Solution :: isValid
	() const
{
	return (theRoPair && theRoPair->isValid());
}

std::pair<ga::Rigid, ga::Rigid>
Solution :: pair
	() const
{
	ro::OriPair oriPair{ ga::Rigid{}, ga::Rigid{} };
	if (isValid())
	{
		oriPair = theRoPair->pair();
	}
	return oriPair;
}

std::string
Solution :: infoString
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
		double const logCond{ std::log10(theCondNum) };
		oss << dat::infoString(*theRoPair, "*theRoPair");
		oss << std::endl;
		oss
			<< dat::infoString(theItCount, "theItCount")
			<< dat::infoString(logCond, "log10Cond")
			;
		oss << std::endl;
		double const logGap{ std::log10(theConvergeGap) };
		oss
			<< "theConvergeGap:"
			<< " " << io::sprintf("%9.6f", theConvergeGap)
			<< dat::infoString(logGap, "log10:")
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

} // ro

