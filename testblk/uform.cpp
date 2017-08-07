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


//! Check basic operations
std::string
blk_form_test1
	()
{
	std::ostringstream oss;

	// simulate a bunch of relative orientations
	std::map<blk::NodeKey, ga::Rigid> const eoSimMap{ blk::sim::cubeEOs() };
	std::vector<blk::EdgeOri> const ropSims{ blk::sim::allROs(eoSimMap) };

	// exercise formation to assemble into a nominal block structure
	std::map<blk::NodeKey, ga::Rigid> const eoBlkMap
		{ blk::form::viaSpan(ropSims) };

	// extract (all) relative orientations from formed block
	std::vector<blk::EdgeOri> const ropBlks{ blk::sim::allROs(eoBlkMap) };

	// check if block orientations agree with expected simulated ones
	size_t const numSim(ropSims.size());
	size_t const numBlk(ropBlks.size());
	if (! (numSim == numBlk))
	{
		oss << "Failure of block return size" << std::endl;
	}
	else
	{
		for (size_t nn{0u} ; nn < numSim ; ++nn)
		{
			blk::EdgeOri const & ropSim = ropSims[nn];
			blk::EdgeOri const & ropBlk = ropBlks[nn];

			// verify test case generating same ROs
			blk::EdgeKey const & keySim = ropSim.first;
			blk::EdgeKey const & keyBlk = ropBlk.first;
			assert(keySim == keyBlk);

			ga::Rigid const & expOri = ropSim.second;
			ga::Rigid const & gotOri = ropBlk.second;
			if (! gotOri.nearlyEquals(expOri))
			{
				oss << "Failure of recovered RO test" << std::endl;
				oss << dat::infoString(expOri, "expOri") << std::endl;
				oss << dat::infoString(gotOri, "gotOri") << std::endl;
				break;
			}

		}
	}

	constexpr bool showValues{ false };
	if (showValues)
	{
		io::out() << blk::infoString(eoSimMap, "eoSimMap") << std::endl;
		io::out() << std::endl;
		io::out() << blk::infoString(eoBlkMap, "eoBlkMap") << std::endl;
		io::out() << std::endl;

		io::out() << blk::infoString(ropSims, "ropSims") << std::endl;
		io::out() << std::endl;
		io::out() << blk::infoString(ropBlks, "ropBlks") << std::endl;
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

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
