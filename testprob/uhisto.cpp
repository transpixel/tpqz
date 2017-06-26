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
\brief  This file contains unit test for prob::histo
*/


#include "libprob/histo.h"

#include "libio/stream.h"

#include <array>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check basic functions
std::string
prob_histo_test1
	()
{
	std::ostringstream oss;

	// simple data collections
	std::vector<uint8_t> const data
		{{ 8u, 8u, 4u, 6u,  20u, 60u,  79u, 79u, 79u, 3u }};
	size_t const expMinBin( 3u);
	size_t const expMaxBin(79u);

	std::array<size_t, 256u> const hist
		(prob::histo::countsForUint8(data.begin(), data.end()));
	size_t const gotMinBin(prob::histo::minBinNotZero(hist));
	size_t const gotMaxBin(prob::histo::maxBinNotZero(hist));

	if (! (gotMinBin == expMinBin))
	{
		oss << "Failure of minBin test" << std::endl;
	}
	if (! (gotMaxBin == expMaxBin))
	{
		oss << "Failure of maxBin test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for prob::histo
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << prob_histo_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
