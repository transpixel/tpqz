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
\brief  This file contains unit test for dat::random
*/


#include "libdat/random.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_random_test0
	()
{
	std::ostringstream oss;
	/*
	dat::random const aNull(dat::nullValue<dat::random>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

	template <size_t Dim>
	void
	checkSamps
		( std::ostream & oss
		, std::array<size_t, Dim> const & ndxSamps
		, size_t const & numElems
		)
	{
		size_t const expSampSize{ ndxSamps.size() };
		std::set<size_t> uniqSamps;
		for (size_t const & ndxSamp : ndxSamps)
		{
			uniqSamps.insert(ndxSamp);
		}

		// check how many - checks no duplicates since set members are unique
		size_t const gotSampSize{ uniqSamps.size() };
		bool const okaySize{ (gotSampSize == expSampSize) };
		if (! okaySize)
		{
			oss << "Failure of sample size test" << std::endl;
			oss << dat::infoString(expSampSize, "expSampSize") << std::endl;
			oss << dat::infoString(gotSampSize, "gotSampSize") << std::endl;
		}
		else
		{
			// check that values are from collection
			for (size_t const & ndxSamp : ndxSamps)
			{
				if (! dat::isValid(ndxSamp))
				{
					oss << "Failure of index validity test" << std::endl;
					break;
				}
				// for test case, sample values must be
				if (! (ndxSamp < numElems))
				{
					oss << "Failure of index value test" << std::endl;
					break;
				}
			}
		}
	}

//! Check random_sample
std::string
dat_random_test1
	()
{
	std::ostringstream oss;

//#define SmallData
#	if defined(SmallData)
	constexpr size_t numElems{ 10u };
	constexpr size_t NSamps{ 9u };
	constexpr size_t numDraws{ 1u };
#	else
	constexpr size_t numElems{ 128u };
	constexpr size_t NSamps{ 110u };
	constexpr size_t numDraws{ 512u };
#	endif

	std::random_device rdev;
	std::mt19937_64 rgen(rdev());
	rgen.seed(7531u);

	// check random sample draw
	for (size_t ndraw{0u} ; ndraw < numDraws ; ++ndraw)
	{
		// draw sample (w/o replacement)
		std::array<size_t, NSamps> const samps
			(dat::random::index_sample<NSamps>(numElems, rgen));

		// check results
		checkSamps(oss, samps, numElems);
		if (! oss.str().empty())
		{
			break;
		}
	}

	// check special case of drawing all
	{
		std::array<size_t, 16u> const samps
			(dat::random::index_sample<16u>(16u, rgen));
		checkSamps(oss, samps, 16u);
	}

	// check for error in case of requesting too many
	{
		std::array<size_t, 4u> const samps
			(dat::random::index_sample<4u>(2u, rgen));
		if (  dat::isValid(samps[0]))
		{
			oss << "Failure of excess sample request test" << std::endl;
			oss << dat::infoString(samps, "samps.excess") << std::endl;
		}
	}
	return oss.str();
}


}

//! Unit test for dat::random
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_random_test0();
	oss << dat_random_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
