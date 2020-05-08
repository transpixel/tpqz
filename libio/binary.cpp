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
\brief Definitions for namespace io::binary
*/


#include "libio/binary.h"

#include <cassert>


namespace io
{

namespace binary
{

bool
prepareStream
	( std::ifstream & ifs
	, std::string const & fpath
	, std::streamsize const & bytesPerRec
	, std::streamsize * const & ptNumRecs
	)
{
	bool okay{ false };

	// open at end of file
	ifs.open
		( fpath
		, std::ios_base::binary | std::ios_base::ate
		);

	// check size
	if (ifs.good())
	{
		// determine number of records
		std::streamsize const allBytes
			{ static_cast<std::streamsize>(ifs.tellg()) };
		std::streamsize const numRecs{ allBytes / bytesPerRec };
		std::streamsize const expBytes{ numRecs * bytesPerRec };
		if (allBytes == expBytes)
		{
			assert(ptNumRecs);
			*ptNumRecs = numRecs;

			// return to start of file
			ifs.seekg(0);
			okay = ifs.good();
		}
		else
		{
			// incongruent filesize
			ifs.close();
		}
	}
	// else // bad file open

	return okay;
}

} // binary

} // io

