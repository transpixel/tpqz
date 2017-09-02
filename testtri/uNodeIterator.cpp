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

	// determine acceptable iterator
	std::set<std::pair<long, long> > expMuNus;
	{
		dat::Area<double> const mnRange{ trigeo.mnAreaForXY(xyDomain) };
		long const muBeg{ (long)std::floor(mnRange[0].min() / da) };
		long const muEnd{ (long)std::ceil(mnRange[0].max() / da) };
		long const nuBeg{ (long)std::floor(mnRange[1].min() / db) };
		long const nuEnd{ (long)std::ceil(mnRange[1].max() / db) };
		for (long ndxNu{nuBeg} ; ndxNu <= nuEnd ; ++ndxNu)
		{
			for (long ndxMu{muBeg} ; ndxMu <= muEnd ; ++ndxMu)
			{
				dat::Spot const mnLoc
					(trigeo.mnLocForNode({ ndxMu, ndxNu }));
				dat::Spot const xyLoc(trigeo.xyLocForMuNu(mnLoc));
				if (xyDomain.contains(xyLoc))
				{
					expMuNus.insert(std::make_pair(ndxMu, ndxNu));
				}
			}
		}
	}
	assert(! expMuNus.empty());

	std::set<std::pair<long, long> > gotMuNus;
	for (tri::NodeIterator iter(trigeo, xyDomain) ; iter ; ++iter)
	{
		gotMuNus.insert(*iter);
	}

	/*
	std::ofstream ofsExp("nodeIter_exp.dat");
	for (std::pair<long, long> const & expMuNu : expMuNus)
	{
		ofsExp << dat::infoString(expMuNu, "expMuNu") << std::endl;
	}
	std::ofstream ofsGot("nodeIter_got.dat");
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

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
