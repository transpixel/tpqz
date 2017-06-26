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
\brief  This file contains unit test for math::smooth
*/


#include "libmath/smooth.h"

#include "libdat/info.h"
#include "libio/stream.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>


namespace
{
	bool
	sameData
		( std::vector<double> const & got
		, std::vector<double> const & exp
		)
	{
		bool same(got.size() == exp.size());
		if (same)
		{
			same = std::equal(exp.begin(), exp.end(), got.begin());
		}
		return same;
	}

//! Check trival case
std::string
math_smooth_test0
	()
{
	std::ostringstream oss;
	
	std::vector<double> const data {1., 2., 3., 4.};
	size_t const halfsize(0u);
	std::vector<double> const expValues(data);
	std::vector<double> const gotValues
		(math::smooth::triangleWrapped<double>(data, halfsize));

	if (! sameData(gotValues, expValues))
	{
		oss << "Failure of triangle zero-window test" << std::endl;
		oss << dat::infoString
			(expValues.begin(), expValues.end(), "expValues") << std::endl;
		oss << dat::infoString
			(gotValues.begin(), gotValues.end(), "gotValues") << std::endl;
	}

	return oss.str();
}

//! Check triangle window
std::string
math_smooth_test1
	()
{
	std::ostringstream oss;

	// verify window shape with simple delta function
	std::vector<double> const delta
		{  400.,   0.,   0.,   0.,  0., 0., 0.,   0.,   0.,   0.  };
//		{    0.,   0.,   0.,   0.,  0., 1., 0.,   0.,   0.,   0.  };
//		{    1.,   0.,   0.,   0.,  0., 0., 0.,   0.,   0.,   0.  };
	size_t const halfsize(3u);
	std::vector<double> const expValues
		{  100.,  75.,  50.,  25.,  0., 0., 0.,  25.,  50.,  75.  };
	std::vector<double> const gotValues
		(math::smooth::triangleWrapped<double>(delta, halfsize));

	if (! sameData(gotValues, expValues))
	{
		oss << "Failure of triangle delta test" << std::endl;
		oss << dat::infoString
			(expValues.begin(), expValues.end(), "expValues") << std::endl;
		oss << dat::infoString
			(gotValues.begin(), gotValues.end(), "gotValues") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for math::smooth
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << math_smooth_test0();
	oss << math_smooth_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
