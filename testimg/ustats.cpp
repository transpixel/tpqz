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
\brief  This file contains unit test for img::stats
*/


#include "libimg/stats.h"

#include "libdat/compare.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
img_stats_test0
	()
{
	std::ostringstream oss;
	/*
	img::stats const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	*/
	return oss.str();
}

//! Check pixel averaging
std::string
img_stats_test1
	()
{
	std::ostringstream oss;

	// classic byte values
	{
		std::vector<uint8_t> const values{ 100u, 200u, 153u, 147u };
		uint8_t const expAve{ 150u };
		uint8_t const gotAve
			{ img::stats::meanOf<uint8_t>(values.begin(), values.end()) };
		if (! dat::nearlyEquals(gotAve, expAve))
		{
			oss << "Failure of average uint8_t test" << std::endl;
			oss << dat::infoString(expAve, "expAve") << std::endl;
			oss << dat::infoString(gotAve, "gotAve") << std::endl;
		}
	}

	// floating point data
	{
		std::vector<float> const values{ 1.f, 2.f, 1.25f, 1.75f };
		float const expAve{ 1.50f };
		float const gotAve
			{ img::stats::meanOf<float>(values.begin(), values.end()) };
		if (! dat::nearlyEquals(gotAve, expAve))
		{
			oss << "Failure of average float test" << std::endl;
			oss << dat::infoString(expAve, "expAve") << std::endl;
			oss << dat::infoString(gotAve, "gotAve") << std::endl;
		}
	}

	// compound data
	{
		using F3 = std::array<float, 3u>;
		std::vector<F3> const values
			{  {{ -7.f, -17.f, 27.f }}
			,  {{ -9.f, -19.f, 29.f }}
			};
		F3 const expAve{ {-8.f, -18.f, 28.f} };
		F3 const gotAve
			( img::stats::meanOf<F3>(values.begin(), values.end()) );
		if (! dat::nearlyEquals(gotAve, expAve))
		{
			oss << "Failure of average F3 test" << std::endl;
			oss << dat::infoString(expAve, "expAve") << std::endl;
			oss << dat::infoString(gotAve, "gotAve") << std::endl;
		}
	}

	return oss.str();
}


}

//! Unit test for img::stats
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << img_stats_test0();
	oss << img_stats_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
