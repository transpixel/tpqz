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

#include "libcam/InnerVertex.h"
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
	//! String of angle values associated with principal distance sampling
	std::string
	infoProfile
		( cam::InnerVertex const & inner
		, std::pair<dat::Spot, dat::Spot> const & meaPair
		, double const & minPD = 0.
		, double const & maxPD = 250.
		, double const & delPD = 10.
		)
	{
		std::ostringstream oss;
		for (double pd{minPD} ; (! (maxPD < pd)) ; pd += delPD)
		{
			geo::VertGangle const gangle{ inner.gangleFor(pd, meaPair) };
			oss
				<< " pd: " << dat::infoString(pd)
				<< " gangle: " << dat::infoString(gangle)
				<< std::endl;
		}
		return oss.str();
	}


	using MeaPair = std::pair<dat::Spot, dat::Spot>;

	//! Spots covering detector size
	std::vector<dat::Spot>
	detSpots
		( dat::Extents const & detSize
		, size_t const & delta = { 100u }
		)
	{
		std::vector<dat::Spot> spots;
		for (size_t row{0u} ; row < detSize.high() ; row += delta)
		{
			for (size_t col{0u} ; col < detSize.wide() ; col += delta)
			{
				dat::Spot const spot{ (double)row, (double)col };
				spots.emplace_back(spot);
			}
		}
		return spots;
	}

	//! Combinatorial pairing of spots
	std::vector<MeaPair>
	detPairs
		( std::vector<dat::Spot> const & spots
		)
	{
		std::vector<MeaPair> pairs;
		pairs.reserve(math::sq(spots.size()));
		for (dat::Spot const & spot1 : spots)
		{
			for (dat::Spot const & spot2 : spots)
			{
				pairs.emplace_back(std::make_pair(spot1, spot2));
			}
		}
		return pairs;
	}

	//! Vector with first two components set to spot coordinates
	inline
	ga::Vector
	detVecFor
		( dat::Spot const & spot
		)
	{
		return ga::Vector{ spot[0], spot[1], 0. };
	}
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

	double const expPD{ 1000. };

	std::vector<MeaPair> const meaPairs{ detPairs(detSpots(detSize, 123u)) };
	size_t count{ 0u };
	for (MeaPair const & meaPair : meaPairs)
	{
		cam::InnerVertex const inner(detSize); // for test comparisons
		double const tol{ dat::diagonalMag(detSize) * std::sqrt(math::eps) };

		// compute interior angle magnitude for test case
		ga::Vector const vecMea1{ detVecFor(meaPair.first) };
		ga::Vector const vecMea2{ detVecFor(meaPair.second) };
		ga::Vector const vecPD{ center + expPD * ga::e3 };
		geo::VertGangle const imgGangle
			{ vecPD, std::make_pair(vecMea1, vecMea2) };
		double const beta{ imgGangle.angleMag() };

		// fit principal distance to this angle
		std::vector<double> const gotPDs
			{ cam::fit::principalDistanceFor(meaPair, beta, detSize) };

		// if same point, expect no solution
		if (dat::nearlyEquals(meaPair.first, meaPair.second))
		{
			if (! gotPDs.empty())
			{
				oss << "Failure of empty return for zero angle test" << '\n';
			}
		}
		else
		{
			// check that solution(s) reproduce interior angle
			for (double const & gotPD : gotPDs)
			{
				double const expBeta{ inner.angleMag(expPD, meaPair) };
				double const gotBeta{ inner.angleMag(gotPD, meaPair) };
				if (! dat::nearlyEquals(gotBeta, expBeta, tol))
				{
					oss << "Failure of angle beta test" << std::endl;
				}
			}

			// check that at least one of the solutions is correct
			bool okay{ false };
			for (double const & gotPD : gotPDs)
			{
				if (dat::nearlyEquals(gotPD, expPD, tol))
				{
					okay = true;
				}
			}
			if (! okay)
			{
				oss << "Failure to recover principal distance test" << '\n';
			}
		}

		// if error encountered, display diagnostic info
		if (! oss.str().empty())
		{
			oss << dat::infoString(expPD, "expPD") << std::endl;
			oss << dat::infoString(vecMea1, "vecMea1") << std::endl;
			oss << dat::infoString(vecMea2, "vecMea2") << std::endl;
			oss << dat::infoString(beta, "beta") << std::endl;

			// display angle info for solution(s)
			double const expBeta{ inner.angleMag(expPD, meaPair) };
			if (gotPDs.empty())
			{
				oss << "<empty solutions>" << std::endl;
			}
			for (double const & gotPD : gotPDs)
			{
				double const gotBeta{ inner.angleMag(gotPD, meaPair) };
				double const difBeta{ gotBeta - expBeta };
				double const difPD{ gotPD - expPD };
				oss
					<< "...   expPD: " << io::sprintf("%12.6f", expPD)
					<< "   gotPD: " << io::sprintf("%12.6f", gotPD)
					<< "   difPD: " << io::sprintf("%12.5e", difPD)
					<< "   tol: " << io::sprintf("%12.5e", tol)
					<< std::endl;
				oss
					<< "... expBeta: " << io::sprintf("%12.6f", expBeta)
					<< " gotBeta: " << io::sprintf("%12.6f", gotBeta)
					<< " difBeta: " << io::sprintf("%12.5e", difBeta)
					<< "   tol: " << io::sprintf("%12.5e", tol)
					<< std::endl;
			}

			// display expected profile
			oss << infoProfile(inner, meaPair, 0., 2000., 100.) << std::endl;

			break;
		}
		else
		{
			++count;
		}
	}

	if (count < 100u)
	{
		oss << "Failure to consider enough cases: count = " << count << '\n';
	}
	// io::out() << count << std::endl;

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
