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
\brief Definitions for prob::Remapper
*/


#include "libprob/Remapper.h"

#include "libdat/info.h"
#include "libio/stream.h"

#include <cassert>
#include <sstream>


namespace prob
{
// ======================================================================

// explicit
Remapper :: Remapper
	( CdfForward const & cdfFwd
	, CdfInverse const & cdfInv
	)
	: theCdfFwd(cdfFwd)
	, theCdfInv(cdfInv)
{
	dat::Range<double> const fwdRange(theCdfFwd.dataRange());
	dat::Range<double> const invRange(theCdfInv.dataRange());
	if (! invRange.nearlyEquals(fwdRange))
	{
		io::err()
			<< "ERROR: Remapper: non-matching CDF data ranges\n"
			<< dat::infoString(fwdRange, "fwdRange") << '\n'
			<< dat::infoString(invRange, "invRange") << '\n'
			<< std::endl;
	}
	assert(invRange.nearlyEquals(fwdRange));
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

bool
Remapper :: isValid
	() const
{
	return
		(  theCdfFwd.isValid()
		&& theCdfInv.isValid()
		);
}

std::string
Remapper :: infoString
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
		oss << dat::infoString(theCdfFwd, "theCdfFwd");

		oss << std::endl;
		oss << dat::infoString(theCdfInv, "theCdfInv");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

// ======================================================================
}

