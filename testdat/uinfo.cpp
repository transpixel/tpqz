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
\brief  This file contains unit test for dat::info
*/


#include "libdat/info.h"
#include "libdat/template.h"
#include "libio/stream.h"

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>


namespace
{
	//! Arbitrary custom class
	struct Custom
	{
		//! "Standard" infoString method
		std::string
		infoString
			( std::string const & title = std::string()
			) const
		{
			return std::string("something interesting" + title);
		}
	};

//! Check for common functions
std::string
dat_info_test0
	()
{
	std::ostringstream oss;

	Custom const someClass;
	(void)someClass.infoString("atitle"); // check expected signature

	// test: check mostly just that compiler can resolve these
	if (dat::infoString(double(0)).empty())
	{
		oss << "Failure of infoString double test" << std::endl;
	}
	if (dat::infoString(int(0)).empty())
	{
		oss << "Failure of infoString int test" << std::endl;
	}
	if (dat::infoString(uint8_t(0)).empty())
	{
		oss << "Failure of infoString uint8_t test" << std::endl;
	}
	if (dat::infoString(std::pair<int, double>(0, 0.)).empty())
	{
		oss << "Failure of infoString pair test" << std::endl;
	}
	if (dat::infoString(someClass).empty())
	{
		oss << "Failure of infoString someClass test" << std::endl;
	}

	return oss.str();
}

//! Check TODO....
std::string
dat_info_test1
	()
{
	std::ostringstream oss;
//	oss << "Failure: implement this test!" << std::endl;
	return oss.str();
}


}

//! Unit test for dat::info
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_info_test0();
	oss << dat_info_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
