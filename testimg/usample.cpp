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
\brief  This file contains unit test for img::sample
*/


#include "libimg/sample.h"

#include "libdat/array.h"
#include "libdat/grid.h"
#include "libdat/info.h"
#include "libio/stream.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

/*
//! Check for common functions
std::string
img_sample_test0
	()
{
	std::ostringstream oss;
	img::sample const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}
*/

	//! True if fish is in pond
	inline
	bool
	contains
		( std::vector<dat::Spot> const & pond
		, dat::Spot const & fish
		)
	{
		std::vector<dat::Spot>::const_iterator const itFind
			(std::find(pond.begin(), pond.end(), fish));
		return (pond.end() != itFind);
	}

	//! Check that size and contents are identical
	bool
	areSame
		( std::vector<dat::Spot> got
		, std::vector<dat::Spot> exp
		)
	{
		// first check sizes
		bool same(got.size() == exp.size());
		if (same)
		{
			// then contents

			// sort values for easier testing
			std::sort(exp.begin(), exp.end());
			std::sort(got.begin(), got.end());

			// check elements
			same = std::equal(got.begin(), got.end(), exp.begin());
		}
		return same;
	}

//! Check basic sampling operations
std::string
img_sample_test1
	()
{
	std::ostringstream oss;

	// fill test grid with location data
	dat::grid<dat::Spot> values(3,4);
	for (size_t row(0u) ; row < values.high() ; ++row)
	{
		for (size_t col(0u) ; col < values.wide() ; ++col)
		{
			values(row, col) = {{ (double)row, (double)col }};
		}
	}
	// note locations not included in test data
	std::vector<dat::Spot> const skipped
		{{ dat::Spot{{ 1., 1. }}
		 , dat::Spot{{ 1., 2. }}
		}};
	// note locations that are included
	std::vector<dat::Spot> expValues;
	expValues.reserve(values.size());
	for (dat::Spot const & value : values)
	{
		if (! contains(skipped, value))
		{
			expValues.push_back(value);
		}
	}

	// exercise sampling function to get boarder samples
	std::vector<dat::Spot> gotValues
		(img::sample::fromBorderBand(values, values.hwSize(), 1u));

	// check results
	bool const sameValues(areSame(gotValues, expValues));
	if (! sameValues)
	{
		oss << "Failure of perimeter sampling test" << std::endl;
		oss << "expValues.size(): " << expValues.size() << std::endl;
		oss << "gotValues.size(): " << gotValues.size() << std::endl;
		bool const showDetail(true);
		if (showDetail)
		{
			oss << "expValues:" << std::endl;
			for (dat::Spot const & value : expValues)
			{
				oss << "... " << dat::infoString(value) << std::endl;
			}
			oss << "gotValues:" << std::endl;
			for (dat::Spot const & value : gotValues)
			{
				oss << "... " << dat::infoString(value) << std::endl;
			}
		}
	}

	return oss.str();
}

	//! Check if got/exp are equal, report if not
	void
	testSamps
		( std::ostream & oss
		, std::vector<int> const & gotSamps
		, std::vector<int> const & expSamps
		, std::string const & tname
		)
	{
		if (gotSamps.size() == expSamps.size())
		{
			bool const sameContent
				( std::equal
					( gotSamps.begin(), gotSamps.end()
					, expSamps.begin()
					)
				);
			if (! sameContent)
			{
				oss << "Failure of " << tname << " content test" << std::endl;
			}
		}
		else
		{
			oss << "Failure of " << tname << " size test" << std::endl;
		}
	}

//! Check basic neighborhood sampling
std::string
img_sample_test2
	()
{
	std::ostringstream oss;

	dat::grid<int> values(15u, 20u);
	std::iota(values.begin(), values.end(), static_cast<int>(0));
	// io::out() << values.infoStringContents("values", "%3d") << std::endl;

	// sample within area of interest
	{
		dat::RowCol const rc{6u, 5u};
		size_t const cellPad(1u);
		std::vector<int> const expSamps
			{{   83,  84,    85,  86,    87,  88
			 ,  103, 104,   105, 106,   107, 108

			 ,  123, 124,   125, 126,   127, 128
			 ,  143, 144,   145, 146,   147, 148

			 ,  163, 164,   165, 166,   167, 168
			 ,  183, 184,   185, 186,   187, 188
			}};
		std::vector<int> const gotSamps
			(img::sample::fromCellsNearTo(rc, values, cellPad));
		testSamps(oss, gotSamps, expSamps, "internal");

	}

	// sample at edge
	{
		dat::RowCol const rc{12u, 18u};
		size_t const cellPad(1u);
		std::vector<int> const expSamps
			{{  216, 217, 218,   219
			 ,  236, 237, 238,   239
			 ,  256, 257, 258,   259
			 ,  276, 277, 278,   279
			 ,  296, 297, 298,   299
			}};
		std::vector<int> const gotSamps
			(img::sample::fromCellsNearTo(rc, values, cellPad));
		testSamps(oss, gotSamps, expSamps, "internal");

	}

	return oss.str();
}

}

//! Unit test for img::sample
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
//	oss << img_sample_test0();
	oss << img_sample_test1();
	oss << img_sample_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
