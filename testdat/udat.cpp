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
\brief  This file contains unit test for dat::
*/


#include "libdat/algorithm.h"
#include "libdat/array.h"
#include "libdat/dat.h"
#include "libdat/info.h"
#include "libio/stream.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

namespace
{

//! Check fundamental functions
std::string
dat__test0
	()
{
	std::ostringstream oss;

	// test general null values
	uint8_t const null_u8(dat::nullValue<uint8_t>());
	long int const null_li(dat::nullValue<long int>());
	float const null_ff(dat::nullValue<float>());
	double const null_dd(dat::nullValue<double>());
	if (dat::isValid(null_u8))
	{
		oss << "failure of u8 null test" << std::endl;
	}
	if (dat::isValid(null_li))
	{
		oss << "failure of long int null test" << std::endl;
	}
	if (dat::isValid(null_ff))
	{
		oss << "failure of float null test" << std::endl;
	}
	if (dat::isValid(null_dd))
	{
		oss << "failure of double null test" << std::endl;
	}

	// test general good values
	uint8_t const good_u8(7u);
	long int const good_li(-1 * 0xF0F0F0F0f0f0f0f0);
	float const good_ff(-1.234f);
	double const good_dd(3.456);
	if (! dat::isValid(good_u8))
	{
		oss << "failure of u8 good test" << std::endl;
	}
	if (! dat::isValid(good_li))
	{
		oss << "failure of long int good test" << std::endl;
	}
	if (! dat::isValid(good_ff))
	{
		oss << "failure of float good test" << std::endl;
	}
	if (! dat::isValid(good_dd))
	{
		oss << "failure of double good test" << std::endl;
	}

	/* -- needs working half::is_floating_point<>
	dat::f16_t const null_f16(dat::nullValue<dat::f16_t>());
	*/
	dat::f16_t const null_f16(std::nanf(""));
	dat::f16_t const good_f16(-2.125);
	if (dat::isValid(null_f16))
	{
		oss << "failure of f16 null test" << std::endl;
	}
	if (! dat::isValid(good_f16))
	{
		oss << "failure of f16 good test" << std::endl;
	}

	// check short-hand values
	double const nullFlt(dat::nullValue<double>());
	double const nullDub(dat::badDub);
	if (dat::isValid(nullFlt))
	{
		oss << "Failure of nullFlt test" << std::endl;
	}

	if (dat::isValid(nullDub))
	{
		oss << "Failure of nullDub test" << std::endl;
	}

	return oss.str();
}


//! Check data range operations
std::string
dat__test3
	()
{
	std::ostringstream oss;

	// check valid range values
	{
		std::pair<double, double> const goodRange(-4.5, 5.75);

		double const valA(-5.6);
		double const valB( 5.6);
		double const valC( 6. );
		double const valX(dat::nullValue<double>());

		double const expA(goodRange.first);
		double const expB(valB);
		double const expC(goodRange.second);

		double const gotA(dat::clamped(valA, goodRange));
		if (! dat::nearlyEquals(gotA, expA))
		{
			oss << "Failure of clamped test for (valA, goodRange)" << std::endl;
		}
		double const gotB(dat::clamped(valB, goodRange));
		if (! dat::nearlyEquals(gotB, expB))
		{
			oss << "Failure of clamped test for (valB, goodRange)" << std::endl;
			oss << "for valB: " << valB << std::endl;
			oss << "and rngB: " << dat::infoString(goodRange) << std::endl;
			oss << "expB: " << expB << std::endl;
			oss << "gotB: " << gotB << std::endl;
		}
		double const gotC(dat::clamped(valC, goodRange));
		if (! dat::nearlyEquals(gotC, expC))
		{
			oss << "Failure of clamped test for (valC, goodRange)" << std::endl;
			oss << "for valC: " << valC << std::endl;
			oss << "and rngC: " << dat::infoString(goodRange) << std::endl;
			oss << "expC: " << expC << std::endl;
			oss << "gotC: " << gotC << std::endl;
		}
		double const gotX(dat::clamped(valX, goodRange));
		if (dat::isValid(gotX))
		{
			oss << "Failure of clamped test for (null, goodRange)" << std::endl;
		}
	}

	// simple tests for bad ranges
	{
		std::pair<uint8_t, uint8_t> const badRange1(120u, 3u);
		std::pair<int, int> const badRange2(3, -3);
		uint8_t const got1(dat::clamped<uint8_t>(20u, badRange1));
		if (dat::isValid(got1))
		{
			oss << "Failure of clamped test for (okay, badRange1)" << std::endl;
		}
		int const got2(dat::clamped(3, badRange2));
		if (dat::isValid(got2))
		{
			oss << "Failure of clamped test for (okay, badRange2)" << std::endl;
		}
	}

	return oss.str();
}

}

//! Unit test for dat::
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat__test0();
	oss << dat__test3();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
