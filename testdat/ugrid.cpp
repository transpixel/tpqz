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
\brief  This file contains unit test for dat::grid
*/


#include "libdat/grid.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <complex>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_grid_test0
	()
{
	std::ostringstream oss;
	dat::grid<std::complex<float> > const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check validity actions
std::string
dat_grid_test1
	()
{
	std::ostringstream oss;

	dat::grid<long> data(7u, 9u);
	std::fill(data.begin(), data.end(), dat::nullValue<long>());

	data(3u, 2u) = 8L;
	data(3u, 4u) = 8L;
	data(3u, 5u) = 8L;
	data(6u, 4u) = 8L;

	static std::vector<size_t> const expRowCounts
		{ 0u, 0u, 0u, 3u, 0u , 0u, 1u };
	static std::vector<size_t> const expColCounts
		{ 0u, 0u, 1u, 0u, 2u , 1u, 0u, 0u, 0u };

	for (size_t row{0u} ; row < data.high() ; ++row)
	{
		size_t const & expRowCount = expRowCounts[row];
		size_t const gotRowCount{ data.validCountInRow(row) };
		if (! dat::nearlyEquals(gotRowCount, expRowCount))
		{
			oss << "Failure in row count test, row: " << row << '\n';
			oss << dat::infoString(expRowCount, "expRowCount") << '\n';
			oss << dat::infoString(gotRowCount, "gotRowCount") << '\n';
		}
	}

	for (size_t col{0u} ; col < data.wide() ; ++col)
	{
		size_t const & expColCount = expColCounts[col];
		size_t const gotColCount{ data.validCountInCol(col) };
		if (! dat::nearlyEquals(gotColCount, expColCount))
		{
			oss << "Failure in col count test, col: " << col << '\n';
			oss << dat::infoString(expColCount, "expColCount") << '\n';
			oss << dat::infoString(gotColCount, "gotColCount") << '\n';
		}
	}

	return oss.str();
}


}

//! Unit test for dat::grid
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_grid_test0();
	oss << dat_grid_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
