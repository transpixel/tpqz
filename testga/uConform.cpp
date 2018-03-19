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
\brief  This file contains unit test for ga::Conform
*/


#include "libga/Conform.h"

#include "libdat/compare.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace
{

//! Check for common functions
std::string
ga_Conform_test0
	()
{
	std::ostringstream oss;
	ga::Conform const aNull;
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check basic operations
std::string
ga_Conform_test1
	()
{
	std::ostringstream oss;

	std::vector<ga::Vector> const expPntsInA
		{ ga::Vector( -3., -4., -5.)
		, ga::Vector(  4., -7., -1.)
		, ga::Vector( -5.,  2., -5.)
		, ga::Vector( -3., -4.,  3.)
		};

	double const expScale{ 2.75 };
	ga::Vector const expTrans(10., 20., 30.);
	ga::BiVector const expAngle(.1, .2, .3);
	ga::Rigid const expRigid(expTrans, ga::Pose(expAngle));
	ga::Conform const conformBwA(expScale, expRigid);

	std::vector<ga::Vector> expPntsInB;
	std::vector<ga::Vector> gotPntsInB;
	for (ga::Vector const & expPntInA : expPntsInA)
	{
		expPntsInB.emplace_back( expScale * expRigid(expPntInA) );
		gotPntsInB.emplace_back( conformBwA(expPntInA) );
	}

	if (! (gotPntsInB.size() == expPntsInB.size()))
	{
		oss << "Failure of forward transform size test" << std::endl;
	}
	for (size_t nn{0u} ; nn < expPntsInB.size() ; ++nn)
	{
		ga::Vector const & expPntInB = expPntsInB[nn];
		ga::Vector const & gotPntInB = gotPntsInB[nn];
		if (! gotPntInB.nearlyEquals(expPntInB))
		{
			oss << "Failure of forward point value test: nn = " << nn
				<< std::endl;
		}
	}

	//! get inverse transform
	ga::Conform const conformAwB{ conformBwA.inverse() };

	std::vector<ga::Vector> gotPntsInA;
	for (ga::Vector const & expPntInB : expPntsInB)
	{
		gotPntsInA.emplace_back(conformAwB(expPntInB));
	}

	if (! (gotPntsInA.size() == expPntsInA.size()))
	{
		oss << "Failure of inverse transform size test" << std::endl;
	}
	for (size_t nn{0u} ; nn < expPntsInA.size() ; ++nn)
	{
		ga::Vector const & expPntInA = expPntsInA[nn];
		ga::Vector const & gotPntInA = gotPntsInA[nn];
		if (! gotPntInA.nearlyEquals(expPntInA))
		{
			oss << "Failure of inverse point value test: nn = " << nn
				<< std::endl;
		}
	}

	return oss.str();
}

//! Check composition operations
std::string
ga_Conform_test2
	()
{
	std::ostringstream oss;

	// define a first transform
	double const sclBwA{ 0.25 };
	ga::Vector const locBwA( -7., 4., -5.);
	ga::BiVector const angBwA( -.4, .3, .2);
	ga::Conform const xBwA(sclBwA, locBwA, angBwA);

	// define a second transform
	double const sclCwB{ 0.125 };
	ga::Vector const locCwB( 8., -3., -1.);
	ga::BiVector const angCwB( .1, -.2, -.3);
	ga::Conform const xCwB(sclCwB, locCwB, angCwB);

	// compute composite
	ga::Conform const xCwA{ xCwB * xBwA };
	ga::Conform const xAwC{ xCwA.inverse() };

	// use a collection of linearly independent points to test
	std::vector<ga::Vector> const pntsInA
		{ ga::vZero, ga::e1, ga::e2, ga::e3, locBwA, locCwB };
	for (ga::Vector const & pntInA : pntsInA)
	{
		ga::Vector const expPntInC{ xCwB(xBwA(pntInA)) };
		ga::Vector const gotPntInC{ xCwA(pntInA) };

		if (! gotPntInC.nearlyEquals(expPntInC))
		{
			oss << "Failure of forward composite transform test" << std::endl;
			oss << dat::infoString(expPntInC, "expPntInC") << std::endl;
			oss << dat::infoString(gotPntInC, "gotPntInC") << std::endl;
		}

		ga::Vector const & expPntInA = pntInA;
		ga::Vector const gotPntInA{ xAwC(expPntInC) };

		if (! gotPntInA.nearlyEquals(expPntInA))
		{
			oss << "Failure of inverse composite transform test" << std::endl;
			oss << dat::infoString(expPntInA, "expPntInA") << std::endl;
			oss << dat::infoString(gotPntInA, "gotPntInA") << std::endl;
		}
	}

	return oss.str();
}

}

//! Unit test for ga::Conform
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ga_Conform_test0();
	oss << ga_Conform_test1();
	oss << ga_Conform_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
