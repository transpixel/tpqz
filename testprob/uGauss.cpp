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
\brief  This file contains unit test for prob::Gauss
*/


#include "libprob/Gauss.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include "libdat/compare.h"
#include "libio/sprintf.h"
#include "libmath/math.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
prob_Gauss_test0
	()
{
	std::ostringstream oss;
	prob::Gauss const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check check for unit probability
std::string
prob_Gauss_test1
	()
{
	std::ostringstream oss;

	constexpr double const sdev{ 1. };
	constexpr double const mean{ 10. };

	prob::Gauss const distro(sdev, mean);

	constexpr double const dx{ 1./1024. };
	constexpr double const xlim{ 24.*sdev };
	double sum{ 0. };
	for (double xx{-xlim} ; xx < xlim ; xx += dx)
	{
		sum += dx * distro(xx);
	}

	// check unit prob condition
	constexpr double tol{ math::eps };
	if (! dat::nearlyEquals(sum, 1., tol))
	{
		double const err{ sum - 1. };
		oss << "Failure of prob integration test" << std::endl;
		oss << "error: " << io::sprintf("%12.5e", err) << std::endl;
	}

	return oss.str();
}


}

//! Unit test for prob::Gauss
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << prob_Gauss_test0();
	oss << prob_Gauss_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
