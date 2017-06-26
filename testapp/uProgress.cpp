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
\brief  This file contains unit test for app::Progress
*/


#include "libapp/Progress.h"

#include "libdat/compare.h"
#include "libdat/info.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
app_Progress_test0
	()
{
	std::ostringstream oss;
	app::Progress const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

	//! Check if current fraction matches expected value
	void
	checkFrac
		( std::ostream & oss
		, app::Progress const & prog
		, double const & expFrac
		, std::string const & note = std::string()
		)
	{
		double const gotFrac{ prog.fractionDone() };
		if (! dat::nearlyEquals(gotFrac, expFrac))
		{
			oss << "Failure of fractionDone test: " << note << std::endl;
			oss << dat::infoString(expFrac, "expFrac") << std::endl;
			oss << dat::infoString(gotFrac, "gotFrac") << std::endl;
		}
	}

//! Check simple operations
std::string
app_Progress_test1
	()
{
	std::ostringstream oss;

	// construct faky meter
	constexpr size_t numMax{ 4u };
	app::Progress prog(numMax);

	// check basic operations
	double const df{ 1. / double(numMax) };
	checkFrac(oss, prog, 0.);
	++prog;
	checkFrac(oss, prog, 1.*df);
	prog.advance(2u);
	checkFrac(oss, prog, 3.*df);

	// check normal termination
	app::Progress copy1(prog);
	++copy1;
	checkFrac(oss, copy1, 1., "normal end");
	++copy1;
	checkFrac(oss, copy1, 1., "after end");

	// check overage termination
	app::Progress copy2(prog);
	copy2.advance(numMax);
	checkFrac(oss, copy2, 1., "pasts end");

	return oss.str();
}


}

//! Unit test for app::Progress
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << app_Progress_test0();
	oss << app_Progress_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
