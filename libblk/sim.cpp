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
allROs
	( std::map<NodeKey, ga::Rigid> const & eos
	)
{
	std::vector<blk::EdgeOri> rops;

	size_t const numNodes{ eos.size() };
	rops.reserve(math::sq(numNodes));

	size_t roCount{ 0u };
	for (size_t ndxI{0u} ; ndxI < numNodes ; ++ndxI)
	{
		blk::NodeKey const keyI{ keyFromNdx(ndxI) };
		std::map<NodeKey, ga::Rigid>::const_iterator const itI
			{ eos.find(keyI) };
		assert(eos.end() != itI);
		ga::Rigid const & oriIwX = itI->second;

		ga::Rigid const oriXwI{ oriIwX.inverse() };
		constexpr size_t maxBand{ 1u };
		size_t numBand{ 0u };
		for (size_t ndxJ{ndxI+1u} ; ndxJ < numNodes ; ++ndxJ)
		{
			blk::NodeKey const keyJ{ keyFromNdx(ndxJ) };
			std::map<NodeKey, ga::Rigid>::const_iterator const itJ
				{ eos.find(keyJ) };
			assert(eos.end() != itJ);
			ga::Rigid const & oriJwX = itJ->second;

			ga::Rigid const oriJwI{ oriJwX * oriXwI };
			if (0u == ((roCount++)%2u))
			{
				blk::EdgeOri const rop
					{ blk::EdgeKey{keyI, keyJ}, oriJwI };
				rops.emplace_back(rop);
			}
			else
			{
				blk::EdgeOri const rop
					{ blk::EdgeKey{keyJ, keyI}, oriJwI.inverse() };
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


} // sim

} // blk

