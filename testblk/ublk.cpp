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
\brief  This file contains unit test for blk::blk
*/


#include "libblk/blk.h"
#include "libblk/info.h"
#include "libblk/sim.h"

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
blk_blk_test0
	()
{
	std::ostringstream oss;
	/*
	blk::blk const aNull(dat::nullValue<blk::blk>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}


//! Check basic operations
std::string
blk_blk_test1
	()
{
	std::ostringstream oss;

	std::vector<ga::Rigid> xWrtSrcs;
	xWrtSrcs.reserve(10u);

	using namespace ga;

	constexpr double qtr{ math::qtrTurn };
	/*
	xWrtSrcs.emplace_back(blk::sim::oriComps(1., 0., 0. , 0., 0., 1.*qtr));
	xWrtSrcs.emplace_back(blk::sim::oriComps(2., 0., 0. , 0., 0., 2.*qtr));
	xWrtSrcs.emplace_back(blk::sim::oriComps(3., 0., 0. , 0., 0., 3.*qtr));
	xWrtSrcs.emplace_back(blk::sim::oriComps(4., 0., 0. , 0., 0., 4.*qtr));
	*/
	xWrtSrcs.emplace_back(blk::sim::oriComps(1., 8., 13. , 1., 2., 1.*qtr));
	xWrtSrcs.emplace_back(blk::sim::oriComps(2., 6., 17. , 3., 3., 2.*qtr));
	xWrtSrcs.emplace_back(blk::sim::oriComps(3., 4., 19. , 5., 4., 3.*qtr));
	xWrtSrcs.emplace_back(blk::sim::oriComps(4., 2., 23. , 7., 5., 4.*qtr));

	// transform all orientations into arbitrary frame
	size_t const ndxA{ 2u };
	ga::Rigid const & oriSrcWrtA = xWrtSrcs[ndxA];
	ga::Rigid const oriTgtWrtA{ ga::Rigid::identity() };
	std::vector<ga::Rigid> const xWrtTmps
		{ blk::transformed(xWrtSrcs, oriTgtWrtA, oriSrcWrtA) };

	size_t const ndxB{ 0u };
	ga::Rigid const & oriSrcWrtB = xWrtTmps[ndxB];
	ga::Rigid const & oriTgtWrtB = xWrtSrcs[ndxB];
	std::vector<ga::Rigid> const xWrtTgt
		{ blk::transformed(xWrtTmps, oriTgtWrtB, oriSrcWrtB) };

	// check that round-trip transformed EO's match source
	std::vector<ga::Rigid> const & expEOs = xWrtSrcs;
	std::vector<ga::Rigid> const & gotEOs = xWrtTgt;
	if (! (expEOs.size() == gotEOs.size()))
	{
		oss << "Failure of size test" << std::endl;
	}
	else
	{
		for (size_t nn{0u} ; nn < expEOs.size() ; ++nn)
		{
			ga::Rigid const & expEO = expEOs[nn];
			ga::Rigid const & gotEO = gotEOs[nn];
			if (! gotEO.nearlyEquals(expEO))
			{
				oss << "Failure of EO compare test" << std::endl;
				oss << dat::infoString(expEO, "expEO") << std::endl;
				oss << dat::infoString(gotEO, "gotEO") << std::endl;
				break;
			}
		}
	}

	constexpr bool showValues{ false };
	if (showValues)
	{
		io::out() << "== Results" << std::endl;
		io::out() << blk::infoString(xWrtSrcs, "xWrtSrcs") << std::endl;
		io::out() << std::endl;
		io::out() << blk::infoString(xWrtTmps, "xWrtTmps") << std::endl;
		io::out() << std::endl;
		io::out() << blk::infoString(xWrtTgt, "xWrtTgt") << std::endl;
		io::out() << std::endl;
	}

	return oss.str();
}


}

//! Unit test for blk::blk
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << blk_blk_test0();
	oss << blk_blk_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
