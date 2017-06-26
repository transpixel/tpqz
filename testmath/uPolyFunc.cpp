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
\brief  This file contains unit test for math::PolyFunc
*/


#include "libmath/PolyFunc.h"

#include "libio/stream.h"
#include "libmath/Partition.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
math_PolyFunc_test0
	()
{
	std::ostringstream oss;
	math::PolyFunc const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check basic operations
std::string
math_PolyFunc_test1
	()
{
	std::ostringstream oss;

	std::vector<dat::Spot> verts;
	verts.push_back(dat::Spot{ 10.000, 100.00 });
	verts.push_back(dat::Spot{ 10.125, 101.25 });
	verts.push_back(dat::Spot{ 12.000, 120.00 });
	verts.push_back(dat::Spot{ 11.000, 102.00 });
	verts.push_back(dat::Spot{ 11.125, 110.00 });

	// create a forward function
	math::PolyFunc const fwdPoly(verts.begin(), verts.end());

	// create an inverse function
	math::PolyFunc const invPoly(fwdPoly.yxVertices());

	/*
	io::out() << fwdPoly.infoStringContents("fwdPoly") << std::endl;
	io::out() << invPoly.infoStringContents("invPoly") << std::endl;
	*/

	// check a few round trip values
	math::Partition const sampPart(fwdPoly.range(), 37u);
	for (size_t nn(0u) ; nn <= sampPart.size() ; ++nn)
	{
		double const expX(sampPart.interpValueFor(double(nn)));
		double const gotY(fwdPoly(expX));
		double const gotX(invPoly(gotY));
		if (! dat::nearlyEquals(gotX, expX))
		{
			oss << "Failure of round trip test" << std::endl;
			oss << dat::infoString(expX, "expX") << std::endl;
			oss << dat::infoString(gotY, "gotY") << std::endl;
			oss << dat::infoString(gotX, "gotX") << std::endl;
			break;
		}
	}

	return oss.str();
}


}

//! Unit test for math::PolyFunc
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << math_PolyFunc_test0();
	oss << math_PolyFunc_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
