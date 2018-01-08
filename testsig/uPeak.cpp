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
\brief  This file contains unit test for sig::Peak
*/


#include "libsig/Peak.h"

#include "libdat/info.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
sig_Peak_test0
	()
{
	std::ostringstream oss;
	sig::Peak const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check degenerate cases
std::string
sig_Peak_test1a
	()
{
	std::ostringstream oss;

	// check stress case - constant data
	dat::grid<double> grid(17u, 19u);
	std::fill(grid.begin(), grid.end(), -3.);
	sig::Peak const gotPeak(sig::Peak::fromGrid(grid));

	// should have zero quality
	if (0. < gotPeak.prominenceRank())
	{
		oss << "Failure of (non)peak for flat data test" << std::endl;
	}

	return oss.str();
}

//! Check finding for a simple, well-defined peak
std::string
sig_Peak_test1b
	()
{
	std::ostringstream oss;

	dat::grid<double> grid(50u, 60u);
	std::fill(grid.begin(), grid.end(), 1.);
	dat::RowCol const expPeakRC{{ 17u, 23u }};
	grid(expPeakRC) = 2.;

	dat::Spot const expPeakSpot
		{{ double(expPeakRC[0]), double(expPeakRC[1]) }};
	sig::Peak const gotPeak(sig::Peak::fromGrid(grid));

	if (! (0. < gotPeak.prominenceRank()))
	{
		oss << "Failure of delta-spike prominenceRank test" << std::endl;
		oss << dat::infoString(gotPeak, "gotPeak") << std::endl;
	}

	dat::Spot const gotPeakSpot(sig::Peak::fitSpotFor(grid, gotPeak));
	if (! dat::nearlyEquals(gotPeakSpot, expPeakSpot))
	{
		oss << "Failure of delta-spike test" << std::endl;
		oss << dat::infoString(expPeakSpot, "expPeakSpot") << std::endl;
		oss << dat::infoString(gotPeakSpot, "gotPeakSpot") << std::endl;
	}

	return oss.str();
}


//! Check peak at edge
std::string
sig_Peak_test2
	()
{
	std::ostringstream oss;

	// generate data
	dat::grid<double> grid(50u, 60u);
	std::iota(grid.begin(), grid.end(), 0.);
	dat::RowCol const expPeakRC{{ grid.high()-1u, grid.wide()-1u }};

	// find initial peak
	constexpr size_t useRadius{ 10u };
	sig::Peak const gotPeak(sig::Peak::fromGrid(grid, useRadius));
	dat::RowCol const & gotPeakRC = gotPeak.theBestRowCol;
	if (! dat::nearlyEquals(gotPeakRC, expPeakRC))
	{
		oss << "Failuire of corner-peak RC test" << std::endl;
		oss << dat::infoString(expPeakRC, "expPeakRC") << std::endl;
		oss << dat::infoString(gotPeakRC, "gotPeakRC") << std::endl;
	}

	dat::Spot const expPeakSpot(dat::cast::Spot(expPeakRC));
	dat::Spot const gotPeakSpot{ sig::Peak::fitSpotFor(grid, gotPeak) };
	double const spotTol{ 15./16. };
	if (! dat::nearlyEquals(gotPeakSpot, expPeakSpot, spotTol))
	{
		oss << "Failuire of corner-peak fit-spot test" << std::endl;
		oss << dat::infoString(expPeakSpot, "expPeakSpot") << std::endl;
		oss << dat::infoString(gotPeakSpot, "gotPeakSpot") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for sig::Peak
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << sig_Peak_test0();
	oss << sig_Peak_test1a();
	oss << sig_Peak_test1b();
	oss << sig_Peak_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
