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


namespace
{
	template <typename CellType>
	dat::grid<CellType>
	simCells
		( dat::Extents const & cellSize
		, CellType const & srcCell
		)
	{
		dat::grid<CellType> cellGrid(cellSize);
		std::fill(cellGrid.begin(), cellGrid.end(), srcCell);
		return cellGrid;
	}

} // {}

bool
operator==
	( img::cfa::Cell<float> const & cA
	, img::cfa::Cell<float> const & cB
	)
{
	return
		(  dat::nearlyEquals(cA.theElems[0][0], cB.theElems[0][0])
		&& dat::nearlyEquals(cA.theElems[0][1], cB.theElems[0][1])
		&& dat::nearlyEquals(cA.theElems[1][0], cB.theElems[1][0])
		&& dat::nearlyEquals(cA.theElems[1][1], cB.theElems[1][1])
		);
}

template <typename CellType>
bool
areSame
	( dat::grid<CellType> const & gridA
	, dat::grid<CellType> const & gridB
	)
{
	bool same{ gridA.hwSize() == gridB.hwSize() };
	if (same)
	{
		using Iter = typename dat::grid<CellType>::const_iterator;
		for (Iter itA{gridA.begin()}, itB{gridB.begin()}
			; gridA.end() != itA ; ++itA, ++itB)
		{
			same &= (*itA == *itB);
			if (! same)
			{
				break;
			}
		}
	}
	return same;
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
	dat::grid<CellType> const expCell{ simCells(cellSize, srcCell) };
	dat::Extents const elemSize{ img::cfa::elemSizeFor(cellSize) };
	dat::grid<CellType::value_type> expNorm(elemSize);
	std::fill(expNorm.begin(), expNorm.end(), 8.f);

	// expand cells into a multiplexed full element grid
	dat::grid<CellType::value_type> const expElem
		{ img::cfa::elemGridFor(expCell) };

	// normalize element grid by applying cell-specific gain factors
	dat::grid<CellType::value_type> const gotNorm
		{ img::cfa::grayGridFor(expElem, cellGain) };

	// compact element data back into cell layout
	dat::grid<CellType> const gotCell
		{ img::cfa::cellGridFor<CellType>(expElem) };

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
		oss << expNorm.infoStringContents("expNorm", "%9.3f") << std::endl;
		oss << gotNorm.infoStringContents("gotNorm", "%9.3f") << std::endl;
	}
	if (! areSame(gotCell, expCell))
	{
		dat::grid<CellType::value_type> const expAsElem
			{ img::cfa::elemGridFor(expCell) };
		dat::grid<CellType::value_type> const gotAsElem
			{ img::cfa::elemGridFor(gotCell) };
		oss << "Failure of uniform output test" << std::endl;
		oss << expAsElem.infoStringContents("expAsElem", "%9.3f") << std::endl;
		oss << gotAsElem.infoStringContents("gotAsElem", "%9.3f") << std::endl;
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
