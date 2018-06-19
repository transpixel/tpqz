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
\brief  This file contains unit test for prob::Stats
*/


#include "libprob/Stats.h"

#include "libdat/dat.h"
#include "libdat/info.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
prob_Stats_test0
	()
{
	std::ostringstream oss;
	prob::Stats const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check TODO....
std::string
prob_Stats_test1
	()
{
	std::ostringstream oss;

	// check trivial case
	{
		std::array<int, 3u> const data{{ 5, 7, -6 }};
		prob::Stats stats;
		stats.add(data[0]);
		if (! dat::nearlyEquals(stats.medianValue(), (double)data[0]))
		{
			oss << "Failure of single value median test" << std::endl;
		}
	}

	// check odd number of elements
	{
		std::array<int, 3u> const data{{ 5, 7, -6 }};
		prob::Stats stats;
		stats.add(data[0]);
		stats.add(data[1]);
		stats.add(data[2]);
		double const expMean((1./3.)*(data[0]+data[1]+data[2]));
		double const gotMean(stats.mean());
		if (! dat::nearlyEquals(gotMean, expMean))
		{
			oss << "Failure of simple mean test" << std::endl;
		}

		double const expMedian(5.); // middle value for odd test
		double const gotMedian(stats.medianValue());
		if (! dat::nearlyEquals(gotMedian, expMedian))
		{
			oss << "Failure of median/odd test" << std::endl;
			oss << "exp: " << expMedian << std::endl;
			oss << "got: " << gotMedian << std::endl;
		}
	}

	// check even number of elements
	{
		std::array<int, 4u> const data{{ 5, 7, -6, -4 }};

		prob::Stats const stats(data.begin(), data.end());
		double const expMean(.5);
		double const gotMean(stats.mean());
		if (! dat::nearlyEquals(gotMean, expMean))
		{
			oss << "Failure of array mean test" << std::endl;
			oss << "exp: " << expMean << std::endl;
			oss << "got: " << gotMean << std::endl;
		}

		double const expMedian(.5); // even: average of middle two
		double const gotMedian(stats.medianValue());
		if (! dat::nearlyEquals(gotMedian, expMedian))
		{
			oss << "Failure of median/even test" << std::endl;
			oss << "exp: " << expMedian << std::endl;
			oss << "got: " << gotMedian << std::endl;
		}
	}

	return oss.str();
}

//! Check simple set of data
std::string
prob_Stats_test2
	()
{
	std::ostringstream oss;

	std::vector<double> const data{ .016, -.003, -.448 };
	dat::MinMax<double> const expMinMax{ -.448, .016 };

	prob::Stats const stats(data.begin(), data.end());
	dat::MinMax<double> const & gotMinMax = stats.theMinMax;

	if (! gotMinMax.nearlyEquals(expMinMax))
	{
		oss << "Failure of MinMax test" << std::endl;
		oss << dat::infoString(expMinMax, "expMinMax") << std::endl;
		oss << dat::infoString(gotMinMax, "gotMinMax") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for prob::Stats
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << prob_Stats_test0();
	oss << prob_Stats_test1();
	oss << prob_Stats_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
