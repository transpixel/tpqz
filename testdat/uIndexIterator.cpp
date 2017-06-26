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
\brief  This file contains unit test for dat::IndexIterator
*/


#include "libdat/IndexIterator.h"

#include "libdat/compare.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <iterator>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_IndexIterator_test0
	()
{
	std::ostringstream oss;
	dat::IndexIterator const aNull;
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Simple operation
std::string
dat_IndexIterator_test1
	()
{
	std::ostringstream oss;

	constexpr size_t expSize{ 7u };
	constexpr size_t expBeg{  5u };
	constexpr size_t expEnd{ expBeg + expSize };
	constexpr size_t expLast{ expEnd - 1u};

	std::ostringstream log;

	log << dat::infoString(expBeg, "expBeg") << std::endl;
	log << dat::infoString(expLast, "expLast") << std::endl;
	log << dat::infoString(expEnd, "expEnd") << std::endl;

	dat::IndexPair const expBegEnd{ expBeg, expEnd };

	// check construction validity
	dat::IndexIterator iter{ expBegEnd };
	if (! iter)
	{
		oss << "Failure of begin validity test" << std::endl;
	}

	// check begin index
	size_t const gotBeg{ *iter };
	if (! dat::nearlyEquals(gotBeg, expBeg))
	{
		oss << "Failure of begin value test" << std::endl;
	}

	// advance and check last
	log << dat::infoString(iter, "   0)++") << std::endl;
	for (size_t nn{0u} ; nn < (expSize-1u) ; ++nn)
	{
		++iter;
		log << dat::infoString(iter, "loop)++") << std::endl;
	}
	log << dat::infoString(iter, "----)++") << std::endl;
	if (! iter)
	{
		oss << "Failure of last validity test" << std::endl;
	}
	size_t const gotLast{ *iter };
	if (! dat::nearlyEquals(gotLast, expLast))
	{
		oss << "Failure of last value test" << std::endl;
		oss << dat::infoString(expLast, "expLast") << std::endl;
		oss << dat::infoString(gotLast, "gotLast") << std::endl;
	}

	// advance to end
	++iter;
	log << dat::infoString(iter, "end1)++") << std::endl;
	if (  iter)
	{
		oss << "Failure of end validity test" << std::endl;
	}
	// advance after end
	++iter;
	log << dat::infoString(iter, "end2)++") << std::endl;
	if (  iter)
	{
		oss << "Failure of after-end validity test" << std::endl;
	}

	// io::out() << log.str() << std::endl;

	return oss.str();
}


}

//! Unit test for dat::IndexIterator
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_IndexIterator_test0();
	oss << dat_IndexIterator_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
