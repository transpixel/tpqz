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
\brief  This file contains unit test for dat::Region
*/


#include "libdat/Region.h"

#include "libdat/compare.h"
#include "libdat/info.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_Region_test0
	()
{
	std::ostringstream oss;

	dat::Region<2u, double> const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}

	// check commnon constructions
	dat::Area<size_t> const nullArea;
	if (nullArea.isValid())
	{
		oss << "Failure of nullArea test" << std::endl;
	}

	dat::Volume<float> const nullVolume;
	if (nullVolume.isValid())
	{
		oss << "Failure of nullVolume test" << std::endl;
	}

	// basic equality test
	dat::Region<3, size_t> const region3
		{ dat::Range<size_t>{ 4u, 9u }
		, dat::Range<size_t>{ 3u, 5u }
		, dat::Range<size_t>{ 2u, 7u }
		};
	dat::Region<3, size_t> const diff3
		{ dat::Range<size_t>{ 4u, 9u }
		, dat::Range<size_t>{ 2u, 7u }
		, dat::Range<size_t>{ 3u, 5u }
		};

	if (! region3.nearlyEquals(region3))
	{
		oss << "Failure of nearlyEquals w/ same test" << std::endl;
	}
	if (  region3.nearlyEquals(diff3))
	{
		oss << "Failure of nearlyEquals w/ diff test" << std::endl;
	}

	return oss.str();
}

//! Check TODO....
std::string
dat_Region_test1
	()
{
	std::ostringstream oss;

	// check common uses
	dat::Range<size_t> const xRange(3u, 7u);
	dat::Range<size_t> const yRange(5u, 19u);

	// check 2D
	{
		dat::Area<size_t> const area{xRange, yRange};

		double const expMag(xRange.magnitude() * yRange.magnitude());
		double const gotMag(area.magnitude());

		if (! dat::nearlyEquals(gotMag, expMag))
		{
			oss << "Failure of area magnitude test" << std::endl;
			oss << dat::infoString(expMag, "expMag") << std::endl;
			oss << dat::infoString(gotMag, "gotMag") << std::endl;
		}

		// check corners
		std::array<size_t, 2u> const gotMins(area.minimums());
		std::array<size_t, 2u> const gotMaxs(area.maximums());
		std::array<size_t, 2u> const expMins{{xRange.min(), yRange.min()}};
		std::array<size_t, 2u> const expMaxs{{xRange.max(), yRange.max()}};

		if (! dat::nearlyEquals(gotMins, expMins))
		{
			oss << "Failure of area minimums test" << std::endl;
			oss << dat::infoString(expMins, "expMins") << std::endl;
			oss << dat::infoString(gotMins, "gotMins") << std::endl;
		}

		if (! dat::nearlyEquals(gotMaxs, expMaxs))
		{
			oss << "Failure of area maximums test" << std::endl;
			oss << dat::infoString(expMaxs, "expMaxs") << std::endl;
			oss << dat::infoString(gotMaxs, "gotMaxs") << std::endl;
		}
	}

	// check 3D
	{
		dat::Range<size_t> const zRange(17u, 19u);
		dat::Volume<size_t> const volume{xRange, yRange, zRange};

		double const expMag
			(xRange.magnitude() * yRange.magnitude() * zRange.magnitude());
		double const gotMag(volume.magnitude());

		if (! dat::nearlyEquals(gotMag, expMag))
		{
			oss << "Failure of volume magnitude test" << std::endl;
			oss << dat::infoString(expMag, "expMag") << std::endl;
			oss << dat::infoString(gotMag, "gotMag") << std::endl;
		}

		// check a return range
		dat::Range<size_t> const & expZ = zRange;
		dat::Range<size_t> const gotZ(volume[2]);
		if (! gotZ.nearlyEquals(expZ))
		{
			oss << "Failure of op[] test" << std::endl;
		}
	}

	return oss.str();
}

//! Check intersection operation
std::string
dat_Region_test2
	()
{
	std::ostringstream oss;

	constexpr size_t rowT{ 27u };
	constexpr size_t rowB{ 47u };
	constexpr size_t colL{ 23u };
	constexpr size_t colR{ 41u };
	dat::Range<size_t> const maskTB{ rowT, rowB };
	dat::Range<size_t> const maskLR{ colL, colR };
	dat::Area<size_t> const mask{ maskTB, maskLR };

	using Rng = dat::Range<size_t>;
	dat::Area<size_t> const small{ Rng{ 30, 40 }, Rng{ 30, 40 } };
	dat::Area<size_t> const large{ Rng{ 20, 50 }, Rng{ 20, 50 } };
	constexpr size_t rowM{ 30u };
	constexpr size_t colM{ 35u };
	dat::Area<size_t> const edgeTL{ Rng{ 20, rowM }, Rng{ 20, colM } };
	dat::Area<size_t> const edgeBR{ Rng{ rowM, 50 }, Rng{ colM, 50 } };

	dat::Area<size_t> const expSmall(small);
	dat::Area<size_t> const expLarge(mask);
	dat::Area<size_t> const expTL{ Rng{ rowT, rowM }, Rng{ colL, colM } };
	dat::Area<size_t> const expBR{ Rng{ rowM, rowB }, Rng{ colM, colR } };

	dat::Area<size_t> const gotSmall(mask.intersectWith(small));
	dat::Area<size_t> const gotLarge(mask.intersectWith(large));
	dat::Area<size_t> const gotTL(mask.intersectWith(edgeTL));
	dat::Area<size_t> const gotBR(mask.intersectWith(edgeBR));

	dat::Area<size_t> const revSmall(small.intersectWith(mask));
	dat::Area<size_t> const revLarge(large.intersectWith(mask));
	dat::Area<size_t> const revTL(edgeTL.intersectWith(mask));
	dat::Area<size_t> const revBR(edgeBR.intersectWith(mask));

	// check commutativity
	if (! gotSmall.nearlyEquals(revSmall))
	{
		oss << "Failure of commutativity Small test" << std::endl;
	}
	if (! gotLarge.nearlyEquals(revLarge))
	{
		oss << "Failure of commutativity Large test" << std::endl;
	}
	if (! gotTL.nearlyEquals(revTL))
	{
		oss << "Failure of commutativity TL test" << std::endl;
	}
	if (! gotBR.nearlyEquals(revBR))
	{
		oss << "Failure of commutativity BR test" << std::endl;
	}

	// check expected value
	if (! gotSmall.nearlyEquals(expSmall))
	{
		oss << "Failure of expected Small test" << std::endl;
		oss << dat::infoString(mask, "mask") << std::endl;
		oss << dat::infoString(small, "small") << std::endl;
		oss << dat::infoString(expSmall, "expSmall") << std::endl;
		oss << dat::infoString(gotSmall, "gotSmall") << std::endl;
	}
	if (! gotLarge.nearlyEquals(expLarge))
	{
		oss << "Failure of expected Large test" << std::endl;
		oss << dat::infoString(mask, "mask") << std::endl;
		oss << dat::infoString(large, "large") << std::endl;
		oss << dat::infoString(expLarge, "expLarge") << std::endl;
		oss << dat::infoString(gotLarge, "gotLarge") << std::endl;
	}
	if (! gotTL.nearlyEquals(expTL))
	{
		oss << "Failure of expected TL test" << std::endl;
	}
	if (! gotBR.nearlyEquals(expBR))
	{
		oss << "Failure of expected BR test" << std::endl;
	}

	return oss.str();
}

//! Check inclusion
std::string
dat_Region_test3
	()
{
	std::ostringstream oss;

	// check contains()
	dat::Range<double> const unitRange{ 0., 1. };
	dat::Area<double> const unitArea{ unitRange, unitRange };

	std::vector<dat::Spot> const inSpots
		{ dat::Spot{{  .5,  .5 }}
		, dat::Spot{{  0.,  0. }} // "near" corner is included
		};
	for (dat::Spot const & inSpot : inSpots)
	{
		if (! unitArea.contains(inSpot))
		{
			oss << "Failure of contains(in) test" << std::endl;
			oss << dat::infoString(inSpot, "inSpot") << std::endl;
		}
	}

	std::vector<dat::Spot> const otSpots
		{ dat::Spot{{ -.1,  .5 }}
		, dat::Spot{{ 1.1,  .5 }}
		, dat::Spot{{  .5, -.1 }}
		, dat::Spot{{  .5, 1.1 }}
		, dat::Spot{{  1.,  1. }} // "far" corner is *excluded* (half open)
		};
	for (dat::Spot const & otSpot : otSpots)
	{
		if (  unitArea.contains(otSpot))
		{
			oss << "Failure of contains(ot) test" << std::endl;
			oss << dat::infoString(otSpot, "otSpot") << std::endl;
		}
	}

	return oss.str();
}

}

//! Unit test for dat::Region
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_Region_test0();
	oss << dat_Region_test1();
	oss << dat_Region_test2();
	oss << dat_Region_test3();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
