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
\brief  This file contains unit test for io::Log
*/


#include "libio/stream.h"

#include "libio/stream.h"
#include "libio/string.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
io_Log_test0
	()
{
	std::ostringstream oss;
	/*
	io::Log const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	*/
	return oss.str();
}

//! Check simple case (to string stream)
std::string
io_Log_test1
	()
{
	std::ostringstream oss;

	// create a simulated log sink
	std::ostringstream sink;

	// attach stream to log
	std::ostream * const ptSink = &sink;
	io::log::addSink(ptSink);

	// put information to log
	std::string const expMsg("test message");
	io::log() << expMsg << std::endl;

	// check that log sink contains the correct info
	std::string const gotMsgPlus(sink.str());
	if (! io::string::contains(gotMsgPlus, expMsg))
	{
		oss << "Failure of log message test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for io::Log
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << io_Log_test0();
	oss << io_Log_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
