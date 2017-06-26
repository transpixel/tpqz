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
\brief  This file contains unit test for dat::discrete
*/


#include "libdat/discrete.h"

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
dat_discrete_test0
	()
{
	std::ostringstream oss;
	/*
	dat::discrete const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	*/
	return oss.str();
}

	void
	checkLoHi
		( std::ostream & oss
		, size_t const & value
		, size_t const & delta
		, size_t const & expLo
		, size_t const & expHi
		)
	{
		size_t const gotLo{ dat::discrete::quantizedLo(value, delta) };
		if (! dat::areSame(gotLo, expLo))
		{
			oss << "Failure of discrete::lo test for value, delta:"
				<< " " << dat::infoString(value)
				<< " " << dat::infoString(delta)
				<< std::endl;
			oss << dat::infoString(expLo, "expLo") << std::endl;
			oss << dat::infoString(gotLo, "gotLo") << std::endl;
		}

		size_t const gotHi{ dat::discrete::quantizedHi(value, delta) };
		if (! dat::areSame(gotHi, expHi))
		{
			oss << "Failure of discrete::hi test for value, delta:"
				<< " " << dat::infoString(value)
				<< " " << dat::infoString(delta)
				<< std::endl;
			oss << dat::infoString(expHi, "expHi") << std::endl;
			oss << dat::infoString(gotHi, "gotHi") << std::endl;
		}
	}

//! Check basic quantization operations
std::string
dat_discrete_test1
	()
{
	std::ostringstream oss;

	constexpr size_t bad{ dat::nullValue<size_t>() };

	// general case
	checkLoHi(oss, 10u,  3u,  9u, 12u);

	// exact conditions
	checkLoHi(oss, 10u,  1u, 10u, 10u);
	checkLoHi(oss, 10u,  5u, 10u, 10u);

	// involving zero values
	checkLoHi(oss,  0u,  7u,  0u,  0u);
	checkLoHi(oss,  1u,  0u, bad, bad);

	return oss.str();
}


}

//! Unit test for dat::discrete
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_discrete_test0();
	oss << dat_discrete_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
