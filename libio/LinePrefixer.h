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

#ifndef io_LinePrefixer_INCL_
#define io_LinePrefixer_INCL_

/*! \file
\brief Declarations for io::LinePrefixer
*/


#include <string>


namespace io
{

/*! \brief Function object to manange multiple line output of many values.

\par Example
\dontinclude testio/uLinePrefixer.cpp
\skip ExampleStart
\until ExampleEnd
*/

struct LinePrefixer
{
	std::string const thePreText{};
	size_t const theNumPerLine{ 25u };
	size_t theFieldCount{ 0u };
	size_t theValueWide{ 4u };

	//! value ctor
	explicit
	LinePrefixer
		( std::string const & preText
		);

	//! Generate start of line text
	std::string
	nextFieldString
		( size_t const & value
		);

}; // LinePrefixer

} // io

// Inline definitions
// #include "libio/LinePrefixer.inl"

#endif // io_LinePrefixer_INCL_

