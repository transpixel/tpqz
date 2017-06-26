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
\brief Definitions for app::ProcessLogger
*/


#include "libapp/ProcessLogger.h"


namespace app
{

// static
void
ProcessLogger :: mark
	( ProcessLogger * const & ptLog
	, std::string const & msg
	)
{
	if (ptLog)
	{
		ptLog->mark(msg);
	}
}


// explicit
ProcessLogger :: ProcessLogger
	( std::vector<std::string> const memKeys
	)
	: theTimer{}
	, theOssMem{}
	, theMemReporter(memKeys)
{
}

void
ProcessLogger :: markMem
	( std::string const & msg
	)
{
	theMemReporter(msg);
}

void
ProcessLogger :: markTime
	( std::string const & msg
	)
{
	theTimer.start(msg);
}

void
ProcessLogger :: mark
	( std::string const & msg
	)
{
	markMem(msg);
	markTime(msg);
}

void
ProcessLogger :: operator()
	( std::string const & msg
	)
{
	mark(msg);
}

std::string
ProcessLogger :: infoStringMem
	( std::string const & title
	) const
{
	return theMemReporter.infoString(title);
}

std::string
ProcessLogger :: infoStringTime
	( std::string const & title
	) const
{
	return theTimer.infoString(title);
}
}

