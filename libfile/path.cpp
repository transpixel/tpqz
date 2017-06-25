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
\brief Definitions for file::path
*/


#include "libfile/path.h"

// This implementation forwards to boost::filesystem
#include "filesystem.hpp"


namespace file
{
namespace path
{

std::string
dirname
	( std::string const & anypath
	)
{
	return boost::filesystem::path(anypath).parent_path().string();
}

std::string
basename
	( std::string const & anypath
	)
{
	return boost::filesystem::path(anypath).stem().string();
}

std::string
extension
	( std::string const & anypath
	)
{
	std::string ext{};
	std::string const withdot
		(boost::filesystem::path(anypath).extension().string());
	if (0u < withdot.size())
	{
		ext = std::string(withdot.begin()+1u, withdot.end());
	}
	return ext;
}

std::string
filename
	( std::string const & anypath
	)
{
	return boost::filesystem::path(anypath).filename().string();
}


// Synthesis
// =========

std::string
fullpathDirFile
	( std::string const & dirname
	, std::string const & filename
	)
{
	std::string path{ dirname };
	path += '/';
	path += filename;
	return path;
}

std::string
fullpathDirBaseExt
	( std::string const & dirname
	, std::string const & basename
	, std::string const & extNoDot
	)
{
	std::string path{ dirname };
	path += '/';
	path += basename;
	path += '.';
	path += extNoDot;
	return path;
}


} // path
} // file


