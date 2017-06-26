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
\brief  This file contains unit test for cam::PinHole
*/


#include "libcam/PinHole.h"

#include "libga/functions.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
cam_PinHole_test0
	()
{
	std::ostringstream oss;
	cam::PinHole const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check basic operations
std::string
cam_PinHole_test1
	()
{
	std::ostringstream oss;

	// perspective projection
	ga::Vector const objpnt(7., 5., -2.);
	double const pd(.25);
	cam::PinHole const camera(pd);

	// check direction computations
	ga::Vector const expdir(ga::unit(objpnt));
	ga::Vector const gotdir(camera.directionTo(objpnt));
	if (! gotdir.nearlyEquals(expdir))
	{
		oss << "Failure of sensor projected direction test" << std::endl;
	}

	// check detector location
	double const zdist(-ga::dot(objpnt, ga::e3).theValue);
	double const sf(pd / zdist);
	dat::Spot const expspot{{ sf*objpnt[0], sf*objpnt[1] }};
	dat::Spot const gotspot(camera.imageSpotFor(objpnt));
	if (! dat::nearlyEquals(gotspot, expspot))
	{
		oss << "Failure of detector location test" << std::endl;
	}

	// return path
	ga::Vector const rtndir(camera.directionOf(expspot));
	if (! rtndir.nearlyEquals(expdir))
	{
		oss << "Failure of sensor restored direction test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for cam::PinHole
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << cam_PinHole_test0();
	oss << cam_PinHole_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
