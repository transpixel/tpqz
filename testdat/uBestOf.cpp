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
\brief  This file contains unit test for dat::BestOf
*/


#include "libdat/BestOf.h"

#include "libdat/compare.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_BestOf_test0
	()
{
	std::ostringstream oss;
	dat::BestOf<int> const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check simple use
std::string
dat_BestOf_test1
	()
{
	std::ostringstream oss;

	std::vector<double> const values
		{ 1., 15., 12., 0., 16., 0., 19., 7., 13., 2.
		, 1., 15., 12., 0., 16., 0., 19., 7., 13., 2.
		};

	// create tracker and add samples
	constexpr size_t const numBest{ 5u };
	dat::BestOf<double, std::greater<double> > besty(numBest);
	for (double const & value : values)
	{
		besty.addSample(value);
	}

	// check if tracking size is correct
	size_t const expSize{ numBest };
	size_t const gotSize{ besty.size() };
	if (! dat::nearlyEquals(gotSize, expSize))
	{
		oss << "Failure of size test" << std::endl;
		oss << dat::infoString(expSize, "expSize") << std::endl;
		oss << dat::infoString(gotSize, "gotSize") << std::endl;
	}
	else
	{
		// use standard sort as comparison condition
		std::vector<double> sorted(values);
		std::sort(sorted.begin(), sorted.end(), std::greater<double>() );
		std::vector<double> const expBest
			(sorted.begin(), sorted.begin()+numBest);

		// check if results match expected sort
		for (size_t nn{0u} ; nn < numBest ; ++nn)
		{
			double const & expNext = expBest[nn];
			double const gotNext{ besty.itemAt(nn) };
			if (! dat::nearlyEquals(gotNext, expNext))
			{
				oss << "Filure of next test: nn = " << nn << std::endl;
				oss << dat::infoString(expNext, "expNext") << std::endl;
				oss << dat::infoString(gotNext, "gotNext") << std::endl;
			}
		}
	}

	return oss.str();
}


}

//! Unit test for dat::BestOf
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_BestOf_test0();
	oss << dat_BestOf_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
