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
\brief  This file contains unit test for geo::LineSeg
*/


#include "libgeo/LineSeg.h"

#include "libga/ga.h"
#include "libga/Rigid.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
geo_LineSeg_test0
	()
{
	std::ostringstream oss;
	geo::LineSeg const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check basic properties
std::string
geo_LineSeg_test1
	()
{
	std::ostringstream oss;

	ga::Vector const expStart{ -5., 3., 7. };
	ga::Vector const someVec{ -5., 7., 2. };
	ga::Vector const expDir{ ga::unit(someVec) };
	double const length{ 2.75 };

	// valid construction
	geo::Ray const ray{ expStart, expDir };
	geo::LineSeg const seg{ ray, length };
	if (! seg.isValid())
	{
		oss << "Failure of validity test" << std::endl;
	}

	// line direction
	ga::Vector const gotDir{ seg.direction() };
	if (! gotDir.nearlyEquals(expDir))
	{
		oss << "Failure of direction test" << std::endl;
	}

	// start point
	ga::Vector const gotStart{ seg.pointAtFraction(0.) };
	if (! gotStart.nearlyEquals(expStart))
	{
		oss << "Failure of start point test" << std::endl;
	}

	// end point test
	ga::Vector const expEnd{ expStart + length*expDir };
	{
		ga::Vector const gotEnd{ seg.pointAtFraction(1.) };
		if (! gotEnd.nearlyEquals(expEnd))
		{
			oss << "Failure of frac end point test" << std::endl;
		}
	}
	{
		ga::Vector const gotEnd{ seg.stop() };
		if (! gotEnd.nearlyEquals(expEnd))
		{
			oss << "Failure of end func point test" << std::endl;
		}
	}

	return oss.str();
}


//! Check fractional operations
std::string
geo_LineSeg_test2
	()
{
	std::ostringstream oss;

	ga::Rigid const xTest
		( ga::Vector(-17., 11., 13)
		, ga::Pose(ga::BiVector(.2, -.4, .3))
		);
	ga::Vector const p0(  0., 0., 0. );
	ga::Vector const p1( 10., 0., 0. );
	geo::LineSeg const aSeg(geo::LineSeg::between(xTest(p0), xTest(p1)));

	using VecFrac = std::pair<ga::Vector, double>;
	std::vector<VecFrac> const nomPFs
		{ { ga::Vector( -5., 0., 0. ), -.5 }
		, {                        p0, 0.  }
		, { ga::Vector(  5., 0., 0. ),  .5 }
		, {                        p1, 1.  }
		, { ga::Vector( 15., 0., 0. ), 1.5 }
		};

	for (VecFrac const nomPF : nomPFs)
	{
		ga::Vector const & nomPnt = nomPF.first;
		double const & expFrac = nomPF.second;
		ga::Vector const aPnt(xTest(nomPnt));

		// check fraction computation
		double const gotFrac{ aSeg.fractionAtProjectionOf(aPnt) };
		if (! dat::nearlyEquals(gotFrac, expFrac))
		{
			oss << "Failure of fracProj test" << std::endl;
			oss << dat::infoString(expFrac, "expFrac") << std::endl;
			oss << dat::infoString(gotFrac, "gotFrac") << std::endl;
		}

		// check contains
		bool const expContains{ (0. <= expFrac) && (expFrac < 1.) };
		bool const gotContains{ aSeg.containsProjectionOf(aPnt) };
		if (! dat::nearlyEquals(gotContains, expContains))
		{
			oss << "failure of containsProj test" << std::endl;
			oss << dat::infoString(expContains, "expContains") << std::endl;
			oss << dat::infoString(gotContains, "gotContains") << std::endl;
		}
	}

	return oss.str();
}


}

//! Unit test for geo::LineSeg
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << geo_LineSeg_test0();
	oss << geo_LineSeg_test1();
	oss << geo_LineSeg_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
