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
\brief  This file contains unit test for img::cfa
*/


#include "libimg/cfa.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include "libdat/grid.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
img_cfa_test0
	()
{
	std::ostringstream oss;
	/*
	img::cfa const aNull(dat::nullValue<img::cfa>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

//! Check basic gain correction
std::string
img_cfa_test1
	()
{
	std::ostringstream oss;

	//  simplified test data
	using CellType = img::cfa::Cell<float>;
	dat::Extents const cellSize(5u, 7u);
	CellType const srcCell{ 1., 2., 4., 8. };
	CellType const cellGain{ 8., 4., 2., 1. };

	// simulate a 'raw' image
	dat::Extents const elemSize{ img::cfa::elemSizeFor(cellSize) };
	dat::grid<CellType::value_type> expNorm(elemSize);
	std::fill(expNorm.begin(), expNorm.end(), 8.);

	// expand cells into a multiplexed full element grid
	dat::grid<CellType> cellGrid(cellSize);
	std::fill(cellGrid.begin(), cellGrid.end(), srcCell);
	dat::grid<CellType::value_type> const elemGrid
		{ img::cfa::elemGridFor(cellGrid) };

	// normalize element grid by applying cell-specific gain factors
	dat::grid<CellType::value_type> const gotNorm
		{ img::cfa::grayGridFor(elemGrid, cellGain) };

	// compare results
	if (! dat::nearlyEquals(gotNorm.size(), expNorm.size()))
	{
		oss << "Failure of size test" << std::endl;
		oss << dat::infoString(expNorm, "expNorm") << std::endl;
		oss << dat::infoString(gotNorm, "gotNorm") << std::endl;
	}
	if (! std::equal(gotNorm.begin(), gotNorm.end(), expNorm.begin()))
	{
		oss << "Failure of uniform output test" << std::endl;
		oss << elemGrid.infoStringContents("elemGrid", "%9.3f") << std::endl;
		oss << expNorm.infoStringContents("expNorm", "%9.3f") << std::endl;
		oss << gotNorm.infoStringContents("gotNorm", "%9.3f") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for img::cfa
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << img_cfa_test0();
	oss << img_cfa_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
