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
\brief  This file contains unit test for dat::Latice
*/


#include "libdat/Latice.h"

#include "libdat/compare.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_Latice_test0
	()
{
	std::ostringstream oss;
	dat::Latice<int> const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check basic operations
std::string
dat_Latice_test1
	()
{
	std::ostringstream oss;

	// construct a simple latice
	constexpr size_t high{ 3u };
	constexpr size_t wide{ 4u };
	constexpr size_t deep{ 5u };
	dat::Latice<size_t> latice(high, wide, deep);

	// fill with data using iterator access
	std::fill(latice.begin(), latice.end(), dat::nullValue<size_t>());

	// check sizes
	constexpr size_t expSize{ high * wide * deep };
	size_t const gotSize{ latice.size() };
	if (! dat::nearlyEquals(gotSize, expSize))
	{
		oss << "Failure of size test" << std::endl;
	}

	// check validity
	if (! latice.isValid())
	{
		oss << "Failure of validity test" << std::endl;
	}
	else
	{
		// fill with data using element access
		size_t expSum{ 0u };
		for (size_t row{0u} ; row < high ; ++row)
		{
			for (size_t col{0u} ; col < wide ; ++col)
			{
				for (size_t lay{0u} ; lay < deep ; ++lay)
				{
					size_t const elem{ 100u*row + 10u*col + lay };
					latice(row, col, lay) = elem;
					expSum += latice(row, col, lay);
				}
			}
		}

		// readback data
		size_t const gotSum
			{ std::accumulate(latice.begin(), latice.end(), 0u) };
		if (! dat::nearlyEquals(gotSum, expSum))
		{
			oss << "Failure of readback sum test" << std::endl;
			oss << dat::infoString(expSum, "expSum") << std::endl;
			oss << dat::infoString(gotSum, "gotSum") << std::endl;
		}
	}

	return oss.str();
}


}

//! Unit test for dat::Latice
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_Latice_test0();
	oss << dat_Latice_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
