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
\brief Definitions for cloud::xform
*/


#include "libcloud/xform.h"

#include "libcloud/PointIterator.h"


namespace cloud
{
namespace xform
{

std::vector<cloud::FixedPoint>
fixedPoints
	( ga::Rigid const & xIntoWrtFrom
	, std::vector<cloud::FixedPoint> const & fpntFroms
	)
{
	std::vector<cloud::FixedPoint> fpntIntos;
	if (! fpntFroms.empty())
	{
		// transform points into expression w.r.t. station
		fpntIntos.reserve(fpntFroms.size());
		for (cloud::PointIterator iter{fpntFroms} ; iter ; ++iter)
		{
			ga::Vector const pntInReel(iter.vectorPoint());
			ga::Vector const pntInSta(xIntoWrtFrom(pntInReel));
			fpntIntos.emplace_back(cloud::cast::FixedPoint(pntInSta));
		}
	}

	return fpntIntos;
}

}
}

