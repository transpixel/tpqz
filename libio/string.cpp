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
\brief Definitions for io::string
*/


#include "libio/string.h"

#include <cctype>


namespace io
{
namespace string
{
//======================================================================

std::string
replaced
	( std::string const & origString
	, std::string const & prevSubString
	, std::string const & nextSubString
	)
{
	std::string nextString;
	size_t const ndx(origString.find(prevSubString));
	if (ndx != std::string::npos)
	{
		nextString = origString;
		nextString.replace(ndx, nextSubString.size(), nextSubString);
	}
	return nextString;
}

std::string
before
	( std::string const & orig
	, std::string const & endString
	)
{
	std::string start;
	size_t const ndx(orig.find(endString));
	start = orig.substr(0, ndx);
	return start;
}

std::string
upper
	( std::string const & orig
	)
{
	std::string str;
	for (size_t nn(0u) ; nn < orig.size() ; ++nn)
	{
		str += std::toupper(orig[nn]);
	}
	return str;
}

bool
upperMatch
	( std::string const strA
	, std::string const strB
	)
{
	return (upper(strA) == upper(strB));
}

bool
contains
	( std::string const & str
	, char const & cc
	)
{
	return (str.find(cc) != std::string::npos);
}

bool
contains
	( std::string const & str
	, std::string const & pattern
	)
{
	return (str.find(pattern) != std::string::npos);
}

bool
isWhiteSpace
	( char const & cc
	)
{
	return contains(sWhiteSpace, cc);
}

std::string
dqStr
	( std::string const & text
	)
{
	static std::string const quote("\"");
	return (quote + text + quote);
}

std::string
sqStr
	( std::string const & text
	)
{
	static std::string const quote("'");
	return (quote + text + quote);
}

//======================================================================
}
}

