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
\brief  This file contains unit test for cam::io
*/


#include "libcam/io.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
cam_io_test0
	()
{
	std::ostringstream oss;
	/*
	cam::io const aNull(dat::nullValue<cam::io>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

	bool
	areSame
		( dat::Spot const & spotA
		, dat::Spot const & spotB
		)
	{
		return
			(  dat::areSame(spotA[0], spotB[0])
			&& dat::areSame(spotA[1], spotB[1])
			);
	}

//! Check basic save/load functions
std::string
cam_io_test1
	()
{
	std::ostringstream oss;

	// create table of (nonsense) image measurements
	constexpr size_t const numPnts{ 20u };
	constexpr size_t const numAcqs{ 10u };
	cam::XRefSpots expSpotTab(numPnts, numAcqs);
	constexpr dat::Spot const nullSpot{ dat::nullValue<dat::Spot>() };
	std::fill(expSpotTab.beginTable(), expSpotTab.endTable(), nullSpot);

	constexpr cam::PntNdx const pntNdxA{  0u };
	constexpr cam::PntNdx const pntNdxB{  3u };
	constexpr cam::PntNdx const pntNdxC{  7u };
	constexpr cam::PntNdx const pntNdxD{ 13u };
	constexpr cam::PntNdx const pntNdxE{ 14u };
	std::map<cam::PntName, cam::PntNdx> const pntNameNdxMap
		{ { cam::io::pntNameForNdx(pntNdxA), pntNdxA }
		, { cam::io::pntNameForNdx(pntNdxB), pntNdxB }
		, { cam::io::pntNameForNdx(pntNdxC), pntNdxC }
		, { cam::io::pntNameForNdx(pntNdxD), pntNdxD }
		, { cam::io::pntNameForNdx(pntNdxE), pntNdxE }
		};

	std::array<cam::AcqNdx, 3u> const acqNdxs{{ 1u, 5u, 8u }};
	constexpr dat::Spot const goodSpot{{ 100., 200. }};

	expSpotTab(pntNdxA, acqNdxs[0]) = goodSpot;
	expSpotTab(pntNdxB, acqNdxs[0]) = goodSpot;
	expSpotTab(pntNdxC, acqNdxs[0]) = goodSpot;

	expSpotTab(pntNdxB, acqNdxs[1]) = goodSpot;
	expSpotTab(pntNdxC, acqNdxs[1]) = goodSpot;
	expSpotTab(pntNdxD, acqNdxs[1]) = goodSpot;

	expSpotTab(pntNdxC, acqNdxs[2]) = goodSpot;
	expSpotTab(pntNdxD, acqNdxs[2]) = goodSpot;
	expSpotTab(pntNdxE, acqNdxs[2]) = goodSpot;

	cam::XRefSpots gotSpotTab(numPnts, numAcqs);
	for (size_t nn{0u} ; nn < acqNdxs.size() ; ++nn)
	{
		cam::AcqNdx const & acqNdx = acqNdxs[nn];
		size_t const expSize{ expSpotTab.numPntsFor(acqNdx) };

		// write acquisition measurements to stream
		std::ostringstream ossTab;
		cam::io::saveToAsciiTrifecta(expSpotTab, ossTab, acqNdx);

		// load stream and check reader
		std::istringstream issTab(ossTab.str());
		cam::MeaGroupOneAcq const meaInfos
			{ cam::io::loadFromAsciiTrifecta(issTab) };
		size_t const gotSize{ meaInfos.size() };
		if (! dat::nearlyEquals(gotSize, expSize))
		{
			oss << "Failure of load size test" << std::endl;
		}

		// update table
		bool const okay
			{ cam::io::insertIntoTable
				(&gotSpotTab, meaInfos, acqNdx, pntNameNdxMap)
			};
		if (! okay)
		{
			oss << "Failure of table insert status test" << std::endl;
		}
	}

	// test overall restored table
	bool const same
		{ std::equal
			( gotSpotTab.beginTable(), gotSpotTab.endTable()
			, expSpotTab.beginTable()
			, areSame
			)
		};
	if (! same)
	{
		oss << "Failure of readback spot table test" << std::endl;
		oss << "=====" << std::endl;
		oss << expSpotTab.infoStringAcqMajor("expSpotTab") << std::endl;
		oss << "=====" << std::endl;
		oss << gotSpotTab.infoStringAcqMajor("gotSpotTab") << std::endl;
		oss << "=====" << std::endl;

	}

	return oss.str();
}


}

//! Unit test for cam::io
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << cam_io_test0();
	oss << cam_io_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
