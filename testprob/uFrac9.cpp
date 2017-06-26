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
\brief  This file contains unit test for prob::Frac9
*/


#include "libprob/Frac9.h"

#include "libdat/dat.h"
#include "libdat/info.h"
#include "libio/stream.h"
#include "libmath/math.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
prob_Frac9_test0
	()
{
	std::ostringstream oss;
	prob::Frac9 const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check basic operations
std::string
prob_Frac9_test1
	()
{
	std::ostringstream oss;

	// uinform CDF
	prob::CdfForward const cdfFwd
		(prob::CdfForward::uniform(dat::Range<double>::uint8));

	// expected values
	double const max(256.);
	std::array<double, 9u> const exp9
		{{      0., .01*max, .10*max, .25*max
		 , .50*max
		 , .75*max, .90*max, .99*max,     max
		}};

	// get actual values
	prob::Frac9 const frac(cdfFwd);
	std::array<double, 9u> const & got9 = frac.theValues;

	// compute error between expecation and actual
	// NOTE: the results are not exact because of integer quantization
	double sumSq(0.);
	for (size_t nn(0u) ; nn < exp9.size() ; ++nn)
	{
		double const diff(got9[nn]-exp9[nn]);
		double const diffSq(math::sq(diff));
		sumSq += diffSq;
	}
	double const gotRmse(std::sqrt(sumSq / (double)exp9.size()));

	double const expRmse(0.);
	double const tolRmse(5./8.); // e.g. allow for quantization effects
	if (! dat::nearlyEquals(gotRmse, expRmse, tolRmse))
	{
		oss << "Failure of rmse percentiles test" << std::endl;
		oss << "expRmse: " << expRmse << std::endl;
		oss << "tolRmse: " << tolRmse << std::endl;
		oss << "gotRmse: " << gotRmse << std::endl;
		oss << dat::infoString(exp9, "exp") << std::endl;
		oss << dat::infoString(got9, "got") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for prob::Frac9
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << prob_Frac9_test0();
	oss << prob_Frac9_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
