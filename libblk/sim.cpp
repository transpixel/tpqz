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
\brief Definitions for blk::sim
*/


#include "libblk/sim.h"

#include "libblk/info.h"



namespace blk
{

namespace sim
{

// #define StringKey
#if ! defined(StringKey)
	constexpr size_t keyOffset{ 100u };
#endif

blk::NodeKey
keyFromNdx
	( size_t const & ndx
	)
{
#	if defined(StringKey)
		std::string const key{ io::sprintf("key_%d", ndx) };
#	else
		size_t const key{ keyOffset + ndx };
#	endif
		return key;
}

size_t
ndxFromKey
	( blk::NodeKey const & key
	)
{
#	if defined(StringKey)
		std::string ndxStr(key.begin()+4u, key.end());
		size_t const ndx{ io::string::from(ndxStr, dat::nullValue<size_t>()) };
#	else
		assert(! (key < keyOffset));
		size_t const ndx{ key - keyOffset };
#	endif
	return ndx;
}

ga::Rigid
oriComps
	( double const & loc1
	, double const & loc2
	, double const & loc3
	, double const & ang1
	, double const & ang2
	, double const & ang3
	)
{
	return ga::Rigid
		( ga::Vector(loc1, loc2, loc3)
		, ga::Pose(ga::BiVector(ang1, ang2, ang3))
		);
}

namespace
{
	ga::Pose
	poseFor
		( double const & xE23
		, double const & xE31
		, double const & xE12
		)
	{
		return ga::Pose( ga::BiVector(xE23, xE31, xE12) );
	}

	ga::Rigid
	oriFor
		( ga::Vector const & loc
		, double const & xE23
		, double const & xE31
		, double const & xE12
		)
	{
		return ga::Rigid(loc, poseFor(xE23, xE31, xE12));
	}
}

std::map<NodeKey, ga::Rigid>
cubeEOs
	( ga::Vector const & center
	, double const & edgeSpan
	)
{
	std::map<NodeKey, ga::Rigid> keyOris;

	double const mag{ .5 * edgeSpan / std::sqrt(3.) };

	using namespace ga;
	size_t ndx{ 0u };
	keyOris[keyFromNdx(ndx++)] = oriFor(center+mag*( e1+e2+e3),  0.,  1.,  1.);
	keyOris[keyFromNdx(ndx++)] = oriFor(center+mag*(-e1+e2+e3),  2.,  3.,  2.);
	keyOris[keyFromNdx(ndx++)] = oriFor(center+mag*( e1-e2+e3),  4.,  5.,  3.);
	keyOris[keyFromNdx(ndx++)] = oriFor(center+mag*(-e1-e2+e3),  6.,  7.,  4.);
	keyOris[keyFromNdx(ndx++)] = oriFor(center+mag*( e1+e2-e3),  8.,  9.,  5.);
	keyOris[keyFromNdx(ndx++)] = oriFor(center+mag*(-e1+e2-e3), 10., 11.,  6.);
	keyOris[keyFromNdx(ndx++)] = oriFor(center+mag*( e1-e2-e3), 12., 13.,  7.);
	keyOris[keyFromNdx(ndx++)] = oriFor(center+mag*(-e1-e2-e3), 14., 15.,  8.);

	// a simple cube
	return keyOris;
}

std::vector<blk::EdgeOri>
bandedROs
	( std::map<NodeKey, ga::Rigid> const & eos
	, size_t const & maxBandWidth
	)
{
	std::vector<blk::EdgeOri> rops;

	// consider all combinations of pairs
	// - each node used at least once
	// - each node paired with maxBandWidth of (lexicographical) neighbors
	size_t const numNodes{ eos.size() };
	rops.reserve(numNodes * maxBandWidth);

	size_t roCount{ 0u };
	// get first node
	for (std::map<NodeKey, ga::Rigid>::const_iterator
		itI{eos.begin()} ; eos.end() != itI ; ++itI)
	{
		blk::NodeKey const & keyI = itI->first;
		ga::Rigid const & oriIwX = itI->second;

		// get second node (until bandwidth exceeded)
		ga::Rigid const oriXwI{ oriIwX.inverse() };
		size_t bandSize{ 0u };
		std::map<NodeKey, ga::Rigid>::const_iterator itJ{ itI };
		for (++itJ ; (eos.end() != itJ) && (bandSize++ < maxBandWidth) ; ++itJ)
		{
			blk::NodeKey const & keyJ = itJ->first;
			ga::Rigid const & oriJwX = itJ->second;

			// compute relative orientation
			ga::Rigid const oriJwI{ oriJwX * oriXwI };

			// alternate edge direction to mix things up a bit
			if (0u == ((roCount++)%2u))
			{
				// "forward" edge (I<J)
				blk::EdgeOri const rop
					{ blk::EdgeKey{keyI, keyJ}, oriJwI };
				rops.emplace_back(rop);
			}
			else
			{
				// "reverse" edge (J<I)
				blk::EdgeOri const rop
					{ blk::EdgeKey{keyJ, keyI}, oriJwI.inverse() };
				rops.emplace_back(rop);
			}
		}
	}

	return rops;
}

std::vector<blk::EdgeOri>
allROs
	( std::map<NodeKey, ga::Rigid> const & eos
	)
{
	std::vector<blk::EdgeOri> rops;

	size_t const numNodes{ eos.size() };
	if (1u < numNodes)
	{
		// consider all combinations of pairs
		rops.reserve((numNodes * (numNodes - 1u)) / 2u);

		// get first node
		for (std::map<NodeKey, ga::Rigid>::const_iterator
			itI{eos.begin()} ; eos.end() != itI ; ++itI)
		{
			blk::NodeKey const & keyI = itI->first;
			ga::Rigid const & oriIwX = itI->second;

			// get second node (until bandwidth exceeded)
			ga::Rigid const oriXwI{ oriIwX.inverse() };
			std::map<NodeKey, ga::Rigid>::const_iterator itJ{ itI };
			for (++itJ ; eos.end() != itJ ; ++itJ)
			{
				blk::NodeKey const & keyJ = itJ->first;
				ga::Rigid const & oriJwX = itJ->second;

				// compute relative orientation
				ga::Rigid const oriJwI{ oriJwX * oriXwI };
				// "forward" edge (I<J)
				blk::EdgeOri const rop{ blk::EdgeKey{keyI, keyJ}, oriJwI };
				rops.emplace_back(rop);
			}
		}
	}

	return rops;
}


} // sim

} // blk

