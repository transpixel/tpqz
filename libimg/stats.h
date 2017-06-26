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

#ifndef img_stats_INCL_
#define img_stats_INCL_

/*! \file
\brief Declarations for img::stats
*/


#include "libdat/grid.h"
#include "libdat/MinMax.h"
#include "libdat/template.h"
#include "libimg/img.h"
#include "libprob/Histogram.h"
#include "libprob/CdfInverse.h"

#include <cstddef>
#include <array>
#include <vector>


namespace img
{

/*! \brief Functions providing probability information about imagery.

\par Example
\dontinclude testimg/ustats.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace stats
{
	//! Min/max pixel value - else fpixBad
	template <typename PixType, typename FwdIter>
	inline
	dat::MinMax<PixType>
	activeMinMax
		( FwdIter const & begin
		, FwdIter const & end
		);

	//! Min/Max pixel values for specified areas of all grids
	template <typename PixType>
	inline
	dat::MinMax<PixType>
	pixelMinMax // TODO could unify with other pixelMinMax functions
		( std::array<dat::grid<PixType>, 3u> const & grids
		, std::vector<dat::Area<size_t> > const & sampAreas
		);

	//! Min/Max pixel values for specified areas of all grids
	template <typename PixType>
	inline
	dat::MinMax<PixType>
	pixelMinMax // TODO could unify with other pixelMinMax functions
		( dat::grid<std::array<PixType, 3u> > const & grid
		, std::vector<dat::Area<size_t> > const & inSampAreas
			= std::vector<dat::Area<size_t> >()
		);

	//! Histogram spanning data range
	template <typename PixType>
	inline
	prob::Histogram
	histogramFor
		( dat::grid<PixType> const & deepGrid
		, std::vector<dat::Area<size_t> > const & sampAreas
		, dat::MinMax<typename PixType::value_type> const & minmax
			= dat::MinMax<typename PixType::value_type>()
		);

	//! Inverse CDF for all values in deepGrid (all channels considered)
	template <typename PixType>
	inline
	prob::CdfInverse
	cdfInverseFor
		( dat::grid<PixType> const & deepGrid
		, std::vector<dat::Area<size_t> > const & sampAreas
		);

	//! Min/max of collection of array (treating each channel separately)
	template
		< typename PixType
		, typename FwdIter
		, dat::EnableIf< std::is_same<PixType, std::array<float, 3u> > >...
		>
	inline
	dat::MinMax<typename PixType::value_type>
	channelMinMax
		( FwdIter const & beg
		, FwdIter const & end
		);

	//! Mean of collection of POD values
	template
		< typename PixType
		, typename FwdIter
		, dat::EnableIf< std::is_arithmetic<PixType> >...
		>
	inline
	PixType
	meanOf
		( FwdIter const & beg
		, FwdIter const & end
		);

	//! Mean of collection of array (treating each channel separately)
	template
		< typename PixType
		, typename FwdIter
		, dat::EnableIf< std::is_same<PixType, std::array<float, 3u> > >...
		>
	inline
	PixType
	meanOf
		( FwdIter const & beg
		, FwdIter const & end
		);

	//! Median of each channel separately
	template <typename FwdIter>
	inline
	std::array<float, 3u>
	medianOf
		( FwdIter const & beg
		, FwdIter const & end
		);

}

}

// Inline definitions
#include "libimg/stats.inl"

#endif // img_stats_INCL_

