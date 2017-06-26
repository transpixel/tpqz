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
\brief  This file contains unit test for ro::QuadForm
*/


#include "libro/QuadForm.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libga/ga.h"
#include "libro/PairRel.h"

#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>


namespace
{

//! Check for common functions
std::string
ro_QuadForm_test0
	()
{
	std::ostringstream oss;
	ro::QuadForm const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

static std::random_device sRandDev;
static std::mt19937_64 sRandGen(sRandDev());
static bool sIsInit{ false };

ga::Vector
simPoint
	()
{
	if (! sIsInit)
	{
		sRandGen.seed(98343u);
		sIsInit = true;
	}
	std::uniform_real_distribution<double> rdistro(1., 2.);
	return ga::Vector
		(rdistro(sRandGen), rdistro(sRandGen), rdistro(sRandGen));
}

ga::Vector
simDir
	()
{
	return ga::unit(simPoint());
}


//! Check consistency with RO triple product
std::string
ro_QuadForm_test1
	()
{
	std::ostringstream oss;

	ro::PairRel const roNull;

	// define test configuration
	ga::Vector const base2w1(-5., 3., 2.);
	ga::BiVector const angle2w1( -3.3, 1.2, 2.5);
	ro::PairRel const roExp
		( ga::Rigid::identity()
		, ga::Rigid(base2w1, ga::Pose(angle2w1))
		);

	// construct test object
	ro::QuadForm const quad(roExp.pair());

	// compare QuadForm with roExp for a bunch of corresponding dirs
	constexpr size_t numPnts{ 1024u };
	for (size_t nn{0u} ; nn < numPnts ; ++nn)
	{
		ro::PairUV const uvPair{ simDir(), simDir() };
		double const expGap{ roExp.tripleProductGap(uvPair) };
		double const gotGap{ quad.tripleProductGap(uvPair) };
		if (! dat::nearlyEquals(gotGap, expGap))
		{
			double const difGap{ gotGap - expGap };
			oss << "Failure of gap compare test" << std::endl;
			oss << "expGap: " << io::sprintf("%21.18f", expGap) << std::endl;
			oss << "gotGap: " << io::sprintf("%21.18f", gotGap) << std::endl;
			oss << "difGap: " << io::sprintf("%21.18f", difGap) << std::endl;
			break;
		}
	}

	return oss.str();
}


}

//! Unit test for ro::QuadForm
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ro_QuadForm_test0();
	oss << ro_QuadForm_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
