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
\brief  This file contains unit test for math::MapSizeArea
*/


#include "libmath/MapSizeArea.h"

#include "libdat/array.h"
#include "libdat/compare.h"
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
math_MapSizeArea_test0
	()
{
	std::ostringstream oss;
	math::MapSizeArea const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

	template <typename DatType, size_t Dim>
	inline
	bool
	areSame
		( std::array<DatType, Dim> const & itemA
		, std::array<DatType, Dim> const & itemB
		);

	template <typename DatType, size_t Dim>
	inline
	bool
	areSame
		( std::array<DatType, Dim> const & itemA
		, std::array<DatType, Dim> const & itemB
		)
	{
		bool same{ false };
		bool const okayA{ dat::isValid(itemA) };
		bool const okayB{ dat::isValid(itemB) };
		if (okayA)
		{
			if (okayB)
			{
				// both are valid - so test for answer
				same = dat::nearlyEquals(itemA, itemB);
			}
			// else A valid, B not - so different
		}
		else
		{
			if (! okayB)
			{
				// both are invalid - so "same"
				same = true;
			}
			// else A not valid, B valid - so different
		}
		return same;
	}

//! Check basic forward inverse relationships
std::string
math_MapSizeArea_test1
	()
{
	std::ostringstream oss;

	constexpr size_t uHigh{ 200u };
	constexpr size_t uWide{ 300u };
	constexpr double dHigh{ double(uHigh) };
	constexpr double dWide{ double(uWide) };

	// [0:uHigh), [0:uWide)
	dat::Extents const hwSize(uHigh, uWide);

	// [-dHigh/2.,+dHigh/2.), [-dWide/2.,+dWide/2.)
	dat::Range<double> const xRange(-.5*dHigh, .5*dHigh);
	dat::Range<double> const yRange(-.5*dWide, .5*dWide);
	dat::Area<double> const xyArea{ xRange, yRange };

	math::MapSizeArea const map(hwSize, xyArea);

	using SizeSpot = dat::Spot;
	using AreaSpot = dat::Spot;
	SizeSpot const nullAreaSpot(dat::nullValue<dat::Spot>());
	AreaSpot const nullSizeSpot(dat::nullValue<dat::Spot>());

	std::vector<std::pair<SizeSpot, AreaSpot> > const expGoodNull
		{ { SizeSpot{{ -.1*dHigh,  .5*dWide }}, nullAreaSpot }
		, { SizeSpot{{ 1.1*dHigh,  .5*dWide }}, nullAreaSpot }
		, { SizeSpot{{  .5*dHigh, -.1*dWide }}, nullAreaSpot }
		, { SizeSpot{{  .5*dHigh, 1.1*dWide }}, nullAreaSpot }
		, { SizeSpot{{  1.*dHigh,  1.*dWide }}, nullAreaSpot }
		};
	std::vector<std::pair<SizeSpot, AreaSpot> > const expGoodGood
		{ { SizeSpot{{  .1*dHigh,  .5*dWide }}, AreaSpot{{ -80.,   0. }} }
		, { SizeSpot{{  .9*dHigh,  .5*dWide }}, AreaSpot{{  80.,   0. }} }
		, { SizeSpot{{  .5*dHigh,  .1*dWide }}, AreaSpot{{   0.,-120. }} }
		, { SizeSpot{{  .5*dHigh,  .9*dWide }}, AreaSpot{{   0., 120. }} }
		, { SizeSpot{{  0.*dHigh,  0.*dWide }}, AreaSpot{{ -100., -150. }} }
		};
	std::vector<std::pair<SizeSpot, AreaSpot> > const expNullGood
		{ { nullSizeSpot, AreaSpot{{ -.6*dHigh,  .1*dWide }} }
		, { nullSizeSpot, AreaSpot{{  .6*dHigh,  .1*dWide }} }
		, { nullSizeSpot, AreaSpot{{  .1*dHigh,  .6*dWide }} }
		, { nullSizeSpot, AreaSpot{{  .1*dHigh, -.6*dWide }} }
		, { nullSizeSpot, AreaSpot{{  1.*dHigh,  1.*dWide }} }
		};

	// add corner exact cases

	{
		std::vector<std::pair<SizeSpot, AreaSpot> >
			const & expSizeAreas = expGoodNull;
		for (std::pair<SizeSpot, AreaSpot> const expSizeArea : expSizeAreas)
		{
			dat::Spot const & expSizeSpot = expSizeArea.first;
			dat::Spot const & expAreaSpot = expSizeArea.second;

			dat::Spot const gotAreaSpot(map.xyAreaSpotFor(expSizeSpot));
			dat::Spot const gotSizeSpot(map.hwSizeSpotFor(expAreaSpot));

			bool isBad{ false };
			if (! areSame(gotAreaSpot, expAreaSpot))
			{
				oss << "Failure of (sN)areaSpotFor test" << std::endl;
				isBad = true;
			}

			if (dat::isValid(gotSizeSpot))
			{
				oss << "Failure of (sN)sizeSpotFor test" << std::endl;
				isBad = true;
			}

			if (isBad)
			{
				oss << dat::infoString(expSizeSpot, "expSizeSpot") << '\n';
				oss << dat::infoString(expAreaSpot, "expAreaSpot") << '\n';
				oss << dat::infoString(gotAreaSpot, "gotAreaSpot") << '\n';
				oss << dat::infoString(gotSizeSpot, "gotSizeSpot") << '\n';
			}
		}
	}
	{
		std::vector<std::pair<SizeSpot, AreaSpot> >
			const & expSizeAreas = expGoodGood;
		for (std::pair<SizeSpot, AreaSpot> const expSizeArea : expSizeAreas)
		{
			dat::Spot const & expSizeSpot = expSizeArea.first;
			dat::Spot const & expAreaSpot = expSizeArea.second;

			dat::Spot const gotAreaSpot(map.xyAreaSpotFor(expSizeSpot));
			dat::Spot const gotSizeSpot(map.hwSizeSpotFor(expAreaSpot));

			bool isBad{ false };
			if (! areSame(gotSizeSpot, expSizeSpot))
			{
				oss << "Failure of (sa)sizeSpotFor test" << std::endl;
				isBad = true;
			}

			if (! areSame(gotAreaSpot, expAreaSpot))
			{
				oss << "Failure of (sa)areaSpotFor test" << std::endl;
				isBad = true;
			}

			if (isBad)
			{
				oss << dat::infoString(expSizeSpot, "expSizeSpot") << '\n';
				oss << dat::infoString(expAreaSpot, "expAreaSpot") << '\n';
				oss << dat::infoString(gotAreaSpot, "gotAreaSpot") << '\n';
				oss << dat::infoString(gotSizeSpot, "gotSizeSpot") << '\n';
			}
		}
	}
	{
		std::vector<std::pair<SizeSpot, AreaSpot> >
			const & expSizeAreas = expNullGood;
		for (std::pair<SizeSpot, AreaSpot> const expSizeArea : expSizeAreas)
		{
			dat::Spot const & expSizeSpot = expSizeArea.first;
			dat::Spot const & expAreaSpot = expSizeArea.second;

			dat::Spot const gotAreaSpot(map.xyAreaSpotFor(expSizeSpot));
			dat::Spot const gotSizeSpot(map.hwSizeSpotFor(expAreaSpot));

			bool isBad{ false };
			if (! areSame(gotSizeSpot, expSizeSpot))
			{
				oss << "Failure of (Na)sizeSpotFor test" << std::endl;
				isBad = true;
			}

			if (dat::isValid(gotAreaSpot))
			{
				oss << "Failure of (Na)areaSpotFor test" << std::endl;
				isBad = true;
			}

			if (isBad)
			{
				oss << dat::infoString(expSizeSpot, "expSizeSpot") << '\n';
				oss << dat::infoString(expAreaSpot, "expAreaSpot") << '\n';
				oss << dat::infoString(gotAreaSpot, "gotAreaSpot") << '\n';
				oss << dat::infoString(gotSizeSpot, "gotSizeSpot") << '\n';
			}
		}
	}

	return oss.str();
}


}

//! Unit test for math::MapSizeArea
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << math_MapSizeArea_test0();
	oss << math_MapSizeArea_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
