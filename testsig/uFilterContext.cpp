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
\brief  This file contains unit test for sig::FilterContext
*/


#include "libsig/FilterContext.h"

#include "libdat/info.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
sig_FilterContext_test0
	()
{
	std::ostringstream oss;
	sig::FilterContext const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check basic methods
std::string
sig_FilterContext_test1
	()
{
	std::ostringstream oss;

	// nominal configuration
	constexpr size_t pRow0{ 14u };
	constexpr size_t pCol0{ 16u };
	constexpr size_t mRow0{ 24u };
	constexpr size_t mCol0{ 26u };

	constexpr dat::RowCol refUL{{ 100u, 100u }};
	constexpr dat::RowCol tgtUL{{ 200u, 200u }};

	constexpr dat::Spot anyHotInMove{{ 2.25, 3.75 }};

	// explore combinations of even/odd for sizes
	for (size_t nn(0u) ; nn < 16u ; ++nn)
	{
		size_t const dpr{ (nn & 0x08) / 0x08 };
		size_t const dpc{ (nn & 0x04) / 0x04 };
		size_t const dmr{ (nn & 0x02) / 0x02 };
		size_t const dmc{ (nn & 0x01) / 0x01 };

		dat::Extents const pSize(pRow0 + dpr, pCol0 + dpc);
		dat::Extents const mSize(mRow0 + dmr, mCol0 + dmc);

		// combined move/patch size "-1" because the two overlap by one pixel
		dat::Extents const bothSize
			( pSize.high() + mSize.high() - 1u
			, pSize.wide() + mSize.wide() - 1u
			);

		// forward compute center locations
		using dat::operator+;
		dat::RowCol const pHalfDelta(pSize.centerMod2());
		dat::RowCol const bothHalfDelta(bothSize.centerMod2());
		dat::RowCol const refCenter(refUL + pHalfDelta);
		dat::RowCol const tgtCenter(tgtUL + bothHalfDelta);

		dat::Spot const pCenterSpot
			{{ double(pSize.high()) / 2.
			 , double(pSize.wide()) / 2.
			}};

		// establish expected values
		dat::SubExtents const expRefCrop(refUL, pSize);
		dat::SubExtents const expTgtCrop(tgtUL, bothSize);
		dat::Spot const expHotInTgt
			{{ double(tgtUL[0]) + anyHotInMove[0] + pCenterSpot[0]
			 , double(tgtUL[1]) + anyHotInMove[1] + pCenterSpot[1]
			}};

		// exercise the class
		sig::FilterContext const fc
			( sig::FilterContext::fromCenters
				(refCenter, tgtCenter, pSize, mSize)
			);
		dat::SubExtents const gotRefCrop(fc.referenceCrop());
		dat::SubExtents const gotTgtCrop(fc.targetCrop());
		dat::Spot const gotHotInTgt(fc.hotSpotForTargetAt(anyHotInMove));

		// check results

		if (! gotRefCrop.nearlyEquals(expRefCrop))
		{
			oss <<  "Failure of reference crop test, nn: " << nn << '\n';
			oss << dat::infoString(expRefCrop, "expRefCrop") << std::endl;
			oss << dat::infoString(gotRefCrop, "gotRefCrop") << std::endl;
		}

		if (! gotTgtCrop.nearlyEquals(expTgtCrop))
		{
			oss <<  "Failure of target crop test, nn: " << nn << '\n';
			oss << dat::infoString(pSize, "pSize") << std::endl;
			oss << dat::infoString(mSize, "mSize") << std::endl;
			oss << dat::infoString(bothSize, "+Size") << std::endl;
			oss << dat::infoString(expTgtCrop, "expTgtCrop") << std::endl;
			oss << dat::infoString(gotTgtCrop, "gotTgtCrop") << std::endl;
		}

		if (! dat::nearlyEquals(gotHotInTgt, expHotInTgt))
		{
			oss <<  "Failure of hot spot test, nn: " << nn << '\n';
			oss << dat::infoString(expRefCrop, "expRefCrop") << std::endl;
			oss << dat::infoString(expTgtCrop, "expTgtCrop") << std::endl;
			oss << dat::infoString(anyHotInMove, "anyHotInMove") << std::endl;
			oss << dat::infoString(pCenterSpot, "pCenterSpot") << std::endl;
			oss << dat::infoString(tgtUL, "tgtUL") << std::endl;
			oss << dat::infoString(expHotInTgt, "expHotInTgt") << std::endl;
			oss << dat::infoString(gotHotInTgt, "gotHotInTgt") << std::endl;
		}

		// bail after first error
		if (! oss.str().empty())
		{
			break;
		}
	}

	return oss.str();
}

//! Check quantization
std::string
sig_FilterContext_test2
	()
{
	std::ostringstream oss;

	/*
	// some general context
	sig::FilterContext const fcNom
		( dat::RowCol{{ 13u, 16u }}
		, dat::RowCol{{ 24u, 27u }}
		, dat::Extents(33u, 34u)
		, dat::Extents(44u, 45u)
		);

	// expected after quantization by 2
	sig::FilterContext const expQuant2
		( dat::RowCol{{ 12u, 16u }}
		, dat::RowCol{{ 24u, 26u }}
		, dat::Extents(34u, 34u)
		, dat::Extents(44u, 46u)
		);

	// expected after quantization by 5
	sig::FilterContext const expQuant5
		( dat::RowCol{{ 10u, 15u }}
		, dat::RowCol{{ 20u, 25u }}
		, dat::Extents(35u, 35u)
		, dat::Extents(45u, 45u)
		);

	// exercise method
	sig::FilterContext const gotQuant2(fcNom.quantized(2u));
	sig::FilterContext const gotQuant5(fcNom.quantized(5u));

	// check results
	if (! gotQuant2.nearlyEquals(expQuant2))
	{
		oss << "Failure of quantization(2) test" << std::endl;
		oss << dat::infoString(fcNom, "fcNom") << std::endl;
		oss << dat::infoString(expQuant2, "expQuant2") << std::endl;
		oss << dat::infoString(gotQuant2, "gotQuant2") << std::endl;
	}

	if (! gotQuant5.nearlyEquals(expQuant5))
	{
		oss << "Failure of quantization(5) test" << std::endl;
		oss << dat::infoString(fcNom, "fcNom") << std::endl;
		oss << dat::infoString(expQuant5, "expQuant5") << std::endl;
		oss << dat::infoString(gotQuant5, "gotQuant5") << std::endl;
	}
	*/

	return oss.str();
}

	struct ExpCase
	{
		dat::SpotX const theBwA;
		dat::SubExtents const theCrop;

		explicit
		ExpCase
			( dat::Spot const & trans
			, dat::RowCol const & ul
			, dat::Extents const & size
			)
			: theBwA(trans)
			, theCrop(ul, size)
		{ }
	};

	void
	checkCrop
		( std::ostream & oss
		, dat::Extents const & cropA
		, dat::Extents const & cropB
		, ExpCase const & exp
		, std::string const & tname = {}
		)
	{
		dat::SpotX const & xBwA = exp.theBwA;
		dat::SubExtents const & expCropInA = exp.theCrop;
		dat::SubExtents const gotCropInA
			(sig::FilterContext::commonCropIn1(cropA, xBwA, cropB));
		if (! gotCropInA.nearlyEquals(expCropInA))
		{
			oss << "Failure of common crop test: " << tname << std::endl;
			oss << dat::infoString(exp.theBwA, " exp.theBwA") << std::endl;
			oss << dat::infoString(expCropInA, " expCropInA") << std::endl;
			oss << dat::infoString(gotCropInA, " gotCropInA") << std::endl;
		}
	}

//! Check overlap computation
std::string
sig_FilterContext_test3
	()
{
	std::ostringstream oss;

	// simple interior case
	checkCrop
		( oss
		, dat::Extents(100u, 100u)
		, dat::Extents( 20u,  30u)
		, ExpCase
			{ dat::Spot{{ 40., 40. }}
			, dat::RowCol{{ 40u, 40u }}
			, dat::Extents(20u, 30u)
			}
		);

	// interior other way round
	checkCrop
		( oss
		, dat::Extents( 20u,  30u)
		, dat::Extents(100u, 100u)
		, ExpCase
			{ dat::Spot{{ -40., -40. }}
			, dat::RowCol{{  0u,  0u }}
			, dat::Extents(20u, 30u)
			}
		);

	// overlap
	checkCrop
		( oss
		, dat::Extents(100u, 100u)
		, dat::Extents(100u, 100u)
		, ExpCase
			{ dat::Spot{{ 50., -50. }}
			, dat::RowCol{{ 50u,  0u }}
			, dat::Extents(50u, 50u)
			}
		);

	// overlap
	checkCrop
		( oss
		, dat::Extents(100u, 100u)
		, dat::Extents(100u, 100u)
		, ExpCase
			{ dat::Spot{{ -50., 10. }}
			, dat::RowCol{{ 0u,  10u }}
			, dat::Extents(50u, 90u)
			}
		);

	return oss.str();
}

}

//! Unit test for sig::FilterContext
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << sig_FilterContext_test0();
	oss << sig_FilterContext_test1();
	oss << sig_FilterContext_test2();
	oss << sig_FilterContext_test3();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
