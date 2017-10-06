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
\brief  This file contains unit test for la::eigen
*/


#include "libla/eigen.h"

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
la_eigen_test0
	()
{
	std::ostringstream oss;
	/*
	la::eigen const aNull(dat::nullValue<la::eigen>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

	template <typename ElemType>
	inline
	dat::grid<ElemType>
	rcGrid
		( size_t const & high
		, size_t const & wide
		, ElemType const & offset = ElemType(100)
		)
	{
		dat::grid<ElemType> grid(high, wide);
		for (size_t row{0u} ; row < high ; ++row)
		{
			ElemType const rval(row);
			for (size_t col{0u} ; col < wide ; ++col)
			{
				ElemType const cval(col);
				grid(row, col) = offset*rval + cval;
			}
		}
		return grid;
	}


//! Demonstrate grid->Eigen use
std::string
la_eigen_test1
	()
{
	std::ostringstream oss;

	using ElemType = double;
	constexpr size_t high{ 5u };
	constexpr size_t wide{ 7u };

	dat::grid<ElemType> const expGrid{ rcGrid<ElemType>(high, wide, 10.) };

	/*
	// cast to fixed size eigen matrix
	using EMatrixFixed = typename Eigen::Matrix
		<ElemType, (int)high, (int)wide, Eigen::RowMajor>;
	Eigen::Map<EMatrixFixed const> const gotFix(expGrid.begin(), high, wide);
	io::out() << "gotFix:\n" << gotFix << std::endl;
	*/

	/*
	// cast to dynamic size eigen matrix
	using EMatrixDyna = typename Eigen::Matrix
		<ElemType, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
	Eigen::Map<EMatrixDyna const> const gotDyn
		(expGrid.begin(), expGrid.high(), expGrid.wide());
	io::out() << "gotDyn:\n" << gotDyn << std::endl;
	*/

	// cast to Eigen readonly interpretation
	la::eigen::ConstMap<ElemType> const gotEM{ la::eigen::withGrid(expGrid) };;

	// cast to Eigen writeable interpretation
	dat::grid<ElemType> gotGrid(expGrid.hwSize()); // NOTE! Must allocate here
	std::fill(gotGrid.begin(), gotGrid.end(), dat::nullValue<ElemType>());
	la::eigen::WriteMap<ElemType> putEM{ la::eigen::withGrid(&gotGrid) };

	// perform eigen library operation
	putEM = gotEM;

	// check if operations succeeded
	if (! dat::nearlyEquals<ElemType>
		( gotGrid.begin(), gotGrid.end()
		, expGrid.begin(), expGrid.end()
		)
	   )
	{
		oss << "Failure of const/write cast test" << std::endl;
		oss << expGrid.infoStringContents("expGrid", "%9.3f") << std::endl;
		oss << gotGrid.infoStringContents("gotGrid", "%9.3f") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for la::eigen
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << la_eigen_test0();
	oss << la_eigen_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
