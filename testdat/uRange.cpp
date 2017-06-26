
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
\brief  This file contains unit test for dat::Range
*/


#include "libdat/dat.h"
#include "libdat/info.h"
#include "libdat/Range.h"
#include "libio/stream.h"

#include <iostream>
#include <list>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_Range_test0
	()
{
	std::ostringstream oss;
	dat::Range<long double> const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}

	// check behavior in undefined situations
	if (aNull.contains(1.2345))
	{
		oss << "Failure of null.contains() test" << std::endl;
	}

	// check cast
	std::pair<uint8_t, uint8_t> const expRange{ 2u, 137u };
	dat::Range<uint8_t> const range(expRange);
	std::pair<uint8_t, uint8_t> const gotRange
		(static_cast<std::pair<uint8_t, uint8_t> >(range));
	if (! dat::nearlyEquals(gotRange, expRange))
	{
		oss << "Failure of cast test" << std::endl;
	}

	// check static values
	dat::Range<float> const expUint8(0.f, 256.f);
	dat::Range<float> const & gotUint8 = dat::Range<float>::uint8;
	if (! gotUint8.nearlyEquals(expUint8))
	{
		oss << "Failure of Range<double>::uint8 test\n";
		oss << dat::infoString(expUint8, "expUint8") << std::endl;
		oss << dat::infoString(gotUint8, "gotUint8") << std::endl;
	}

	return oss.str();
}

//! Check basic operations
std::string
dat_Range_test1
	()
{
	std::ostringstream oss;

	typedef float AType;

	// verify with 'unusual' arithemetic type
	AType const expMin(3.5);
	AType const expMax(7.5);
	AType const valueLo(expMin - dat::smallValue<AType>());
	AType const valueHi(expMax + dat::smallValue<AType>());

	dat::Range<float> const range(expMin, expMax);

	AType const expSpan(expMax - expMin);
	AType const expMid(.5*expMax + .5*expMin);
	bool const expInLo(false);
	bool const expInMin(true);
	bool const expInMid(true);
	bool const expInMax(false);
	bool const expInHi(false);

	if (! dat::nearlyEquals(range.min(), expMin))
	{
		oss << "Failure of min test" << std::endl;
	}
	if (! dat::nearlyEquals(range.max(), expMax))
	{
		oss << "Failure of max test" << std::endl;
	}

	if (! dat::nearlyEquals(range.magnitude(), expSpan))
	{
		oss << "Failure of magnitude test" << std::endl;
	}

	if (! dat::nearlyEquals(range.middle(), expMid))
	{
		oss << "Failure of middle test" << std::endl;
	}

	bool const gotInLo(range.contains(valueLo));
	if (! (gotInLo == expInLo))
	{
		oss << "Failure of expInLo test" << std::endl;
	}
	bool const gotInMin(range.contains(expMin));
	if (! (gotInMin == expInMin))
	{
		oss << "Failure of expInMin test" << std::endl;
	}
	bool const gotInMax(range.contains(expMax));
	if (! (gotInMax == expInMax))
	{
		oss << "Failure of expInMax test" << std::endl;
	}
	bool const gotInMid(range.contains(expMid));
	if (! (gotInMid == expInMid))
	{
		oss << "Failure of expInMid test" << std::endl;
	}
	bool const gotInHi(range.contains(valueHi));
	if (! (gotInHi == expInHi))
	{
		oss << "Failure of expInHi test" << std::endl;
	}

	return oss.str();
}


//! Check static function
std::string
dat_Range_test2
	()
{
	std::ostringstream oss;

	std::list<float> const list{ 5., 7., 19. };
	dat::Range<float> const expRange(list.front(), list.back());
	dat::Range<float> const gotRange
		(dat::Range<float>::spanning(list.begin(), list.end()));

	if (! gotRange.nearlyEquals(expRange))
	{
		oss << "Failure of spanning function test" << std::endl;
		oss << expRange.infoString("expRange") << '\n';
		oss << gotRange.infoString("gotRange") << '\n';
	}

	return oss.str();
}

//! Check intersection operations
std::string
dat_Range_test3
	()
{
	std::ostringstream oss;

	// check overlap intersection
	dat::Range<size_t> const fullRange{ 17u, 29u };
	dat::Range<size_t> const partRange{ 13u, 23u };
	dat::Range<size_t> const expRange{ 17u, 23u };
	dat::Range<size_t> const gotRangeA{ fullRange.intersectWith(partRange) };
	dat::Range<size_t> const gotRangeB{ partRange.intersectWith(fullRange) };

	if (! gotRangeA.nearlyEquals(expRange))
	{
		oss << "Failure of range intersection test A" << std::endl;
	}
	if (! gotRangeB.nearlyEquals(expRange))
	{
		oss << "Failure of range intersection test B" << std::endl;
	}

	return oss.str();
}



}

//! Unit test for dat::Range
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_Range_test0();
	oss << dat_Range_test1();
	oss << dat_Range_test2();
	oss << dat_Range_test3();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
