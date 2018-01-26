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
\brief Definitions for ro::io
*/


#include "libro/io.h"

#include "libro/model.h"

#include <algorithm>
#include <cmath>


namespace ro
{
namespace io
{

namespace gnuplot
{

	void
	putRecord
		( std::ostream & ostrm
		, ga::Vector const & sta
		, ga::Vector const & pnt
		)
	{
		ga::Vector const del{ pnt - sta };
		ostrm
			<< " " << dat::infoString(sta)
			<< " " << dat::infoString(del)
			<< " " << dat::infoString(pnt)
			<< '\n';
	}

	ga::Vector
	meaPoint
		( ga::Vector const & dir
		)
	{
		assert(dat::isValid(dir));
		double const zMag{ std::abs(dir[2]) };
		assert(0. < zMag);
		return ((1. / zMag) * dir);
	}

bool
saveImageRays
	( std::ostream & ostrm
	, WhichOne const & which
	, OriPair const & oriPair
	, std::vector<PairUV> const & uvPairs
	)
{
	bool okay{ false };

	// select data
	ga::Vector sta;
	std::vector<ga::Vector> meaPnts(uvPairs.size());
	if (First == which)
	{
		sta = oriPair.first.location();
		std::transform
			( uvPairs.begin(), uvPairs.end()
			, meaPnts.begin()
			, [] (PairUV const & uvPair) { return meaPoint(uvPair.first); }
			);
	}
	else
	if (Second == which)
	{
		sta = oriPair.second.location();
		std::transform
			( uvPairs.begin(), uvPairs.end()
			, meaPnts.begin()
			, [] (PairUV const & uvPair) { return meaPoint(uvPair.second); }
			);
	}

	for (ga::Vector const & meaPnt : meaPnts)
	{
		putRecord(ostrm, sta, meaPnt);
	}

	return okay;
}

bool
saveModelRays
	( std::ostream & ostrm
	, OriPair const & oriPair
	, std::vector<PairUV> const & uvPairs
	)
{
	bool okay{false};
	if (dat::isValid(oriPair))
	{
		ga::Vector const & sta1 = oriPair.first.location();
		ga::Vector const & sta2 = oriPair.second.location();

		for (PairUV const & uvPair : uvPairs)
		{
			PntPair const pntPair{ model::pointPair(uvPair, oriPair) };
			ga::Vector const & pnt1 = pntPair.first;
			ga::Vector const & pnt2 = pntPair.second;
			putRecord(ostrm, sta1, pnt1);
			putRecord(ostrm, sta2, pnt2);
		}

		okay = (! ostrm.fail());
	}
	return okay;
}

} // gnuplot


} // io

} // ro

