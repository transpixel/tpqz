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
\brief Definitions for ro::model
*/


#include "libro/model.h"

#include "libgeo/intersect.h"


namespace ro
{
namespace model
{

PairUV
uvPair
	( ga::Vector const & pntIn0
	, OriPair const & oriPairIn0
	)
{
	ga::Vector const uVec{ oriPairIn0.first(pntIn0) };
	ga::Vector const vVec{ oriPairIn0.second(pntIn0) };
	return { ga::unit(uVec), ga::unit(vVec) };
}

std::vector<PairUV>
uvPairs
	( std::vector<ga::Vector> const & pntsIn0
	, OriPair const & oriPairIn0
	)
{
	std::vector<PairUV> uvs;
	uvs.reserve(pntsIn0.size());
	for (ga::Vector const & pntIn0 : pntsIn0)
	{
		uvs.emplace_back(uvPair(pntIn0, oriPairIn0));
	}
	return uvs;
}

namespace
{
	PntPair
	pointPair
		( PairUV const & uvPair
		, ga::Vector const & sta1w0
		, ga::Vector const & sta2w0
		, ga::Pose const & att0w1
		, ga::Pose const & att0w2
		)
	{
		PntPair uvPnt;

		// access measurements
		ga::Vector const & uIn1 = uvPair.first;
		ga::Vector const & vIn2 = uvPair.second;

		// compute point
		ga::Vector const uIn0{ att0w1(uIn1) };
		ga::Vector const vIn0{ att0w2(vIn2) };

		geo::Ray const ray1(sta1w0, uIn0);
		geo::Ray const ray2(sta2w0, vIn0);
		std::pair<double, double> const uvRanges
			{ geo::intersect::rangesFor(ray1, ray2) };

		if (dat::isValid(uvRanges))
		{
			double const & uRange = uvRanges.first;
			double const & vRange = uvRanges.second;
			if ((0. < uRange) && (0. < vRange))
			{
				uvPnt = { sta1w0 + uRange*uIn0, sta2w0 + vRange*vIn0 };
			}
		}

		return uvPnt;
	}
}

PntPair
pointPair
	( PairUV const & uvPair
	, OriPair const & oriPairIn0
	)
{
	PntPair uvPnt;
	if (dat::isValid(uvPair) && dat::isValid(oriPairIn0))
	{
		// model parameters
		ga::Rigid const & ori1w0 = oriPairIn0.first;
		ga::Rigid const & ori2w0 = oriPairIn0.second;
		ga::Vector const & uSta = ori1w0.location();
		ga::Vector const & vSta = ori2w0.location();
		ga::Pose const att0w1{ ori1w0.pose().inverse() };
		ga::Pose const att0w2{ ori2w0.pose().inverse() };

		// compute point
		uvPnt = pointPair(uvPair, uSta, vSta, att0w1, att0w2);
	}
	return uvPnt;
}

std::vector<PntPair>
pointPairs
	( std::vector<PairUV> const & uvPairs
	, OriPair const & oriPairIn0
	)
{
	std::vector<PntPair> uvPnts;
	uvPnts.reserve(uvPairs.size());

	// model parameters
	ga::Rigid const & ori1w0 = oriPairIn0.first;
	ga::Rigid const & ori2w0 = oriPairIn0.second;
	ga::Vector const & uSta = ori1w0.location();
	ga::Vector const & vSta = ori2w0.location();
	ga::Pose const att0w1{ ori1w0.pose().inverse() };
	ga::Pose const att0w2{ ori2w0.pose().inverse() };

	// compute pointpair for each observation correspondence
	for (PairUV const & uvPair : uvPairs)
	{
		uvPnts.emplace_back(pointPair(uvPair, uSta, vSta, att0w1, att0w2));
	}
	return uvPnts;
}

size_t
numForward
	( OriPair const & oriPair
	, FiveOf<PairUV const * const> const & ptrQuintUV
	)
{
	size_t fwdCount{ 0u };
	if (dat::isValid(oriPair))
	{
		for (PairUV const * const & ptrUV : ptrQuintUV)
		{
			PairUV const & uvPair = *ptrUV;
			PntPair const pntPair{ pointPair(uvPair, oriPair) };
			if (dat::isValid(pntPair))
			{
				++fwdCount;
			}
		}
	}
	return fwdCount;
}


} // model
} // ro

