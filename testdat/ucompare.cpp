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
\brief  This file contains unit test for dat::compare
*/


#include "libdat/compare.h"
#include "libdat/info.h"
#include "libio/stream.h"

#include <array>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <string>


namespace
{

//! Check nearlyEquals for POD
std::string
dat_compare_test0
	()
{
	std::ostringstream oss;

	// integral type
	uint8_t const a1(7u);
	uint8_t const b1(9u);

	uint8_t const a2(a1);
	if (dat::nearlyEquals(a1, b1))
	{
		oss << "Failure of a1,b1 test" << std::endl;
		oss << "a1: " << static_cast<int>(a1) << std::endl;
		oss << "b1: " << static_cast<int>(b1) << std::endl;
		oss << "same: " << dat::nearlyEquals(a1,b1) << std::endl;
	}
	if (! dat::nearlyEquals(a1, a2))
	{
		oss << "Failure of a1,a2/different test" << std::endl;
	}
	if (! dat::nearlyEquals<uint8_t>(a1, b1, (b1-a1+1u))) // checks < vs. <= tol
	{
		oss << "Failure of a1,b1/same test" << std::endl;
	}
	if (dat::nearlyEquals<uint8_t>(a1, b1, 1u)) // checks < vs. <= tol
	{
		oss << "Failure of a1,b1/same test" << std::endl;
		oss << "a1: " << static_cast<int>(a1) << std::endl;
		oss << "b1: " << static_cast<int>(b1) << std::endl;
		oss << "same: " << dat::nearlyEquals<uint8_t>(a1,b1,(b1-a1))
			<< std::endl;
	}

	// float type
	double const delta(1./1024.);
	double const bigtol(1.01 * delta);
	double const smltol(0.99 * delta);
	double const x1(7.);
	double const x2(x1 + delta);
	if (dat::nearlyEquals(x1, x2, smltol))
	{
		oss << "Failure of x1,x2/smltol test" << std::endl;
	}
	if (! dat::nearlyEquals(x1, x2, bigtol))
	{
		oss << "Failure of x1,x2/bigtol test" << std::endl;
	}

	// bool type
	bool const expTT{ true };
	bool const expTF{ false };
	bool const expFT{ false };
	bool const expFF{ true };

	bool const gotTT{ dat::nearlyEquals(true, true) };
	bool const gotTF{ dat::nearlyEquals(true, false) };
	bool const gotFT{ dat::nearlyEquals(false, true) };
	bool const gotFF{ dat::nearlyEquals(false, false) };

	if (! (gotTT == expTT))
	{
		oss << "Failure of bool nearly equals test" << std::endl;
		oss << std::boolalpha << "TT: " << expTT << " " << gotTT << std::endl;
	}
	if (! (gotTF == expTF))
	{
		oss << "Failure of bool nearly equals test" << std::endl;
		oss << std::boolalpha << "TF: " << expTF << " " << gotTF << std::endl;
	}
	if (! (gotFT == expFT))
	{
		oss << "Failure of bool nearly equals test" << std::endl;
		oss << std::boolalpha << "FT: " << expFT << " " << gotFT << std::endl;
	}
	if (! (gotFF == expFF))
	{
		oss << "Failure of bool nearly equals test" << std::endl;
		oss << std::boolalpha << "FF: " << expFF << " " << gotFF << std::endl;
	}

	return oss.str();
}

	//! Structure for which tolerance type is difference from self' type
	struct Compound
	{
		int foo;
		int bar;

		bool
		nearlyEquals
			( Compound const & other
			, int const & tol
			) const
		{
			return 
				(  dat::nearlyEquals(foo, other.foo, tol)
				&& dat::nearlyEquals(bar, other.bar, tol)
				);
		}

		std::string
		infoString
			( std::string const & title = {}
			) const
		{
			return
				title
				+ " " + dat::infoString(foo)
				+ " " + dat::infoString(bar)
				;
		}
	};


//! Check nearlyEquals for more complex structures
std::string
dat_compare_test1
	()
{
	std::ostringstream oss;

	// simple collection compare
	std::array<float, 5u> a1{{ 1.f, 2.f, 5.125f, 6.f, 3.f }};
	std::array<float, 5u> a2{{ 1.f, 2.f, 5.625f, 6.f, 3.f }};
	if (! dat::nearlyEquals(a1, a1))
	{
		oss << "Failure of array a1,a1 test" << std::endl;
	}
	if (dat::nearlyEquals(a1, a2))
	{
		oss << "Failure of array a1,a2 test" << std::endl;
	}

	// compute collection compare
	std::vector<Compound> const data1{ {10, 1}, {20, 2}, {30, 3}, {32, 4} };
	std::vector<Compound> const same(data1.begin(), data1.end());
	std::vector<Compound> const diffItems(data1.rbegin(), data1.rbegin());
	std::vector<Compound> const diffSizes(data1.begin(), data1.begin() + 2);

	constexpr int tol{ 1 };
	if (! dat::nearlyEquals(data1, same, tol))
	{
		oss << "Failure of same test" << std::endl;
	}
	if (  dat::nearlyEquals(data1, diffItems, tol))
	{
		oss << "Failure of items test" << std::endl;
	}
	if (  dat::nearlyEquals(data1, diffSizes, tol))
	{
		oss << "Failure of sizes test" << std::endl;
		oss << dat::infoString(data1.begin(), data1.end()) << std::endl;
		oss << dat::infoString(diffSizes.begin(), diffSizes.end()) << std::endl;
	}

	return oss.str();
}

//! Check unsigned comparision functions
std::string
dat_compare_test1a
	()
{
	std::ostringstream oss;

	// check can subtract function
	std::vector<std::pair<bool, bool> > const gotexps
		{ { dat::canSubtract<size_t>(7u, 3u) , true }
		, { dat::canSubtract<size_t>(3u, 7u) , false}
		, { dat::canSubtract<size_t>(3u, 3u) , true }
		};
	for (size_t nn(0u) ; nn < gotexps.size() ; ++nn)
	{
		bool const & got = gotexps[nn].first;
		bool const & exp = gotexps[nn].second;
		if (! (got == exp))
		{
			oss << "Failure of can subtract test: case: " << nn << std::endl;
		}
	}

	return oss.str();
}

//! Check parity tests;
std::string
dat_compare_test2
	()
{
	std::ostringstream oss;

	if (! dat::isOdd(3u))
	{
		oss << "Failure of isOdd() test" << std::endl;
	}

	if (! dat::isEven(4u))
	{
		oss << "Failure of isEven() test" << std::endl;
	}

	if (! dat::isSameParity(2u, 4u))
	{
		oss << "Failure of isSameParity(2u, 4u) test" << std::endl;
	}
	if (! dat::isSameParity(3u, 5u))
	{
		oss << "Failure of isSameParity(3u, 5u) test" << std::endl;
	}
	if (dat::isSameParity(2u, 5u))
	{
		oss << "Failure of isSameParity(2u, 5u) test" << std::endl;
	}
	if (dat::isSameParity(3u, 4u))
	{
		oss << "Failure of isSameParity(3u, 4u) test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for dat::compare
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_compare_test0();
	oss << dat_compare_test1();
	oss << dat_compare_test1a();
	oss << dat_compare_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
