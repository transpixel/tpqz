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

#ifndef sig_match_INCL_
#define sig_match_INCL_

/*! \file
\brief Declarations for sig::match
*/


#include "libdat/Extents.h"
#include "libdat/grid.h"
#include "libdat/Spot.h"
#include "libdat/SpotX.h"
#include "libdat/SubExtents.h"
#include "libsig/FilterContext.h"
#include "libsig/MatchConfig.h"
#include "libsig/Peak.h"

#include <utility>
#include <vector>

#include <string>


namespace sig
{

/*! \brief Functions associated with raster grid matching

\par Example
\dontinclude testsig/umatch.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace match
{
	//! References to source data used in matching
	struct SourceRefs
	{
		dat::grid<float> const & theFullGridA;
		dat::grid<float> const & theFullGridB;
		std::array<float, 3u> const & theRgbGains;
	};

	//! Central crop area
	dat::SubExtents
	overlapCrop
		( dat::Extents const & sizeA
		, dat::SpotX const & xBwA
		, dat::Extents const & sizeB
		);

	//! Central crop area
	dat::SubExtents
	activeCrop
		( dat::SubExtents const overCrop
		, dat::Extents const & filterSize
		);

	//! Grid of score defined over scoreSize
	dat::grid<double>
	scoringGridFor
		( dat::grid<float> const & fullTgtGrid
		, dat::Extents const & moveSize
		, dat::grid<float> const & hunkRefGrid
		, size_t const & maxNullCount = 0u
		);

	//! Establish filter contexts for grid locations
	std::vector<sig::FilterContext>
	filterContextsFor
		( dat::Extents const & sampHW //!< Sampling size in high/wide
		, sig::MoveConfig const & moveConfig
		, MatchConfig const & matchBwA
		);

	//! Corresponding spots in A and B
	std::pair<dat::Spot, dat::Spot>
	spotPairAB
		( sig::FilterContext const & fcon
			//!< Specification of crop geometry for both A & B
		, dat::grid<float> const & hunkRefGrid
			//!< Full-size signal source grid from which to crop ref signal
		, dat::grid<float> const & fullTgtGrid
			//!< Full-size signal source grid from which to crop target signal
		, sig::Peak * const & ptPeakInB = {}
			//!< If !null, copy found peak contents to here
		, std::string const & saveName = {}
			//!< If !empty, write scoring text info to this filepath
		);

	//! Run matching filter and return corresponding spots
	std::vector<std::pair<dat::Spot, dat::Spot> >
	spotPairsFor
		( std::vector<sig::FilterContext> const & allFCons
		, SourceRefs const sourceRefs
		, size_t const & numJobs
		);

	//! Save contents to text stream (true if success)
	bool
	saveScoreAsText
		( dat::grid<double> const & scoreGrid
		, std::ostream & ostrm
		, double const & bestScore
		);
}

}

// Inline definitions
// #include "libsig/match.inl"

#endif // sig_match_INCL_

