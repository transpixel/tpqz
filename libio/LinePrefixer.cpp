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
\brief Definitions for io::LinePrefixer
*/


#include "libio/LinePrefixer.h"

#include <iomanip>
#include <sstream>


namespace io
{

// explicit
LinePrefixer :: LinePrefixer
	( std::string const & preText
	)
	: thePreText(preText)
{ }

std::string
LinePrefixer :: nextFieldString
	( size_t const & value
	)
{
	std::ostringstream oss;
	size_t const preWide{ thePreText.size() };

	// check for new line condition
	if (0u == (theFieldCount % theNumPerLine))
	{
		oss << std::endl;

		// add appropriate line prefix
		if (0u == theFieldCount)
		{
			oss << std::setw(preWide) << thePreText;
		}
		else
		{
			oss << std::setw(preWide) << "";
		}

		// add line start indicator
		oss << ": ";
	}

	oss << " " << std::setw(theValueWide) << value;

	return oss.str();
}

} // io

