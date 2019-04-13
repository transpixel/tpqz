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
\brief  This file contains unit test for prob::mean
*/


#include "libprob/mean.h"

#include "libdat/compare.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace
{

//! Check for common functions
std::string
prob_mean_test0
	()
{
	std::ostringstream oss;
	/*
	prob::mean const aNull(dat::nullValue<prob::mean>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

//! Check several mean v
std::string
prob_mean_test1
	()
{
	std::ostringstream oss;

	// Check error cases (involving negative numbers)
	{
		// only arithmetic mean is defined if non positive values
		std::vector<double> const mix{ 2., 1., -2., 3. };
		double const expAri{ 1. };
		double const expGeo{ dat::nullValue<double>() };
		double const expHar{ dat::nullValue<double>() };
		double const gotAri{ prob::mean::arithmetic(mix.begin(), mix.end()) };
		double const gotGeo{ prob::mean::geometric(mix.begin(), mix.end()) };
		double const gotHar{ prob::mean::harmonic(mix.begin(), mix.end()) };
		if (! dat::nearlyEquals(gotAri, expAri))
		{
			oss << "Failure of arithmetic mean mixed data test" << std::endl;
			oss << dat::infoString(expAri, "expAri") << std::endl;
			oss << dat::infoString(gotAri, "gotAri") << std::endl;
		}
		if (dat::isValid(gotGeo))
		{
			oss << "Failure of geometric mean mixed data test" << std::endl;
			oss << dat::infoString(expGeo, "expGeo") << std::endl;
			oss << dat::infoString(gotGeo, "gotGeo") << std::endl;
		}
		if (dat::isValid(gotHar))
		{
			oss << "Failure of harmonic mean mixed data test" << std::endl;
			oss << dat::infoString(expHar, "expHar") << std::endl;
			oss << dat::infoString(gotHar, "gotHar") << std::endl;
		}
	}

	// check standard case (all positive numbers)
	{
		std::vector<double> const pos{ 1., 2., 3., 4. };
		std::vector<double> inv{ 1./pos[0], 1./pos[1], 1./pos[2], 1./pos[3] };
		double const scale{ 1./double(pos.size()) };

		using std::log;
		double const expAri{ scale * (pos[0] + pos[1] + pos[2] + pos[3]) };
		double const sumLogs
			{ log(pos[0]) + log(pos[1]) + log(pos[2]) + log(pos[3]) };
		double const expGeo{ std::exp(scale * sumLogs) };
		// identify between HM and AM
		double const ariOfInv{ scale * (inv[0] + inv[1] + inv[2] + inv[3]) };
		double const expHar{ 1. / ariOfInv };
		double const gotAri{ prob::mean::arithmetic(pos.begin(), pos.end()) };
		double const gotGeo{ prob::mean::geometric(pos.begin(), pos.end()) };
		double const gotHar{ prob::mean::harmonic(pos.begin(), pos.end()) };
		if (! dat::nearlyEquals(gotAri, expAri))
		{
			oss << "Failure of arithmetic mean pos data test" << std::endl;
			oss << dat::infoString(expAri, "expAri") << std::endl;
			oss << dat::infoString(gotAri, "gotAri") << std::endl;
		}
		if (! dat::nearlyEquals(gotGeo, expGeo))
		{
			oss << "Failure of geometric mean pos data test" << std::endl;
			oss << dat::infoString(expGeo, "expGeo") << std::endl;
			oss << dat::infoString(gotGeo, "gotGeo") << std::endl;
		}
		if (! dat::nearlyEquals(gotHar, expHar))
		{
			oss << "Failure of harmonic mean pos data test" << std::endl;
			oss << dat::infoString(expHar, "expHar") << std::endl;
			oss << dat::infoString(gotHar, "gotHar") << std::endl;
		}
	}

	return oss.str();
}


}

//! Unit test for prob::mean
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << prob_mean_test0();
	oss << prob_mean_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
