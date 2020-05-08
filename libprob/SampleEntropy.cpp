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
\brief Definitions for prob::SampleEntropy
*/


#include "libprob/SampleEntropy.h"

#include <iomanip>
#include <sstream>


namespace prob
{
// ======================================================================

// explicit
SampleEntropy :: SampleEntropy
	()
	: theCount(0u)
	, theEntropy(dat::nullValue<double>())
{ }

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

bool
SampleEntropy :: isValid
	() const
{
	return dat::isValid(theEntropy);
}

double
SampleEntropy :: perElement
	( double const & base
	) const
{
	return (theEntropy / std::log(base));
}

double
SampleEntropy :: inTotal
	( double const & base
	) const
{
	return (static_cast<double>(theCount) * perElement(base));
}

std::string
SampleEntropy :: infoString
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
			<< std::fixed << std::setprecision(3u)
			<< " " << std::setw(9u) << theCount
			<< " " << std::setw(6u) << perElement()
			<< " " << std::setw(12u) << inTotal()
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

// ======================================================================
}

