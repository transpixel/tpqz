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

namespace
{
	//! Interior geometry for particular test case
	struct Interior
	{
		dat::Extents const theDetSize;
		dat::Spot const theCenterRC;
		ga::Vector const theCenterVec;

		Interior
			( dat::Extents const & detSize
			)
			: theDetSize{ detSize }
			, theCenterRC{ dat::centerOf(theDetSize) }
			, theCenterVec{ theCenterRC[0], theCenterRC[1], 0. }
		{ }

		geo::VertGangle
		gangleFor
			( double const & pd
			, std::pair<dat::Spot, dat::Spot> const & meaPair
			) const
		{
			dat::Spot const & meaRowCol1 = meaPair.first;
			dat::Spot const & meaRowCol2 = meaPair.second;
			ga::Vector const vecMea1{ meaRowCol1[0], meaRowCol1[1], 0. };
			ga::Vector const vecMea2{ meaRowCol2[0], meaRowCol2[1], 0. };
			ga::Vector const vecPD{ theCenterVec + pd * ga::e3 };
			return geo::VertGangle{ vecPD, std::make_pair(vecMea1, vecMea2) };
		}

		void
		showData
			( std::pair<dat::Spot, dat::Spot> const & meaPair
			) const
		{
			for (double pd{0.} ; pd < 250. ; pd += 10.)
			{
				geo::VertGangle const gangle{ gangleFor(pd, meaPair) };
				io::out()
					<< " pd: " << dat::infoString(pd)
					<< " gangle: " << dat::infoString(gangle)
					<< std::endl;
			}
		}


	}; // Interior

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

	double const expPD{ 100. };
	double const delta{ expPD };
	using dat::operator+;
	using dat::operator-;
	dat::Spot const meaRowCol1{ rcCenter + dat::Spot{ delta, 500. } };
	dat::Spot const meaRowCol2{ rcCenter - dat::Spot{ delta, 500. } };

	std::pair<dat::Spot, dat::Spot> const meaPair{ meaRowCol1, meaRowCol2 };
	ga::Vector const vecMea1{ meaRowCol1[0], meaRowCol1[1], 0. };
	ga::Vector const vecMea2{ meaRowCol2[0], meaRowCol2[1], 0. };
	ga::Vector const vecPD{ center + expPD * ga::e3 };

	geo::VertGangle const imgGangle{ vecPD, std::make_pair(vecMea1, vecMea2) };
	double const beta{ imgGangle.angleMag() };
	std::vector<double> const gotPDs
		{ cam::fit::principalDistanceFor(meaPair, beta, detSize) };

	// display all solutions
	io::out() << dat::infoString(gotPDs.begin(), gotPDs.end(), "expPD") << '\n';
	io::out() << dat::infoString(beta, "beta") << '\n';
	for (double const & gotPD : gotPDs)
	{
		io::out() << io::sprintf("... %25.18f", gotPD) << std::endl;
	}

	// display expected profile
	Interior const inner(detSize);
	inner.showData(meaPair);

	bool okay{ false };
	double const tol{ dat::diagonalMag(detSize) * math::eps };
	for (double const & gotPD : gotPDs)
	{
		if (dat::nearlyEquals(gotPD, expPD, tol))
		{
			okay = true;
		}
	}

	if (! okay)
	{
		oss << "Failure to recover principal distance test" << std::endl;
		oss << dat::infoString(expPD, "expPD") << std::endl;
		for (double const & gotPD : gotPDs)
		{
			oss << io::sprintf("... %25.18f", gotPD) << std::endl;
		}
		oss << dat::infoString(meaRowCol1, "meaRowCol1") << std::endl;
		oss << dat::infoString(meaRowCol2, "meaRowCol2") << std::endl;
		oss << dat::infoString(beta, "beta") << std::endl;
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
