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
\brief Definitions for sig::match
*/


#include "libsig/match.h"

#include "libapp/Timer.h" // TODO ?
#include "libdat/iter.h"
#include "libdat/layout.h"
#include "libdat/RowCol.h"
#include "libimg/cfa.h"
#include "libimg/convert.h"
#include "libimg/geo.h"
#include "libsig/filter.h"
#include "libsys/job.h"

#include <algorithm>
#include <cassert>
#include <utility>

#include <fstream>
#include <string>


namespace
{
	//! Run matching filter and return corresponding spots
	std::vector<std::pair<dat::Spot, dat::Spot> >
	runSpotPairs
		( std::vector<sig::FilterContext> const & fconSamps
		, sig::match::SourceRefs const & sourceRefs
		)
	{
		std::vector<std::pair<dat::Spot, dat::Spot> > runPairs;

		// access source data
		dat::grid<float> const & fullGridA = sourceRefs.theFullGridA;
		dat::grid<float> const & fullGridB = sourceRefs.theFullGridB;
		std::array<float, 3u> const & rgbGains = sourceRefs.theRgbGains;

		size_t runSize{ fconSamps.size() };
		runPairs.reserve(runSize);

		for (sig::FilterContext const & fconSamp : fconSamps)
		{
			// define sample crop area
			dat::SubExtents const cropSampA(fconSamp.referenceCrop());
			dat::SubExtents const cropSampB(fconSamp.targetCrop());

			// if samples don't fit, then computations above are bad
			assert(cropSampA.fitsWithin(fullGridA.hwSize()));
			assert(cropSampB.fitsWithin(fullGridB.hwSize()));

			// crop sample patches and demultiplex into individual bands
			dat::grid<float> const workGridA
				(img::convert::grayGridFrom(fullGridA, cropSampA, rgbGains));
			dat::grid<float> const workGridB
				(img::convert::grayGridFrom(fullGridB, cropSampB, rgbGains));

			// find match points
			std::pair<dat::Spot, dat::Spot> const spotPairAB
				{ sig::match::spotPairAB(fconSamp, workGridA, workGridB) };

			runPairs.emplace_back(spotPairAB);
		}

		return runPairs;
	}

	//! Job functor for matching a collection of filter contexts
	class JobMatch : public sys::JobBase
	{

	public:

		using SpotPair = std::pair<dat::Spot, dat::Spot>;
		using JobResult = std::vector<SpotPair>;

	private:

		std::vector<sig::FilterContext> const theFCons;
		sig::match::SourceRefs const theSourceRefs;
		std::shared_ptr<std::vector<SpotPair> > const & thePtResults;

	public:

		//! Configure job to put results at ptSpotPairs
		JobMatch
			( std::vector<sig::FilterContext>::const_iterator const & fconBeg
			, std::vector<sig::FilterContext>::const_iterator const & fconEnd
			, sig::match::SourceRefs const sourceRefs
			, std::shared_ptr<std::vector<SpotPair> > const & ptResults
			)
			: JobBase{ "JobMatch" }
			, theFCons(fconBeg, fconEnd)
			, theSourceRefs(sourceRefs)
			, thePtResults(ptResults)
		{
			assert(thePtResults);
			assert(thePtResults->size() == theFCons.size());
		}

	protected:

		//! perform matching for all samples in configuration
		virtual
		void
		run
			() const
		{
			// perform matching
			std::vector<SpotPair> const results
				(runSpotPairs(theFCons, theSourceRefs));

			// copy results to consumer-provided space
			assert(thePtResults->size() == results.size());
			std::copy
				( results.begin(), results.end()
				, thePtResults->begin()
				);
		}
	};
}


namespace sig
{
namespace match
{

dat::SubExtents
overlapCrop
	( dat::Extents const & sizeA
	, dat::SpotX const & xBwA
	, dat::Extents const & sizeB
	)
{
	dat::SubExtents overCrop;
	if (sizeA.isValid() && sizeB.isValid() && xBwA.isValid())
	{
		// determine area - expressed in sizeB - where A and B full overlap
		overCrop = sig::FilterContext::commonCropIn1(sizeA, xBwA, sizeB);
	}
	assert(overCrop.isValid()); // if no overlap, something is very bad
	return overCrop;
}

dat::SubExtents
activeCrop
	( dat::SubExtents const overCrop
	, dat::Extents const & filterSize
	)
{
	dat::SubExtents workCrop;
	if (overCrop.isValid() && filterSize.isValid())
	{
		// Pad needed around center values
		double const halfHigh{ double(filterSize.high()) / 2. };
		double const halfWide{ double(filterSize.wide()) / 2. };

		// adjusted the start area to allow for filter size
		dat::RowCol const & overUL = overCrop.theUL;
		dat::RowCol const rcUL
			{{ static_cast<size_t>
				 (std::ceil(static_cast<double>(overUL[0]) + halfHigh))
			 , static_cast<size_t>
				 (std::ceil(static_cast<double>(overUL[0]) + halfWide))
			}};

		// adjust the active size to allow for filter size
		dat::Extents const useSize
			( overCrop.high() - filterSize[0] - 1u
			, overCrop.wide() - filterSize[1] - 1u
			);

		// return active area
		workCrop = dat::SubExtents(rcUL, useSize);
	}
	return workCrop;
}


dat::grid<double>
scoringGridFor
	( dat::grid<float> const & fullTgtGrid
	, dat::Extents const & moveSize
	, dat::grid<float> const & hunkRefGrid
	, size_t const & maxNullCount
	)
{
	dat::grid<double> scoreGrid(moveSize);
	assert(fullTgtGrid.isValid());
	assert(moveSize.isValid());
	assert(hunkRefGrid.isValid());

	// compute filter response of patch moving over look extents
	scoreGrid = sig::filter::ssdResponseGrid<double, float>
		(fullTgtGrid, moveSize, hunkRefGrid, maxNullCount);

	// not needed for peak detect - but useful for interpretation
	double const scale{ 1. / double(hunkRefGrid.size()) };
	std::transform
		( scoreGrid.begin(), scoreGrid.end()
		, scoreGrid.begin()
		, [&scale] (double const & pix) { return -scale * std::sqrt(pix); }
		);

	return scoreGrid;
}

std::vector<sig::FilterContext>
filterContextsFor
	( dat::Extents const & sampHW
	, sig::MoveConfig const & moveConfig
	, MatchConfig const & matchBwA
	)
{
	std::vector<sig::FilterContext> fconSamps;

	dat::SubExtents const workCropInA(matchBwA.workCropInReference());
	dat::Extents const & workSize = workCropInA.theSize;
	dat::SpotX const & xSrcBWrtSrcA = matchBwA.theTgtWrtRef;
	dat::SpotX const xSrcAWrtWork(matchBwA.xReferenceWrtWork());

	// determine a grid of locations at which to compute match
	std::vector<dat::RowCol> const rcInWorks
		(dat::layout::expanded::rowcols(workSize, sampHW));

	// establish all filter contexts
	fconSamps.reserve(rcInWorks.size());
	for (dat::RowCol const & rcInWork : rcInWorks)
	{
		// define sampling location in each source
		dat::Spot const spotInWork(dat::cast::Spot(rcInWork));
		dat::Spot const spotInA(xSrcAWrtWork(spotInWork));
		dat::Spot const nearInB(xSrcBWrtSrcA(spotInA));

		// quantized window center associated with sample spots
		dat::RowCol const midInA(dat::cast::RowCol(spotInA));
		dat::RowCol const midInB(dat::cast::RowCol(nearInB));

		dat::Extents const & hunkSize = moveConfig.theHunkSize;
		dat::Extents const & lookSize = moveConfig.theMoveSize;
		// compute filter context
		fconSamps.emplace_back
			( sig::FilterContext::fromCenters
				(midInA, midInB, hunkSize, lookSize)
			);
	}

	return fconSamps;
}

std::pair<dat::Spot, dat::Spot>
spotPairAB
	( sig::FilterContext const & fcon
	, dat::grid<float> const & hunkRefGrid
	, dat::grid<float> const & fullTgtGrid
	, sig::Peak * const & ptPeakInB
	, std::string const & saveName
	)
{
	std::pair<dat::Spot, dat::Spot> spotPair
		{ dat::nullValue<dat::Spot>(), dat::nullValue<dat::Spot>() };

	app::Timer timer;
	// create a metric response grid
	timer.start("match..scoringGridFor");
	dat::Extents const moveSize{ fcon.targetMoveSize() };

	size_t const maxNullCount{ 0u };
	dat::grid<double> const evalGridB
		(scoringGridFor(fullTgtGrid, moveSize, hunkRefGrid, maxNullCount));

	// find peak location
	timer.start("match..peakfromGrid");
	constexpr double topRadius{ 2. };
	sig::Peak const peakInB(sig::Peak::fromGrid(evalGridB, topRadius));

	constexpr double peakTol{ .00 }; // doesn't seem to matter much
	if (peakTol < peakInB.prominenceRank())
	{
		// refine peak location
		timer.start("match..peakfitSpot");
		dat::Spot const spotInLookB
			(sig::Peak::fitSpotFor(evalGridB, peakInB));
		timer.stop();

		// note hotspots w.r.t. full sample
		spotPair.first = fcon.hotSpotForReference();
		spotPair.second = fcon.hotSpotForTargetAt(spotInLookB);
		if (! saveName.empty())
		{
			std::ofstream ofs(saveName);
			saveScoreAsText(evalGridB, ofs, peakInB.theBestScore);

			io::out() << peakInB.infoString("peakInB") << '\n';
		//	io::out() << peakInB.infoStringDetail("peakInB") << '\n';
			io::out() << timer.infoString() << std::endl;
		}
	}

	if (ptPeakInB)
	{
		*ptPeakInB = peakInB;
	}

	return spotPair;
}

std::vector<std::pair<dat::Spot, dat::Spot> >
spotPairsFor
	( std::vector<sig::FilterContext> const & allFCons
	, SourceRefs const sourceRefs
	, size_t const & numJobs
	)
{
	std::vector<std::pair<dat::Spot, dat::Spot> > allPairs;
	allPairs.reserve(allFCons.size());

	// input (filter context) sample partition
	using ItFCon = std::vector<sig::FilterContext>::const_iterator;
	using ItPair = std::pair<ItFCon, ItFCon>;
	std::vector<ItPair> const fconGroups
		(dat::iter::groups(allFCons.begin(), allFCons.end(), numJobs));

	// space for match results
	using JobResult = JobMatch::JobResult;
	std::vector<std::shared_ptr<JobResult> > jobResults;
	jobResults.reserve(numJobs);

	// configure jobs
	std::vector<std::shared_ptr<sys::JobBase> > allJobs;
	for (ItPair const & fconGroup : fconGroups)
	{
		ItFCon const & fconBeg = fconGroup.first;
		ItFCon const & fconEnd = fconGroup.second;

		// allocate space for results - to match input size
		size_t const sampSize
			{ static_cast<size_t>(std::distance(fconBeg, fconEnd)) };
		jobResults.emplace_back(std::make_shared<JobResult>(sampSize));

		// setup match job
		std::shared_ptr<JobResult> & results = jobResults.back();
		allJobs.emplace_back
			( std::make_shared<JobMatch>
				(fconBeg, fconEnd, sourceRefs, results)
			);
	}
	// run all jobs
	sys::job::Factory factory(allJobs);
	factory.processAll();

	// gather results
	for (size_t nj{0u} ; nj < numJobs ; ++nj)
	{
		std::shared_ptr<JobResult> const & results = jobResults[nj];
		allPairs.insert(allPairs.end(), results->begin(), results->end());
	}

	return allPairs;
}

bool
saveScoreAsText
	( dat::grid<double> const & scoreGrid
	, std::ostream & ostrm
	, double const & bestScore
	)
{
	for (dat::ExtentsIterator iter(scoreGrid.hwSize()) ; iter ; ++iter)
	{
		dat::RowCol const & rowcol = *iter;
		double const & score = scoreGrid(rowcol);
		double const drop{ std::log(bestScore / score) };
		ostrm << dat::infoString(rowcol)
			<< " " << dat::infoString(score)
			<< " " << dat::infoString(drop)
			<< '\n';
	}
	ostrm << std::endl;
	return (! ostrm.bad());
}


}
}

