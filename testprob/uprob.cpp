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
\brief  This file contains unit test for prob::prob
*/


#include "libprob/prob.h"

#include "libio/stream.h"
#include "libprob/histo.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check basic function operations
std::string
prob_prob_test1
	()
{
	std::ostringstream oss;

	// synthesize data samples
	std::vector<uint8_t> const samps
		{{ 6u, 5u, 6u, 8u, 6u, 9u, 3u, 5u, 8u }};
	uint8_t const expMin( *(std::min_element(samps.begin(), samps.end())) );
	uint8_t const expMax( *(std::max_element(samps.begin(), samps.end())) );

	// check histogram data limits
	std::array<size_t, 256u> const hist
		(prob::histo::countsForUint8(samps.begin(), samps.end()));

	// exercise functions
	size_t const gotMin(prob::histo::minBinNotZero(hist));
	size_t const gotMax(prob::histo::maxBinNotZero(hist));

	// check results
	if (! (gotMin == expMin))
	{
		oss << "Failure of min test" << std::endl;
		oss << "expMin: " << (int)expMin << std::endl;
		oss << "gotMin: " << (int)gotMin << std::endl;
	}
	if (! (gotMax == expMax))
	{
		oss << "Failure of max test" << std::endl;
		oss << "expMax: " << (int)expMax << std::endl;
		oss << "gotMax: " << (int)gotMax << std::endl;
	}

	return oss.str();
}


}

//! Unit test for prob::prob
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << prob_prob_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
