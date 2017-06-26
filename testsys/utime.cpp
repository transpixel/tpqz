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
\brief  This file contains unit test for sys::time
*/


#include "libsys/time.h"

#include "libio/stream.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>


namespace
{

//! Check for basics
std::string
sys_time_test0
	()
{
	std::ostringstream oss;

	double const absT0(sys::time::now());
	double const relT0(sys::time::relativeNow());

	long const microSleep(123456l);
	std::this_thread::sleep_for(std::chrono::microseconds(microSleep));

	double const absT1(sys::time::now());
	double const relT1(sys::time::relativeNow());

	double const absDelta(absT1 - absT0);
	double const relDelta(relT1 - relT0);

	// check within a few micro seconds
	double const microDif(std::abs(1.e6*(relDelta - absDelta)));
	static double const microTol(4.);  // allow for run time of code
	if (! (microDif < microTol))
	{
		oss << "Failure of relative time test" << std::endl;
		oss << "absT0: " << absT0 << std::endl;
		oss << "absT1: " << absT1 << std::endl;
		oss << "absDelta: " << absDelta << std::endl;
		oss << "relT0: " << relT0 << std::endl;
		oss << "relT1: " << relT1 << std::endl;
		oss << "relDelta: " << relDelta << std::endl;
		oss << "microDif: " << microDif << std::endl;
		oss << "microTol: " << microTol << std::endl;
	}

	return oss.str();
}


}

//! Unit test for sys::time
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << sys_time_test0();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
