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
\brief  This file contains unit test for ext::stb
*/


#include "extstb/stb_image.h"
#include "extstb/stb_image_write.h"

#include "libdat/array.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"
#include "libmath/math.h"

#include "libdat/grid.h"

#include <array>
#include <iostream>
#include <sstream>
#include <string>


namespace
{
	using PixRGB = std::array<uint8_t, 3>;

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

	void
	compareGrids
		( std::ostream & oss
		, dat::grid<PixRGB> const & expGrid
		, dat::grid<PixRGB> const & gotGrid
		, std::string const & title
		, size_t const & tolPixDif
		)
	{
		assert(expGrid.size() == gotGrid.size()); // else above code is bad
		size_t maxPixDif{ 0u };
		for ( dat::grid<PixRGB>::const_iterator
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

	dat::grid<PixRGB>
	gridFrom
		( int const & gotHigh
		, int const & gotWide
		, unsigned char const * const rgb8
		)
	{
		assert(0 < gotHigh);
		assert(0 < gotWide);
		dat::grid<PixRGB> grid((size_t)gotHigh, (size_t)gotWide);

		// copy image for compare later
		unsigned char const * ptIn{ rgb8 };
		dat::grid<PixRGB>::iterator itOut{ grid.begin() };
		for (size_t nn{0u} ; nn < grid.size() ; ++nn)
		{
			uint8_t const red{ *ptIn++ };
			uint8_t const grn{ *ptIn++ };
			uint8_t const blu{ *ptIn++ };
			*itOut++ = PixRGB{ red, grn, blu };
		}

		return grid;
	}

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
				grid = gridFrom(gotHigh, gotWide, imgdat);
			}

		}
		return grid;
	}

//! Check JPEG
std::string
img_stb_test0
	()
{
	std::ostringstream oss;

	constexpr size_t const tolPixDif{ 1u }; // jpeg compression loss

	std::string const fpath{ "ustb_foo.jpg" };

	// simulate image
	constexpr int const expHigh{ 73 };
	constexpr int const expWide{ 35 };
	constexpr int const expDeep{ 3 };
	dat::grid<PixRGB> const expGrid{ simRGB(expHigh, expWide) };
	dat::grid<PixRGB> gotGrid;

	// write to file
	constexpr int const expQual{ 100 };
	int const wstat
		{ stbi_write_jpg
			(fpath.c_str(), expWide, expHigh, expDeep, expGrid.begin(), expQual)
		};

	// check write status
	bool const okayWrite{ (0 < wstat) };
	if (! okayWrite)
	{
		oss << "Failure of stbi_write_jpg test" << std::endl;
		oss << "wstat = " << wstat << std::endl;
	}

	else // if (okayWrite); have test image in file
	{
		// read jpeg image
		int gotWide{}, gotHigh{}, gotDeep{};
		unsigned char * imgdat
			{ stbi_load(fpath.c_str(), &gotWide, &gotHigh, &gotDeep, expDeep) };
		gotGrid = checkedGridFrom
			(oss, gotHigh, gotWide, gotDeep, imgdat, "JPG reload", expGrid);
		stbi_image_free(imgdat);
	}

	// check contents of loaded data
	bool const okayReadBack{ gotGrid.isValid() };
	if (okayWrite && okayReadBack)
	{
		assert(expGrid.size() == gotGrid.size()); // else above code is bad
		compareGrids(oss, expGrid, gotGrid, "jpg max pix dif", tolPixDif);
	}

	// cleanup test data
	std::remove(fpath.c_str());

	return oss.str();
}

//! Check PNG image i/o
std::string
img_stb_test1
	()
{
	std::ostringstream oss;

	constexpr size_t const tolPixDif{ 0u }; // png lossless encoding

	std::string const fpath{ "ustb_foo.png" };

	// simulate image
	constexpr int const expHigh{ 73 };
	constexpr int const expWide{ 35 };
	constexpr int const expDeep{ 3 };
	dat::grid<PixRGB> const expGrid{ simRGB(expHigh, expWide) };
	dat::grid<PixRGB> gotGrid;

	// write to file
	stbi_write_png_compression_level = 9;
	int const rowBytes{ expWide * expDeep * sizeof(uint8_t) };
	void const * const ptData{ expGrid.begin() };
	int const wstat
		{ stbi_write_png
			(fpath.c_str(), expWide, expHigh, expDeep, ptData, rowBytes)
		};

	// check write status
	bool const okayWrite{ (0 < wstat) };
	if (! okayWrite)
	{
		oss << "Failure of stbi_write_png test" << std::endl;
		oss << "wstat = " << wstat << std::endl;
	}

	else // if (okayWrite); have test image in file
	{
		// read jpeg image
		int gotWide{}, gotHigh{}, gotDeep{};
		unsigned char * imgdat
			{ stbi_load(fpath.c_str(), &gotWide, &gotHigh, &gotDeep, expDeep) };
		gotGrid = checkedGridFrom
			(oss, gotHigh, gotWide, gotDeep, imgdat, "PNG reload", expGrid);
		stbi_image_free(imgdat);
	}

	// check contents of loaded data
	bool const okayReadBack{ gotGrid.isValid() };
	if (okayWrite && okayReadBack)
	{
		assert(expGrid.size() == gotGrid.size()); // else above code is bad
		compareGrids(oss, expGrid, gotGrid, "png max pix dif", tolPixDif);
	}

	// cleanup test data
	std::remove(fpath.c_str());

	return oss.str();
}


}

//! Unit test for ext::stb
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << img_stb_test0();
	oss << img_stb_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
