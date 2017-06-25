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
\brief Definitions for io::file
*/


#include "libio/file.h"
#include "libio/stream.h"

#include <iostream> // debug


namespace io
{
namespace file
{
//======================================================================

std::string
fullPathForPosix
	( std::string const & anyPath
	)
{
	std::string fpath;

	char * const rp(realpath(anyPath.c_str(), nullptr));
	if (rp)
	{
		fpath = std::string(rp);
		free(rp);
	}

	return fpath;
}

size_t
nthPosFromEnd
	( std::string const str
	, size_t const & nth
	, char const & sep
	)
{
	size_t hits(0u);
	size_t pos = str.rfind(sep);
	while (0u < pos)
	{
		++hits;
		if (hits < nth)
		{
			pos = str.rfind(sep, pos-1);
		}
		else
		{
			break;
		}
//io::out() << "pos,hits: " << pos << " " << hits << std::endl;
	}
	return pos;
}

//! trailing part of path
std::string
elidedPath
	( std::string const & anyPath
	, size_t const & numParts
	, char const & sep
	)
{
	std::string epath;
	std::string const fpath(fullPathForPosix(anyPath));
	size_t const pos(nthPosFromEnd(fpath, numParts, sep));
	if (0u < pos)
	{
		// replace start with ellipsis
		epath = "...";
	}
	// else // use full path

	// assign trailing part of input path
	epath.append(fpath.begin() + pos, fpath.end());

	return epath;
}

//======================================================================
}
}

