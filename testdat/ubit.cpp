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
\brief  This file contains unit test for dat::bit
*/


#include "libdat/bit.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace
{

//! Check for common functions
std::string
dat_bit_test0
	()
{
	std::ostringstream oss;
	/*
	dat::bit const aNull(dat::nullValue<dat::bit>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

//! Check basic operations
std::string
dat_bit_test1
	()
{
	std::ostringstream oss;

	uint8_t mask{ 0x05 };
	std::vector<bool> const expOns
		{ true, false, true, false , false, false, false, false };
	for (size_t bitNdx{0u} ; bitNdx < 8u ; ++bitNdx)
	{
		bool const & expOn = expOns[bitNdx];
		bool const gotOn{ dat::bit::isSet(mask, bitNdx) };
		if (! (gotOn == expOn))
		{
			oss << "Failure of isSet test bit " << bitNdx << std::endl;
		}
		bool const expOff{ (! expOn) };
		bool const gotOff{ dat::bit::isClear(mask, bitNdx) };
		if (! (gotOff == expOff))
		{
			oss << "Failure of isClear test bit " << bitNdx << std::endl;
		}
	}

	dat::bit::clear(&mask, 0u);
	if (! (0x04 == mask))
	{
		oss << "Failure of bit clear test" << std::endl;
	}
	if (! dat::bit::isClear(mask, 0u))
	{
		oss << "Failure of bit isClear test" << std::endl;
	}

	dat::bit::set(&mask, 7u);
	if (! (0x84 == mask))
	{
		oss << "Failure of bit set test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for dat::bit
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_bit_test0();
	oss << dat_bit_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
