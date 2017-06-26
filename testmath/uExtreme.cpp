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
\brief  This file contains unit test for math::Extreme
*/


#include "libmath/Extreme.h"

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
math_Extreme_test0
	()
{
	std::ostringstream oss;
	math::Extreme<std::array<int, 0u>::const_iterator> const aNull;
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

inline
bool
isLess
	( long const & valA
	, long const & valB
	)
{
	return (valA < valB);
}

//! Check basic minimum finding operation
std::string
math_Extreme_test1
	()
{
	std::ostringstream oss;

	// some arbitrary data
	long const expExVal{ -9L };
	std::vector<long> const values
		{ dat::nullValue<long>()
		, expExVal+3
		, dat::nullValue<long>()
		, expExVal
		, expExVal+20L
		};
	std::vector<long>::const_iterator const expExIter{ values.begin() + 3u };

	// find minimum value
	math::Extreme<std::vector<long>::const_iterator> const extrema
		(values.begin(), values.end(), isLess);

	// check extreme value
	long const & gotExVal = extrema.theExVal;
	if (! dat::nearlyEquals(gotExVal, expExVal))
	{
		oss << "Failure of minimum test" << std::endl;
		oss << dat::infoString(expExVal, "expExVal") << std::endl;
		oss << dat::infoString(gotExVal, "gotExVal") << std::endl;
	}

	// check extreme interator
	std::vector<long>::const_iterator const gotExIter{ extrema.theExIter };
	if (values.end() == gotExIter)
	{
		oss << "Failure of valid end iterator test" << std::endl;
		oss << dat::infoString(*gotExIter, "gotExIter") << std::endl;
		oss << dat::infoString(*gotExIter, "gotExIter") << std::endl;
	}
	if (! (gotExIter == expExIter))
	{
		oss << "Failure of extreme iterator test" << std::endl;
	}

	// check null data case
	std::vector<float> const allbad
		{ dat::nullValue<float>()
		, dat::nullValue<float>()
		, dat::nullValue<float>()
		};
	using FIter = std::vector<float>::const_iterator;
	math::Extreme<FIter> const noEx
		( allbad.begin()
		, allbad.end()
		, [] (float const & f1, float const & f2) { return (f1 < f2); }
		);
	FIter const gotNoIter{ noEx.theExIter };
	if (! (allbad.end() == gotNoIter))
	{
		oss << "Failure of all-bad iterator test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for math::Extreme
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << math_Extreme_test0();
	oss << math_Extreme_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
