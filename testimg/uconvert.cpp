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
\brief  This file contains unit test for img::convert
*/


#include "libimg/convert.h"

#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

/*
//! Check for common functions
std::string
img_convert_test0
	()
{
	std::ostringstream oss;
	img::convert const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}
*/

	//! Fill image offset-pattern
	dat::grid<uint8_t>
	aGridLayer
		( dat::Extents const & area
		, uint8_t const & offset
		)
	{
		dat::grid<uint8_t> grid(area);
		for (dat::grid<uint8_t>::iterator
			iter(grid.begin()) ; grid.end() != iter ; ++iter)
		{
			ptrdiff_t const delta(iter - grid.begin());
			*iter = offset + delta;
		}
		return grid;
	}

	//! Fill color channel with offset-pattern
	cv::Mat
	aCvMat
		( dat::Extents const & area
		, uint8_t const & r0
		, uint8_t const & g0
		, uint8_t const & b0
		)
	{
		cv::Mat mat(area.high(), area.wide(), CV_8UC3);
		uint8_t delta(0u);
		for (size_t row(0u) ; row < area.high() ; ++row)
		{
			for (size_t col(0u) ; col < area.wide() ; ++col)
			{
				mat.ptr<uint8_t>(row, col)[0] = r0 + delta;
				mat.ptr<uint8_t>(row, col)[1] = g0 + delta;
				mat.ptr<uint8_t>(row, col)[2] = b0 + delta;
				++delta;
			}
		}

		return mat;
	}

	// true if mat1 and mat2 are identical
	bool
	areSame
		( cv::Mat const & mat1
		, cv::Mat const & mat2
		)
	{
		bool same(mat1.size() == mat2.size());
		if (same)
		{
			same = std::equal
				( mat1.begin<uint8_t>(), mat1.end<uint8_t>() 
				, mat2.begin<uint8_t>()
				);
		}
		return same;
	}

//! Check OpenCV matrix conversion
std::string
img_convert_test1
	()
{
	std::ostringstream oss;

	//
	// Check mult-chanel image conversion
	//

	// generate a simple image
	dat::Extents const area(3u, 2u);
	constexpr uint8_t red( 10u);
	constexpr uint8_t grn(110u);
	constexpr uint8_t blu(210u);
	std::array<dat::grid<uint8_t>, 3u> const bands
		{{ aGridLayer(area, red)
		 , aGridLayer(area, grn)
		 , aGridLayer(area, blu)
		}};

	// fill opencvs struct with expected pattern
	cv::Mat const expMat(aCvMat(area, blu, grn, red));

	// convert dat structure to opencv format
	cv::Mat const gotMat(img::convert::cvMatFromBands(bands));

	//! check that results match
	if (! areSame(gotMat, expMat))
	{
		oss << "Failure of cvMatFromBands multi-band test" << std::endl;
		oss << "expMat:\n" << expMat << std::endl;
		oss << "gotMat:\n" << gotMat << std::endl;
	}

	//
	// check single channel conversion
	//

	// get a channel from the expected image
	size_t const ndx(1u); // use green since same offset in both data structs
	cv::Mat copyMat(expMat.clone());
	cv::Mat expChans[3];
	cv::split(copyMat, expChans);
	cv::Mat expChan(expChans[ndx]);

	// exercise grid conversion
	cv::Mat const gotChan(img::convert::cvMatFromGrid(bands[ndx]));

	// check results
	if (! areSame(gotChan, expChan))
	{
		oss << "Failure of cvMatFromGrid single-band test" << std::endl;
		oss << "expChan:\n" << expChan << std::endl;
		oss << "gotChan:\n" << gotChan << std::endl;
	}

	return oss.str();
}


}

//! Unit test for img::convert
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
//	oss << img_convert_test0();
	oss << img_convert_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
