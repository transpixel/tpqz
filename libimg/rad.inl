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
\brief Inline definitions for img::rad
*/


#include "libimg/sample.h"
#include "libprob/histo.h"
#include "libprob/Remapper.h"

#include <algorithm>



namespace img
{
namespace rad
{
//======================================================================

template <typename PixType>
inline
dat::grid<PixType>
equalized
	( dat::grid<PixType> const & from
	, math::Partition const & part
	)
{
	dat::grid<PixType> into(from.hwSize());

	// use only active pixels for stats
	std::vector<PixType> const samps
		( img::sample::activeValuesFrom<PixType>
			(from.begin(), from.end())
		);

	// compute forward cumulative distribution
	std::vector<size_t> const hist(prob::histo::countsFromSamps(samps, part));
	prob::CdfForward const cdfFwd
		(prob::CdfForward::fromFreqs(hist.begin(), hist.end(), part.range()));

	// set inverse cumulative distribution
	prob::CdfInverse const cdfInv(prob::CdfInverse::uniform(part.range()));

	// remap values to uniform distribution
	prob::Remapper const rem(cdfFwd, cdfInv);
	std::transform(from.begin(), from.end(), into.begin(), rem);

	// return results
	return into;
}

template <typename PixType>
std::pair<prob::Histogram, prob::Histogram>
histogramAllActive
	( std::array<dat::grid<PixType>, 3u> const & bands
	, math::Partition const & intenPartition
	, std::vector<dat::Area<size_t> > const & sampAreas
	)
{
	std::pair<prob::Histogram, prob::Histogram> histPair;
	prob::Histogram & allHist = histPair.first;
	prob::Histogram & actHist = histPair.second;
	allHist = prob::Histogram(intenPartition);
	actHist = prob::Histogram(intenPartition);

	// accumulate all bands into histogram
	for (size_t chan(0u) ; chan < 3u ; ++chan)
	{
		// get samples for active area of this band
		std::vector<PixType> const allSamps
			(img::sample::fromImageAreas(bands[chan], sampAreas));
		// add to histogram
		allHist.addSamples(allSamps.begin(), allSamps.end());

		// select active pixels
		std::vector<PixType> const actSamps
			( img::sample::activeValuesFrom<PixType>
				(allSamps.begin(), allSamps.end())
			);
		// add to histogram
		actHist.addSamples(actSamps.begin(), actSamps.end());
	}
	return histPair;
}

//======================================================================
}
}

