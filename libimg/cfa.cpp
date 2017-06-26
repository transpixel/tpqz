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
\brief Definitions for img::cfa
*/


#include "libimg/cfa.h"
#include "libimg/img.h"

#include <cassert>


namespace img
{
namespace cfa
{
//======================================================================

namespace
{
	/*! Bayer phases

	  A     B
	  (0,0) (0,1)
	   R G   G R
	   G B   B G

	  C     D
	  (1,0) (1,1)
	   G B   B G
	   R G   G R

	*/
	enum Phase
	{
		  A_RG_GB
		, B_GR_BG
		, C_GB_RG
		, D_BG_GR
	};

	//! Pattern phase at (row,col) location
	inline
	Phase
	phaseAt
		( size_t const & row
		, size_t const & col
		)
	{
		bool const row0(0u == (row % 2));
		bool const col0(0u == (col % 2));
		if (row0) // A,B
		{
			if (col0) // A,C
			{
				return A_RG_GB;
			}
			else // B,D
			{
				return B_GR_BG;
			}
		}
		else // C,D
		{
			if (col0) // A,C
			{
				return C_GB_RG;
			}
			else // B,D
			{
				return D_BG_GR;
			}
		}
	}

	static fpix_t const badFlt(fpixBad);
	static std::array<fpix_t, 3u> const badPix{{ badFlt, badFlt, badFlt }};

	//! Hack demosaicking algorithm
	inline
	std::array<fpix_t, 3u>
	rgbAt
		( size_t const & row0
		, size_t const & col0
		, dat::grid<fpix_t> const & rawGrid
		)
	{
		std::array<fpix_t, 3u> rgb(badPix);
		size_t const row1(row0 + 1);
		size_t const col1(col0 + 1);

		// reference the input pixel values
		fpix_t const & pix00 = rawGrid(row0, col0);
		fpix_t const & pix01 = rawGrid(row0, col1);
		fpix_t const & pix10 = rawGrid(row1, col0);
		fpix_t const & pix11 = rawGrid(row1, col1);

		// only process pixels for which all contributors are valid
		if ( img::isValid(pix00) || img::isValid(pix01)
		  || img::isValid(pix10) || img::isValid(pix11)
		   ) // else use default badPix
		{
			fpix_t & red = rgb[0];
			fpix_t & grn = rgb[1];
			fpix_t & blu = rgb[2];

			// check for under/over-flow values
			if ( img::isUnder(pix00) || img::isUnder(pix01)
			  || img::isUnder(pix10) || img::isUnder(pix11)
			   )
			{
				red = img::fpixUnder;
				grn = img::fpixUnder;
				blu = img::fpixUnder;
			}
			else
			if ( img::isOver(pix00) || img::isOver(pix01)
			  || img::isOver(pix10) || img::isOver(pix11)
			   )
			{
				red = img::fpixOver;
				grn = img::fpixOver;
				blu = img::fpixOver;
			}
			else
			{
				Phase const phz(phaseAt(row0, col0));
				if (A_RG_GB == phz)
				{
					red = pix00;
					grn = .5f*(pix01 + pix10);
					blu = pix11;
				}
				else
				if (B_GR_BG == phz)
				{
					red = pix01;
					grn = .5f*(pix00 + pix11);
					blu = pix10;
				}
				else
				if (C_GB_RG == phz)
				{
					red = pix10;
					grn = .5f*(pix00 + pix11);
					blu = pix01;
				}
				else
				if (D_BG_GR == phz)
				{
					red = pix11;
					grn = .5f*(pix01 + pix10);
					blu = pix00;
				}
			}
		}

		return rgb;
	}

	//! Put value into boarder pixels
	void
	copyBorderPixels
		( dat::grid<fpix_t> * const ptGrid
		)
	{
		dat::grid<fpix_t> & grid = *ptGrid;

		size_t const eRow(grid.high()-1u);
		size_t const eCol(grid.wide()-1u);

		// set pixel on left and right edge
		for (size_t irow(0u); irow < grid.high(); ++irow)
		{
			grid(irow, eCol) = grid(irow, eCol-1u);
		}

		// set pixel on top and bottom edge
		for (size_t jcol(0u); jcol < grid.wide(); ++jcol)
		{
			grid(eRow, jcol) = grid(eRow-1u, jcol);
		}
	}

	//! Put value into boarder pixels
	void
	setBorderPixels
		( dat::grid<fpix_t> * const ptGrid
		, fpix_t const & value
		)
	{
		dat::grid<fpix_t> & grid = *ptGrid;

		size_t const bRow(0u);
		size_t const eRow(grid.high()-1u);
		size_t const bCol(0u);
		size_t const eCol(grid.wide()-1u);

		// set pixel on left and right edge
		for (size_t irow(0u); irow < grid.high(); ++irow)
		{
			grid(irow, bCol) = value;
			grid(irow, eCol) = value;
		}

		// set pixel on top and bottom edge
		for (size_t jcol(0u); jcol < grid.wide(); ++jcol)
		{
			grid(bRow, jcol) = value;
			grid(eRow, jcol) = value;
		}
	}
}

std::array<dat::grid<fpix_t>, 3u>
rgbFastFromRGGB
	( dat::grid<fpix_t> const & rawGrid
	, std::array<fpix_t, 3u> const & rgbGainFactors
	, bool const & setEdgeToNull
	)
{
	std::array<dat::grid<fpix_t>, 3u> rgbGrids;

	if (rawGrid.isValid())
	{
		// verify consistent inputs
		dat::Extents const inSize(rawGrid.hwSize());
		assert(2u < inSize.high());
		assert(2u < inSize.wide());

		// allocate space
		rgbGrids[0] = dat::grid<fpix_t>(inSize);
		rgbGrids[1] = dat::grid<fpix_t>(inSize);
		rgbGrids[2] = dat::grid<fpix_t>(inSize);

		// access input data
		dat::grid<fpix_t> & rGrid = rgbGrids[0];
		dat::grid<fpix_t> & gGrid = rgbGrids[1];
		dat::grid<fpix_t> & bGrid = rgbGrids[2];

		// not needed (other than tests) - every pixel is filled explicitly
		/*
		std::fill(rGrid.begin(), rGrid.end(), 0.f);
		std::fill(gGrid.begin(), bGrid.end(), 0.f);
		std::fill(gGrid.begin(), bGrid.end(), 0.f);
		*/

		// process each pixel
		size_t const lastRow(inSize.high() - 1u);
		size_t const lastCol(inSize.wide() - 1u);
		for (size_t jrow(0u) ; jrow < lastRow ; ++jrow)
		{
			for (size_t icol(0u) ; icol < lastCol ; ++icol)
			{
				std::array<fpix_t, 3u> const rgb(rgbAt(jrow, icol, rawGrid));
				rGrid(jrow, icol) = rgbGainFactors[0]* rgb[0];
				gGrid(jrow, icol) = rgbGainFactors[1]* rgb[1];
				bGrid(jrow, icol) = rgbGainFactors[2]* rgb[2];
			}
		}
		// Set outside border to nan since not fully interpolated there
		if (setEdgeToNull)
		{
			setBorderPixels(& rGrid, fpixBad);
			setBorderPixels(& gGrid, fpixBad);
			setBorderPixels(& bGrid, fpixBad);
		}
		else
		{
			copyBorderPixels(& rGrid);
			copyBorderPixels(& gGrid);
			copyBorderPixels(& bGrid);
		}
	}

	return rgbGrids;
}

dat::grid<fpix_t>
grayFastFrom2x2
	( dat::grid<fpix_t> const & rawGrid
	, std::array<fpix_t, 3u> const & rgbGainFactors
	, bool const & setEdgeToNull
	)
{
	dat::grid<fpix_t> grayGrid;

	if (rawGrid.isValid())
	{
		// verify consistent inputs
		dat::Extents const inSize(rawGrid.hwSize());
		assert(2u < inSize.high());
		assert(2u < inSize.wide());

		// allocate space
		grayGrid = dat::grid<fpix_t>(inSize);

		// compute channel weights - e.g. as simple average
		std::array<fpix_t, 3u> const chanWeights
			{{ (rgbGainFactors[0] / 3.f)
			 , (rgbGainFactors[1] / 3.f)
			 , (rgbGainFactors[2] / 3.f)
			}};

		// process each pixel
		size_t const lastRow(inSize.high() - 1u);
		size_t const lastCol(inSize.wide() - 1u);
		for (size_t jrow(0u) ; jrow < lastRow ; ++jrow)
		{
			for (size_t icol(0u) ; icol < lastCol ; ++icol)
			{
				std::array<fpix_t, 3u> const rgb(rgbAt(jrow, icol, rawGrid));
				grayGrid(jrow, icol) = 
					{ chanWeights[0]* rgb[0]
					+ chanWeights[1]* rgb[1]
					+ chanWeights[2]* rgb[2]
					};
			}
		}
		// Set outside border to nan since not fully interpolated there
		if (setEdgeToNull)
		{
			setBorderPixels(& grayGrid, fpixBad);
		}
		else
		{
			copyBorderPixels(& grayGrid);
		}
	}

	return grayGrid;
}


//======================================================================
}
}


