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

#ifndef io_string_INCL_
#define io_string_INCL_

/*! \file
\brief Declarations for io::string
*/


#include <string>


namespace io
{

//! \brief String manipulation function.
namespace string
{
	//! Characters considered to be "white space"
	constexpr char sWhiteSpace[] = " \n\t\v\r\f";

	//! Decode string as a double.
	template <typename DataType>
	inline
	DataType
	from
		( std::string const & str
		, DataType const & badValue
		);

	//! String with previous substring replaced with new one
	std::string
	replaced
		( std::string const & origString //!< From this string,
		, std::string const & prevSubString //!< find and remove this,
		, std::string const & nextSubString //!< and replace with this
		);

	//! String contents previous to (and excluding) endString
	std::string
	before
		( std::string const & orig
		, std::string const & endString
		);

	//! Convert orig to upper case
	std::string
	upper
		( std::string const & orig
		);

	//! True if the upper() conversion of both strings match
	bool
	upperMatch
		( std::string const strA
		, std::string const strB
		);
	
	//! True if cc is in string
	bool
	contains
		( std::string const & str
		, char const & cc
		);

	//! True if pattern is contained within str
	bool
	contains
		( std::string const & str
		, std::string const & pattern
		);

	//! True if cc is in white space list (ref code).
	bool
	isWhiteSpace
		( char const & cc
		);

	//! Double-Quoted String
	std::string
	dqStr
		( std::string const & text
		);

	//! Single-Quoted String
	std::string
	sqStr
		( std::string const & text
		);

}

}

// Inline definitions
#include "libio/string.inl"

#endif // io_string_INCL_

