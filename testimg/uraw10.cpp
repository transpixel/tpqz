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
\brief  This file contains unit test for img::raw10
*/


#include "libimg/raw10.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/sprintf.h"
#include "libio/stream.h"

#include <algorithm>
#include <bitset>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
img_raw10_test0
	()
{
	std::ostringstream oss;
	/*
	img::raw10 const aNull(dat::nullValue<img::raw10>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

namespace
{

	std::string
	infoStringFor
		( int const & value
		, std::string const & title = {}
		)
	{
		std::ostringstream oss;
		std::bitset<10u> const bits(value);
		oss << title
			<< " " << "b." << bits
			<< " " << std::setw(4u) << value
			<< " " << io::sprintf("0x%04X", value)
			;
		return oss.str();
	}

	template <typename Type>
	void
	checkAll4
		( std::ostream & oss
		, std::array<Type, 4u> const & exp
		, std::array<Type, 4u> const & got
		, std::string const & vName = {}
		)
	{
		for (size_t nn{0u} ; nn < 4u ; ++nn)
		{
			oss
				<< dat::infoString(nn)
				<< " " << infoStringFor(exp[nn], "exp"+vName)
				<< " " << infoStringFor(got[nn], "got"+vName)
				;
			if (! (got[nn] == exp[nn]))
			{
				oss << "  <==";
			}
			oss
				<< std::endl;
		}
	}

} // {}

//! Check pixel byte encoding
std::string
img_raw10_test1
	()
{
	std::ostringstream oss;

	// generate bit pattern for testing
	std::array<uint8_t, 4u> const expHiBytes{ 0x11, 0x22, 0x33, 0x44 };
	std::array<uint8_t, 4u> const expLoBytes{ 0x0, 0x1, 0x2, 0x3 };
	uint8_t const expLoMix
		{ static_cast<uint8_t>
			( (expLoBytes[0] << 6u)
			| (expLoBytes[1] << 4u)
			| (expLoBytes[2] << 2u)
			| (expLoBytes[3] << 0u)
			)
		};
	std::array<uint16_t, 4u> const expVals
		{ static_cast<uint16_t>(4u*0x11 + 0u)
		, static_cast<uint16_t>(4u*0x22 + 1u)
		, static_cast<uint16_t>(4u*0x33 + 2u)
		, static_cast<uint16_t>(4u*0x44 + 3u)
		};

	// construct test instance
	img::raw10::FourPix const quad{ expHiBytes, expLoMix };

	// fetch data from quad
	std::array<uint16_t, 4> const gotVals
		{ img::raw10::pixelValues<uint16_t>(quad) };
	std::array<uint8_t, 4> const gotHiBytes
		{ img::raw10::pixelHiBytes(quad) };
	std::array<uint8_t, 4> const gotLoBytes
		{ img::raw10::pixelLoBytes(quad) };

	// check values
	if (! std::equal(gotVals.begin(), gotVals.end(), expVals.begin()))
	{
		oss << "Failure of pixelValues<uint16_t> test" << std::endl;
		oss << img::raw10::infoString(quad, "quad") << std::endl;
		checkAll4(oss, expVals, gotVals, "Vals");
	}
	if (! std::equal(gotHiBytes.begin(), gotHiBytes.end(), expHiBytes.begin()))
	{
		oss << "Failure of pixelHiVals test" << std::endl;
		oss << img::raw10::infoString(quad, "quad") << std::endl;
		checkAll4(oss, expHiBytes, gotHiBytes, "HiBytes");
	}
	if (! std::equal(gotLoBytes.begin(), gotLoBytes.end(), expLoBytes.begin()))
	{
		oss << "Failure of pixelLoVals test" << std::endl;
		oss << img::raw10::infoString(quad, "quad") << std::endl;
		checkAll4(oss, expLoBytes, gotLoBytes, "LoBytes");
	}

	return oss.str();
}


}

//! Unit test for img::raw10
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << img_raw10_test0();
	oss << img_raw10_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
