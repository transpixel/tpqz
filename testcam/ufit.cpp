//
//
// MIT License
//
// Copyright (c) 2020 Stellacore Corporation.
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
\brief  This file contains unit test for cam::fit
*/


#include "libcam/fit.h"

#include "libga/spin.h"
#include "libgeo/VertGangle.h"

#include "libdat/info.h"
#include "libdat/ops.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
cam_fit_test0
	()
{
	std::ostringstream oss;
	/*
	cam::fit const aNull(dat::nullValue<cam::fit>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

//! Check principal distance calibration
std::string
cam_fit_test1
	()
{
	std::ostringstream oss;

	// simulate a detector
	dat::Extents const detSize{ 2000u, 3000u };
	dat::Spot const rcCenter{ dat::centerOf(detSize) };
	ga::Vector const center{ rcCenter[0], rcCenter[1], 0. };

	dat::Spot const meaRowCol1{  500., 2300. };
	dat::Spot const meaRowCol2{ 1500., 1700. };
	double const expPD{ 137. };

	std::pair<dat::Spot, dat::Spot> const meaPair{ meaRowCol1, meaRowCol2 };
	ga::Vector const vecMea1{ meaRowCol1[0], meaRowCol1[1], 0. };
	ga::Vector const vecMea2{ meaRowCol2[0], meaRowCol2[1], 0. };
	ga::Vector const vecPD{ center + expPD * ga::e3 };

	geo::VertGangle const imgGangle{ vecPD, std::make_pair(vecMea1, vecMea2) };
	double const beta{ imgGangle.angleMag() };
	std::vector<double> const gotPDs
		{ cam::fit::principalDistanceFor(meaPair, beta, detSize) };

	bool okay{ false };
	for (double const & gotPD : gotPDs)
	{
		if (dat::nearlyEquals(gotPD, expPD))
		{
			okay = true;
		}
	}

	if (! okay)
	{
		oss << "Failure to recover principal distance test" << std::endl;
		oss << dat::infoString(expPD, "expPD") << std::endl;
		oss << dat::infoString(gotPDs.begin(), gotPDs.end(), "gotPDs")
			<< std::endl;
		oss << dat::infoString(meaRowCol1, "meaRowCol1") << std::endl;
		oss << dat::infoString(meaRowCol2, "meaRowCol2") << std::endl;

	}

	return oss.str();
}


}

//! Unit test for cam::fit
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << cam_fit_test0();
	oss << cam_fit_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
