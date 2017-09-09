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
\brief  This file contains unit test for tri::NodeIterator
*/


#include "libtri/NodeIterator.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/sprintf.h"
#include "libio/stream.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
tri_NodeIterator_test0
	()
{
	std::ostringstream oss;
	/*
	tri::NodeIterator const aNull;
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
	//	oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

//! Check basic iterator operations
std::string
tri_NodeIterator_test1
	()
{
	std::ostringstream oss;

	dat::Range<double> xRange{ -10., 10. };
	dat::Range<double> yRange{ -20., 20. };
	dat::Area<double> xyBounds{ xRange, yRange };
	tri::Domain const xyDomain{ xyBounds };

	constexpr std::array<double, 2u> aDir{{ 0., 1. }};
	constexpr double da{ 5. };
	constexpr double db{ 7. };
	tri::IsoGeo const trigeo( da, db, aDir);

	// determine acceptable nodes
	std::set<std::pair<long, long> > expMuNus;
	{
		dat::Area<double> const mnRange{ trigeo.tileAreaForDomain(xyDomain) };
		long const muBeg{ (long)std::floor(mnRange[0].min() / da) };
		long const muEnd{ (long)std::ceil(mnRange[0].max() / da) };
		long const nuBeg{ (long)std::floor(mnRange[1].min() / db) };
		long const nuEnd{ (long)std::ceil(mnRange[1].max() / db) };
		for (long ndxJ{nuBeg} ; ndxJ <= nuEnd ; ++ndxJ)
		{
			for (long ndxI{muBeg} ; ndxI <= muEnd ; ++ndxI)
			{
				tri::NodeNdxPair const ndxIJ{ ndxI, ndxJ };
				tri::IsoGeo::QuantPair const fracPair
					(trigeo.fracPairForIndices(ndxIJ));
				dat::Spot const xyLoc(trigeo.refSpotForFracPair(fracPair));
				if (xyDomain.contains(xyLoc))
				{
					expMuNus.insert(std::make_pair(ndxI, ndxJ));
				}
			}
		}
	}
	assert(! expMuNus.empty());

	// get nodes from iterator
	std::set<std::pair<long, long> > gotMuNus;
	for (tri::NodeIterator iter(trigeo, xyDomain) ; iter ; ++iter)
	{
		gotMuNus.insert(iter.indexPair());
	}

	/*
	std::ofstream ofsExp("test_exp.dat");
	for (std::pair<long, long> const & expMuNu : expMuNus)
	{
		ofsExp << dat::infoString(expMuNu, "expMuNu") << std::endl;
	}
	std::ofstream ofsGot("test_got.dat");
	for (std::pair<long, long> const & gotMuNu : gotMuNus)
	{
		ofsGot << dat::infoString(gotMuNu, "gotMuNu") << std::endl;
	}
	*/

	// check sizes
	if (gotMuNus.size() != expMuNus.size())
	{
		oss << "Failure of size test" << std::endl;
		oss << dat::infoString(expMuNus.size(), "expSize") << std::endl;
		oss << dat::infoString(gotMuNus.size(), "gotSize") << std::endl;
	}

	// check for same contents
	std::set<std::pair<long, long> > difMuNus;
	std::set_difference
		( gotMuNus.begin(), gotMuNus.end()
		, expMuNus.begin(), expMuNus.end()
		, std::inserter(difMuNus, difMuNus.end())
		);
	if (! difMuNus.empty())
	{
		oss << "Failure of zero-differnce test" << std::endl;
		for (std::pair<long, long> const & difMuNu : difMuNus)
		{
			oss << dat::infoString(difMuNu, "difMuNu") << std::endl;
		}
	}

	return oss.str();
}

	//! Find all nodes within radius via brute force evaluation
	std::set<tri::NodeNdxPair>
	nodePairsInRadius
		( tri::IsoGeo const & trigeo
		, tri::Domain const & domain
		, dat::Spot const & center
		, double const & radius
		)
	{
		std::set<tri::NodeNdxPair> nodesIn;
		for (tri::NodeIterator iter(trigeo, domain) ; iter ; ++iter)
		{
			tri::NodeNdxPair const ndxIJ{ iter.indexPair() };
			dat::Spot const refIJ(trigeo.refSpotForIndices(ndxIJ));
			if (domain.contains(refIJ))
			{
				using dat::operator-;
				double const dist{ dat::magnitude(refIJ - center) };
				if (dist <= radius)
				{
					nodesIn.insert(ndxIJ);
				}
			}
		}
		return nodesIn;
	}

//! Check proximity methods
std::string
tri_NodeIterator_test2
	()
{
	std::ostringstream oss;

	// small domain area
//#define EasyData
#	if defined(EasyData)
	dat::Range<double> const xRange(- 5., 15.);
	dat::Range<double> const yRange(- 7., 13.);
	std::array<double, 2u> const refPnt{{ 10., -2. }};
	double const refDist{ 2.5 };
	std::array<double, 2u> const adir{{ 1., 0. }};
	double const deltaA{ .1 };
	double const deltaB{ .1 };
#	else
	dat::Range<double> const xRange(-11., -7.);
	dat::Range<double> const yRange(3., 5.);
	std::array<double, 2u> const refPnt{{ -9., 4.125 }};
	std::array<double, 2u> const adir{{ .4, .3 }};
	double const refDist{ 1.5 };
	double const deltaA{ .25 };
	double const deltaB{ .40 };
#	endif

	tri::IsoGeo const trigeo(deltaA, deltaB, adir);
	dat::Area<double> const refArea{ xRange, yRange };
	tri::Domain const domain(refArea);
	assert(domain.contains(refPnt));

	tri::NodeNdxPair const ndxAt{ trigeo.indicesForRefSpot(refPnt) };
	dat::Spot const refNodeLoc(trigeo.refSpotForIndices(ndxAt));
	assert(domain.contains(refNodeLoc));

	//
	// check all encompassing case
	//

	{
		constexpr double wayBig{ 1.e6 };

		// determine nodes via brute force evaluation
		std::set<tri::NodeNdxPair> const expNdxPairs
			{ nodePairsInRadius(trigeo, domain, refNodeLoc, wayBig) };

		// request "near" nodes such that all are included
		tri::NodeIterator const itBeg(trigeo, domain);

		// TODO move nodesNear to IsoGeo
		std::vector<tri::NodeIterator::DistNode> const gotDistNodes
			{ itBeg.nodesNearTo(ndxAt, wayBig) };

		/*
		std::ofstream ofsExp("test_exp.dat");
		for (tri::NodeNdxPair const & ndxIJ : expNdxPairs)
		{
		ofsExp << dat::infoString(ndxIJ, "ndxIJ") << std::endl;
		}
		std::ofstream ofsGot("test_got.dat");
		for (tri::NodeIterator::DistNode const & gotDistNode : gotDistNodes)
		{
		ofsGot << dat::infoString(gotDistNode.second, "ndxIJ") << std::endl;
		}
		*/

		assert(! expNdxPairs.empty());
		size_t const expNears{ expNdxPairs.size() - 1L }; // skip 'at'
		size_t const gotNears{ gotDistNodes.size() };
		if (! dat::nearlyEquals(gotNears, expNears))
		{
			oss << "Failure of full coverage test" << std::endl;
			oss << dat::infoString(expNears, "expNears") << std::endl;
			oss << dat::infoString(gotNears, "gotNears") << std::endl;
		}
	}

	//
	// Check typical case
	//

	{
		// determine nodes via brute force evaluation
		std::set<tri::NodeNdxPair> const expNdxPairs
			{ nodePairsInRadius(trigeo, domain, refNodeLoc, refDist) };

		// get neighbor node info
		tri::NodeIterator const itBeg(trigeo, domain);
		std::vector<tri::NodeIterator::DistNode> const gotDistNodes
			{ itBeg.nodesNearTo(ndxAt, refDist) };

		// check that returned data are within neighborhood
		size_t errCnt{ 0u };
		for (tri::NodeIterator::DistNode const & gotDistNode : gotDistNodes)
		{
			double const & gotDist = gotDistNode.first;
			tri::NodeNdxPair const & ndxIJ = gotDistNode.second;
			if (refDist < gotDist)
			{
				oss << "Failure of returned radius test" << std::endl;
				oss << dat::infoString(ndxIJ, "ndxIJ") << std::endl;
				oss << dat::infoString(gotDist, "gotDist") << std::endl;
				oss << dat::infoString(refDist, "refDist") << std::endl;
				++errCnt;
			}

			dat::Spot const gotAt(trigeo.refSpotForIndices(ndxIJ));
			using dat::operator-;
			dat::Spot const refDelta(gotAt - refNodeLoc);
			double const chkDist{ dat::magnitude(refDelta) };
			if (refDist < chkDist)
			{
				oss << "Failure of check radius test" << std::endl;
				oss << dat::infoString(ndxIJ, "ndxIJ") << std::endl;
				std::string const fmt{ "%21.18f" };
				oss << "chkDist: " << io::sprintf(fmt, chkDist) << std::endl;
				oss << "refDist: " << io::sprintf(fmt, refDist) << std::endl;
				++errCnt;
			}
			if (0u < errCnt)
			{
				break;
			}
		}
	}

	return oss.str();
}


}

//! Unit test for tri::NodeIterator
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << tri_NodeIterator_test0();
	oss << tri_NodeIterator_test1();
	oss << tri_NodeIterator_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
