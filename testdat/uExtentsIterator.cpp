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
\brief  This file contains unit test for dat::ExtentsIterator
*/


#include "libdat/ExtentsIterator.h"

#include "libdat/info.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_ExtentsIterator_test0
	()
{
	std::ostringstream oss;
	dat::ExtentsIterator const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
	//	oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

	//! Test iterator validity via explicit exercise
	void
	checkIter
		( std::ostream & oss
		, size_t const & nrows
		, size_t const & ncols
		, std::string const & tname
		)
	{
		dat::Extents const extents{ nrows, ncols };
		dat::ExtentsIterator iter(extents);

		// check start condition
		if (! (extents.isValid() == iter.isValid()))
		{
			oss << "Failure of valid on test:"
				<< " " << tname << std::endl;
			oss << "expValid: " << dat::infoString(extents.isValid()) << '\n';
			oss << "gotValid: " << dat::infoString(iter.isValid()) << '\n';
		}

		// check intermediate condition
		for (size_t row(0u) ; row < extents.high() ; ++row)
		{
			for (size_t col(0u) ; col < extents.wide() ; ++col)
			{
				if (! (static_cast<bool>(iter) == iter.isActive()))
				{
					oss << "Failure of bool==isActive while active";
				}

				// should be valid "before" operations on current element
				if (! iter)
				{
					oss << "Failure of increment active test:"
					<< " " << tname << std::endl;
					goto Bail;
				}

				// check values
				dat::RowCol const expRowCol{{ row, col }};
				dat::RowCol const gotRowCol(*iter);
				if (! dat::nearlyEquals(gotRowCol, expRowCol))
				{
					oss << "Failure of row/col test: " << tname << std::endl;
					oss << dat::infoString(expRowCol, "expRowCol") << '\n';
					oss << dat::infoString(gotRowCol, "gotRowCol") << '\n';
				}

				// move to next element
				++iter;
			}
		}
Bail:

		// check end condition
		if (iter.isValid())
		{
			if (! (static_cast<bool>(iter) == iter.isActive()))
			{
				oss << "Failure of bool==isActive at end";
			}
		}
		if (iter.isActive())
		{
			oss << "Failure of increment end test:"
				<< " " << tname << std::endl;
		}
	}

//! Check basic travseral operations
std::string
dat_ExtentsIterator_test1
	()
{
	std::ostringstream oss;

	// check with null data
	checkIter(oss, 0u, 0u, "empty00");
	checkIter(oss, 0u, 3u, "empty03");
	checkIter(oss, 2u, 0u, "empty20");

	// check with simple data
	checkIter(oss, 1u, 7u, "row");
	checkIter(oss, 5u, 1u, "col");
	checkIter(oss, 2u, 3u, "area");

	// simple cases
	checkIter(oss, 1u, 1u, "row");

	return oss.str();
}


}

//! Unit test for dat::ExtentsIterator
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_ExtentsIterator_test0();
	oss << dat_ExtentsIterator_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
