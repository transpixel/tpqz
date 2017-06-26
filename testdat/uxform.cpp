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
\brief  This file contains unit test for dat::xform
*/


#include "libdat/xform.h"

#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_xform_test0
	()
{
	std::ostringstream oss;
	/*
	dat::xform const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	*/
	return oss.str();
}

//! Example class w/ expected methods
class Xform
{
	bool theFakeValue{ true };

public:

	Xform
		() = default;

	//! True if this instance is valid (e.g. can be used in operations)
	bool
	isValid
		() const
	{
		return theFakeValue;
	}

	//! Resultant transformation equivalent to this after prior
	Xform
	compositeAfter
		( Xform const & // prior
		) const
	{
		return Xform{};
	}

	//! Inverse of this
	Xform
	inverse
		() const
	{
		return Xform{};
	}

};

//! Check for presence of basic operations
std::string
dat_xform_test1
	()
{
	std::ostringstream oss;

	Xform const xBwA{};
	Xform const xCwB{};
	Xform const xCwA(dat::xform::composite(xCwB, xBwA));
	Xform const xAwC(dat::xform::inverse(xCwA));

	// pretend to do something to keep compiler quite
	if (! xAwC.isValid())
	{
		oss << "Failure of fake test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for dat::xform
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_xform_test0();
	oss << dat_xform_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
