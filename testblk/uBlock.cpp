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
\brief  This file contains unit test for blk::Block
*/


#include "libblk/Block.h"

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
blk_Block_test0
	()
{
	std::ostringstream oss;
	blk::Block const aNull;
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
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
		oris.emplace_back(Rigid(vZero, Pose::identity()));
		oris.emplace_back(Rigid( -e1, Pose(BiVector(0., 5., 3.))));
		oris.emplace_back(Rigid(  e1, Pose(BiVector(1., 4., 6.))));
		oris.emplace_back(Rigid( -e2, Pose(BiVector(2., 3., 9.))));
		oris.emplace_back(Rigid(  e2, Pose(BiVector(3., 2., 2.))));
		oris.emplace_back(Rigid( -e3, Pose(BiVector(4., 1., 5.))));
		oris.emplace_back(Rigid(  e3, Pose(BiVector(5., 0., 8.))));

		// a simple cube
		return oris;
	}

	//! Pairwise orientations within simulated block
	std::vector<blk::OriPair>
	simRelOris
		()
	{
		std::vector<blk::OriPair> rops;

		std::vector<ga::Rigid> const eos{ simAbsOris() };

		size_t const numNodes{ eos.size() };
		assert(1u < numNodes);
		rops.reserve(numNodes*(numNodes - 1u));

		for (blk::NodeNdx ndxI{0u} ; ndxI < numNodes ; ++ndxI)
		{
			ga::Rigid const & oriIwX = eos[ndxI];
			ga::Rigid const oriXwI{ oriIwX.inverse() };
			for (blk::NodeNdx ndxJ{ndxI+1u} ; ndxJ < numNodes ; ++ndxJ)
			{
				ga::Rigid const & oriJwX = eos[ndxJ];

				ga::Rigid const oriJwI{ oriJwX * oriXwI };
				if (0u == ((ndxI+ndxJ)%2u))
				{
					blk::OriPair const rop(ndxI, ndxJ, oriJwI);
					rops.emplace_back(rop);
				}
				else
				{
					blk::OriPair const rop(ndxJ, ndxI, oriJwI.inverse());
					rops.emplace_back(rop);
				}
			}
		}

		return rops;
	}


//! Check basic operations
std::string
blk_Block_test1
	()
{
	std::ostringstream oss;

	// simulate a bunch of relative orientations
	std::vector<blk::OriPair> const rops{ simRelOris() };

	for (blk::OriPair const & rop : rops)
	{
		io::out() << dat::infoString(rop.principalPair(), "rop") << std::endl;
	}

	// assemble into a nominal block structure
//	blk::Block const block{ blockFrom(rops.begin(), rops.end() };

oss << "Failure: implement this test!" << std::endl;
	return oss.str();
}


}

//! Unit test for blk::Block
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << blk_Block_test0();
	oss << blk_Block_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
