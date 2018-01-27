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
\brief  This file contains unit test for ga::groups
*/


#include "libga/groups.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"
#include "libga/ga.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
ga_groups_test0
	()
{
	std::ostringstream oss;
	/*
	ga::groups const aNull(dat::nullValue<ga::groups>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

#if defined(DoExperiement)
namespace
{
	double
	neighborAngleMagFor
		( std::vector<ga::Spinor>::const_iterator const & itAt
		, std::vector<ga::Spinor> const & spins
		)
	{
		double nMag{ dat::nullValue<double>() };
		double minAngMag{ dat::nullValue<double>() };
		for (std::vector<ga::Spinor>::const_iterator
			iter{spins.begin()} ; spins.end() != iter ; ++iter)
		{
			if (itAt != iter)
			{
				ga::Spinor const & spinAtWrtRef = *itAt;
				ga::Spinor const & spinNeWrtRef = *iter;
				ga::Spinor const spinRefWrtAt{ spinAtWrtRef.reverse() };
				ga::Spinor const spinNeWrtAt{ spinNeWrtRef * spinRefWrtAt };
				ga::BiVector const angNeWrtAt
					{ ga::spin::physicalAngleFrom(spinNeWrtAt) };
				double const scalarAngle
					{ math::principalAngle(ga::magnitude(angNeWrtAt)) };
				double const angMag{ std::abs(scalarAngle) };
				if ((! dat::isValid(minAngMag)) || (angMag < minAngMag))
				{
					minAngMag = angMag;
				}
				/*
				io::out()
				//	<< " : " << dat::infoString(spinAtWrtRef)
				//	<< " : " << dat::infoString(spinNeWrtRef)
					<< " : " << dat::infoString(spinNeWrtAt)
					<< " : " << dat::infoString(angNeWrtAt)
					<< " : " << dat::infoString(scalarAngle)
					<< " : " << dat::infoString(angMag)
					<< std::endl;
				*/
			}
		}
		if (dat::isValid(minAngMag))
		{
			nMag = minAngMag;
		}
		return nMag;
	}

	std::vector<double>
	neighborAngleMags
		( std::vector<ga::Spinor> const & spins
		)
	{
		std::vector<double> neighMags;
		neighMags.reserve(spins.size());
		for (std::vector<ga::Spinor>::const_iterator
			iter{spins.begin()} ; spins.end() != iter ; ++iter)
		{
			double const neighMag{ neighborAngleMagFor(iter, spins) };
			neighMags.emplace_back(neighMag);
		}
		return neighMags;
	}

	void
	showNeighborAngMags
		( std::vector<ga::Spinor> const & spins
		)
	{
		std::vector<double> const neighAngMags{ neighborAngleMags(spins) };
		for (double const & neighAngMag : neighAngMags)
		{
			io::out() << dat::infoString(neighAngMag, "neighAngMag")
				<< std::endl;
		}
	}

	std::vector<ga::Spinor>
	funkySpinors
		()
	{
		return std::vector<ga::Spinor>
			{ ga::exp(.5 * ga::BiVector(-0., -0.,  0.))
			, ga::exp(.5 * ga::BiVector(-0., -0.,  1.))
			, ga::exp(.5 * ga::BiVector(-0., -0.,  2.))
			, ga::exp(.5 * ga::BiVector(-0., -0.,  3.))
			, ga::exp(.5 * ga::BiVector(-0., -0.,  4.))
			, ga::exp(.5 * ga::BiVector(-0., -0.,  5.))
			, ga::exp(.5 * ga::BiVector(-0., -0.,  6.))
			, ga::exp(.5 * ga::BiVector(-0., -0.,  7.))
			};
	}
}
#endif

//! Check symmetry of groups
std::string
ga_groups_test1
	()
{
	std::ostringstream oss;

#	if defined(DoExperiement)
	// Experiments with spacing inbetween
	io::out() << "cube:" << std::endl;
	showNeighborAngMags(ga::groups::cubeSpinors());
	io::out() << "tetra:" << std::endl;
	showNeighborAngMags(ga::groups::tetraSpinors());
	io::out() << "octa:" << std::endl;
	showNeighborAngMags(ga::groups::octaSpinors());
	io::out() << "spread:" << std::endl;
	showNeighborAngMags(ga::groups::spreadOfSpinors());

	io::out() << "funky:" << std::endl;
	showNeighborAngMags(funkySpinors());
#	endif

	std::vector<ga::Spinor> const spinIdent{ ga::exp(ga::bZero) };
	std::vector<ga::Spinor> const spinCube{ ga::groups::cubeSpinors() };
	std::vector<ga::Spinor> const spinTetra{ ga::groups::tetraSpinors() };
	std::vector<ga::Spinor> const spinOcta{ ga::groups::octaSpinors() };
	std::array<size_t, 5u> expSizes{{ 1u, 3u, 8u, 12u }};
	if ( (! dat::nearlyEquals(expSizes[0], spinIdent.size()))
	  || (! dat::nearlyEquals(expSizes[1], spinCube.size()))
	  || (! dat::nearlyEquals(expSizes[2], spinTetra.size()))
	  || (! dat::nearlyEquals(expSizes[3], spinOcta.size()))
	   )
	{
		oss << "Failure of polyhedra size tests" << std::endl;
	}

	// check that spread contains all contributions
	std::vector<ga::Spinor> const spinSpread{ ga::groups::spreadOfSpinors() };
	size_t const allSize
		{ std::accumulate(expSizes.begin(), expSizes.end(), 0u) };
	if (! dat::nearlyEquals(allSize, spinSpread.size()))
	{
		oss << "Failure of spread size tests" << std::endl;
	}

	// check if all spinors are shortest convention
	for (ga::Spinor const spin : spinSpread)
	{
		if (spin.theS.theValue < 0.)
		{
			oss << "Failure of positive scalar test" << std::endl;
			oss << dat::infoString(spin, "spin") << std::endl;
		}
	}

	return oss.str();
}

//! Check angle extraction
std::string
ga_groups_test2
	()
{
	std::ostringstream oss;

	std::vector<ga::Spinor> const spins{ ga::groups::spreadOfSpinors() };
	std::vector<ga::BiVector> const gotAngels
		{ ga::groups::physicalAnglesFor(spins) };

	// check for shortest rotation convention
	for (ga::BiVector const & gotAngle : gotAngels)
	{
		double const gotMag{ ga::magnitude(gotAngle) };
		if (math::pi < gotMag)
		{
			oss << "Failure of principal magnitude return test" << std::endl;
			oss << dat::infoString(gotAngle, "gotAngle") << std::endl;
			oss << dat::infoString(gotMag, "gotMag") << std::endl;
			break;
		}
	}

	return oss.str();
}


}

//! Unit test for ga::groups
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ga_groups_test0();
	oss << ga_groups_test1();
	oss << ga_groups_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
