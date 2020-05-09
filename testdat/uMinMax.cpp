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
\brief  This file contains unit test for dat::MinMax
*/


#include "libdat/MinMax.h"

#include "libio/stream.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_MinMax_test0
	()
{
	std::ostringstream oss;
	dat::MinMax<double> const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}

	if (aNull.nearlyEquals(aNull))
	{
		oss << "Failure of null/null nearlyEquals test" << std::endl;
	}
	dat::MinMax<double> const goodA(.2, M_PI);
	dat::MinMax<double> const goodB(1./5., std::atan2(0., -1.));
	if (! goodA.nearlyEquals(goodB))
	{
		oss << "Failure of good/good nearlyEquals test" << std::endl;
	}

	return oss.str();
}

//! Check basic properties
std::string
dat_MinMax_test1
	()
{
	std::ostringstream oss;

	// check for bad constructions
	dat::MinMax<int> const bad1(5, -7);
	if (bad1.isValid())
	{
		oss << "Failure of bad1 test" << std::endl;
	}

	// check for good construction
	float const expMin(-7.f);
	float const expMax( 9.f);
	dat::MinMax<float> const mm(expMin, expMax);
	if (! mm.isValid())
	{
		oss << "Failure of mm validity test" << std::endl;
	}
	float const gotMin(mm.min());
	float const gotMax(mm.max());

	// test can be exact since not operations are involved
	if (! (gotMin == expMin))
	{
		oss <<  "Failure of min value test" << std::endl;
	}
	if (! (gotMax == expMax))
	{
		oss <<  "Failure of max value test" << std::endl;
	}
	std::pair<float, float> const expRange(expMin, expMax);
	std::pair<float, float> const gotRange(mm.pair());
	if (! dat::nearlyEquals(gotRange, expRange))
	{
		oss << "Failure of pair value test" << std::endl;
	}

	// check operations
	float const isIn(.125f * (expMin + expMax));
	float const isOut(1.0625f * expMax);
	float const inEnd(expMin);
	float const outEnd(expMax);
	if (! mm.containsInclusive(isIn))
	{
		oss << "Failure of containsInclusive in test" << std::endl;
	}
	if (  mm.containsInclusive(isOut))
	{
		oss << "Failure of containsInclusive out test" << std::endl;
	}
	if (! mm.containsInclusive(inEnd))
	{
		oss << "Failure of containsInclusive inMin test" << std::endl;
	}
	if (! mm.containsInclusive(outEnd))
	{
		oss << "Failure of containsInclusive outMax test" << std::endl;
	}

	// check expansion of values
	dat::MinMax<float> grow;
	grow = grow.expandedWith(isIn);
	if (! grow.isValid())
	{
		oss << "Failure of expanded null test" << std::endl;
	}
	grow = grow.expandedWith(expMin);
	grow = grow.expandedWith(expMax);

	if (! grow.nearlyEquals(mm))
	{
		oss << "Failure of expansion (by scalar) test" << std::endl;
	}

	// check expansion with another min/max
	{
		typedef dat::MinMax<int> mm_t;
		mm_t const mm0(-4, 5);
		// testCase, expected
		std::vector<std::pair<dat::MinMax<int>, dat::MinMax<int> > >
			const mmSpans
			{{ std::make_pair(mm_t(-3,  4), mm_t(-4,  5))
			 , std::make_pair(mm_t(-5,  6), mm_t(-5,  6))
			 , std::make_pair(mm_t(-5, -1), mm_t(-5,  5))
			 , std::make_pair(mm_t( 1,  6), mm_t(-4,  6))
			}};
		for (size_t nn(0u) ; nn < mmSpans.size() ; ++nn)
		{
			mm_t const & mmT = mmSpans[nn].first;
			mm_t const & expMM = mmSpans[nn].second;
			mm_t const gotMM(mm0.expandedWith(mmT));
			if (! gotMM.nearlyEquals(expMM))
			{
				oss << "Failure of expandedWith(MM) test" << std::endl;
				oss << mmT.infoString("mmT") << std::endl;
				oss << expMM.infoString("expMM") << std::endl;
				oss << gotMM.infoString("gotMM") << std::endl;
			}
		}
	}

	return oss.str();
}

//! Check expand
std::string
dat_MinMax_test2
	()
{
	std::ostringstream oss;

	// basic 'normal' operations
	{
		dat::MinMax<double> mmA(1., 2.);
		dat::MinMax<double> mmB(1.5, 2.5);
		dat::MinMax<double> expMM(1., 2.5);
		dat::MinMax<double> gotAB{ mmA.expandedWith(mmB) };
		dat::MinMax<double> gotBA{ mmB.expandedWith(mmA) };
		if (! gotAB.nearlyEquals(expMM))
		{
			oss << "Failure of expanded AB test" << std::endl;
		}
		if (! gotBA.nearlyEquals(expMM))
		{
			oss << "Failure of expanded BA test" << std::endl;
		}
	}

	// check null handling
	{
		double const nan{ dat::nullValue<double>() };
		dat::MinMax<double> mmXX(nan, nan);
		dat::MinMax<double> mmXo(nan,  1.);
		dat::MinMax<double> mmoX( 1., nan);
		dat::MinMax<double> mmoo( 1.,  1.);
		dat::MinMax<double> null{};
		dat::MinMax<double> gotXX{ null.expandedWith(mmXX) };
		dat::MinMax<double> gotXo{ null.expandedWith(mmXo) };
		dat::MinMax<double> gotoX{ null.expandedWith(mmoX) };
		dat::MinMax<double> gotoo{ null.expandedWith(mmoo) };
		if (gotXX.isValid() || gotXo.isValid() || gotoX.isValid())
		{
			oss << "Failure of null-expand-null test" << std::endl;
		}
		if (! gotoo.isValid())
		{
			oss << "Failure of null-expand-good test" << std::endl;
		}
		dat::MinMax<double> gotGood{ mmoo.expandedWith(null) };
		if (! gotGood.isValid())
		{
			oss << "Failure of good-expand-null test" << std::endl;
		}
	}


	return oss.str();
}

}

//! Unit test for dat::MinMax
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_MinMax_test0();
	oss << dat_MinMax_test1();
	oss << dat_MinMax_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
