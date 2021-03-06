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

#ifndef ga_io_INCL_
#define ga_io_INCL_

/*! \file
\brief Declarations for ga::io
*/


#include "libga/ga.h"

#include <string>
#include <vector>


namespace ga
{

/*! \brief Functions providing input/output for GA entities.

\par Example
\dontinclude testga/uio.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace io
{
	//! True if points successfully saved (gnuplot compatible ascii format)
	bool
	saveToAscii
		( std::vector<ga::Vector> const & points
		, std::string const & fpath
		);

	//! Load points from a (white-space separated) ascii file
	std::vector<ga::Vector>
	fromAscii
		( std::string const & fpath
		);


} // io

} // ga

// Inline definitions
// #include "libga/io.inl"

#endif // ga_io_INCL_

