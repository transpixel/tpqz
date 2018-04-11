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

#ifndef io_binary_INCL_
#define io_binary_INCL_

/*! \file
\brief Declarations for namespace io::binary
*/


#include <fstream>
#include <string>
#include <vector>


namespace io
{

/*! \brief Declarations and Definitions for libio.

\par General Concept:

Package-specific conventions for Input/Output involving strings and streams.

\par Special Notes:

+ Provides thread-save wrapping for output streams.


*/
namespace binary
{
	//! Open stream and verify data size
	bool
	prepareStream
		( std::ifstream & ifs
		, std::string const & fpath
		, std::streamsize const & bytesPerRec
		, std::streamsize * const & ptNumRecs
		);

	//! Load collection of objects from binary file
	template <typename Type>
	inline
	bool
	save
		( std::vector<Type> const & items
		, std::string const & fpath
		);

	//! Load collection of objects from binary file
	template <typename Type>
	inline
	std::vector<Type>
	load
		( std::string const & fpath
		);

} // binary

} // io

// Inline definitions
#include "libio/binary.inl"

#endif // io_binary_INCL_

