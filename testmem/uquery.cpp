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
\brief  This file contains unit test for mem::query
*/


#include "libmem/query.h"

#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace
{

//! Check for common functions
std::string
mem_query_test0
	()
{
	std::ostringstream oss;
	/*
	mem::query const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	*/
	return oss.str();
}

//! Check check memory use states
std::string
mem_query_test1
	()
{
	std::ostringstream oss;

	// structures large enough to dominate process space
	struct Size1k
	{
		uint8_t theBytes[1024u];
	};
	struct Size1M
	{
		Size1k theks[1024u];
	};

	std::vector<std::string> const keys{ "VmSize" };
	mem::query::Reporter report(keys);

	std::string gotLo1;
	std::string gotHi;
	std::string gotLo2;
	report("start");
	{
		std::vector<Size1M> const a100M_a(100u);
		gotLo1 = mem::query::infoString("", keys);
		report("after 100M a:");
		{
			std::vector<Size1M> a100M_b(1024u);
			gotHi = mem::query::infoString("", keys);
			report("after 100M b:");
		}
		report("free 100M b:");
		gotLo2 = mem::query::infoString("", keys);
	}
	report("free 100M a:");

	if (gotLo1 == gotHi)
	{
		oss << "Failure of Lo1!=Hi test" << std::endl;
		oss << "gotLo1: " << gotLo1 << std::endl;
		oss << "gotHi : " << gotHi << std::endl;
		oss << "gotLo2: " << gotLo2 << std::endl;
	}
	if (! (gotLo1 == gotLo2))
	{
		oss << "Failure of Lo1==Lo2 test" << std::endl;
		oss << "gotLo1: " << gotLo1 << std::endl;
		oss << "gotLo2: " << gotLo2 << std::endl;
	}

	// oss << report.infoString();

	return oss.str();
}


}

//! Unit test for mem::query
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << mem_query_test0();
	oss << mem_query_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
