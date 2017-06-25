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

#ifndef geo_io_INCL_
#define geo_io_INCL_

/*! \file
\brief Declarations for geo::io
*/


#include "libdat/validity.h"
#include "libga/ga.h"

#include <iostream>
#include <vector>


namespace geo
{

/*! \brief Functions for input/output of geometric entitities.

\par Example
\dontinclude testgeo/uio.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace io
{
	//! Load locations from comma separated values format
	std::vector<ga::Vector>
	loadFromCSV
		( std::istream & istrm
		, size_t const & expectSize = dat::nullValue<size_t>()
			//!< if not 0, reserve this much space
		);

	//! Save locations to comma separated values format
	bool
	saveToCSV
		( std::ostream & ostrm
		, std::vector<ga::Vector> const & pnts
		);
}

}

// Inline definitions
// #include "libgeo/io.inl"

#endif // geo_io_INCL_

