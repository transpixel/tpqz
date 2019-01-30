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

#ifndef stb_testfunc_INCL_
#define stb_testfunc_INCL_

/*! \file
\brief Declarations for stb::testfunc
*/

#include "libdat/array.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libmath/math.h"

#include "libimg/convert.h"
#include "libdat/grid.h"

#include <array>
#include <cstddef>
#include <iostream>
#include <string>


namespace stb
{

/*! \brief Utilities for unit tests of basic image i/o

\par Example
\dontinclude teststb/utestfunc.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace testfunc
{
	using PixRGB = std::array<uint8_t, 3>;

	// Simulate an 8-bit RGB raster
	dat::grid<PixRGB>
	simRGB
		( size_t const & high
		, size_t const & wide
		)
	{
		dat::grid<PixRGB> grid(high, wide);
		for (size_t row{0u} ; row < high ; ++row)
		{
			for (size_t col{0u} ; col < wide ; ++col)
			{
				size_t const delta{ (10u * (row % 5u)) + (col % 7u) };
				size_t const red{   0u + delta };
				size_t const grn{ 100u + delta };
				size_t const blu{ 200u + delta };
				PixRGB const pix{ (uint8_t)red, (uint8_t)grn, (uint8_t)blu };
				grid(row,col) = pix;
			}
		}
		return grid;
	}

	size_t
	maxAbsPixDif
		( uint8_t const & pixA
		, uint8_t const & pixB
		)
	{
		long const lA{ static_cast<long>(pixA) };
		long const lB{ static_cast<long>(pixB) };
		long const lDif{ lA - lB };
		long const lMag{ std::abs(lDif) };
		return static_cast<size_t>(lMag);
	}

	//! Magnitude of largest difference between corresponding channel values
	size_t
	maxAbsPixDif
		( PixRGB const & pixA
		, PixRGB const & pixB
		)
	{
		using IPix = std::array<int, 3>;
		IPix const ipA{ (int)pixA[0], (int)pixA[1], (int)pixA[2] };
		IPix const ipB{ (int)pixB[0], (int)pixB[1], (int)pixB[2] };
		using dat::operator-;
		IPix const idiff{ ipA - ipB };
		int const maxAbs
			{ math::maxAbs<int>(idiff.begin(), idiff.end()) };
		return static_cast<size_t>(maxAbs);
	}

	//! Check if expGrid and gotGrid are the same (within tolPixDif)
	template <typename PixType>
	void
	checkGrids
		( std::ostream & oss
		, dat::grid<PixType> const & expGrid
		, dat::grid<PixType> const & gotGrid
		, std::string const & title
		, size_t const & tolPixDif
		)
	{
		assert(expGrid.size() == gotGrid.size()); // else above code is bad
		size_t maxPixDif{ 0u };
		for ( typename dat::grid<PixType>::const_iterator
				itExp{expGrid.begin()}, itGot{gotGrid.begin()}
			; expGrid.end() != itExp
			; ++itExp, ++itGot
			)
		{
			size_t const pixMagDif{ maxAbsPixDif(*itExp, *itGot) };
			if (maxPixDif < pixMagDif)
			{
				maxPixDif = pixMagDif;
			}
		}
		if (tolPixDif < maxPixDif)
		{
			oss << "Failure of " << title << " test" << std::endl;
			oss << dat::infoString(tolPixDif, "tolPixDif") << std::endl;
			oss << dat::infoString(maxPixDif, "maxPixDif") << std::endl;
		}
	}

	//! A grid<PixRGB> for imgdat data block after performing size tests
	dat::grid<PixRGB>
	checkedGridFrom
		( std::ostream & oss
		, int const & gotHigh
		, int const & gotWide
		, int const & gotDeep
		, unsigned char const * const imgdat
		, std::string const & title
		, dat::grid<PixRGB> const & expGrid
		)
	{
		dat::grid<PixRGB> grid;
		if (! imgdat)
		{
			oss << "Failure: " << title << "unable to load image" << std::endl;
		}
		else
		{
			bool okaySizes{ true };
			int const expDeep{ (int)PixRGB{}.size() };
			if (! (gotDeep == expDeep))
			{
				okaySizes = false;
				oss << "Failure of channel deep check: " << title << std::endl;
				oss << dat::infoString(expDeep, "exp") << std::endl;
				oss << dat::infoString(gotDeep, "got") << std::endl;
			}

			dat::Extents const expHW{ expGrid.hwSize() };
			dat::Extents const gotHW(gotHigh, gotWide);
			if (! (gotHW == expHW))
			{
				okaySizes = false;
				oss << "Failure of hwSize check: " << title << std::endl;
				oss << dat::infoString(expHW, "expHW") << std::endl;
				oss << dat::infoString(gotHW, "gotHW") << std::endl;
			}

			if (okaySizes)
			{
				// copy image for compare later
				grid = img::convert::gridRgb8From(gotHigh, gotWide, imgdat);
			}

		}
		return grid;
	}


} // testfunc

} // stb

// Inline definitions
// #include "libstb/testfunc.inl"

#endif // stb_testfunc_INCL_

