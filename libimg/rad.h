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

#ifndef img_rad_INCL_
#define img_rad_INCL_

/*! \file
\brief Declarations for img::rad
*/


#include "libdat/grid.h"
#include "libmath/Partition.h"
#include "libdat/Region.h"
#include "libprob/Histogram.h"

#include <utility>
#include <array>
#include <vector>

namespace img
{

//! Radiometric processing functions
namespace rad
{

	//! Histogram-equalized grid -- TODO move someplace (to libprob?)
	template <typename PixType>
	inline
	dat::grid<PixType>
	equalized
		( dat::grid<PixType> const & from
		, math::Partition const & part
		);

	//! Histograms for (all, only active) pixels within areas
	template <typename PixType>
	std::pair<prob::Histogram, prob::Histogram>
	histogramAllActive
		( std::array<dat::grid<PixType>, 3u> const & bands
		, math::Partition const & intenPartition
		, std::vector<dat::Area<size_t> > const & sampAreas
		);
}

}

// Inline definitions
#include "libimg/rad.inl"

#endif // img_rad_INCL_

