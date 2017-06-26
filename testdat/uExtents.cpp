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
\brief  This file contains unit test for dat::Extents
*/


#include "libdat/Extents.h"

#include "libdat/dat.h"
#include "libdat/info.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_Extents_test0
	()
{
	std::ostringstream oss;
	dat::Extents const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check basic members
std::string
dat_Extents_test1
	()
{
	std::ostringstream oss;

	// check empty case
	dat::Extents const empty(0u, 0u);
	if (! empty.isValid()) // allow zero size as valid
	{
		oss << "Failure of empty (non)valid test" << std::endl;
	}

	size_t const expHigh(4u);
	size_t const expWide(7u);
	dat::Extents const area(expHigh, expWide);
	if (! (expHigh == area.high()) )
	{
		oss << "Failure of height test" << std::endl;
	}
	if (! (expWide == area.wide()) )
	{
		oss << "Failure of width test" << std::endl;
	}

	return oss.str();
}


//! Check operations
std::string
dat_Extents_test2
	()
{
	std::ostringstream oss;

	dat::Extents const asize(3u, 4u);
	dat::RowCol const expInBR{{ 2u, 3u }};

	// check inside cornerBR
	dat::RowCol const gotInBR(asize.insideCornerBR());
	if (! dat::nearlyEquals(gotInBR, expInBR))
	{
		oss << "Failure of insideCornerBR test" << std::endl;
	}

	// check fitsWithin
	dat::Extents const otherBig(4u, 4u);
	dat::Extents const otherFit(asize);
	dat::Extents const otherSml(4u, 3u);

	bool const expBig{ true };
	bool const expFit{ true };
	bool const expSml{ false };

	bool const gotBig{ asize.fitsWithin(otherBig) };
	bool const gotFit{ asize.fitsWithin(otherFit) };
	bool const gotSml{ asize.fitsWithin(otherSml) };

	if (! dat::nearlyEquals(gotBig, expBig))
	{
		oss << "Failure of fitsWithin Big test" << std::endl;
		oss << dat::infoString(expBig, "expBig") << std::endl;
		oss << dat::infoString(gotBig, "gotBig") << std::endl;
		oss << dat::infoString(asize, "asize") << std::endl;
		oss << dat::infoString(otherBig, "otherBig") << std::endl;
	}
	if (! dat::nearlyEquals(gotFit, expFit))
	{
		oss << "Failure of fitsWithin Fit test" << std::endl;
		oss << dat::infoString(expBig, "expBig") << std::endl;
		oss << dat::infoString(gotBig, "gotBig") << std::endl;
		oss << dat::infoString(asize, "asize") << std::endl;
		oss << dat::infoString(otherFit, "otherFit") << std::endl;
	}
	if (! dat::nearlyEquals(gotSml, expSml))
	{
		oss << "Failure of fitsWithin Sml test" << std::endl;
		oss << dat::infoString(expBig, "expBig") << std::endl;
		oss << dat::infoString(gotBig, "gotBig") << std::endl;
		oss << dat::infoString(asize, "asize") << std::endl;
		oss << dat::infoString(otherSml, "otherSml") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for dat::Extents
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_Extents_test0();
	oss << dat_Extents_test1();
	oss << dat_Extents_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
