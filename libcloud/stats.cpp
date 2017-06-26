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
\brief Definitions for cloud::stats
*/


#include "libcloud/stats.h"

#include "libdat/MinMax.h"
#include "libcloud/cast.h"
#include "libcloud/PointIterator.h"

#include <array>


namespace cloud
{
namespace stats
{

dat::Volume<double>
boundingVolumeOf
	( std::vector<FixedPoint> const & fpnts
	)
{
	dat::Volume<double> bounds;

	if (! fpnts.empty())
	{
		// Determine extents of each coordinate component
		std::array<dat::MinMax<double>, 3u> xyzMinMax;
		for (PointIterator iter{fpnts} ; iter ; ++iter)
		{
			ga::Vector const point(iter.vectorPoint());
			xyzMinMax[0] = xyzMinMax[0].expandedWith(point[0]);
			xyzMinMax[1] = xyzMinMax[1].expandedWith(point[1]);
			xyzMinMax[2] = xyzMinMax[2].expandedWith(point[2]);
		}

		// return volume
		if ( dat::isValid(xyzMinMax[0])
		  && dat::isValid(xyzMinMax[1])
		  && dat::isValid(xyzMinMax[2])
		   )
		{
			bounds = dat::Volume<double>
				{ dat::Range<double>(xyzMinMax[0].pair())
				, dat::Range<double>(xyzMinMax[1].pair())
				, dat::Range<double>(xyzMinMax[2].pair())
				};
		}
	}

	return bounds;
}

}
}

