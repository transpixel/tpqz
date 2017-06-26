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
\brief  This file contains unit test for dat::SpotX
*/


#include "libdat/SpotX.h"

#include "libdat/xform.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_SpotX_test0
	()
{
	std::ostringstream oss;
	dat::SpotX const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}

	dat::SpotX const aBad(dat::nullValue<dat::RowCol>());
	if (aBad.isValid())
	{
		oss << "Failure of bad construction test" << std::endl;
	}

	return oss.str();
}

//! Check basic transformation operations
std::string
dat_SpotX_test1
	()
{
	std::ostringstream oss;

	using dat::operator+;
	using dat::operator-;

	dat::Spot const posBwA{{ 7., 5. }};
	dat::SpotX const xBwA(posBwA);

	// check basic transformation properties
	dat::Spot const expInA{{ 11., 13. }};
	dat::Spot const expInB(expInA - posBwA);
	dat::Spot const gotInB(xBwA(expInA));

	// check composition
	dat::Spot const posCwB{{ -2., 3. }};
	dat::SpotX const xCwB(posCwB);

	dat::Spot const posCwA(posCwB + posBwA);
	dat::SpotX const expCwA(posCwA);
	dat::SpotX const gotCwA(dat::xform::composite(xCwB, xBwA));

	dat::Spot const posAwC(-posCwA);
	dat::SpotX const expAwC(posAwC);

	// check inverse of composite
	{
		dat::SpotX const gotAwC(dat::xform::inverse(gotCwA));
		if (! gotAwC.nearlyEquals(expAwC))
		{
			oss << "Failure of compose/inverse test" << std::endl;
		}
	}

	// check composite of inverses
	{
		dat::SpotX const xAwB(dat::xform::inverse(xBwA));
		dat::SpotX const xBwC(dat::xform::inverse(xCwB));
		dat::SpotX const gotAwC(dat::xform::composite(xAwB, xBwC));
		if (! gotAwC.nearlyEquals(expAwC))
		{
			oss << "Failure of inverse/compose test" << std::endl;
		}
	}

	// check round-trip consistency
	dat::SpotX const gotRT(dat::xform::composite(expAwC, expCwA));
	dat::SpotX const expRT(dat::SpotX::identity());
	if (! gotRT.nearlyEquals(expRT))
	{
		oss << "Failure of roundtrip identity test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for dat::SpotX
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_SpotX_test0();
	oss << dat_SpotX_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
