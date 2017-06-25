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

#ifndef io_sprintf_INCL_
#define io_sprintf_INCL_

/*! \file
\brief Declarations for io::sprintf
*/

#include <string>

namespace io
{

	//! \brief single parameter sprintf function.
	template < typename Type >
	std::string
	sprintf
		( std::string const & fmt
		, Type const & val
		);

	//! override for iostreams mistreatment of unsigned char
	inline
	std::string
	sprintf
		( std::string const & fmt
		, unsigned char const & val
		)
	{
		return io::sprintf<unsigned short>(fmt, (unsigned short)val);
	}

	//! repeat functionality
	std::string
	repeat
		( std::string const & val
		, unsigned const & times
		);

	//! repeat functionality
	std::string
	repeat
		( char const & val
		, unsigned const & times
		);

	//! convert string number to add thousands separator
	template < typename Type >
	std::string
	formatThousands
		( Type const & val
		, std::string const & sep=" "
		);

	//! wrap string in double quotes
	std::string
	quote
		( std::string const & input
		, std::string const & str="'"
		);

	//! utility struct to use with std::for_each
	template < typename Type >
	struct Print
	{
		std::ostream & theOstr;
		std::string const & theFmt;

		explicit
		Print
			( std::ostream & ostr
			, std::string const & fmt
			)
			: theOstr(ostr)
			, theFmt(fmt)
		{ }

		void
		operator()
			( Type const & val
			) const
		{
			theOstr << io::sprintf(theFmt, val);
		}
	};

}

// Inline definitions
#include "libio/sprintf.inl"

#endif // io_sprintf_INCL_
