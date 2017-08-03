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

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

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

	//! Individual node orientations for simulated block
	std::vector<ga::Rigid>
	simAbsOris
		()
	{
		std::vector<ga::Rigid> oris;
		oris.reserve(10u);

		using namespace ga;

if (false)
{
		ga::Vector const off( 10., 0., 0. );
		constexpr double qtr{ math::qtrTurn };

oris.emplace_back(Rigid(off +e1   , Pose(BiVector(0., 0., qtr))));
oris.emplace_back(Rigid(off +e1+e2, Pose(BiVector(0., 0., 2.*qtr))));
oris.emplace_back(Rigid(off    +e2, Pose(BiVector(0., 0., 3.*qtr))));
oris.emplace_back(Rigid(off       , Pose(BiVector(0., 0., .0))));

	std::vector<ga::Rigid> const eosIn1(oris);;
	std::vector<ga::Rigid> eosIn2;
	std::vector<ga::Rigid> eosIn3;

	{
	io::out() << "== Transform: A" << std::endl;
	size_t const ndxFit{ 0u };
	ga::Rigid const & oriFitWrtChk = ga::Rigid::identity();
	eosIn2 = blk::fitOnto(eosIn1, ndxFit, oriFitWrtChk);
	}

	{
	io::out() << "== Transform: B" << std::endl;
	size_t const ndxFit{ 2u };
	ga::Rigid const & oriFitWrtChk = eosIn1[ndxFit];
	eosIn3 = blk::fitOnto(eosIn2, ndxFit, oriFitWrtChk);
	}

	io::out() << "== Results" << std::endl;
	io::out() << blk::infoString(eosIn1, "eosIn1") << std::endl;
	io::out() << std::endl;
	io::out() << blk::infoString(eosIn2, "eosIn2") << std::endl;
	io::out() << std::endl;
	io::out() << blk::infoString(eosIn3, "eosIn3") << std::endl;
	io::out() << std::endl;

exit(8);
}

		ga::Vector const off( 10., 20., 20. );
		oris.emplace_back(Rigid(off-e1, Pose(BiVector(0., 5., 3.))));
		oris.emplace_back(Rigid(off+e1, Pose(BiVector(1., 4., 6.))));
		oris.emplace_back(Rigid(off-e2, Pose(BiVector(2., 3., 9.))));
		oris.emplace_back(Rigid(off+e2, Pose(BiVector(3., 2., 2.))));
		oris.emplace_back(Rigid(off-e3, Pose(BiVector(4., 1., 5.))));
		oris.emplace_back(Rigid(off+e3, Pose(BiVector(5., 0., 8.))));

		// a simple cube
		return oris;
	}

	//! Pairwise orientations within simulated block
	std::vector<blk::OriPair>
	simRelOris
		( std::vector<ga::Rigid> const & eos
		)
	{
		std::vector<blk::OriPair> rops;

		size_t const numNodes{ eos.size() };
		assert(1u < numNodes);
		rops.reserve(numNodes*(numNodes - 1u));

		for (blk::NodeNdx ndxI{0u} ; ndxI < numNodes ; ++ndxI)
		{
			ga::Rigid const & oriIwX = eos[ndxI];
			ga::Rigid const oriXwI{ oriIwX.inverse() };
			constexpr size_t maxBand{ 1u };
			size_t numBand{ 0u };
			for (blk::NodeNdx ndxJ{ndxI+1u} ; ndxJ < numNodes ; ++ndxJ)
			{
				ga::Rigid const & oriJwX = eos[ndxJ];

				size_t const keyI{ 10u + ndxI };
				size_t const keyJ{ 10u + ndxJ };

				ga::Rigid const oriJwI{ oriJwX * oriXwI };
				if (0u == ((keyI+keyJ)%2u))
				{
					blk::OriPair const rop(10+keyI, 10+keyJ, oriJwI);
					rops.emplace_back(rop);
				}
				else
				{
					blk::OriPair const rop(10+keyJ, 10+keyI, oriJwI.inverse());
					rops.emplace_back(rop);
				}

				if (maxBand < ++numBand)
				{
					break;
				}
			}
		}

		return rops;
	}

//! Check basic operations
std::string
blk_form_test1
	()
{
	std::ostringstream oss;

	// simulate a bunch of relative orientations
	std::vector<ga::Rigid> const eosInSim{ simAbsOris() };
	std::vector<blk::OriPair> const ropSims{ simRelOris(eosInSim) };

	// assemble into a nominal block structure
	std::vector<ga::Rigid> const eosInBlk{ blk::form::viaSpan(ropSims) };

	// extract (all) relative orientations from formed block
	std::vector<blk::OriPair> const ropBlks{ simRelOris(eosInSim) };

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
			blk::OriPair const & ropSim = ropSims[nn];
			blk::OriPair const & ropBlk = ropBlks[nn];
			// verify test case generating same ROs
			assert(dat::nearlyEquals(ropBlk.ndxI(), ropSim.ndxI()));
			assert(dat::nearlyEquals(ropBlk.ndxJ(), ropSim.ndxJ()));

			ga::Rigid const & expOri = ropSim.theOriJwI;
			ga::Rigid const & gotOri = ropBlk.theOriJwI;
			if (! gotOri.nearlyEquals(expOri))
			{
				oss << "Failure of recovered RO test" << std::endl;
				oss << dat::infoString(expOri, "expOri") << std::endl;
				oss << dat::infoString(gotOri, "gotOri") << std::endl;
				break;
			}

		}
	}

	/*
	io::out() << blk::infoString(ropSims, "ropSims") << std::endl;
	io::out() << std::endl;
	io::out() << blk::infoString(ropBlks, "ropBlks") << std::endl;
	io::out() << std::endl;
	*/

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
