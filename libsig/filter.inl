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
\brief Inline definitions for sig::filter
*/


#include "libdat/ExtentsIterator.h"

#include "libio/stream.h"
#include "libdat/info.h"


namespace sig
{
namespace filter
{

template <typename OutType, typename InType, typename PixFunc, typename SumFunc>
inline
OutType
hunkResponse
	( dat::grid<InType> const & fullGrid
	, dat::RowCol const & ulCropWrtFull
	, dat::grid<InType> const & hunkGrid
	, PixFunc const & pairMetricFunc
	, SumFunc const & hunkSumFunc
	, HunkStats<OutType> * const & ptStats
	)
{
	OutType response{ dat::nullValue<OutType>() };

#	if ! defined(NDEBUG)
		dat::Extents const fullGridSize(fullGrid.hwSize());
		dat::Extents const hunkGridSize(hunkGrid.hwSize());
		dat::SubExtents const cropInFull(ulCropWrtFull, hunkGridSize);
		if (! cropInFull.fitsWithin(fullGridSize))
		{
			dat::Extents const moveGridSize
				(moveExtentsFor(fullGridSize, hunkGridSize));
			io::out()
				<< dat::infoString(fullGridSize, "fullGridSize") << '\n'
				<< dat::infoString(hunkGridSize, "hunkGridSize") << '\n'
				<< dat::infoString(moveGridSize, "moveGridSize") << '\n'
				<< dat::infoString(cropInFull, "cropInFull") << '\n'
				;
			assert(cropInFull.fitsWithin(fullGridSize));
		}
#	endif

	OutType sumMet{ 0 };
	OutType sumFull{ 0 };
	OutType sumHunk{ 0 };
	size_t numValid{ 0u };
	for (dat::ExtentsIterator iter(hunkGrid.hwSize()) ; iter ; ++iter)
	{
		// determine input data locations
		using dat::operator+;
		dat::RowCol const & hunkRC = *iter;
		dat::RowCol const fullRC(hunkRC + ulCropWrtFull);

		// evaluate metric for individual pixel
		InType const & fullValue = fullGrid(fullRC);
		InType const & hunkValue = hunkGrid(hunkRC);

		if (dat::isValid(fullValue) && dat::isValid(hunkValue))
		{
			OutType const pixMet(pairMetricFunc(fullValue, hunkValue));

			// accumulate individual metrics into hunk
			if (dat::isValid(pixMet))
			{
				sumMet = hunkSumFunc(pixMet, sumMet);

				sumFull = sumFull + fullValue;
				sumHunk = sumHunk + hunkValue;
				++numValid;
			}
		}
	}

	if (0u < numValid)
	{
		response = sumMet;
	}

	// return ancillary data if requested
	if (ptStats)
	{
		ptStats->theSumFull = sumFull;
		ptStats->theSumHunk = sumHunk;
		ptStats->theCount = numValid;
	}

	return response;
}

template <typename OutType, typename InType, typename PixFunc, typename SumFunc>
inline
dat::grid<OutType>
moveResponseGrid
	( dat::grid<InType> const & fullGrid
	, dat::Extents const & moveSize
	, dat::grid<InType> const & hunkGrid
	, PixFunc const & pairMetricFunc
	, SumFunc const & hunkSumFunc
	)
{
	dat::grid<OutType> response;
	if (fullGrid.isValid() && moveSize.isValid() && hunkGrid.isValid())
	{
		// check if response values can be computed for the entire move area
		dat::Extents const fullSize(fullGrid.hwSize());
		dat::Extents const hunkSize(hunkGrid.hwSize());
		dat::SubExtents const cropInFull({{ 0u, 0u }}, hunkSize);

		if (cropInFull.fitsWithin(fullSize))
		{
			// allocate space for output
			response = dat::grid<OutType>(moveSize);

			// evaluate metric function at each move position
			typename dat::grid<OutType>::iterator itOut(response.begin());
			for (dat::ExtentsIterator iter(moveSize) ; iter ; ++iter)
			{
				dat::RowCol const & moveRC = *iter;

				// evaluate hunk response at this move position
				*itOut++ = hunkResponse<OutType, InType, PixFunc, SumFunc>
					( fullGrid
					, moveRC
					, hunkGrid
					, pairMetricFunc
					, hunkSumFunc
					);
			}
		}
	}
	return response;
}

template <typename OutType, typename InType>
inline
dat::grid<OutType>
ssdResponseGrid
	( dat::grid<InType> const & fullGrid
	, dat::Extents const & moveSize
	, dat::grid<InType> const & hunkGrid
	, size_t const & maxNullCount
	)
{
	dat::grid<OutType> response;
	if (fullGrid.isValid() && moveSize.isValid() && hunkGrid.isValid())
	{
		// allocate space for output
		response = dat::grid<OutType>(moveSize);

		assert(maxNullCount <= hunkGrid.size());
		size_t const reqMinValid{ hunkGrid.size() - maxNullCount };
		size_t const tolMinValid{ std::max(size_t(1u), reqMinValid) };

		// evaluate metric function at each move position
		typename dat::grid<OutType>::iterator itOut(response.begin());
		for (dat::ExtentsIterator iter(moveSize) ; iter ; ++iter)
		{
			dat::RowCol const & moveRC = *iter;

			// evaluate hunk response at this move position
			HunkStats<OutType> hunkStats{};
			OutType const ssdObserved
				( hunkResponse
					( fullGrid
					, moveRC
					, hunkGrid
					, SqDiff<OutType, InType>()
					, std::plus<OutType>()
					, &hunkStats
					)
				);

			// remove overall relative magnitude offset bias
			OutType ssdUnbiased{ dat::nullValue<OutType>() };
			if (dat::isValid(ssdObserved))
			{
				OutType const & sumFull = hunkStats.theSumFull;
				OutType const & sumHunk = hunkStats.theSumHunk;
				size_t const & numValid = hunkStats.theCount;
				if (tolMinValid <= numValid)
				{
				//	assert(0u < numValid); // otherwise ssd should be null
					// normalize valid responses
					double const sdSq{ math::sq(sumFull - sumHunk) };
					ssdUnbiased = ssdObserved - sdSq/double(numValid);
				}
			}
			*itOut++ = ssdUnbiased;
		}
	}
	return response;
}


}
}

