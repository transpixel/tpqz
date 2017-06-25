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

#ifndef file_path_INCL_
#define file_path_INCL_

/*! \file
\brief Declarations for file::path
*/


#include <string>


namespace file
{

/*! \brief Functions for file system path management.

\par Example
\dontinclude testfile/upath.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace path
{
	//! Directory component extracted from anypath
	std::string
	dirname
		( std::string const & anypath
		);

	//! Filename component (basename+.+extension) from anypath
	std::string
	filename
		( std::string const & anypath
		);

	//! Leading part of filename (before last '.')
	std::string
	basename
		( std::string const & anypath
		);

	//! Trailing part of filename() (after last '.')
	std::string
	extension
		( std::string const & anypath
		);

	//
	// Synthesis
	//

	//! Full path from components
	std::string
	fullpathDirFile
		( std::string const & dirname
		, std::string const & filename
		);

	//! Full path from components
	std::string
	fullpathDirBaseExt
		( std::string const & dirname
		, std::string const & basename
		, std::string const & extNoDot
		);

}

}

// Inline definitions
// #include "libfile/path.inl"

#endif // file_path_INCL_

