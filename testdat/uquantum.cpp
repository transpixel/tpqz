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
\brief  This file contains unit test for dat::quantum
*/


#include "libdat/quantum.h"

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
dat_quantum_test0
	()
{
	std::ostringstream oss;
	/*
	dat::quantum const aNull(dat::nullValue<dat::quantum>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/

	constexpr double delta{ .25 };
	dat::quantum::Splitter<long, double> const split{ delta };

	// check general convention
	{
		/*
		*/
		std::pair<long, double> const expPos{ 3L*4L + 3L, 0. };
		std::pair<long, double> const gotPos{ split(3.75) };
		if (! dat::nearlyEquals(gotPos, expPos))
		{
			oss << "Failure of whole delta positive value test" << '\n';
			oss << dat::infoString(expPos, "expPos") << std::endl;
			oss << dat::infoString(gotPos, "gotPos") << std::endl;
		}

		std::pair<long, double> const expNeg{ -4L*4L + 1L, 0. };
		std::pair<long, double> const gotNeg{ split(-3.75) };
		if (! dat::nearlyEquals(gotNeg, expNeg))
		{
			oss << "Failure of whole delta negative value test" << '\n';
			oss << dat::infoString(expNeg, "expNeg") << std::endl;
			oss << dat::infoString(gotNeg, "gotNeg") << std::endl;
		}
	}

	// check fraction interpretations
	{
		std::pair<long, double> const expPos{ 3L, .25 };
		std::pair<long, double> const gotPos{ split(( (3.+.25))*delta) };
		if (! dat::nearlyEquals(gotPos, expPos))
		{
			oss << "Failure of fractional delta positive value test" << '\n';
			oss << dat::infoString(expPos, "expPos") << std::endl;
			oss << dat::infoString(gotPos, "gotPos") << std::endl;
		}

		std::pair<long, double> const expNeg{ -4L, .75 };
		std::pair<long, double> const gotNeg{ split((-(3.+.25))*delta) };
		if (! dat::nearlyEquals(gotNeg, expNeg))
		{
			oss << "Failure of fractional delta negative value test" << '\n';
			oss << dat::infoString(expNeg, "expNeg") << std::endl;
			oss << dat::infoString(gotNeg, "gotNeg") << std::endl;
		}
	}


	return oss.str();
}


	//! Generate test cases for specified types (need to support negatives)
	template <typename BaseType, typename DataType>
	void
	runTest
		( std::ostream & oss
		, std::string const & name
		)
	{
		using FracType = DataType;
		constexpr DataType delta{ 7 };
		dat::quantum::Splitter<BaseType, FracType> const quantizer{ delta };
		using FloorResid = std::pair<BaseType, FracType>;
		std::vector<std::pair<DataType, FloorResid> > const valExpPairs
			{ {  9, {  1, DataType(2)/delta } }
			, {  3, {  0, DataType(3)/delta } }
			, {  0, {  0, DataType(0)/delta } }
			, { -3, { -1, DataType(4)/delta } }
			, { -9, { -2, DataType(5)/delta } }
			};

		for (std::pair<DataType, FloorResid> const & valExpPair : valExpPairs)
		{
			DataType const & value = valExpPair.first;
			FloorResid const & expFR = valExpPair.second;
			FloorResid const gotFR{ quantizer(value) };
			if (! dat::nearlyEquals(gotFR, expFR))
			{
				oss << "Failure of quantizer test: " << name << std::endl;
				oss << dat::infoString(expFR, "expFR") << std::endl;
				oss << dat::infoString(gotFR, "gotFR") << std::endl;
			}
		}
	}


//! Check quantization functor
std::string
dat_quantum_test1
	()
{
	std::ostringstream oss;

	runTest<int, float>(oss, "int,float");
	runTest<double, float>(oss, "double,float");
	runTest<long, double>(oss, "long,double");

	return oss.str();
}


}

//! Unit test for dat::quantum
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_quantum_test0();
	oss << dat_quantum_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
