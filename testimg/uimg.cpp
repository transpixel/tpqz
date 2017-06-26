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
\brief  This file contains unit test for img::img
*/


#include "libimg/img.h"

#include "libio/stream.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>


namespace
{
	//! Pixel state flags
	struct State
	{
		bool theIsValid;
		bool theIsUnder;
		bool theIsActive;
		bool theIsOver;

		//! value constructor
		explicit
		State
			( bool const & valid
			, bool const & under
			, bool const & active
			, bool const & over
			)
			: theIsValid(valid)
			, theIsUnder(under)
			, theIsActive(active)
			, theIsOver(over)
		{ }

		//! Descriptive information about this instance
		std::string
		infoString
			( std::string const & title
			) const
		{
			std::ostringstream oss;
			oss
				<< title
				<< " " << "VUAO:"
				<< std::boolalpha
				<< " " << std::setw(6u) << theIsValid
				<< " " << std::setw(6u) << theIsUnder
				<< " " << std::setw(6u) << theIsActive
				<< " " << std::setw(6u) << theIsOver
				;
			return oss.str();
		}
	};

	//! true if states match
	inline
	bool
	operator==
		( State const & s1
		, State const & s2
		)
	{
		bool const sameValid(s1.theIsValid == s2.theIsValid);
		bool const sameUnder(s1.theIsUnder == s2.theIsUnder);
		bool const sameActive(s1.theIsActive == s2.theIsActive);
		bool const sameOver(s1.theIsOver == s2.theIsOver);
		return (sameValid && sameUnder && sameActive && sameOver);
	}

	//! Evaluate state conditions for pix
	inline
	State
	stateFor
		( img::fpix_t const & pix
		)
	{
		// exercise each pixel test
		return State
			( img::isValid(pix)
			, img::isUnder(pix)
			, img::isActive(pix)
			, img::isOver(pix)
			);
	}

	//! true if states match
	inline
	bool
	check
		( std::ostream & oss
		, img::fpix_t const & pix
		, State const & exp
		, std::string const & testname
		)
	{
		State const got(stateFor(pix));
		bool same(got == exp);
		if (! same)
		{
			oss << std::fixed;
			oss << "Failure of " << testname << " test" << std::endl;
			oss << "value: " << pix << std::endl;
			oss << exp.infoString("exp") << std::endl;
			oss << got.infoString("got") << std::endl;
			oss << std::endl;
		}
		return same;
	}

//! Check basic constant values
std::string
img_img_test0
	()
{
	std::ostringstream oss;

	img::fpix_t const pixBad(img::fpixBad);
	img::fpix_t const pixBadN(-1.23f);
	img::fpix_t const pixUnder(img::fpixUnder);
	img::fpix_t const pixGoodZ(0.f);
	img::fpix_t const pixGoodP( 1.23f);
	img::fpix_t const pixOver(img::fpixOver);
	img::fpix_t const pixInfN(-std::numeric_limits<img::fpix_t>::infinity());
	img::fpix_t const pixInfP( std::numeric_limits<img::fpix_t>::infinity());

	State const expBad(false, false, false, false);
	State const expBadN(true, false, true, false);
	State const expUnder(true, true, false, false);
	State const expGoodZ(true, false, true, false);
	State const expGoodP(true, false, true, false);
	State const expOver(true, false, false, true);
	State const expInfN(true, true, false, false);
	State const expInfP(true, false, false, true);

	check(oss, pixBad, expBad, "Bad");
	check(oss, pixBadN, expBadN, "BadN");
	check(oss, pixUnder, expUnder, "Under");
	check(oss, pixGoodZ, expGoodZ, "GoodZ");
	check(oss, pixGoodP, expGoodP, "GoodP");
	check(oss, pixOver, expOver, "Over");
	check(oss, pixInfN, expInfN, "InfN");
	check(oss, pixInfP, expInfP, "InfP");

	return oss.str();
}

/*
//! Check TODO....
std::string
img_img_test1
	()
{
	std::ostringstream oss;
	oss << "Failure: implement this test!" << std::endl;
	return oss.str();
}
*/


}

//! Unit test for img::img
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << img_img_test0();
//	oss << img_img_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
