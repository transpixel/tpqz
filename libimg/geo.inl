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
\brief Inline definitions for img::geo
*/


#include "libdat/ExtentsIterator.h"

#include "libdat/info.h"
#include "libimg/stats.h"
#include "libio/stream.h"
#include "libmath/Extreme.h"

#include <cassert>


namespace img
{
namespace geo
{
//======================================================================


template <typename OutType, typename InType>
inline
dat::grid<OutType>
croppedGrid
	( dat::grid<InType> const & srcGrid
	, dat::SubExtents const & cropSub
	)
{
	dat::grid<OutType> outgrid(cropSub.theSize);;

	if (srcGrid.isValid() && cropSub.isValid())
	{
		dat::RowCol const & srcUL = cropSub.theUL;
		typename dat::grid<OutType>::iterator itOut(outgrid.begin());
		for (dat::ExtentsIterator iter(cropSub.theSize) ; iter ; ++iter)
		{
			dat::RowCol const & cropRowCol = *iter;
			using dat::operator+;
			dat::RowCol const srcRowCol(cropRowCol + srcUL);
			*itOut++ = static_cast<OutType>(srcGrid(srcRowCol));
		}
	}

	return outgrid;
}

template <typename OutType, typename InType>
inline
dat::grid<OutType>
centerCroppedGrid
	( dat::grid<InType> const & srcGrid
	, dat::Extents const & outSize
	)
{
	dat::grid<OutType> outgrid;

	assert(dat::isSameParity(srcGrid.high(), outSize.high()));
	assert(dat::isSameParity(srcGrid.wide(), outSize.wide()));

	dat::Extents const srcSize(srcGrid.hwSize());
	dat::RowCol const srcHalf(srcSize.centerMod2());
	dat::RowCol const outHalf(outSize.centerMod2());
	if (dat::canSubtract(srcHalf, outHalf))
	{
		using dat::operator-;
		dat::RowCol const srcUL(srcHalf - outHalf);
		dat::SubExtents const cropSub(srcUL, outSize);
		outgrid = croppedGrid<OutType, InType>(srcGrid, cropSub);
	}

	/* -- direct copy probably faster if no cast operation needed
	// Subgrid from fullGrid for all data valid
	template <typename PixType>
	dat::grid<PixType>
	subGridFromValid
		( dat::grid<PixType> const & fullGrid
		, dat::SubExtents const & crop
		)
	{
		dat::grid<PixType> subgrid(crop.theSize);
		for (size_t subRow(0u) ; subRow < crop.high() ; ++subRow)
		{
			// get iterator into source
			using InIter = typename dat::grid<PixType>::const_iterator;
			std::pair<InIter, InIter> const inIterBE
				{ crop.iterFullBegEnd(fullGrid, subRow) };

			// set iterator into destination
			using OutIter = typename dat::grid<PixType>::iterator;
			OutIter const itOutBeg(subgrid.beginRow(subRow));

			// perform copy of this row section
			std::copy(inIterBE.first, inIterBE.second, itOutBeg);
		}
		return subgrid;
	}
	*/

	return outgrid;
}

template <typename OutType, typename InType>
inline
void
insertSubGrid
	( dat::grid<OutType> * const & ptOutGrid
	, dat::SubExtents const & srcSub
	, dat::grid<InType> const & srcGrid
	)
{
	if (ptOutGrid)
	{
		if (srcGrid.isValid() && srcSub.isValid() && ptOutGrid->isValid())
		{
			dat::RowCol const & outUL = srcSub.theUL;
			typename dat::grid<OutType>::iterator itOut(ptOutGrid->begin());
			dat::Extents const outSize{ ptOutGrid->hwSize() };
			for (dat::ExtentsIterator iter{srcSub.theSize} ; iter ; ++iter)
			{
				dat::RowCol const & srcRowCol = *iter;
				using dat::operator+;
				dat::RowCol const outRowCol(srcRowCol + outUL);
				if (outSize.includes(outRowCol))
				{
					(*ptOutGrid)(outRowCol)
						= static_cast<OutType>(srcGrid(srcRowCol));
				}
			}
		}
	}
}

namespace
{
	size_t
	rowsToFit
		( size_t const & numItems
		, size_t const & numCols
		)
	{
		size_t numRows{ numItems / numCols };
		if (0u < (numItems % numCols))
		{
			++numRows;
		}
		return numRows;
	}
}

namespace
{
	template <typename PixType>
	void
	fillWithDither
		( dat::grid<PixType> * const & ptGrid
		, PixType const & value0
		, PixType const & value1
		)
	{
		for (dat::ExtentsIterator iter{ptGrid->hwSize()} ; iter ; ++iter)
		{
			dat::RowCol const & rowcol = *iter;
			bool const rowParity{ dat::isEven(rowcol[0]) };
			bool const colParity{ dat::isEven(rowcol[1]) };
			if (rowParity == colParity)
			{
				(*ptGrid)(rowcol) = value0;
			}
			else
			{
				(*ptGrid)(rowcol) = value1;
			}
		}
	}

	//! min/max active pixel value of entire collection
	template <typename PixType, typename FwdIter>
	dat::MinMax<PixType>
	activeMinMax
		( FwdIter const & beg
		, FwdIter const & end
		)
	{
		dat::MinMax<PixType> allMinMax;
		for (FwdIter iter{beg} ; end != iter ; ++iter)
		{
			dat::grid<PixType> const & grid = *iter;
			dat::MinMax<PixType> const minmax
				{ img::stats::activeMinMax<PixType>(grid.begin(), grid.end()) };
			allMinMax = allMinMax.expandedWith(minmax);
		}
		return allMinMax;
	}
}

template <typename PixType, typename FwdIter>
inline
dat::grid<PixType>
montageOf
	( FwdIter const & beg
	, FwdIter const & end
	, size_t const & numCols
	, PixType const & // backValue
	, size_t const & pad
	, PixType const & // foreValue
	)
{
	using PixGrid = dat::grid<PixType>;
	PixGrid montage;

	if (end != beg)
	{
		// determine maximum item grid extents
		using GridType = typename std::iterator_traits<FwdIter>::value_type;
		math::Extreme<FwdIter> const exHigh
			( beg, end
			, [] (GridType const & dA, GridType const & dB)
				{ return (dA.high() < dB.high()); }
			);
		math::Extreme<FwdIter> const exWide
			( beg, end
			, [] (GridType const & dA, GridType const & dB)
				{ return (dA.wide() < dB.wide()); }
			);
		size_t const gap{ 2u*pad };
		size_t const itemHigh{ exHigh.theExVal.high() + gap };
		size_t const itemWide{ exWide.theExVal.wide() + gap };

		// allocate space for the full montage - and init to backValue
		size_t const numItems{ (size_t)std::distance(beg, end) };
		size_t const numRows{ rowsToFit(numItems, numCols) };

		montage = PixGrid(numRows * itemHigh, numCols * itemWide);
	//	std::fill(montage.begin(), montage.end(), backValue);
		dat::MinMax<PixType> const minmax{ activeMinMax<PixType>(beg, end) };
		fillWithDither(&montage, minmax.min(), minmax.max());

		// copy each source item into montage
		FwdIter itSrc{ beg };
		for (size_t row{0u} ; row < numRows ; ++row)
		{
			size_t const rowOut{ pad + itemHigh * row };
			for (size_t col{0u} ; (col < numCols) && (end != itSrc); ++col)
			{
				// access source grid
				PixGrid const & srcGrid = *itSrc;
				if (srcGrid.isValid())
				{
					// define output area
					size_t const colOut{ pad + itemWide * col };
					dat::RowCol const ulOut{ rowOut, colOut };
					dat::SubExtents const crop(ulOut, srcGrid.hwSize());
					// copy source elements into output
					insertSubGrid(&montage, crop, srcGrid);
				}
				++itSrc;
			}
		}
	}

	return montage;
}

//======================================================================
}
}


