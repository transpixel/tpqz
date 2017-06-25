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

#ifndef cam_xref_INCL_
#define cam_xref_INCL_

/*! \file
\brief Declarations for cam::xref
*/


#include "libcam/XRefRays.h"
#include "libcam/XRefDists.h"

#include <limits>


namespace cam
{

/*! \brief Functions which interact with XRef* classes.

\par Example
\dontinclude testcam/uxref.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace xref
{

	//! Points corresponding to most probability distances along primeRays
	std::vector<ga::Vector> // corresponds 1:1 with pntNdx rows in rayTab
	likelyPrimePoints
		( cam::XRefRays const & rayTab
		, std::vector<cam::AcqNdx> const & primeAcqNdxs
		, dat::Range<double> const & distRange
		, double const & rayAngleSigma
		);

	//! Like above, but uses combinatorial search algorithm
	std::vector<ga::Vector> // corresponds 1:1 with pntNdx rows in rayTab
	likelyComboPoints
		( cam::XRefRays const & rayTab
		, dat::Range<double> const & distRange
		, double const & rayAngleSigma
		);

	//! Distances along rays to closest
	XRefDists
	distanceTable
		( XRefRays const & rayTab
		, std::vector<ga::Vector> const & pntsForRows
			//!< corresponds 1:1 with pntNdx rows in rayTab
		, double const & rejMagMaxTol = std::numeric_limits<double>::max()
		);

	//! Average of points at dist along rays (only if dist&ray are both valid)
	std::vector<ga::Vector> // corresponds 1:1 with pntNdx rows in rayTab
	averagePoints
		( cam::XRefRays const & rayTab
		, XRefDists const & distTab
		, size_t const & minNearCount = 2u
			//!< Require at least this many valid rays per point
		);

} // xref

} // cam

// Inline definitions
// #include "libcam/xref.inl"

#endif // cam_xref_INCL_

