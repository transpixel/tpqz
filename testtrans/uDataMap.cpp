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
\brief  This file contains unit test for trans::DataMap
*/


#include "libtrans/DataMap.h"

#include "libio/stream.h"
#include "libmath/Partition.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
trans_DataMap_test0
	()
{
	std::ostringstream oss;
	trans::DataMap const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check TODO....
std::string
trans_DataMap_test1
	()
{
	std::ostringstream oss;

	// simple small data set
	std::vector<double> const data{ -187., -37., 13.  };

	// create data mapping function
	trans::DataMap const func(data.begin(), data.end());

	// check extreme cases
	double const tooSml(0. - .125);
	if (dat::isValid(func(tooSml)))
	{
		oss << "Failure of data tooSml test" << std::endl;
	}

	double const tooBig(1. + .125);
	if (dat::isValid(func(tooBig)))
	{
		oss << "Failure of data tooBig test" << std::endl;
	}

	// check mapping for various test cases
	double const delta{ 1. / static_cast<double>(data.size()) };
	double const eps(dat::smallValue<double>());
	std::vector<std::pair<double, double> > const tExpGots
		{ std::make_pair( 0.00, func(0.  * delta))
		, std::make_pair(3./8., func( .5 * delta))
		, std::make_pair( 0.75, func(1.  * delta))
		, std::make_pair(7./8., func(1.5 * delta))
		, std::make_pair( 1.00, func(2.  * delta - eps))
		};
	for (std::pair<double, double> const tExpGot : tExpGots)
	{
		double const & expOut = tExpGot.first;
		double const & gotOut = tExpGot.second;
		if (! dat::nearlyEquals(gotOut, expOut))
		{
			oss << "Failure of data value test" << std::endl;
			oss << dat::infoString(expOut, "expOut") << '\n';
			oss << dat::infoString(gotOut, "gotOut") << '\n';
		}
	}


	math::Partition const rangePart
		(dat::Range<double>(data.front(), data.back()), data.size());

	return oss.str();
}


}

//! Unit test for trans::DataMap
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << trans_DataMap_test0();
	oss << trans_DataMap_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
