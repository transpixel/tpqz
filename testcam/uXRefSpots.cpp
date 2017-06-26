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
\brief  This file contains unit test for cam::XRefSpots
*/

#include "libcam/XRefSpots.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
cam_XRefSpots_test0
	()
{
	std::ostringstream oss;
	cam::XRefSpots const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check basic operations
std::string
cam_XRefSpots_test1
	()
{
	std::ostringstream oss;

	constexpr size_t numPnts{ 10u };
	constexpr size_t numAcqs{ 16u };
	cam::XRefSpots spotTab(numPnts, numAcqs);

	if (! (numPnts == spotTab.pntCapacity()))
	{
		oss << "Failure of pointCapacity() test" << std::endl;
	}
	if (! (numAcqs == spotTab.acqCapacity()))
	{
		oss << "Failure of acquisitionCapacity() test" << std::endl;
	}

	dat::Spot const expSpot{{ -3.2, 1.6 }};
	cam::PntNdx const pntNdx{ 7u };
	cam::AcqNdx const acqNdx{ 9u };
	spotTab(pntNdx, acqNdx) = expSpot;
	dat::Spot const & gotSpot = spotTab(pntNdx, acqNdx);
	if (! dat::nearlyEquals(gotSpot, expSpot))
	{
		oss << "Failure of simple set/get test" << std::endl;
	}

	constexpr size_t expNumAcqs{ 1u };
	constexpr size_t expNumPnts{ 1u };
	size_t const gotNumAcqs{ spotTab.numAcqsFor(pntNdx) };
	size_t const gotNumPnts{ spotTab.numPntsFor(acqNdx) };
	if (! dat::nearlyEquals(gotNumAcqs, expNumAcqs))
	{
		oss << "Failure of NumAcqs test" << std::endl;
		oss << dat::infoString(expNumAcqs, "expNumAcqs") << std::endl;
		oss << dat::infoString(gotNumAcqs, "gotNumAcqs") << std::endl;
	}
	if (! dat::nearlyEquals(gotNumPnts, expNumPnts))
	{
		oss << "Failure of NumPnts test" << std::endl;
		oss << dat::infoString(expNumPnts, "expNumPnts") << std::endl;
		oss << dat::infoString(gotNumPnts, "gotNumPnts") << std::endl;
	}

	std::vector<cam::PntNdx> const pntNdxs{ spotTab.pntIndicesFor(acqNdx) };
	std::vector<cam::PntNdx>::const_iterator const itPntFind
		(std::find(pntNdxs.begin(), pntNdxs.end(), pntNdx));
	if (! (pntNdxs.end() != itPntFind))
	{
		oss << "Failure of pntNdx find test" << std::endl;
	}
	std::vector<cam::AcqNdx> const acqNdxs{ spotTab.acqIndicesFor(pntNdx) };
	std::vector<cam::AcqNdx>::const_iterator const itAcqFind
		(std::find(acqNdxs.begin(), acqNdxs.end(), acqNdx));
	if (! (acqNdxs.end() != itAcqFind))
	{
		oss << "Failure of acqNdx find test" << std::endl;
	}

	return oss.str();
}

	//! Check if spots is the correct size
	void
	checkSize
		( std::ostream & oss
		, std::vector<dat::Spot> const & spots
		, size_t const & expSize
		, std::string const & tname
		)
	{
		size_t const gotSize{ spots.size() };
		if (! dat::nearlyEquals(gotSize, expSize))
		{
			oss << "Failure of valid size test: " << tname << std::endl;
			oss << dat::infoString(expSize, "expSize") << std::endl;
			oss << dat::infoString(gotSize, "gotSize") << std::endl;
		}
	}

//! Check iterator
std::string
cam_XRefSpots_test2
	()
{
	std::ostringstream oss;

	cam::XRefSpots const null{};
	size_t const expCountNull{ 0u };
	size_t gotCountNull{ 0u };
	for (cam::XRefSpots::const_iterator iter(null.begin()) ; iter ; ++iter)
	{
		++gotCountNull;
	}
	if (! dat::nearlyEquals(gotCountNull, expCountNull))
	{
		oss << "Failure of countNull iteration test" << std::endl;
	}

	constexpr size_t numPnts{ 24u };
	constexpr size_t numAcqs{ 30u };
	cam::XRefSpots spots(numPnts, numAcqs);
	spots( 1u,  3u) = dat::Spot{{ 1.,  3. }};
	spots( 1u,  5u) = dat::Spot{{ 1.,  5. }};
	spots( 1u, 17u) = dat::Spot{{ 1., 17. }};
	spots(11u, 13u) = dat::Spot{{11., 13. }};
	spots(11u, 17u) = dat::Spot{{11., 17. }};
	spots(11u, 19u) = dat::Spot{{11., 19. }};
	spots(23u, 29u) = dat::Spot{{23., 29. }};
	size_t const expCountSome{ 7u };

	// check retrieval by pntNdx
	std::vector<dat::Spot> const spotsPnt1(spots.validItemsForPnt(1u));
	std::vector<dat::Spot> const spotsPnt2(spots.validItemsForPnt(2u));
	checkSize(oss, spotsPnt1, 3u, "pnt1");
	checkSize(oss, spotsPnt2, 0u, "pnt2");
	// check retrieval by acqNdx
	std::vector<dat::Spot> const spotsAcq17(spots.validItemsForAcq(17u));
	std::vector<dat::Spot> const spotsAcq18(spots.validItemsForAcq(18u));
	checkSize(oss, spotsAcq17, 2u, "acq17");
	checkSize(oss, spotsAcq18, 0u, "acq18");

	size_t gotCountSome{ 0u };
	size_t badMea{ 0u };
	size_t badPnt{ 0u };
	size_t badAcq{ 0u };
	for (cam::XRefSpots::const_iterator iter(spots.begin()) ; iter ; ++iter)
	{
		dat::Spot const & gotSpot = *iter;
		cam::SpotItem const gotMea(iter.paItem());
		if (! dat::nearlyEquals(gotSpot, gotMea.theItem))
		{
			++badMea;
		}
		if (! dat::nearlyEquals((size_t)(gotSpot[0]), gotMea.thePntNdx))
		{
			++badPnt;
		}
		if (! dat::nearlyEquals((size_t)(gotSpot[1]), gotMea.theAcqNdx))
		{
			++badAcq;
		}
		++gotCountSome;
	}
	if (0u < badMea)
	{
		oss << "Failure of data return test: badMea" << std::endl;
	}
	if (0u < badPnt)
	{
		oss << "Failure of data return test: badPnt" << std::endl;
	}
	if (0u < badAcq)
	{
		oss << "Failure of data return test: badAcq" << std::endl;
	}
	if (! dat::nearlyEquals(gotCountSome, expCountSome))
	{
		oss << "Failure of countSome iteration test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for cam::XRefSpots
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << cam_XRefSpots_test0();
	oss << cam_XRefSpots_test1();
	oss << cam_XRefSpots_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
