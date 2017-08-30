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
\brief  This file contains unit test for dat::QuantumFrac
*/


#include "libdat/QuantumFrac.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>


namespace
{

//! Check for common functions
std::string
dat_QuantumFrac_test0
	()
{
	std::ostringstream oss;
	dat::QuantumFrac const aNull;
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check basic properties
std::string
dat_QuantumFrac_test1
	()
{
	std::ostringstream oss;

	dat::quantum::Splitter<long, double> const delta{ 5. };
	std::vector<std::pair<double, dat::QuantumFrac> > const valExpPairs
		{ {  5.00, dat::QuantumFrac( 1L,  .00, delta) }
		, {  6.00, dat::QuantumFrac( 1L, 1.00, delta) }
		, {  6.25, dat::QuantumFrac( 1L, 1.25, delta) }
		, { 10.00, dat::QuantumFrac( 2L,  .00, delta) }
		, {   .00, dat::QuantumFrac( 0L, 0.00, delta) }
		, { -5.00, dat::QuantumFrac(-1L,  .00, delta) }
		, { -6.25, dat::QuantumFrac(-2L, 3.75, delta) }
		, {-10.00, dat::QuantumFrac(-2L,  .00, delta) }
		};

	for (std::pair<double, dat::QuantumFrac> const & valExpPair : valExpPairs)
	{
		double const & value = valExpPair.first;
		dat::QuantumFrac const & expQF = valExpPair.second;
		dat::QuantumFrac const gotQF(value, delta);
		if (! gotQF.nearlyEquals(expQF))
		{
			oss << "Failure of quantization test" << std::endl;
			oss << dat::infoString(value, "value") << std::endl;
			oss << dat::infoString(expQF, "expQF") << std::endl;
			oss << dat::infoString(gotQF, "gotQF") << std::endl;
		}
	}

	return oss.str();
}


}

//! Unit test for dat::QuantumFrac
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_QuantumFrac_test0();
	oss << dat_QuantumFrac_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
