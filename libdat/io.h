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

#ifndef dat_io_INCL_
#define dat_io_INCL_

/*! \file
\brief Declarations for dat::io
*/


#include <fstream>


namespace dat
{

/*! \brief General data/IO functions.

\par Example
\dontinclude testdat/uio.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace io
{
	//! Save values to binary stream
	template <typename FwdIter>
	inline
	bool
	saveBinary
		( std::ostream & ostrm
		, FwdIter const & beg //!< *iter = FwdIter::value_type
		, FwdIter const & end
		);

	//! Load binary values from stream
	template <typename FwdIter>
	inline
	bool
	loadBinary
		( std::istream & istrm
		, FwdIter const & beg //!< *iter = FwdIter::value_type
		, FwdIter const & end
		, std::streamsize * const & ptGotBytes = nullptr
		);
}

}

// Inline definitions
#include "libdat/io.inl"

#endif // dat_io_INCL_

