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
\brief  This file contains unit test for trans::RangeMap
*/


#include "libtrans/RangeMap.h"

#include "libdat/dat.h"
#include "libdat/Range.h"
#include "libio/stream.h"
#include "libmath/interp.h"
#include "libmath/math.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
trans_RangeMap_test0
	()
{
	std::ostringstream oss;
	trans::RangeMap const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check basic domain/range limits
std::string
trans_RangeMap_test1
	()
{
	std::ostringstream oss;

	dat::Range<double> const domain(125., 135.);
	std::function<double(double const &)> const mapping
		([] (double const & xx) { return std::sin(xx * math::halfPi); });
	trans::RangeMap const func(domain, mapping);

	// check that function is calling appropriate mapping
	double const xAt(domain.middle());
	double const yFrac(mapping(.5));
	dat::Range<double> const & yRange = domain;
	double const expValue(math::interp::valueAtValid<double>(yFrac, yRange));
	double const gotValue(func(xAt));
	if (! dat::nearlyEquals(gotValue, expValue))
	{
		oss << "Failure of basic mapping evaluation test" << std::endl;
		oss << dat::infoString(expValue, "expValue") << std::endl;
		oss << dat::infoString(gotValue, "gotValue") << std::endl;
	}

	// check out of range arguments
	double const xMin(domain.min());
	double const gotYMin(func(xMin));
	if (! dat::isValid(gotYMin))
	{
		oss << "Failure of validity at min test" << std::endl;
		oss << dat::infoString(gotYMin, "gotYMin") << std::endl;
	}
	else
	{
		if (! dat::nearlyEquals(gotYMin, yRange.min()))
		{
			oss << "Failure of value at min test" << std::endl;
			oss << dat::infoString(gotYMin, "gotYMin") << std::endl;
		}
	}

	double const xMax(domain.max());
	double const fAtMax(func(xMax));
	if (dat::isValid(fAtMax))
	{
		oss << "Failure of (in)validity at max test" << std::endl;
	}

	return oss.str();
}

	//! Check if both null or else nearly equal
	template <typename Type>
	bool
	sameResult
		( Type const & value1
		, Type const & value2
		, Type const & tol
		)
	{
		bool same(false);
		bool const ok1(dat::isValid(value1));
		bool const ok2(dat::isValid(value2));
		if (ok1 == ok2) // includes null==null
		{
			same = true;
			if (ok1) // if one is valid, so is the other
			{
				// so then test contents
				same = dat::nearlyEquals(value1, value2, tol);
			}
		}
		// else // either null,okay or okay,null - so not the same
		return same;
	}

//! Check basic remapping operations
std::string
trans_RangeMap_test2
	()
{
	std::ostringstream oss;

	// define test case
	double const ypx(1./10.);
	dat::Range<double> const xRange(10.000, 12.550);
	dat::Range<double> const yRange(ypx*xRange.min(), ypx*xRange.max());

	// setup remapper
	std::function<double(double const &)> const mapping
		([] (double const & xx) { return math::sq(xx); });
	trans::RangeMap const func(xRange, mapping, yRange);

	// compute expected results
	typedef std::pair<double, double> XYPair;
	double const badValue(dat::nullValue<double>());
	double const eps(xRange.min() * dat::smallValue<double>());
	//
	XYPair const badLo{xRange.min() - 1., badValue};
	XYPair const xyMin{xRange.min(), yRange.min()};
	double const ySpan(yRange.magnitude());
	XYPair const xySml{xRange.min()+eps, yRange.min()+mapping(eps)*ySpan};
	XYPair const xyMid{xRange.middle() , yRange.min()+mapping(.5)*ySpan};
	XYPair const xyBig{xRange.max()-eps, yRange.min()+mapping(1.-eps)*ySpan};
	XYPair const xyMax{xRange.max(), badValue};
	XYPair const badHi{xRange.max() + 1., badValue};

	// assemble test cases
	std::map<std::string, XYPair> const testcases
		{ std::make_pair("a)badLo", badLo)
		, std::make_pair("b)xyMin", xyMin)
		, std::make_pair("c)xySml", xySml)
		, std::make_pair("d)xyMid", xyMid)
		, std::make_pair("e)xyBig", xyBig)
		, std::make_pair("f)xyMax", xyMax)
		, std::make_pair("g)badHi", badHi)
		};

	for (std::map<std::string, XYPair>::const_iterator
		iter(testcases.begin()) ; testcases.end() != iter ; ++iter)
	{
		// access test values
		std::string const & tname = iter->first;
		XYPair const & xypair = iter->second;
		double const & xAt = xypair.first;
		double const & expY = xypair.second;

		// exercise class
		double const gotY(func(xAt));

		// check results
		double const & tolY = 2.*xAt*eps;
		if (! sameResult(gotY, expY, tolY))
		{
			double const difY(gotY - expY);
			oss << "Failure of remapping test:"
				<< " '" << tname << "'" << std::endl;
			oss << dat::infoString(expY, "expY") << '\n';
			oss << dat::infoString(gotY, "gotY") << '\n';
			oss << dat::infoString(difY, "difY", 21u, 18u) << '\n';
			oss << dat::infoString(tolY, "tolY", 21u, 18u) << '\n';
		}
	}

	return oss.str();
}


}

//! Unit test for trans::RangeMap
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << trans_RangeMap_test0();
	oss << trans_RangeMap_test1();
	oss << trans_RangeMap_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
