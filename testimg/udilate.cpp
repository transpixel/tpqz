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
\brief  This file contains unit test for img::dilate
*/


#include "libimg/dilate.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
img_dilate_test0
	()
{
	std::ostringstream oss;
	/*
	img::dilate const aNull(dat::nullValue<img::dilate>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

	using ElemType = float;
	constexpr ElemType const sBackElem{ dat::nullValue<float>() };
	constexpr ElemType const expValueTL{ 11 };
	constexpr ElemType const expValueTR{ 13 };
	constexpr ElemType const expValueBL{ 33 };
	constexpr ElemType const expValueBR{ 31 };

	//! Create a grid of background and only a few isolated pixels set
	dat::grid<ElemType>
	sourceGrid
		()
	{
		dat::grid<ElemType> grid(128u, 255u);
		std::fill(grid.begin(), grid.end(), sBackElem);
		grid(1u*32u, 1u*64u) = expValueTL;
		grid(1u*32u, 3u*64u) = expValueTR;
		grid(3u*32u, 3u*64u) = expValueBL;
		grid(3u*32u, 1u*64u) = expValueBR;
		return grid;
	}

	//! True if any background value pixels are in grid
	bool
	hasBackground
		( dat::grid<ElemType> const & grid
		)
	{
		bool hasNull{ false };
		for (ElemType const & elem : grid)
		if (! dat::isValid(elem))
		{
			hasNull = true;
			break;
		}
		return hasNull;
	}

//! Check simple dilation/fill operation
std::string
img_dilate_test1
	()
{
	std::ostringstream oss;

	// check degenerate case - all background grid
	dat::grid<ElemType> const backGrid(5u, 7u, dat::nullValue<ElemType>());
	dat::grid<ElemType> const gotBack{ img::dilate::floodFilled(backGrid) };
	if (! hasBackground(gotBack))
	{
		oss << "Failure of all-background fill test" << std::endl;
		oss << dat::infoString(gotBack(0u, 0u), "gotBack(0,0)") << std::endl;
	}

	// start with grid that has a few set pixels
	dat::grid<ElemType> const srcGrid{ sourceGrid() };
	// dialate repeatedly until entire grid is full
	dat::grid<ElemType> const grid{ img::dilate::floodFilled(srcGrid) };
	// check if grid is full
	bool const isFull{ (! hasBackground(grid)) };
	if (! isFull)
	{
		oss << "Failure of fillAll, full test" << std::endl;
	}

	// check that quadrants inherit correct value
	ElemType const gotValueTL{ grid(0u, 0u) };
	ElemType const gotValueTR{ grid(0u, grid.wide()-1u) };
	ElemType const gotValueBL{ grid(grid.high()-1u, 0u) };
	ElemType const gotValueBR{ grid(grid.high()-1u, grid.wide()-1u) };
	if ( (! dat::nearlyEquals(gotValueTR, expValueTR))
	  || (! dat::nearlyEquals(gotValueTR, expValueTR))
	  || (! dat::nearlyEquals(gotValueTR, expValueTR))
	  || (! dat::nearlyEquals(gotValueTR, expValueTR))
	   )
	{
		oss << "Failure of quadrant value test" << std::endl;
		oss << "TL:exp,got:"
			<< " " << dat::infoString(expValueTL)
			<< " " << dat::infoString(gotValueTL)
			<< std::endl;
		oss << "TR:exp,got:"
			<< " " << dat::infoString(expValueTR)
			<< " " << dat::infoString(gotValueTR)
			<< std::endl;
		oss << "BL:exp,got:"
			<< " " << dat::infoString(expValueBL)
			<< " " << dat::infoString(gotValueBL)
			<< std::endl;
		oss << "BR:exp,got:"
			<< " " << dat::infoString(expValueBR)
			<< " " << dat::infoString(gotValueBR)
			<< std::endl;
	}

	return oss.str();
}


}

//! Unit test for img::dilate
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << img_dilate_test0();
	oss << img_dilate_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
