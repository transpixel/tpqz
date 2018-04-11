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
\brief  This file contains unit test for io::binary
*/


#include "libio/binary.h"

#include "libdat/compare.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
io_binary_test0
	()
{
	std::ostringstream oss;
	/*
	io::binary const aNull(dat::nullValue<io::binary>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

//! Check simple I/O
std::string
io_binary_test1
	()
{
	std::ostringstream oss;

	struct Custom
	{
		char theC;
		int theI;
		double theD;

		bool
		nearlyEquals
			( Custom const & other
			) const
		{
			return
				(  dat::nearlyEquals(theC, other.theC)
				&& dat::nearlyEquals(theI, other.theI)
				&& dat::nearlyEquals(theD, other.theD)
				);
		}
	};

	std::vector<Custom> const expData
		{ Custom{ 'a', 1, 1.25 }
		, Custom{ 'b', 2, 2.50 }
		};

	std::string const fpath{ "ubinary_TmpTile.dat" };
	bool const okaySave{ io::binary::save<Custom>(expData, fpath) };
	std::vector<Custom> const gotData{ io::binary::load<Custom>(fpath) };

	if (! okaySave)
	{
		oss << "Failure of save test" << std::endl;
	}

	size_t const expSize{ expData.size() };
	size_t const gotSize{ gotData.size() };
	if (! dat::nearlyEquals(gotSize, expSize))
	{
		oss << "Failure of save/load size test" << std::endl;
		oss << dat::infoString(expSize, "expSize") << std::endl;
		oss << dat::infoString(gotSize, "gotSize") << std::endl;
	}
	else
	{
		bool const sameValues
			(  gotData[0].nearlyEquals(expData[0])
			&& gotData[1].nearlyEquals(expData[1])
			);
		if (! sameValues)
		{
			oss << "Failure of same value test" << std::endl;
		}
	}

	// cleanup
	std::remove(fpath.c_str());

	return oss.str();
}


}

//! Unit test for io::binary
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << io_binary_test0();
	oss << io_binary_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
