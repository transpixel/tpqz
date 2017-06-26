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
\brief  This file contains unit test for prob::Histogram
*/


#include "libprob/Histogram.h"

#include "libdat/compare.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
prob_Histogram_test0
	()
{
	std::ostringstream oss;
	prob::Histogram const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check basic construction
std::string
prob_Histogram_test1
	()
{
	std::ostringstream oss;

	// create trivial histogram
	double const min(173.);
	double const delta(173.);
	size_t const numBins(3u);
	math::Partition const part(min, delta, numBins);
	prob::Histogram hist(part);
	hist.addSample(min + 1.2*delta);

	// check basic PDF creation
	std::vector<double> const expProbs{ 0., 1., 0. };
	std::vector<double> const gotProbs(hist.pdf());
	if (! (gotProbs.size() == expProbs.size()))
	{
		oss << "Failure of pdf size test" << std::endl;
	}
	else
	{
		if (! dat::nearlyEquals<double>
				( gotProbs.begin(), gotProbs.end()
				, expProbs.begin(), expProbs.end()
				)
		   )
		{
			oss << "Failure of pdf value test" << std::endl;
		}
	}

	// io::out() << hist.infoStringContents("hist") << std::endl;

	return oss.str();
}


}

//! Unit test for prob::Histogram
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << prob_Histogram_test0();
	oss << prob_Histogram_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
