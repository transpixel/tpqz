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
\brief Definitions for img::sample
*/


#include "libdat/info.h"

// TODO - probably libimg dependency can be removed
// - put 'sampling' operations some place more basic
#include "libimg/img.h"
#include "libdat/array.h"
#include "libio/stream.h"

#include <cassert>


namespace img
{
namespace sample
{
//======================================================================

//! Private utilities for img::sample implementations
namespace priv
{
	//! Range clipped to be within bounds
	inline
	dat::Range<size_t>
	boundedRange
		( size_t const & center
		, size_t const & deltaNeg
		, size_t const & deltaPos
		, size_t const & maxValue
		)
	{
		size_t min(0u);
		if (deltaNeg < center)
		{
			min = center - deltaNeg;
		}
		size_t const max{ std::min(maxValue, center+deltaPos) };
		return dat::Range<size_t>{ min, max };
	}
}

inline
dat::Area<size_t>
areaAround
	( dat::RowCol const & rowcol
	, dat::Extents const & bounds
	, size_t const & deltaNeg
	, size_t const & deltaPos
	)
{
	dat::Area<size_t> area;
	if (dat::isValid(rowcol) && bounds.isValid())
	{
		size_t const & row0 = rowcol[0];
		size_t const & col0 = rowcol[1];

		dat::Range<size_t> const rowRange
			{ priv::boundedRange(row0, deltaNeg, deltaPos, bounds.high()) };
		dat::Range<size_t> const colRange
			{ priv::boundedRange(col0, deltaNeg, deltaPos, bounds.wide()) };
		area = dat::Area<size_t>{ rowRange, colRange };
	}
	return area;
}

inline
std::vector<dat::Area<size_t> >
areasAround
	( std::vector<dat::RowCol> const & rowcols
	, dat::Extents const & bounds
	, size_t const & deltaNeg
	, size_t const & deltaPos
	)
{
	std::vector<dat::Area<size_t> > areas;
	areas.reserve(rowcols.size());

	// interpret default argument
	size_t dPos{ deltaPos };
	if (! dat::isValid(dPos))
	{
		dPos = deltaNeg + 1u;
	}

	for (dat::RowCol const & rowcol : rowcols)
	{
		areas.emplace_back(areaAround(rowcol, bounds, deltaNeg, dPos));
	}
	return areas;
}

inline
size_t
totalSize
	( std::vector<dat::Area<size_t> > const & areas
	)
{
	size_t sum(0u);
	for (dat::Area<size_t> const & area : areas)
	{
		if (area.isValid())
		{
			sum += area.magnitude();
		}
	}
	return sum;
}

template <typename PixType>
inline
std::vector<PixType>
fromImageArea
	( dat::grid<PixType> const & grid
	, dat::Area<size_t> const & sampArea
	)
{
	std::vector<PixType> samps;
	if (grid.isValid() && (0u < sampArea.magnitude()))
	{
		dat::RowCol const cornerTL{{ sampArea[0].min(), sampArea[1].min() }};
		dat::RowCol const cornerBR{{ sampArea[0].max(), sampArea[1].max() }};
		samps = fromRectangle(grid, cornerTL, cornerBR);
	}
	return samps;
}

template <typename PixType>
inline
std::vector<PixType>
fromImageAreas
	( dat::grid<PixType> const & grid
	, std::vector<dat::Area<size_t> > const & sampAreas
	)
{
	std::vector<PixType> allSamps;

	// allocate space for expected number of samples
	allSamps.reserve(totalSize(sampAreas));

	// pull samples from image
	for (dat::Area<size_t> const & sampArea : sampAreas)
	{
		if (sampArea.isValid())
		{
			std::vector<PixType> const samps
				(img::sample::fromImageArea(grid, sampArea));
			allSamps.insert(allSamps.end(), samps.begin(), samps.end());
		}
	}

	return allSamps;
}

template <typename PixType>
inline
std::vector<PixType>
fromRectangle
	( dat::grid<PixType> const & grid
	, dat::RowCol const & cornerTL
	, dat::RowCol const & cornerBR
	)
{
	std::vector<PixType> samps;
	size_t const row1(img::uClamp(0u, cornerTL[0], grid.high()));
	size_t const row2(img::uClamp(0u, cornerBR[0], grid.high()));
	size_t const col1(img::uClamp(0u, cornerTL[1], grid.wide()));
	size_t const col2(img::uClamp(0u, cornerBR[1], grid.wide()));
	assert(row1 <= row2);
	assert(col1 <= col2);
	size_t const numSamps((row2 - row1 + 1u) * (col2 - col1 + 1u));
	samps.reserve(numSamps);
	for (size_t row(row1) ; row < row2 ; ++row)
	{
		for (size_t col(col1) ; col < col2 ; ++col)
		{
			samps.push_back(grid(row, col));
		}
	}
	return samps;
}

namespace priv
{
	//! convenient syntax for conditional push_back
	inline
	void
	pushSpot
		( std::vector<dat::Spot> * const & ptSpots
		, size_t const & row
		, size_t const & col
		)
	{
		if (ptSpots)
		{
			ptSpots->push_back( dat::Spot{{ (double)row, (double)col }} );
		}
	}
}

template <typename PixType>
inline
std::vector<PixType>
fromBorderBand
	( dat::grid<PixType> const & grid
	, dat::Extents const & outerEdge
	, size_t const & bandSize
	)
{
	std::vector<PixType> samps;

	if (grid.isValid() && outerEdge.isValid() && (0u < bandSize))
	{
		assert(bandSize < outerEdge.high());
		assert(bandSize < outerEdge.wide());
		assert(outerEdge.high() <= grid.high());
		assert(outerEdge.wide() <= grid.wide());

		size_t const highExtra(grid.high() - outerEdge.high());
		size_t const wideExtra(grid.wide() - outerEdge.wide());

		// define rows of interest - centered on grid
		size_t const rBeg(highExtra / 2u);
		size_t const rEnd(rBeg + outerEdge.high());
		size_t const r0(rBeg);
		size_t const r1(rBeg + bandSize);
		size_t const r2(rEnd - bandSize);
		size_t const r3(rEnd);

		// define columns of interest - centered on grid
		size_t const cBeg(wideExtra / 2u);
		size_t const cEnd(cBeg + outerEdge.wide());
		size_t const c0(cBeg);
		size_t const c1(cBeg + bandSize);
		size_t const c2(cEnd - bandSize);
		size_t const c3(cEnd);

		// estimate (generously) sample size
		size_t const numRows(outerEdge.high());
		size_t const numCols(outerEdge.wide());
		// compute the sample size
		size_t const numSamps
			(      bandSize * numCols // entire top
			+ 2u * bandSize * numRows // both entire left/right
			+      bandSize * numCols // entire bottom
			- 4u * bandSize * bandSize // less corner overlaps
			);

		constexpr bool debug{ false };
		if (debug)
		{
			io::out() << std::endl;
			io::out() << dat::infoString(grid, "grid") << std::endl;
			io::out() << dat::infoString(outerEdge, "outerEdge") << std::endl;
			io::out() << dat::infoString(bandSize, "bandSize") << std::endl;
			io::out() << std::endl;
			io::out() << dat::infoString(r0, "r0") << std::endl;
			io::out() << dat::infoString(r1, "r1") << std::endl;
			io::out() << dat::infoString(r2, "r2") << std::endl;
			io::out() << dat::infoString(r3, "r3") << std::endl;
			io::out() << std::endl;
			io::out() << dat::infoString(cBeg, "cBeg") << std::endl;
			io::out() << dat::infoString(cEnd, "cEnd") << std::endl;
			io::out() << dat::infoString(c0, "c0") << std::endl;
			io::out() << dat::infoString(c1, "c1") << std::endl;
			io::out() << dat::infoString(c2, "c2") << std::endl;
			io::out() << dat::infoString(c3, "c3") << std::endl;
			io::out() << std::endl;
		}

		// reserve space for samples
		samps.reserve(numSamps);

		// extract samples

		// top row
		for (size_t row(r0) ; row < r1 ; ++row)
		{
			for (size_t col(cBeg) ; col < cEnd ; ++col)
			{
				samps.push_back(grid(row, col));
			}
		}

		// middle row: left and right edges
		for (size_t row(r1) ; row < r2 ; ++row)
		{
			for (size_t col(c0) ; col < c1 ; ++col)
			{
				samps.push_back(grid(row, col));
			}
			for (size_t col(c2) ; col < c3 ; ++col)
			{
				samps.push_back(grid(row, col));
			}
		}

		// bottom row
		for (size_t row(r2) ; row < r3 ; ++row)
		{
			for (size_t col(cBeg) ; col < cEnd ; ++col)
			{
				samps.push_back(grid(row, col));
			}
		}
	}

	return samps;
}

template <typename PixType, typename FwdIter>
inline
std::vector<PixType>
activeValuesFrom
	( FwdIter const & beg
	, FwdIter const & end
	)
{
	std::vector<PixType> active;
	size_t const numElem(std::distance(beg, end));
	active.reserve(numElem);
	for (FwdIter iter(beg) ; end != iter ; ++iter)
	{
		PixType const & pix = *iter;
		if (isActive(pix))
		{
			active.push_back(pix);
		}
	}
	return active;
}

template <typename PixType>
inline
std::vector<PixType>
fromCellsNearTo
	( dat::RowCol const & atRowCol
	, dat::grid<PixType> const & grid
	, size_t const & numCellsPad
	)
{
	std::vector<PixType> samps;

	size_t const & rowAt(atRowCol[0]);
	size_t const & colAt(atRowCol[1]);

	bool const okay
		(  (grid.isValid())
		&& (rowAt < grid.high())
		&& (colAt < grid.wide())
		);
	if (okay)
	{
		// define sample window (asymmetric to equalize CFA sampling!)
		size_t const numPixPad(2u * numCellsPad);
		//
		size_t const drU(     numPixPad); // upward
		size_t const drD(1u + numPixPad); // downward
		size_t const dcL(     numPixPad); // to left
		size_t const dcR(1u + numPixPad); // to right
		//
		size_t const lastRow(grid.high() - 1u);
		size_t const lastCol(grid.wide() - 1u);
		size_t const r0(rowAt - std::min(rowAt, drU));
		size_t const r1(std::min(lastRow, (rowAt + drD)) + 1u);
		size_t const c0(colAt - std::min(colAt, dcL));
		size_t const c1(std::min(lastCol, (colAt + dcR)) + 1u);

		// prepare storage
		size_t const numRows(r1 - r0);
		size_t const numCols(c1 - c0);
		size_t const numSamps(numRows * numCols);

		bool const debug(false);
		if (debug)
		{
			io::out()
				<< "padCells,numPix:"
				<< " " << std::setw(4) << numCellsPad
				<< " " << std::setw(4) << numPixPad
				<< std::endl;
			io::out()
				<< "row,col:At:"
				<< " " << std::setw(4) << rowAt
				<< " " << std::setw(4) << colAt
				<< std::endl;
			io::out()
				<< "drU,drD,dcL,dcR"
				<< "   "
				<< " " << std::setw(4) << drU
				<< " " << std::setw(4) << drD
				<< " " << std::setw(4) << dcL
				<< " " << std::setw(4) << dcR
				<< "  :  "
				<< " " << std::setw(4) << numRows
				<< " " << std::setw(4) << numCols
				<< std::endl;
			io::out()
				<< " r0, r1, c0, c1"
				<< "   "
				<< " " << std::setw(4) << r0
				<< " " << std::setw(4) << r1
				<< " " << std::setw(4) << c0
				<< " " << std::setw(4) << c1
				<< "  :  "
				<< " " << std::setw(4) << numRows
				<< " " << std::setw(4) << numCols
				<< std::endl;
			io::out()
				<< "sampsize: " << samps.capacity()
				<< std::endl;
		}

		// allocate storage
		size_t const oneSide(1u + 2u*numCellsPad);
		size_t const expNumCells(oneSide * oneSide);
		size_t const expNumSamps(4u * expNumCells);
		if (expNumSamps < numSamps)
		{
			io::err()
				<< "ERROR: " << '\n'
				<< "... samplesNear: size estimation error" << '\n'
				<< "... expNumSamps: " << expNumSamps << '\n'
				<< "... gotNumSamps: " << numSamps << '\n'
				<< std::endl;
			assert(numSamps <= expNumSamps);
		}
		samps.reserve(numSamps);

		// gather samples
		for (size_t row(r0) ; row < r1 ; ++row)
		{
			for (size_t col(c0) ; col < c1 ; ++col)
			{
				samps.push_back(grid(row, col));
			}
		}

	}
	return samps;
}

//======================================================================
}
}

