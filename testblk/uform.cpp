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
\brief  This file contains unit test for blk::form
*/


#include "libblk/form.h"
#include "libblk/info.h"
#include "libblk/sim.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libio/string.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace
{

//! Check for common functions
std::string
blk_form_test0
	()
{
	std::ostringstream oss;
	/*
	blk::form const aNull;
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

	void
	checkEoMaps
		( std::ostream & oss
		, std::map<blk::NodeKey, ga::Rigid> const & expEoMap
		, std::map<blk::NodeKey, ga::Rigid> const & gotEoMap
		, std::string const & tname
		)
	{
		if (! (expEoMap.size() == gotEoMap.size()))
		{
			oss << "Failure of size for test = " << tname << std::endl;
		}
		else
		{
			for (std::map<blk::NodeKey, ga::Rigid>::const_iterator
				itExp{expEoMap.begin()} ; expEoMap.end() != itExp ; ++itExp)
			{
				std::map<blk::NodeKey, ga::Rigid>::const_iterator
					const itGot{ gotEoMap.find(itExp->first) };
				if (gotEoMap.end() == itGot)
				{
					oss << "Failure to find got entry for exp: test = "
						<< tname << std::endl;
				}
				else
				{
					ga::Rigid const & expEo = itExp->second;
					ga::Rigid const & gotEo = itGot->second;
					if (! gotEo.nearlyEquals(expEo))
					{
						oss << "Failure of EO value test = " << tname << '\n';
						oss << expEo.infoStringShort("exp") << std::endl;
						oss << gotEo.infoStringShort("got") << std::endl;
						break;
					}
				}
			}
		}
	}

//! Check basic operations
std::string
blk_form_test1
	()
{
	std::ostringstream oss;

	// simulate a bunch of relative orientations
	std::map<blk::NodeKey, ga::Rigid> const eoSimMap{ blk::sim::cubeEOs() };
	std::vector<blk::EdgeOri> const meaEdges{ blk::sim::bandedROs(eoSimMap) };
	assert(! eoSimMap.empty());
	assert(! meaEdges.empty());

	// exercise formation to assemble into a nominal block structure
	std::map<blk::NodeKey, ga::Rigid> const eoBlkMap
		{ blk::form::viaSpan(meaEdges) };

	// extract (all) relative orientations for comparison
	std::vector<blk::EdgeOri> const expEdges{ blk::sim::allROs(eoSimMap) };
	std::vector<blk::EdgeOri> const gotEdges{ blk::sim::allROs(eoBlkMap) };

	// check if block orientations agree with expected simulated ones
	size_t const numSim(expEdges.size());
	size_t const numBlk(gotEdges.size());
	if (! (numSim == numBlk))
	{
		oss << "Failure of block return size" << std::endl;
	}
	else
	{
		for (size_t nn{0u} ; nn < numSim ; ++nn)
		{
			blk::EdgeOri const & expEdge = expEdges[nn];
			blk::EdgeOri const & gotEdge = gotEdges[nn];

			// verify test case generating same ROs
			blk::EdgeKey const & keySim = expEdge.first;
			blk::EdgeKey const & keyBlk = gotEdge.first;
			assert(keySim == keyBlk);

			ga::Rigid const & expOri = expEdge.second;
			ga::Rigid const & gotOri = gotEdge.second;
			if (! gotOri.nearlyEquals(expOri))
			{
				oss << "Failure of recovered RO test" << std::endl;
				oss << dat::infoString(expOri, "expOri") << std::endl;
				oss << dat::infoString(gotOri, "gotOri") << std::endl;
				break;
			}

		}
	}

	//! Compare individual orientations in absolute frame
	std::map<blk::NodeKey, ga::Rigid> const & expEOs = eoSimMap;
	ga::Rigid const & oriInExp = expEOs.begin()->second;
	ga::Rigid const & oriInGot = eoBlkMap.begin()->second;
	std::map<blk::NodeKey, ga::Rigid> const gotEOs
		{ blk::transformed(eoBlkMap, oriInExp, oriInGot) };

	checkEoMaps(oss, expEOs, gotEOs, "test1");

	constexpr bool showValues{ false };
	if (showValues)
	{
		io::out() << blk::infoString(eoSimMap, "eoSimMap") << std::endl;
		io::out() << std::endl;
		io::out() << blk::infoString(eoBlkMap, "eoBlkMap") << std::endl;
		io::out() << std::endl;

		io::out() << blk::infoString(meaEdges, "meaEdges") << std::endl;
		io::out() << std::endl;
		io::out() << blk::infoString(expEdges, "expEdges") << std::endl;
		io::out() << std::endl;
		io::out() << blk::infoString(gotEdges, "gotEdges") << std::endl;
		io::out() << std::endl;

		io::out() << blk::infoString(expEOs, "expEOs") << std::endl;
		io::out() << std::endl;
		io::out() << blk::infoString(gotEOs, "gotEOs") << std::endl;
		io::out() << std::endl;
	}

	return oss.str();
}

	//! Simple orientation
	inline
	ga::Rigid
	simOri
		( double const & relOffset
		, double const & relAngle
		)
	{
		return blk::sim::oriComps
			(1000., 100., 10.+relOffset , 0., 0., relAngle);
	}

//! Check absolute frame restoration
std::string
blk_form_test2
	()
{
	std::ostringstream oss;

	// simulate a bunch of relative orientations
	constexpr double qtr{ math::qtrPi };
	std::map<blk::NodeKey, ga::Rigid> expEoMap
		{ { 91u, simOri(1.,  .0) }
		, { 92u, simOri(2., 1.*qtr) }
		, { 93u, simOri(3., 2.*qtr) }
		, { 94u, simOri(3.,  18.) }
		, { 95u, simOri(4., -19.) }
		, { 96u, blk::sim::oriComps(17., 19., 23. , .4, .5, .3) }
		, { 97u, blk::sim::oriComps(29., 31., 37. , -.5, .3, .7) }
		};

	std::vector<blk::EdgeOri> const meaEdges{ blk::sim::allROs(expEoMap) };
	assert(! expEoMap.empty());
	assert(! meaEdges.empty());

	// exercise formation to assemble into a nominal block structure
	std::map<blk::NodeKey, ga::Rigid> const blkEoMap
		{ blk::form::viaSpan(meaEdges) };
	std::vector<blk::EdgeOri> const blkEdges{ blk::sim::allROs(blkEoMap) };

	// reorient now-formed block to match initial simulation
	std::map<blk::NodeKey, ga::Rigid> const gotEoMap
		{ blk::transformed
			(blkEoMap, expEoMap.begin()->second, blkEoMap.begin()->second)
		};
	std::vector<blk::EdgeOri> const gotEdges{ blk::sim::allROs(gotEoMap) };

	// check that final EO's match sim EO's
	checkEoMaps(oss, expEoMap, gotEoMap, "test2");

	constexpr bool showValues{ false };
	if (showValues)
	{
		io::out() << std::endl;
		io::out() << blk::infoString(expEoMap, "expEoMap") << std::endl;
	//	io::out() << blk::infoString(meaEdges, "meaEdges") << std::endl;

		io::out() << std::endl;
		io::out() << blk::infoString(blkEoMap, "blkEoMap") << std::endl;
	//	io::out() << blk::infoString(blkEdges, "blkEdges") << std::endl;

		io::out() << std::endl;
		io::out() << blk::infoString(gotEoMap, "gotEoMap") << std::endl;
	//	io::out() << blk::infoString(gotEdges, "gotEdges") << std::endl;

		io::out() << std::endl;
	}

	return oss.str();
}


}

//! Unit test for blk::form
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << blk_form_test0();
	oss << blk_form_test1();
	oss << blk_form_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
