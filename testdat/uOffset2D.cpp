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
\brief  This file contains unit test for dat::Offset2D
*/


#include "libdat/Offset2D.h"

#include "libdat/compare.h"
#include "libdat/info.h"
#include "libdat/RowCol.h"
#include "libdat/Spot.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_Offset2D_test0
	()
{
	std::ostringstream oss;
	dat::Offset2D<uint16_t, int16_t> const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check operation with basic offset
std::string
dat_Offset2D_test1
	()
{
	std::ostringstream oss;

	using UType = uint8_t;
	using SType = int8_t;

	// check positive offset
	{
		dat::Offset2D<UType, SType> const off(7, 8);
		std::array<SType, 2u> const value{{ 9, 8 }};
		std::array<UType, 2u> const expVal{{ 2u, 0u }};
		std::array<UType, 2u> const gotVal(off(value));
		if (! dat::nearlyEquals(gotVal, expVal))
		{
			oss << "Failure of unsigned oPos test" << std::endl;
			oss << dat::infoString(expVal, "expVal") << std::endl;
			oss << dat::infoString(gotVal, "gotVal") << std::endl;
		}
	}

	// check negative offset
	{
		dat::Offset2D<UType, SType> const off(-7, -8);
		std::array<SType, 2u> const value{{ -6, -8 }};
		std::array<UType, 2u> const expVal{{ 1u, 0u }};
		std::array<UType, 2u> const gotVal(off(value));
		if (! dat::nearlyEquals(gotVal, expVal))
		{
			oss << "Failure of unsigned oNeg test" << std::endl;
			oss << dat::infoString(expVal, "expVal") << std::endl;
			oss << dat::infoString(gotVal, "gotVal") << std::endl;
		}
	}

	return oss.str();
}

//! Check with more meaningful case
std::string
dat_Offset2D_test2
	()
{
	std::ostringstream oss;

	dat::Offset2D<size_t, long> off{{ -7L, -5L }};
	std::array<long, 2u> const value{{ -0L, -4L }};
	dat::RowCol const expVal{{ 7u, 1u }};
	dat::RowCol const gotVal(off(value));

	if (! dat::nearlyEquals(gotVal, expVal))
	{
		oss << "Failure of unsigned longRowCol test" << std::endl;
		oss << dat::infoString(expVal, "expVal") << std::endl;
		oss << dat::infoString(gotVal, "gotVal") << std::endl;
	}

	return oss.str();
}

//! Check casting consistency with signed values
std::string
dat_Offset2D_test3
	()
{
	std::ostringstream oss;

	dat::Offset2D<double, float> off{{ -7.f, -5.f }};
	std::array<float, 2u> const value{{ -0.f, -4.f }};
	dat::Spot const expVal{{ 7., 1. }};
	dat::Spot const gotVal(off(value));

	if (! dat::nearlyEquals(gotVal, expVal))
	{
		oss << "Failure of unsigned doubleSpot test" << std::endl;
		oss << dat::infoString(expVal, "expVal") << std::endl;
		oss << dat::infoString(gotVal, "gotVal") << std::endl;
	}

	return oss.str();
}

}

//! Unit test for dat::Offset2D
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_Offset2D_test0();
	oss << dat_Offset2D_test1();
	oss << dat_Offset2D_test2();
	oss << dat_Offset2D_test3();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
