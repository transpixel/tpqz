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
\brief  This file contains unit test for dat::SubExtents
*/


#include "libdat/SubExtents.h"

#include "libdat/info.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_SubExtents_test0
	()
{
	std::ostringstream oss;
	/*
	dat::SubExtents const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	*/
	return oss.str();
}

	//! Quantized SubExtents instance
	dat::SubExtents
	gotFor
		( size_t const & ul0
		, size_t const & ul1
		, size_t const & high
		, size_t const & wide
		, size_t const & delta
		)
	{
		dat::RowCol const ul{{  ul0,  ul1 }};
		dat::Extents const size( high, wide  );
		dat::SubExtents const orig(ul, size);
		return dat::SubExtents::quantized(orig, delta);
	}

	//! UnQuantized SubExtents instance
	dat::SubExtents
	expFor
		( size_t const & ul0
		, size_t const & ul1
		, size_t const & high
		, size_t const & wide
		)
	{
		dat::RowCol const ul{{  ul0,  ul1 }};
		dat::Extents const size( high, wide  );
		return dat::SubExtents(ul, size);
	}

	bool
	areSame
		( dat::SubExtents const & got
		, dat::SubExtents const & exp
		)
	{
		bool same{ false };
		bool const bothNull((! exp.isValid()) && (! got.isValid()));
		if (! bothNull)
		{
			same = got.nearlyEquals(exp);
		}
		return same;
	}

	void
	checkSubs
		( std::ostream & oss
		, dat::SubExtents const & got
		, dat::SubExtents const & exp
		)
	{
		if (! areSame(got, exp))
		{
			oss << "Failure of sub extents quantization test" << std::endl;
			oss << dat::infoString(exp, "exp") << std::endl;
			oss << dat::infoString(got, "got") << std::endl;
		}
	}

//! Check basic operations
std::string
dat_SubExtents_test1
	()
{
	std::ostringstream oss;

	dat::Extents const fullSize(104u, 106u);

	// check centering with even sizes
	{
		dat::Extents const cropSize(   4u,   6u   );
		dat::RowCol const expBegUL {{  10u,  10u }};
		dat::RowCol const absCenterRC{{  12u,  13u }};
		dat::RowCol const relCenterRC{{   2u,   3u }};

		dat::SubExtents const expSubSize(expBegUL, cropSize);
		dat::SubExtents const gotSubSize
			(dat::SubExtents::centeredOn(absCenterRC, cropSize));
		if (! gotSubSize.nearlyEquals(expSubSize))
		{
			oss << "Failure of simple even size test" << std::endl;
		}

		// check recovery of location w.r.t.
		dat::RowCol const gotAbsCenterRC
			(gotSubSize.fullRowColFor(relCenterRC));
		dat::RowCol const & expAbsCenterRC = absCenterRC;
		if (! dat::nearlyEquals(gotAbsCenterRC, expAbsCenterRC))
		{
			oss << "Failure of even absolute center recovery" << '\n';
			oss << dat::infoString(expAbsCenterRC, "expAbsCenterRC") << '\n';
			oss << dat::infoString(gotAbsCenterRC, "gotAbsCenterRC") << '\n';
		}

		// check subpixel version
		dat::Spot const expAbsCenterSpot
			{{ double(expAbsCenterRC[0]), double(expAbsCenterRC[1]) }};
		dat::Spot const relCenterSpot
			{{ double(relCenterRC[0]), double(relCenterRC[1]) }};
		dat::Spot const gotAbsCenterSpot
			(gotSubSize.fullSpotFor(relCenterSpot));
		if (! dat::nearlyEquals(gotAbsCenterSpot, expAbsCenterSpot))
		{
			oss << "Failure of even absolute center spot recovery" << '\n';
			oss << dat::infoString(expAbsCenterSpot, "expAbsCenterSpot")
				<< '\n';
			oss << dat::infoString(gotAbsCenterSpot, "gotAbsCenterSpot")
				<< '\n';
		}

		// check evaluate of local location from source
		dat::RowCol const & expRelCenterRC = relCenterRC;
		dat::RowCol const gotRelCenterRC
			(gotSubSize.subRowColFor(absCenterRC));
		if (! dat::nearlyEquals(gotRelCenterRC, expRelCenterRC))
		{
			oss << "Failure of even absolute center recovery" << '\n';
			oss << dat::infoString(expRelCenterRC, "expRelCenterRC") << '\n';
			oss << dat::infoString(gotRelCenterRC, "gotRelCenterRC") << '\n';
		}

	}
	// check centering with odd size
	{
		dat::Extents const cropSize(   3u,   5u   );
		dat::RowCol const expBegUL {{  10u,  10u }};
		dat::RowCol const expCenter{{  11u,  12u }};
		dat::RowCol const relCenter{{   1u,   2u }};

		dat::SubExtents const expSubSize(expBegUL, cropSize);
		dat::SubExtents const gotSubSize
			(dat::SubExtents::centeredOn(expCenter, cropSize));
		if (! gotSubSize.nearlyEquals(expSubSize))
		{
			oss << "Failure of simple even size test" << std::endl;
		}

		// check recovery of location w.r.t.
		dat::RowCol const gotCenter(gotSubSize.fullRowColFor(relCenter));
		if (! dat::nearlyEquals(gotCenter, expCenter))
		{
			oss << "Failure of odd absolute center recovery" << '\n';
			oss << dat::infoString(expCenter, "expCenter") << '\n';
			oss << dat::infoString(gotCenter, "gotCenter") << '\n';
		}
	}


	// check quantization
	{
		dat::SubExtents const got(gotFor( 0u, 0u, 3u, 3u  , 2u));
		dat::SubExtents const exp(expFor( 0u, 0u, 4u, 4u ));
		checkSubs(oss, got, exp);
	}
	{
		dat::SubExtents const got(gotFor( 7u, 8u, 3u, 3u  , 2u));
		dat::SubExtents const exp(expFor( 6u, 8u, 4u, 4u ));
		checkSubs(oss, got, exp);
	}


	return oss.str();
}

//! Check croppedCenter operations
std::string
dat_SubExtents_test2
	()
{
	std::ostringstream oss;

	// completely contained test
	{
		using dat::SubExtents;
		dat::Extents const fullSize(5u, 7u);
		dat::RowCol const subCenter{{ 2u, 3u }};
		dat::Extents const subSize(1u, 2u);
		SubExtents const expSub // for this case, compare to full center
			{ SubExtents::centeredOn(subCenter, subSize) };
		SubExtents const gotSub
			{ SubExtents::croppedCenterSize(subCenter, subSize, fullSize) };
		if (! gotSub.nearlyEquals(expSub))
		{
			oss << "Failure of cropped - interior test" << std::endl;
			oss << dat::infoString(expSub, "expSub") << std::endl;
			oss << dat::infoString(gotSub, "gotSub") << std::endl;
		}
	}

	//! Check cropped case UL corner
	{
		using dat::SubExtents;
		dat::Extents const fullSize(5u, 7u);
		dat::RowCol const subCenter{{ 0u, 0u }};
		dat::Extents const subSize(2u, 3u);
		SubExtents const expSub
			( dat::NdxBegEnd{ 0u, 1u }, dat::NdxBegEnd{ 0u, 2u }
			);
		SubExtents const gotSub
			{ SubExtents::croppedCenterSize(subCenter, subSize, fullSize) };
		if (! gotSub.nearlyEquals(expSub))
		{
			oss << "Failure of cropped - interior UL test" << std::endl;
			oss << dat::infoString(expSub, "expSub") << std::endl;
			oss << dat::infoString(gotSub, "gotSub") << std::endl;
		}
	}

	//! Check cropped case BR corner
	{
		using dat::SubExtents;
		dat::Extents const fullSize(5u, 7u);
		dat::RowCol const subCenter{{ 4u, 6u }};
		dat::Extents const subSize(2u, 3u);
		SubExtents const expSub
			( dat::NdxBegEnd{ 3u, 5u }, dat::NdxBegEnd{ 5u, 7u }
			);
		SubExtents const gotSub
			{ SubExtents::croppedCenterSize(subCenter, subSize, fullSize) };
		if (! gotSub.nearlyEquals(expSub))
		{
			oss << "Failure of cropped - interior BR test" << std::endl;
			oss << dat::infoString(expSub, "expSub") << std::endl;
			oss << dat::infoString(gotSub, "gotSub") << std::endl;
		}
	}

	return oss.str();
}


}

//! Unit test for dat::SubExtents
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_SubExtents_test0();
	oss << dat_SubExtents_test1();
	oss << dat_SubExtents_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
