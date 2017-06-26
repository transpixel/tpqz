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
\brief  This file contains unit test for dat::layout
*/


#include "libdat/layout.h"

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
dat_layout_test0
	()
{
	std::ostringstream oss;
	/*
	dat::layout const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	*/
	return oss.str();
}

namespace center
{
	//! Test that expected values are as expected
	void
	checkIndices
		( std::ostream & oss
		, size_t const & max
		, size_t const & num
		, std::vector<size_t> const & expIndices
		, std::string const & tname
		)
	{
		std::vector<size_t> const gotIndices
			(dat::layout::centered::indices(max, num));
		bool const same
			( dat::nearlyEquals<size_t>
				( gotIndices.begin(), gotIndices.end()
				, expIndices.begin(), expIndices.end()
				)
			);
		if (! same)
		{
			oss << "Failure of index test: " << tname << std::endl;
			oss << "exp: ";
			for (size_t const exp : expIndices)
			{
				oss << " " << exp;
			}
			oss << std::endl;
			oss << "got: ";
			for (size_t const got : gotIndices)
			{
				oss << " " << got;
			}
			oss << std::endl;
		}
	}
}


//! Check basic geometry operations
std::string
dat_layout_test1
	()
{
	std::ostringstream oss;

	using Ndxen = std::vector<size_t>;
	using center::checkIndices;

	// 0 1 2 3 4 5 6

	// - - - 3 - - -
	checkIndices(oss, 7u, 1u, Ndxen{ 3u }, "7,1");

	// - 1 - - - 5 -
	checkIndices(oss, 7u, 2u, Ndxen{ 1u, 5u }, "7,2");

	// - 1 - 3 - 5 -
	checkIndices(oss, 7u, 3u, Ndxen{ 1u, 3u, 5u }, "7,3");

	// 0 - 2 - 4 - 6
	checkIndices(oss, 7u, 4u, Ndxen{ 0u, 2u, 4u, 6u }, "7,4");

	// 0 - 2 3 4 - 6
	checkIndices(oss, 7u, 5u, Ndxen{ 0u, 2u, 3u, 4u, 6u }, "7,5");

	// 0 1 2 - 4 5 6
	checkIndices(oss, 7u, 6u, Ndxen{ 0u, 1u, 2u, 4u, 5u, 6u }, "7,6");

	// 0 1 2 3 4 5 6
	checkIndices(oss, 7u, 7u, Ndxen{ 0u, 1u, 2u, 3u, 4u, 5u, 6u }, "7,7");

	// xx
	checkIndices(oss, 7u, 8u, Ndxen{ }, "7,8");


	// 0 1 2 3 4 5

	// - - - 3 - -
	checkIndices(oss, 6u, 1u, Ndxen{ 3u }, "6,1");

	// - 1 - - 4 -
	checkIndices(oss, 6u, 2u, Ndxen{ 1u, 4u }, "6,2");

	// - 1 - 3 - 5
	checkIndices(oss, 6u, 3u, Ndxen{ 1u, 3u, 5u }, "6,3");

	// 0 - 2 3 - 5
	checkIndices(oss, 6u, 4u, Ndxen{ 0u, 2u, 3u, 5u }, "6,4");

	// 0 1 - 3 4 5
	checkIndices(oss, 6u, 5u, Ndxen{ 0u, 1u, 3u, 4u, 5u }, "6,5");

	// 0 1 2 3 4 5
	checkIndices(oss, 6u, 6u, Ndxen{ 0u, 1u, 2u, 3u, 4u, 5u }, "6,6");

	// xx
	checkIndices(oss, 6u, 7u, Ndxen{ }, "6,7");


	// hack check on rowcols
	dat::Extents const domain(7u, 9u);
	dat::Extents const hwSize(5u, 4u);
	std::vector<dat::RowCol> const gotRCs
		(dat::layout::centered::rowcols(domain, hwSize));
	size_t const expCount{ hwSize.size() };
	size_t const gotCount{ gotRCs.size() };
	if (! dat::nearlyEquals(gotCount, expCount))
	{
		oss << "Failure of rowcols count test" << std::endl;
		oss << dat::infoString(expCount, "expCount") << std::endl;
		oss << dat::infoString(gotCount, "gotCount") << std::endl;
	}

	return oss.str();
}

	void
	checkFor
		( std::ostream & oss
		, std::vector<dat::Spot> const & gotSpots
		, std::vector<dat::Spot> const & expSpots
		)
	{
		for (dat::Spot const & expSpot : expSpots)
		{
			std::vector<dat::Spot>::const_iterator const itFind
				{ std::find(gotSpots.begin(), gotSpots.end(), expSpot) };
			if (gotSpots.end() == itFind)
			{
				oss << "Failure to find spot test" << std::endl;
				oss << dat::infoString(expSpot, "expSpot") << std::endl;
				break;
			}
		}
	}

//! Check floating point functions
std::string
dat_layout_test2
	()
{
	std::ostringstream oss;

	dat::Range<double> const xRange(4., 5.);
	dat::Range<double> const yRange(-11., -9.);
	dat::Area<double> const area{ xRange, yRange };
	dat::Extents const hwSize(2u, 3u);
	size_t const expNum{ hwSize.size() };
	std::vector<dat::Spot> const expSpots
		{ dat::Spot{{4., -11.}}, dat::Spot{{4., -10.}}, dat::Spot{{4., -9.}}
		, dat::Spot{{5., -11.}}, dat::Spot{{5., -10.}}, dat::Spot{{5., -9.}}
		};

	std::vector<dat::Spot> const gotSpots
		{ dat::layout::expanded::spots(area, hwSize) };
	size_t const gotNum{ gotSpots.size() };

	// check number of samples
	if (! dat::nearlyEquals(gotNum, expNum))
	{
		oss << "Failure of point size test" << std::endl;
		oss << dat::infoString(expNum, "expNum") << std::endl;
		oss << dat::infoString(gotNum, "gotNum") << std::endl;
	}
	else
	{
		// check values in returned collection
		checkFor(oss, gotSpots, expSpots);
	}

	return oss.str();
}


}

//! Unit test for dat::layout
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_layout_test0();
	oss << dat_layout_test1();
	oss << dat_layout_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
