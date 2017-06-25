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

#ifndef sig_filter_INCL_
#define sig_filter_INCL_

/*! \file
\brief Declarations for sig::filter
*/


#include "libdat/grid.h"
#include "libdat/RowCol.h"
#include "libdat/SubExtents.h"
#include "libmath/math.h"

#include <functional>


namespace sig
{

/*! \brief Functions related to digital filtering operations.

\par Example
\dontinclude testsig/ufilter.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace filter
{
	//
	// Moving filter relationships
	//

	//
	// NOTE: Relationships:
	//
	//  H: (hunk size) aka window, patch, template
	//  M: (move size)
	//  F: (full size)
	//
	//  H = F - M + 1
	//  M = F - H + 1
	//  F = M + H - 1
	//

	//! Full size required for hunk (aka window) size to change by moveSize
	size_t
	hunkSizeFor
		( size_t const & moveSize
		, size_t const & fullSize
		);

	//! Extents of motion for hunk within fullsize area
	size_t
	moveSizeFor
		( size_t const & fullSize
		, size_t const & hunkSize
		);

	//! Extents of hunk that allow complete movement within full size
	size_t
	fullSizeFor
		( size_t const & hunkSize
		, size_t const & moveSize
		);

	//! Full size extents using hunkSizeFor on each dimension.
	dat::Extents
	hunkExtentsFor
		( dat::Extents const & moveExtents
		, dat::Extents const & fullExtents
		);

	//! Move extents using moveSizeFor on each dimension.
	dat::Extents
	moveExtentsFor
		( dat::Extents const & fullExtents
		, dat::Extents const & hunkExtents
		);

	//! Full extents using fullSizeFor on each dimension.
	dat::Extents
	fullExtentsFor
		( dat::Extents const & hunkExtents
		, dat::Extents const & moveExtents
		);

	//
	// General functions
	//

	//! Conventional Squared-Difference metric
	template <typename OutType, typename InType>
	struct SqDiff
	{
		//! Squared difference between values after cast to OutType
		inline
		OutType
		operator()
			( InType const & valA
			, InType const & valB
			) const
		{
			return math::sq(OutType(valA) - OutType(valB));
		}
	};

	//! Simple container for patch data statistics
	template <typename Type>
	struct HunkStats
	{
		Type theSumFull{ dat::nullValue<Type>() };
		Type theSumHunk{ dat::nullValue<Type>() };
		size_t theCount{ dat::nullValue<size_t>() };
	};

	//! Result of generic "product and sum" operation between source and patch
	template 
		<typename OutType, typename InType, typename PixFunc, typename SumFunc>
	inline
	OutType
	hunkResponse
		( dat::grid<InType> const & sourceGrid
		, dat::RowCol const & sourceUL
		, dat::grid<InType> const & patchGrid
		, PixFunc const & pairMetricFunc
		, SumFunc const & patchSumFunc = std::plus<OutType>()
		, HunkStats<OutType> * const & ptStats = nullptr
		);

	//! Result after applying hunkResponse() to every offset within moveSize
	template 
		<typename OutType, typename InType, typename PixFunc, typename SumFunc>
	inline
	dat::grid<OutType>
	moveResponseGrid
		( dat::grid<InType> const & sourceGrid
		, dat::Extents const & moveSize
		, dat::grid<InType> const & patchGrid
		, PixFunc const & pairMetricFunc
		, SumFunc const & patchSumFunc = std::plus<OutType>()
		);

	//! Unbiased SSD response. NOTE: OutType expected to be floating point
	template <typename OutType, typename InType>
	inline
	dat::grid<OutType>
	ssdResponseGrid
		( dat::grid<InType> const & sourceGrid
		, dat::Extents const & moveSize
		, dat::grid<InType> const & patchGrid
		, size_t const & maxNullCount = 0u
		);

}

}

// Inline definitions
#include "libsig/filter.inl"

#endif // sig_filter_INCL_

