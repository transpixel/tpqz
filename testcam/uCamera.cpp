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
\brief  This file contains unit test for cam::Camera
*/


#include "libcam/Camera.h"

#include "libdat/cast.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
cam_Camera_test0
	()
{
	std::ostringstream oss;
	cam::Camera const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check basic projection operations
std::string
cam_Camera_test1
	()
{
	std::ostringstream oss;

	// classic projection case
	double const camPD{ 100. };
	dat::Extents const detSize(100u, 100u);
	cam::Camera const camera(camPD, detSize);

	ga::Vector const objPnt( 1.1, 2.2, -10.);
	dat::Spot const expImgSpot{{ 11., 22. }};
	dat::Spot const expDetSpot{{ (50.+expImgSpot[0]), (50.+expImgSpot[1]) }};
	dat::RowCol const expDetRC(dat::cast::RowCol(expDetSpot));

	// check basic perspective projection
	{
		dat::Spot const gotImgSpot(camera.imageSpotFor(objPnt));
		if (! dat::nearlyEquals(gotImgSpot, expImgSpot))
		{
			oss << "Failure of forward image projection test" << std::endl;
		}
	}

	// check detector from image relationship
	{
		dat::Spot const gotDetSpot(camera.detectorSpotFor(expImgSpot));
		if (! dat::nearlyEquals(gotDetSpot, expDetSpot))
		{
			oss << "Failure of detSpot from image spot test" << std::endl;
		}
	}

	// check image from detector
	{
		dat::Spot const gotImgSpot(camera.imageSpotFor(expDetSpot));
		if (! dat::nearlyEquals(gotImgSpot, expImgSpot))
		{
			oss << "Failure of roundtrip image projection test" << std::endl;
		}
	}

	// check detector from object point
	{
		dat::RowCol const gotDetRC(camera.detectorRowColFor(expImgSpot));
		if (! dat::nearlyEquals(gotDetRC, expDetRC))
		{
			oss << "Failure of detRC from image spot test" << std::endl;
		}
	}

	// check out of bounds conversion attempts
	constexpr bool isOkay{ true };
	constexpr bool isBad{ false };
	ga::Vector const darkPnt(-10., 40., -5);
	dat::Spot const darkImgSpot(camera.imageSpotFor(darkPnt));
	dat::Spot const badDetSpot{{ -1., 100. }};

	ga::Vector const & goodPnt = objPnt;
	dat::Spot const & goodImgSpot = expImgSpot;
	dat::Spot const & goodDetSpot = expDetSpot;

	/*
x	imageSpotFor(objpnt)
x	detectorRowColFor(objpnt)
x	detectorSpotFor(imgspot)
x	imageSpotFor(detspot)
	*/


	// image projections valid (even if off format)
	if (! (isOkay == dat::isValid(camera.imageSpotFor(goodPnt))))
	{
		oss << "Failure of imageSpotFor(goodPnt) test" << std::endl;
	}
	if (! (isOkay == dat::isValid(camera.imageSpotFor(darkPnt))))
	{
		oss << "Failure of imageSpotFor(darkPnt) test" << std::endl;
	}

	// detector values must always be in format
	if (! (isOkay == dat::isValid(camera.detectorRowColFor(goodPnt))))
	{
		oss << "Failure of detectorRowColFor(goodPnt) test" << std::endl;
	}
	if (! (isBad == dat::isValid(camera.detectorRowColFor(darkPnt))))
	{
		oss << "Failure of detectorRowColFor(darkPnt) test" << std::endl;
	}

	// detector values must always be in format
	if (! (isOkay == dat::isValid(camera.detectorSpotFor(goodImgSpot))))
	{
		oss << "Failure of detectorSpotFor(goodImgSpot) test" << std::endl;
	}
	if (! (isBad == dat::isValid(camera.detectorSpotFor(darkImgSpot))))
	{
		oss << "Failure of detectorSpotFor(darkImgSpot) test" << std::endl;
	}

	// image spot from detector okay with out of bound (but valid) input
	if (! (isOkay == dat::isValid(camera.imageSpotFor(goodDetSpot))))
	{
		oss << "Failure of imageSpotFor(goodDetSpot) test" << std::endl;
	}
	if (! (isOkay == dat::isValid(camera.imageSpotFor(badDetSpot))))
	{
		oss << "Failure of imageSpotFor(badDetSpot) test" << std::endl;
	}

	// check visibility test
	if (! camera.isVisible(expImgSpot))
	{
		oss << "Failure of isVisible(expImgSpot) test" << std::endl;
	}
	if (  camera.isVisible(darkImgSpot))
	{
		oss << "Failure of isVisible(darkImgSpot) test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for cam::Camera
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << cam_Camera_test0();
	oss << cam_Camera_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
