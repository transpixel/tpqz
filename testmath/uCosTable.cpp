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
\brief  This file contains unit test for math::CosTable
*/


#include "libmath/CosTable.h"

#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
math_CosTable_test0
	()
{
	std::ostringstream oss;
	math::CosTable<void *> const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}

	// check empty table
	math::CosTable<float> const emptyTab(0u, dat::nullValue<float>());
	if (emptyTab.isValid())
	{
		oss << "Failure of empty table validity" << std::endl;
	}

	// check domain of validity
	std::vector<int> const someValues{ 3, -2, -3, 5 };
	math::CosTable<int> someTab(someValues.size(), dat::nullValue<int>());
	someTab.assignValueAt(someValues[0],  .9);
	someTab.assignValueAt(someValues[1],  .4);
	someTab.assignValueAt(someValues[2], -.5);
	someTab.assignValueAt(someValues[3], -.9);

	int const gotTop(someTab[someTab.indexForCosine( 1.)]);
	if (! dat::isValid(gotTop))
	{
		oss << "Failure of top pole validity test" << std::endl;
	}
	int const gotBot(someTab[someTab.indexForCosine(-1.)]);
	if (! dat::isValid(gotBot))
	{
		oss << "Failure of bottom pole validity test" << std::endl;
	}
	constexpr double sml{ 4. * math::eps };
	int const gotOver(someTab[someTab.indexForCosine( 1. + sml)]);
	if (dat::isValid(gotOver))
	{
		oss << "Failure of over top pole invalidity test" << std::endl;
		oss << dat::infoString(gotOver, "gotOver") << std::endl;
	}
	int const gotUnder(someTab[someTab.indexForCosine(-1. - sml)]);
	if (dat::isValid(gotUnder))
	{
		oss << "Failure of under bottom pole invalidity test" << std::endl;
		oss << dat::infoString(gotUnder, "gotUnder") << std::endl;
	}

	return oss.str();
}

//! Check basic operations
std::string
math_CosTable_test1
	()
{
	std::ostringstream oss;

	// Define test values from top to bottom for convenience
	std::vector<double> const zAngles{ .1, 1.5, 1.6, 3. };
	std::vector<std::string> const expNames{ "top", "hi", "lo", "bot" };

	// fill table with names - NOTE: reverse to match increasing cos function
	math::CosTable<std::string> ztab
		(expNames.rbegin(), expNames.rend(), std::string{});

	// check retrieval
	for (size_t nn(0u) ; nn < ztab.size() ; ++nn)
	{
		double const & zAngle = zAngles[nn];
		double const zCos(std::cos(zAngle));

		std::string const & expName = expNames[nn];
		std::string const gotName(ztab[ztab.indexForCosine(zCos)]);
		if (! (gotName == expName))
		{
			oss << "Failure of gotName test" << std::endl;
			oss << dat::infoString(expName, "expName") << std::endl;
			oss << dat::infoString(gotName, "gotName") << std::endl;
		}
	}

	return oss.str();
}


}

//! Unit test for math::CosTable
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << math_CosTable_test0();
	oss << math_CosTable_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
