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

#ifndef img_sample_INCL_
#define img_sample_INCL_

/*! \file
\brief Declarations for img::sample
*/


#include "libdat/Area.h"
#include "libdat/dat.h"
#include "libdat/grid.h"
#include "libdat/Region.h"

#include <vector>


namespace img
{
//! functions for sampling image data
namespace sample
{
	//! Pixel area surrounding location
	inline
	dat::Area<size_t>
	areaAround
		( dat::RowCol const & rowcol
		, dat::Extents const & bounds
		, size_t const & deltaNeg
		, size_t const & deltaPos = dat::nullValue<size_t>() //! use pos+1
		);

	//! Collection of areaAround() instances
	inline
	std::vector<dat::Area<size_t> >
	areasAround
		( std::vector<dat::RowCol> const & rowcols
		, dat::Extents const & bounds
		, size_t const & deltaNeg
		, size_t const & deltaPos = dat::nullValue<size_t>() //! use pos+1
		);

	//! Cumulative area of all sample areas
	inline
	size_t
	totalSize
		( std::vector<dat::Area<size_t> > const & areas
		);

	//! Samples from a specific area of the grid
	template <typename PixType>
	inline
	std::vector<PixType>
	fromImageArea
		( dat::grid<PixType> const & grid
		, dat::Area<size_t> const & sampArea
		);

	//! Combined samples from multiple areas of the grid
	template <typename PixType>
	inline
	std::vector<PixType>
	fromImageAreas
		( dat::grid<PixType> const & grid
		, std::vector<dat::Area<size_t> > const & sampAreas
		);

	//! Extract samples from an area
	template <typename PixType>
	inline
	std::vector<PixType>
	fromRectangle
		( dat::grid<PixType> const & grid
		, dat::RowCol const & cornerTL
		, dat::RowCol const & cornerBR //!< *Ex*clusive
		);

	//! Extract samples from outer border region of grid
	template <typename PixType>
	inline
	std::vector<PixType>
	fromBorderBand
		( dat::grid<PixType> const & grid
		, dat::Extents const & outerEdge
		, size_t const & bandSize
		);

	//! Values from allValues which satisify "isActive()"
	template <typename PixType, typename FwdIter>
	inline
	std::vector<PixType>
	activeValuesFrom
		( FwdIter const & beg
		, FwdIter const & end
		);

	//! Samples that span an integral number of 2x2 cells
	template <typename PixType>
	inline
	std::vector<PixType>
	fromCellsNearTo
		( dat::RowCol const & atRowCol
		, dat::grid<PixType> const & grid
		, size_t const & numCellsPad
		);

}

}

// Inline definitions
#include "libimg/sample.inl"

#endif // img_sample_INCL_

