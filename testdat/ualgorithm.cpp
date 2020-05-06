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
\brief  This file contains unit test for dat::algorithm
*/


#include "libdat/dat.h"
#include "libdat/info.h"
#include "libdat/algorithm.h"
#include "libio/stream.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>


namespace
{
	inline
	bool
	sameIndex
		( size_t const & got
		, size_t const & exp
		)
	{
		bool same(false);
		if (dat::isValid(exp))
		{
			same = dat::nearlyEquals(got, exp);
		}
		else
		{
			same = (! dat::isValid(got));
		}
		return same;
	}

	inline
	bool
	sameIndices
		( std::pair<size_t, size_t> const & pairA
		, std::pair<size_t, size_t> const & pairB
		)
	{
		return
			(  sameIndex(pairA.first, pairB.first)
			&& sameIndex(pairA.second, pairB.second)
			);
	}

//! Check index bracket computations
std::string
dat_algorithm_test0
	()
{
	std::ostringstream oss;

	bool const runEmpty(true);
	bool const runOneLo(true);
	bool const runOneAt(true);
	bool const runOneHi(true);
	bool const runGeneral(true);

	size_t const badNdx(dat::nullValue<size_t>());
	// check empty collection
	if (runEmpty)
	{
		std::vector<int> const data;
		std::pair<size_t, size_t> const gotPair
			(dat::indexBounds(0., data.begin(), data.end()));
		if (dat::isValid(gotPair))
		{
			oss << "Failure of empty data test" << std::endl;
		}
	}

	// check trivial case
	{
		std::vector<double> const data{5.25};

		// small value test
		if (runOneLo)
		{
			double const value(data[0] - 1.);
			std::pair<size_t, size_t> const expPair(badNdx, 0u);
			std::pair<size_t, size_t> const gotPair
				(dat::indexBounds(value, data.begin(), data.end()));
			if (dat::isValid(gotPair.first))
			{
				oss << "Failure of 1 element Lo/Lo test" << std::endl;
			}
			if (! (gotPair.second == expPair.second))
			{
				oss << "Failure of 1 element Lo/Hi test" << std::endl;
			}
		}
		// large value test
		if (runOneHi)
		{
			double const value(data[0] + 1.);
			std::pair<size_t, size_t> const expPair(0u, badNdx);
			std::pair<size_t, size_t> const gotPair
				(dat::indexBounds(value, data.begin(), data.end()));
			if (dat::isValid(gotPair.second))
			{
				oss << "Failure of 1 element Hi/Lo test" << std::endl;
			}
			if (! (gotPair.first == expPair.first))
			{
				oss << "Failure of 1 element Hi/Hi test" << std::endl;
			}
		}
		// matching value test
		if (runOneAt)
		{
			double const value(data[0]);
			std::pair<size_t, size_t> const expPair(0u, badNdx);
			std::pair<size_t, size_t> const gotPair
				(dat::indexBounds(value, data.begin(), data.end()));
			if (dat::isValid(gotPair.second))
			{
				oss << "Failure of 1 element exact second test" << std::endl;
			}
			if (! (gotPair.first == expPair.first))
			{
				oss << "Failure of 1 element exact first test" << std::endl;
			}
		}
	}

	struct GenData
	{
		std::vector<double> const theData;

		GenData
			( std::vector<double> const & data
			)
			: theData(data)
		{ }

		// expected pair of indices for test value
		std::pair<size_t, size_t>
		indicesFor
			( double const & value
			) const
		{
			std::pair<size_t, size_t> ndxLoHi
				(dat::nullValue<size_t>(), dat::nullValue<size_t>());
			size_t & ndxLo = ndxLoHi.first;
			size_t & ndxHi = ndxLoHi.second;

			size_t ndxMax(theData.size() - 1u);

			// set expected values
			double delta(value - 70.);
			if (0. <= delta)
			{
				ndxLo = std::min
					(static_cast<size_t>(std::floor(delta)), ndxMax);
			}

			if (value < theData[ndxMax])
			{
				ndxHi = static_cast<size_t>(std::max(0., std::ceil(delta)));
			}

			if (ndxLo == ndxHi)
			{
				++ndxHi;
			}

			/*
			// display test data
			if ((ndxLo < theData.size()) && (ndxHi < theData.size()))
			{
				io::out() << "(theData[Lo] <= value < theData[hi]):"
					<< std::fixed << std::setprecision(3u)
					<< " " << std::setw(9u) << theData[ndxLo]
					<< " " << std::setw(9u) << value
					<< " " << std::setw(9u) << theData[ndxHi]
					<< std::endl;
			}
			else
			{
				io::out() << "value,ndxLo,Hi:"
					<< std::fixed << std::setprecision(3u)
					<< " " << std::setw(7u) << value
					<< " " << std::setw(20u) << ndxLo
					<< " " << std::setw(20u) << ndxHi
					<< std::endl;
			}
			*/

			return ndxLoHi;
		}
	};

	// check general case
	if (runGeneral)
	{
		std::vector<double> const data{70., 71., 72.};
		GenData const gen(data);
		double const delV(1./4.);
		double const minV(data[0] - delV);
		double const maxV(data[data.size() - 1u] + delV);
		for (double value(minV) ; value <= maxV ; value += delV)
		{
			std::pair<size_t, size_t> const expPair(gen.indicesFor(value));
			std::pair<size_t, size_t> const gotPair
				(dat::indexBounds(value, data.begin(), data.end()));
			if (! sameIndices(gotPair, expPair))
			{
				oss << "Failure of general test" << std::endl;
				oss << dat::infoString(data.begin(), data.end(), "data")
					<< std::endl;
				oss << "value: " << value << std::endl;
				oss << dat::infoString(expPair, "expPair") << std::endl;
				oss << dat::infoString(gotPair, "gotPair") << std::endl;
				break;
			}
		}

	}

	return oss.str();
}

/*
//! Check TODO....
std::string
dat_algorithm_test1
	()
{
	std::ostringstream oss;
	oss << "Failure: implement this test!" << std::endl;
	return oss.str();
}
*/


}

//! Unit test for dat::algorithm
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_algorithm_test0();
//	oss << dat_algorithm_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
