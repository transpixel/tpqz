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

#ifndef cam_io_INCL_
#define cam_io_INCL_

/*! \file
\brief Declarations for cam::io
*/


#include "libcam/XRefDists.h"
#include "libcam/XRefRays.h"
#include "libcam/XRefSpots.h"

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>


namespace cam
{

/*! \brief Functions for input/output of libcam entities

\par Example
\dontinclude testcam/uio.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace io
{
	//! Name used by save*() functions in association with point index
	PntName
	pntNameForNdx
		( cam::PntNdx const & pntNdx
		, std::string const & ndxFmt = { "%05d" }
		);

	/*! \brief Load measurements data from ascii file (trifecta convention).
	 *
	 * Record format is: 
	 *   <pntID row col [srr, src, scr, scc]>
	 *
	 * Note: Covariance values are currently ignored
	 *
	 */
	MeaGroupOneAcq
	loadFromAsciiTrifecta
		( std::istream & istrm
		, std::set<PntName> * const ptNames = nullptr
		);

	//! Insert mea into table col(acqNdx), using map for pntNdx lookup
	bool
	insertIntoTable
		( cam::XRefSpots * const & ptSpotTab
		, MeaGroupOneAcq const & meaInfos
		, AcqNdx const & acqNdx
		, std::map<PntName, PntNdx> const & pntNameNdxMap
		);

	//! Save measurements to stream in ascii format (trifecta convention)
	bool
	saveToAsciiTrifecta
		( cam::XRefSpots const & spotTab
		, std::ostream & ostrm
		, AcqNdx const & acqNdx
		);

	//! Save measurements to output files in (trifecta convention)
	bool
	saveToAsciiTrifecta
		( cam::XRefSpots const & spotTab
		, std::vector<std::string> const & acqnames
		);

	//! Save ray data to ascii format suitable for gnuplot (with vectors)
	bool
	saveToAsciiGnuplot
		( XRefRays const & rayTab
		, XRefDists const & distTab
		, std::string const & fpath
		);

} // io

} // cam

// Inline definitions
// #include "libcam/io.inl"

#endif // cam_io_INCL_

