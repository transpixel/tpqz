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

#ifndef cloud_io_INCL_
#define cloud_io_INCL_

/*! \file
\brief Declarations for cloud::io
*/


#include "libcloud/cast.h"
#include "libcloud/cloud.h"
#include "libcloud/FixedPoint.h"
#include "libga/ga.h"

#include <array>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "libdat/info.h"
#include "libio/stream.h"


namespace cloud
{
	//! Format used by helix first-stage processing s/w
	using RecordBin = std::array<float, 7u>;

/*! \brief Functions for input/output of point cloud data.

\par Example
\dontinclude testcloud/uio.cpp
\skip ExampleStart
\until ExampleEnd
*/
namespace io
{
	//! Load data in direct binary format
	std::vector<RecordBin>
	loadAsBinary
		( std::string const & fpath
		);

	//! Load and cast data to compacted representation
	std::vector<FixedPoint>
	loadAsFixed
		( std::string const & fpath
		);

	//! Save cloud data in ascii format
	template <typename FwdIter>
	inline
	bool
	saveFixedPointAsAscii
		( std::ostream & ostrm
		, FwdIter const & beg //!< *iter: FixedPoint
		, FwdIter const & end
		, std::string const & fmt = {"%9.6f"}
		);

	//! Convenience access to general func
	bool
	saveAsAscii
		( std::ostream & ostrm
		, std::vector<FixedPoint> const & fpnts
		, std::string const & fmt = {"%9.6f"}
		);

}

}

// Inline definitions
#include "libcloud/io.inl"

#endif // cloud_io_INCL_

