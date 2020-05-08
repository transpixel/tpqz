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
\brief  This file contains unit test for std::array operations
*/


#include "libdat/array.h"

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
dat_array_test0
	()
{
	std::ostringstream oss;
	std::array<double, 3u> const aNull(dat::nullValue<double, 3u>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check support for vector-like operations
std::string
dat_array_test1
	()
{
	std::ostringstream oss;

	using Type = long double;
	using Vec3D = std::array<Type, 3u>;

	Vec3D const aVec{{ 3., 0., 0. }};
	Vec3D const bVec{{ 0., 4., 0. }};

	// check basic syntax and operations
	using dat::operator-;
	using dat::operator+;
	Vec3D const cVec(-(aVec + bVec));
	Type const aMag{ dat::magnitude(aVec) };
	Type const bMag{ dat::magnitude(bVec) };
	Type const cMag{ dat::magnitude(cVec) };
	Type const magErr{ (cMag*cMag) - (aMag*aMag) - (bMag*bMag) };
	// test not really important here, just to avoid compiler warnings
	if (! (magErr < std::numeric_limits<Type>::epsilon()))
	{
		oss << "Failure of triangle magnitude test" << std::endl;
	}

	// mean vector
	Vec3D const dVec(bVec - aVec);
	using dat::operator*;
	Vec3D const mVec(.5L * (cVec + dVec));
	// projection
	Vec3D const cPrj(dat::dot(cVec, mVec) * dat::unit(mVec));
	Vec3D const dPrj(dat::dot(dVec, mVec) * dat::unit(mVec));
	if (! dat::nearlyEquals(cPrj, dPrj))
	{
		oss << "Failure of projection test" << std::endl;
		oss << dat::infoString(cPrj, "cPrj") << std::endl;
		oss << dat::infoString(dPrj, "dPrj") << std::endl;
	}

	return oss.str();
}

//! Element-wise operations
std::string
dat_array_test2
	()
{
	std::ostringstream oss;

	using Elems = std::array<float, 5u>;

	Elems const values{{ 3.f, 4.f, 5.f, -6.f, -7.f }};
	Elems const recips(dat::reciprocals(values));
	float const expDot{ static_cast<float>(values.size()) };
	float const gotDot{ dat::dot(values, recips) };
	if (! dat::nearlyEquals(gotDot, expDot))
	{
		oss << "Failure of reciprocals test" << std::endl;
		oss << dat::infoString(expDot, "expDot") << std::endl;
		oss << dat::infoString(gotDot, "gotDot") << std::endl;
	}

	return oss.str();
}

}

//! Unit test for std::array
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_array_test0();
	oss << dat_array_test1();
	oss << dat_array_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
