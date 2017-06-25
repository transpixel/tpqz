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
\brief Definitions for io::text
*/


#include "libio/text.h"

#include "libio/string.h"


namespace io
{
namespace text
{

bool
hasKey
	( std::vector<std::string> const & candidates
	, std::string const & typekey
	)
{
	bool found{ false };
	for (std::string const & cand : candidates)
	{
		if (cand == typekey)
		{
			found = true;
			break;
		}
	}
	return found;
}


std::vector<std::string>
linesFrom
	( std::string const & bigString
	)
{
	std::vector<std::string> lines;
	std::istringstream iss(bigString);
	std::string line;
	while (iss && (! iss.eof()))
	{
		std::getline(iss, line);
		lines.push_back(line);
	}
	return lines;
}

}
}

