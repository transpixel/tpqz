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
\brief  This file contains unit test for math::
*/


#include "libmath/math.h"

#include "libdat/dat.h"
#include "libio/stream.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace
{

//! Check fundamental functions
std::string
math__test0
	()
{
	std::ostringstream oss;

	return oss.str();
}

//! Check basic angle functions
std::string
math__test1
	()
{
	std::ostringstream oss;

	// define a few angles
	using math::pi;
	std::vector<double> const anyAngles
		{ -7.
		, -5.
		, -pi
		, 0.
		, 1.
		, pi
		, 5.
		, 7.
		};
	std::vector<double> const expPvs
		{ -7. + 2.*pi
		, -5. + 2.*pi
		, -pi
		, 0.
		, 1.
		, -pi // [-pi, pi)
		, 5. - 2.*pi
		, 7. - 2.*pi
		};
	// compute values
	std::vector<double> gotPvs;
	gotPvs.reserve(anyAngles.size());
	for (double const & anyAngle : anyAngles)
	{
		gotPvs.push_back(math::principalAngle(anyAngle));
	}
	assert(gotPvs.size() == expPvs.size());


	// check if all members are the same
	bool const same
		( std::equal
			( gotPvs.begin(), gotPvs.end(), expPvs.begin()
			, []
				(double const & v1, double const & v2)
				{ return dat::nearlyEquals(v1, v2); }
			)
		);
	if (! same)
	{
		oss << "Failure of same principal angle test" << std::endl;
		size_t const ndig(18u);
		oss << std::fixed << std::setprecision(ndig);
		for (size_t nn(0u) ; nn < expPvs.size() ; ++nn)
		{
			oss << "exp,got:"
				<< "  " << std::setw(4+ndig) << expPvs[nn]
				<< "  " << std::setw(4+ndig) << gotPvs[nn]
				<< std::endl;
		}
	}

	return oss.str();
}


//! Check basic arithmetic functions
std::string
math__test2
	()
{
	std::ostringstream oss;

	double const valueD(11.);
	double const expSqD(121.);
	double const expCubeD(1331.);

	uint16_t const valueU(11u);
	uint16_t const expSqU(121u);
	uint16_t const expCubeU(1331u);

	double const gotSqD(math::sq(valueD));
	double const gotSqU(math::sq(valueU));
	double const gotCubeD(math::cube(valueD));
	double const gotCubeU(math::cube(valueU));

	// comparisons should be exact for these values
	if (! (gotSqD == expSqD))
	{
		oss << "Failure of squared double test" << std::endl;
	}
	if (! (gotCubeD == expCubeD))
	{
		oss << "Failure of cubed double test" << std::endl;
	}
	if (! (gotSqU == expSqU))
	{
		oss << "Failure of squared uint16_t test" << std::endl;
	}
	if (! (gotCubeU == expCubeU))
	{
		oss << "Failure of cubed uint16_t test" << std::endl;
	}

	return oss.str();
}

//! Check algorithms
std::string
math__test3
	()
{
	std::ostringstream oss;

	{
		std::vector<int> const data;
		int const got(math::maxAbs<int>(data.begin(), data.end()));
		if (dat::isValid(got))
		{
			oss << "Failure of maxAbs empty data test" << std::endl;
		}
	}

	float const exp(8.f);
	{
		std::vector<float> const data{-5.f, 4.f, -8.f};
		float const got(math::maxAbs<float>(data.begin(), data.end()));
		if (! dat::nearlyEquals(got, exp))
		{
			oss << "Failure of maxAbs general test" << std::endl;
		}
	}

	{
		std::vector<float> const data{-5.f, -4.f, -8.f};
		float const got(math::maxAbs<float>(data.begin(), data.end()));
		if (! dat::nearlyEquals(got, exp))
		{
			oss << "Failure of maxAbs all negative test" << std::endl;
		}
	}

	{
		std::vector<float> const data{ 5.f,  4.f,  8.f};
		float const got(math::maxAbs<float>(data.begin(), data.end()));
		if (! dat::nearlyEquals(got, exp))
		{
			oss << "Failure of maxAbs all positive test" << std::endl;
		}
	}

	return oss.str();
}


//! Check difference operations
std::string
math__test4
	()
{
	std::ostringstream oss;

	// unsigned
	{
		uint8_t const expSep(23u);
		uint8_t const valA(73u);
		uint8_t const valB(valA + expSep);
		uint8_t const gotSep1(math::separation(valA, valB));
		uint8_t const gotSep2(math::separation(valB, valA));
		if (! dat::nearlyEquals(expSep, gotSep1))
		{
			oss << "Failure of uint8_t separation (<) test" << std::endl;
		}
		if (! dat::nearlyEquals(expSep, gotSep2))
		{
			oss << "Failure of uint8_t separation (>) test" << std::endl;
		}
	}

	// signed type
	{
		double const expSep(17.);
		double const valA(-5.);
		double const valB(valA + expSep);
		double const gotSep1(math::separation(valA, valB));
		double const gotSep2(math::separation(valB, valA));
		if (! dat::nearlyEquals(expSep, gotSep1))
		{
			oss << "Failure of double separation (<) test" << std::endl;
		}
		if (! dat::nearlyEquals(expSep, gotSep2))
		{
			oss << "Failure of double separation (>) test" << std::endl;
		}
	}

	//! signed with overflow
	{
		int8_t const expSep(dat::nullValue<int8_t>());
		int8_t const valA(-26);
		int8_t const valB(126);
		int8_t const gotSep1(math::separation(valA, valB));
		int8_t const gotSep2(math::separation(valB, valA));
		if (! (expSep == gotSep1))
		{
			oss << "Failure of int8_t separation (<) test" << std::endl;
			oss << " expSep: " << +expSep << std::endl;
			oss << "gotSep1: " << +gotSep1 << std::endl;
		}
		if (! (expSep == gotSep2))
		{
			oss << "Failure of int8_t separation (>) test" << std::endl;
			oss << " expSep: " << +expSep << std::endl;
			oss << "gotSep2: " << +gotSep2 << std::endl;
		}
	}

	/*
	// std::abs -- example of unexpected result (return 104)
	{
		int8_t const valA(-26);
		int8_t const valB(126);
		int8_t const diff1(valA - valB);
		int8_t const diff2(valB - valA);
		int8_t const gotSep1(std::abs(diff1));
		int8_t const gotSep2(std::abs(diff2));

		io::out() << "   valA: " << +valA << std::endl;
		io::out() << "   valB: " << +valB << std::endl;
		io::out() << "  diff1: " << +  diff1 << std::endl;
		io::out() << "  diff2: " << +  diff2 << std::endl;
		io::out() << "gotSep1: " << +gotSep1 << std::endl;
		io::out() << "gotSep2: " << +gotSep2 << std::endl;
	}
	*/

	return oss.str();
}

}

//! Unit test for math::
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << math__test0();
	oss << math__test1();
	oss << math__test2();
	oss << math__test3();
	oss << math__test4();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
