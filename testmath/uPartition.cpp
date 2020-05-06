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
\brief  This file contains unit test for math::Partition
*/


#include "libmath/Partition.h"

#include "libdat/compare.h"
#include "libdat/info.h"
#include "libio/stream.h"

#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace
{

//! Check for common functions
std::string
math_Partition_test0
	()
{
	std::ostringstream oss;
	math::Partition const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check basic data functions
std::string
math_Partition_test0a
	()
{
	std::ostringstream oss;

	// useful shorthand
	typedef std::pair<double, double> DataPair;

	// trivial case
	double const valMin(5.);
	double const delta(2.);
	size_t const expSize(1u);
	double const valMax{ valMin + static_cast<double>(expSize)*delta };

	// establish expectation
	DataPair const expFullRange(valMin, valMax);
	DataPair const expBinRange(expFullRange); // for only one part
	double const expValue(valMin + .5*delta);
	double const expIndex(.5);

	// exercise code
	math::Partition part(valMin, delta, expSize);
	size_t const gotSize(part.size());
	DataPair const gotFullRange(part.range());
	DataPair const gotBinRange(part.rangeForBin(0u));
	double const gotValue(part.interpValueFor(expIndex));
	double const gotIndex(part.interpIndexFor(expValue));

	// check results
	if (! dat::nearlyEquals(gotSize, expSize))
	{
		oss << "Failure of simple case size test" << std::endl;
		oss << dat::infoString(expSize, "expSize") << '\n';
		oss << dat::infoString(gotSize, "gotSize") << '\n';
	}
	if (! dat::nearlyEquals(gotFullRange, expFullRange))
	{
		oss << "Failure of simple case full range test" << std::endl;
		oss << dat::infoString(expFullRange, "expFullRange") << '\n';
		oss << dat::infoString(gotFullRange, "gotFullRange") << '\n';
	}
	if (! dat::nearlyEquals(gotBinRange, expBinRange))
	{
		oss << "Failure of simple case one bin range test" << std::endl;
		oss << dat::infoString(expBinRange, "expBinRange") << '\n';
		oss << dat::infoString(gotBinRange, "gotBinRange") << '\n';
	}
	if (! dat::nearlyEquals(gotValue, expValue))
	{
		oss << "Failure of simple case value test" << std::endl;
		oss << dat::infoString(expValue, "expValue") << '\n';
		oss << dat::infoString(gotValue, "gotValue") << '\n';
	}
	if (! dat::nearlyEquals(gotIndex, expIndex))
	{
		oss << "Failure of simple case index test" << std::endl;
		oss << dat::infoString(expIndex, "expIndex") << '\n';
		oss << dat::infoString(gotIndex, "gotIndex") << '\n';
	}

	return oss.str();
}


//! Check basic partition
std::string
math_Partition_test1
	()
{
	std::ostringstream oss;

	// simple case
	{
		std::deque<float> const samps{ -5.5, -4.5, -3.5 };
		constexpr size_t numParts(2u);

		// check basic partition
		math::Partition const expPart(samps[0], 1., numParts);
		math::Partition const gotPart
			(math::Partition::spanning(samps.begin(), samps.end(), numParts));
		if (! gotPart.nearlyEquals(expPart))
		{
			oss << "Failure of simple float part test" << std::endl;
			oss << gotPart.infoString("gotPart") << std::endl;
			oss << expPart.infoString("expPart") << std::endl;
		}

		// check bin range
		{
			std::pair<double, double> const expRange(-4.5, -3.5);
			std::pair<double, double> const gotRange(gotPart.rangeForBin(1u));
			if (! dat::nearlyEquals(gotRange, expRange))
			{
				oss << "Failure of simple float bin range test" << std::endl;
				oss << dat::infoString(expRange, "expRange") << std::endl;
				oss << dat::infoString(gotRange, "gotRange") << std::endl;
			}
		}

		// check entire range function
		{
			std::pair<double, double> const expRange
				(samps.front(), samps.back());
			std::pair<double, double> const gotRange(gotPart.range());
			if (! dat::nearlyEquals(gotRange, expRange))
			{
				oss << "Failure of simple float full range test" << std::endl;
				oss << dat::infoString(expRange, "expRange") << std::endl;
				oss << dat::infoString(gotRange, "gotRange") << std::endl;
			}
		}
	}

	// simple test with unsigned values
	{
		std::deque<uint8_t> const samps{ 5u, 6u, /*7u, 8u, 9u,*/ 10u };
		constexpr size_t numParts(5u);
		math::Partition const expPart(samps[0], 1., numParts);
		math::Partition const gotPart
			(math::Partition::spanning(samps.begin(), samps.end(), numParts));
		if (! gotPart.nearlyEquals(expPart))
		{
			oss << "Failure of simple uint8_t test" << std::endl;
			oss << gotPart.infoString("gotPart") << std::endl;
			oss << expPart.infoString("expPart") << std::endl;
		}

		// check other ctors
		math::Partition const gotCopy(expPart.range(), expPart.size());
		if (! gotCopy.nearlyEquals(expPart))
		{
			oss << "Failure of range ctor test" << std::endl;
			oss << gotCopy.infoString("gotCopy") << std::endl;
			oss << expPart.infoString("expPart") << std::endl;
		}

	}

	return oss.str();
}

//! Check basic methods
std::string
math_Partition_test2
	()
{
	std::ostringstream oss;

	// setup test [data configuration
	double const valMin(-53.);
	double const valMax( -5.);
	size_t const expSize(48.);
	double const range(valMax - valMin);
	double const delta(range / double(expSize));

	// useful shorthand
	typedef std::pair<double, double> DataPair;

	// specify expected values
	double const expValue(-37.);
	double const expIndex( 16.);
	DataPair const expFullRange{ valMin, valMax };
	DataPair const expBinRange{ expValue, (expValue + delta) };
	size_t const expBinIndex{ static_cast<size_t>(std::floor(expIndex)) };

	// exercise the functions
	math::Partition const part(valMin, delta, expSize);
	double const gotIndex(part.interpIndexFor(expValue));
	double const gotValue(part.interpValueFor(expIndex));
	DataPair const gotFullRange(part.range());
	DataPair const gotBinRange(part.rangeForBin(expIndex));
	size_t const gotBinIndex(part.binIndexFor(expValue));

	// check results
	if (! dat::nearlyEquals(gotIndex, expIndex))
	{
		oss << "Failure of interpIndexFor test" << std::endl;
		oss << dat::infoString(expIndex, "expIndex") << '\n';
		oss << dat::infoString(gotIndex, "gotIndex") << '\n';
	}
	if (! dat::nearlyEquals(gotValue, expValue))
	{
		oss << "Failure of interpValueFor test" << std::endl;
		oss << dat::infoString(expValue, "expValue") << '\n';
		oss << dat::infoString(gotValue, "gotValue") << '\n';
	}
	if (! dat::nearlyEquals(gotFullRange, expFullRange))
	{
		oss << "Failure of full range test" << std::endl;
		oss << dat::infoString(expFullRange, "expFullRange") << '\n';
		oss << dat::infoString(gotFullRange, "gotFullRange") << '\n';
	}
	if (! dat::nearlyEquals(gotBinRange, expBinRange))
	{
		oss << "Failure of bin range test" << std::endl;
		oss << dat::infoString(expBinRange, "expBinRange") << '\n';
		oss << dat::infoString(gotBinRange, "gotBinRange") << '\n';
	}
	if (! dat::nearlyEquals(gotBinIndex, expBinIndex))
	{
		oss << "Failure of bin index test" << std::endl;
		oss << dat::infoString(expBinIndex, "expBinIndex") << '\n';
		oss << dat::infoString(gotBinIndex, "gotBinIndex") << '\n';
	}

	return oss.str();
}

//! Check index/value consistency (esp at end points
std::string
math_Partition_test3
	()
{
	std::ostringstream oss;

	math::Partition const part(2000., 100., 9u);
	double const maxNdx{ static_cast<double>(part.size()) };
	for (double expNdx(0.) ; expNdx < maxNdx ; expNdx += (1./8.))
	{
		double const iValue(part.interpValueFor(expNdx));
		double const gotNdx(part.interpIndexFor(iValue));
		if (! dat::nearlyEquals(gotNdx, expNdx))
		{
			oss << "Failure of index round trip test" << std::endl;
			oss << dat::infoString(expNdx, "expNdx") << std::endl;
			oss << dat::infoString(gotNdx, "gotNdx") << std::endl;
		}
	}

	return oss.str();
}


}

//! Unit test for math::Partition
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << math_Partition_test0();
	oss << math_Partition_test0a();
	oss << math_Partition_test1();
	oss << math_Partition_test2();
	oss << math_Partition_test3();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
