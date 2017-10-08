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

#ifndef cloud_convert_INCL_
#define cloud_convert_INCL_

/*! \file
\brief Declarations for cloud::convert
*/

#include "libdat/grid.h"

#include <vector>

#include "libcloud/cast.h"
#include "libcloud/cloud.h"
#include "libdat/validity.h"
#include "libmath/interp.h"

namespace cloud
{

/*! \brief Functions for converting point cloud data to/from various forms.

\par Example
\dontinclude testcloud/uconvert.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace convert
{
	//! Convert point information to display grid via outFromPoint transform
	template <typename OutType, typename OutFunc>
	dat::grid<OutType>
	gridVisualization
		( dat::grid<NdxType> const & ndxGrid
		, std::vector<cloud::FixedPoint> const & fpnts
		, OutFunc const & outFromPoint //!< OutType::op()(ga::Vector)
		, OutType const & backValue = {}
		)
	{
		// allocate space and fill with background value
		dat::grid<OutType> ugrid(ndxGrid.hwSize());
		std::fill(ugrid.begin(), ugrid.end(), backValue);

		// convert each point to output type interpretation
		dat::grid<NdxType>::const_iterator itIn{ ndxGrid.begin() };
		typename dat::grid<OutType>::iterator itOut{ ugrid.begin() };
		for ( ; ugrid.end() != itOut ; ++itOut, ++itIn)
		{
			NdxType const & ndx = *itIn;

			OutType outVal = backValue;
			if (dat::isValid(ndx))
			{
				// hack data relative to test22 run
				ga::Vector const point(cloud::cast::Vector(fpnts[ndx]));

				outVal = outFromPoint(point);

			}
			*itOut = outVal;
		}

		return ugrid;
	}
}

}

// Inline definitions
// #include "libcloud/convert.inl"

#endif // cloud_convert_INCL_

