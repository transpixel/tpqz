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
\brief  This file contains unit test for img::geo
*/


#include "libimg/geo.h"

#include "libdat/ExtentsIterator.h"
#include "libdat/RowCol.h"
#include "libimg/io.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
img_geo_test0
	()
{
	std::ostringstream oss;
	/*
	img::geo const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	*/
	return oss.str();
}

	//! Simulated data grid
	dat::grid<short>
	simGrid
		( dat::Extents const & size
		)
	{
		dat::grid<short> agrid(size);
		for (dat::ExtentsIterator iter(agrid.hwSize()) ; iter ; ++iter)
		{
			dat::RowCol const & rowcol = *iter;
			agrid(rowcol) = 100u + 10u*rowcol[0] + rowcol[1];
		}
		return agrid;
	}

	//! Simulated data grid
	dat::grid<double>
	simSub1
		( dat::SubExtents * const & ptCrop
		)
	{
		dat::RowCol const ul{{ 1u, 2u }};
		dat::grid<double> acrop(3u, 2u);

		size_t row{ 0u };
		acrop(row, 0u) = 112.;
		acrop(row, 1u) = 113.;
		++row;

		acrop(row, 0u) = 122.;
		acrop(row, 1u) = 123.;
		++row;

		acrop(row, 0u) = 132.;
		acrop(row, 1u) = 133.;
		++row;

		*ptCrop = dat::SubExtents(ul, acrop.hwSize());

		return acrop;
	}

	//! Simulated data grid
	dat::grid<double>
	simSub2
		( dat::SubExtents * const & ptCrop
		)
	{
		dat::RowCol const ul{{ 3u, 4u }};
		dat::grid<double> acrop(2u, 1u);

		size_t row{ 0u };
		acrop(row, 0u) = 134u;
		++row;

		acrop(row, 0u) = 144u;
		++row;

		*ptCrop = dat::SubExtents(ul, acrop.hwSize());

		return acrop;
	}

	//! Simulated data grid
	dat::grid<double>
	simSub3
		( dat::Extents * const & ptROI
		)
	{
		dat::grid<double> acrop(4u, 4u);
		*ptROI = acrop.hwSize();

		acrop(0u, 0u) = 122u;
		acrop(0u, 1u) = 123u;
		acrop(0u, 2u) = 124u;
		acrop(0u, 3u) = 125u;

		acrop(1u, 0u) = 132u;
		acrop(1u, 1u) = 133u;
		acrop(1u, 2u) = 134u;
		acrop(1u, 3u) = 135u;

		acrop(2u, 0u) = 142u;
		acrop(2u, 1u) = 143u;
		acrop(2u, 2u) = 144u;
		acrop(2u, 3u) = 145u;

		acrop(3u, 0u) = 152u;
		acrop(3u, 1u) = 153u;
		acrop(3u, 2u) = 154u;
		acrop(3u, 3u) = 155u;

		return acrop;
	}

	bool
	areSame
		( dat::grid<double> const & got
		, dat::grid<double> const & exp
		)
	{
		return dat::nearlyEquals<double>
			( got.begin(), got.end()
			, exp.begin(), exp.end()
			);
	}

//! Check crop operations
std::string
img_geo_test1
	()
{
	std::ostringstream oss;

	// check general crop and fill
	{
		using FullType = short;
		using CropType = double;

		dat::Extents const someSize(8u, 9u);
		dat::grid<FullType> const fullgrid(simGrid(someSize));
		dat::SubExtents crop;
		dat::grid<CropType> const expSub(simSub1(&crop));
		dat::grid<CropType> const gotSub
			(img::geo::croppedGrid<CropType, FullType>(fullgrid, crop));
		if (! areSame(gotSub, expSub))
		{
			oss << "Failure of simSub1 crop test" << std::endl;
			oss << fullgrid.infoStringContents("fullgrid", "%7d") << '\n';
			oss << expSub.infoStringContents("expSub", "%7d") << '\n';
			oss << gotSub.infoStringContents("gotSub", "%7d") << '\n';
		}

		// fill a blank grid with the crop area
		dat::grid<FullType> tmpgrid(fullgrid.hwSize());
		std::fill(tmpgrid.begin(), tmpgrid.end(), dat::nullValue<FullType>());
		img::geo::insertSubGrid(&tmpgrid, crop, gotSub);

		// re-crop the area - check match to original crop
		dat::grid<CropType> const reSub
			(img::geo::croppedGrid<CropType, FullType>(tmpgrid, crop));
		if (! areSame(reSub, gotSub))
		{
			oss << "Failure of fill from crop test" << std::endl;
			oss << tmpgrid.infoStringContents("tmpgrid", "%7d") << '\n';
			oss << gotSub.infoStringContents("gotSub", "%7d") << '\n';
			oss << reSub.infoStringContents("reSub", "%7d") << '\n';
		}

	}

	// check center crop for even/odd size
	{
		dat::Extents const someSize(8u, 9u);
		dat::grid<short> const fullgrid(simGrid(someSize));
		dat::SubExtents crop;
		dat::grid<double> const expSub(simSub2(&crop));
		dat::grid<double> const gotSub
			( img::geo::centerCroppedGrid<double, short>
				(fullgrid, crop.theSize)
			);
		if (! areSame(gotSub, expSub))
		{
			oss << "Failure of simSub2 crop test" << std::endl;
			oss << fullgrid.infoStringContents("fullgrid", "%7d") << '\n';
			oss << expSub.infoStringContents("expSub", "%7d") << '\n';
			oss << gotSub.infoStringContents("gotSub", "%7d") << '\n';
		}
	}

	// check center crop for even sized grid (like a cfa image)
	{
		dat::Extents const someSize(8u, 8u);
		dat::grid<short> const fullgrid(simGrid(someSize));
		dat::Extents roi;
		dat::grid<double> const expSub(simSub3(&roi));
		dat::grid<double> const gotSub
			(img::geo::centerCroppedGrid<double, short>(fullgrid, roi));
		if (! areSame(gotSub, expSub))
		{
			oss << "Failure of simSub2 crop test" << std::endl;
			oss << fullgrid.infoStringContents("fullgrid", "%7d") << '\n';
			oss << expSub.infoStringContents("expSub", "%7d") << '\n';
			oss << gotSub.infoStringContents("gotSub", "%7d") << '\n';
		}
	}

	return oss.str();
}

//! Check montage operations
std::string
img_geo_test2
	()
{
	std::ostringstream oss;

	dat::grid<uint8_t> const tile(17u, 36u, 127u);
	std::vector<dat::grid<uint8_t> > const tiles(7u, tile);

	constexpr size_t numPerRow{ 4u };
	constexpr size_t pad{ 2u };
	constexpr uint8_t backVal{ 100u };
	constexpr uint8_t foreVal{ 200u };
	dat::grid<uint8_t> const montage
		{ img::geo::montageOf
			(tiles.begin(), tiles.end(), numPerRow, backVal, pad, foreVal)
		};

	size_t const expHigh
		{ pad
		+ tile.high()
		+ pad
		+ pad
		+ tile.high()
		+ pad
		};
	size_t const expWide
		{ pad + tile.wide() + pad
		+ pad + tile.wide() + pad
		+ pad + tile.wide() + pad
		+ pad + tile.wide() + pad
		};
	dat::Extents const expSize(expHigh, expWide);
	dat::Extents const gotSize{ montage.hwSize() };

	if (! (gotSize == expSize))
	{
		oss << "Failure of montage pad size test" << std::endl;
		oss << dat::infoString(expSize, "expSize") << std::endl;
		oss << dat::infoString(gotSize, "gotSize") << std::endl;
	}

	// img::io::savePng(montage, "montage.png");

	return oss.str();
}


}

//! Unit test for img::geo
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << img_geo_test0();
	oss << img_geo_test1();
	oss << img_geo_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
