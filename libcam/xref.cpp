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
\brief Definitions for cam::xref
*/


#include "libcam/xref.h"

#include "libga/ga.h"
#include "libgeo/ProbRay.h"
#include "libgeo/RayConvergence.h"
#include "libgeo/Ray.h"
#include "libgeo/stats.h"

#include <cassert>
#include <vector>

#include <fstream>
#include "libio/sprintf.h"


// set only one
#define UseRayRay // use ray-ray ProbRay intersection
//#define UseRayCone // use ray-cone ProbRay intersection


namespace cam
{
namespace xref
{

namespace
{
	std::vector<geo::Ray>
	rayCollection
		( cam::XRefRays const & rayTab
		, cam::PntNdx const & pntNdx
		, cam::AcqNdx const & primeAcqNdx
		)
	{
		std::vector<geo::Ray> rays;

		// allocate space
		cam::AcqNdx const numAcqs{ rayTab.acqCapacity() };
		rays.reserve(numAcqs);

		// put primary ray first in the collection
		geo::Ray const & primeRay = rayTab(pntNdx, primeAcqNdx);
		if (primeRay.isValid()) // otherwise, not much sense in doing work
		{
			rays.emplace_back(primeRay);
		}

		// put other (not-primary) rays into collection
		for (cam::AcqNdx acqNdx{0u} ; acqNdx < numAcqs ; ++acqNdx)
		{
			if (primeAcqNdx != acqNdx)
			{
				geo::Ray const & otherRay = rayTab(pntNdx, acqNdx);
				if (otherRay.isValid())
				{
					rays.emplace_back(otherRay);
				}
			}
		}

		return rays;
	}
}

std::vector<ga::Vector>
likelyPrimePoints
	( cam::XRefRays const & rayTab
	, std::vector<cam::AcqNdx> const & primeAcqNdxs
	, dat::Range<double> const & distRange
	, double const & rayAngleSigma
	)
{
	std::vector<ga::Vector> pnts;

	// interpolation produces a small fraction of this
	constexpr double numSampsPerMeter{ 100. }; // e.g. cm level sampling
	double const numMeters{ distRange.magnitude() };
	size_t const numSamps{ size_t(numSampsPerMeter * numMeters + 1.) };

	// allocate space / init to null
	size_t const numPnts{ rayTab.pntCapacity() };
	pnts.resize(numPnts, ga::Vector{});

	assert(numPnts == primeAcqNdxs.size());

	// evaluate each fuzzy point in turn
	for (cam::PntNdx pntNdx{0u} ; pntNdx < numPnts ; ++pntNdx)
	{
		// select primary ray of interest
		size_t const primeAcqNdx{ primeAcqNdxs[pntNdx] };

		// get rays for this point
		std::vector<geo::Ray> const rays
			{ rayCollection(rayTab, pntNdx, primeAcqNdx) };

		// estimate (relative) probability density along current ray
		math::Partition const distPart(distRange, numSamps);
#		if defined(UseRayRay)
		geo::ProbRay const probRay
			{ geo::ProbRay::from
				( rays.begin(), rays.end()
				, rays.begin() // first is prime
				, distPart
				, rayAngleSigma
				)
			};
#		endif
#		if defined(UseRayCone)
		static ga::Vector const coneAxis{ ga::e1 };
		geo::ProbRay const probRay
			{ geo::ProbRay::fromCones
				( rays.begin(), rays.end()
				, rays.begin() // first is prime
				, distPart
				, coneAxis
				, rayAngleSigma
				)
			};
#		endif

		// development info
		constexpr bool saveProbs{ true };
		if (saveProbs)
		{
			std::string const pntstr{ io::sprintf("%03d", pntNdx) };
			std::string const fname{ "demoR_probPnt" + pntstr + ".dat" };
			std::ofstream ofs(fname);
			probRay.saveDistProbs(ofs);
		}

		// extract most likely location along the primary ray
		ga::Vector const modePnt{ probRay.likelyPoint() };
		if (modePnt.isValid())
		{
			pnts[pntNdx] = modePnt;
		}

	}
	return pnts;
}

std::vector<ga::Vector>
likelyComboPoints
	( cam::XRefRays const & rayTab
	, dat::Range<double> const & distRange
	, double const & rayAngleSigma
	)
{
	std::vector<ga::Vector> pnts;

	// interpolation produces a small fraction of this
	constexpr double numSampsPerMeter{ 100. }; // e.g. cm level sampling
	double const numMeters{ distRange.magnitude() };
	size_t const numSamps{ size_t(numSampsPerMeter * numMeters + 1.) };

	// allocate space / init to null
	size_t const numPnts{ rayTab.pntCapacity() };
	pnts.resize(numPnts, ga::Vector{});

#if defined(UseRayRay)
	io::out() << "@@@@@ - using RayRay intersections" << std::endl;
#endif
#if defined(UseRayCone)
	io::out() << "@@@@@ - using RayCone intersections" << std::endl;
#endif

	// evaluate each fuzzy point in turn
	for (cam::PntNdx pntNdx{0u} ; pntNdx < numPnts ; ++pntNdx)
	{
		// get all valid rays  this point
		std::vector<geo::Ray> const rays{ rayTab.validItemsForPnt(pntNdx) };

		// Consider combinatorial intersection candidates
		double probBest{ -1. };
		ga::Vector modePnt;
		for (std::vector<geo::Ray>::const_iterator
			itCurrRay{rays.begin()} ; rays.end() != itCurrRay ; ++itCurrRay)
		{
			// estimate (relative) probability density along current ray
			math::Partition const distPart(distRange, numSamps);
#			if defined(UseRayRay)
			geo::ProbRay const probRay
				{ geo::ProbRay::from
					( rays.begin(), rays.end()
					, itCurrRay
					, distPart
					, rayAngleSigma
					)
				};
#			endif
#			if defined(UseRayCone)
			static ga::Vector const coneAxis{ ga::e1 };
			geo::ProbRay const probRay
				{ geo::ProbRay::fromCones
					( rays.begin(), rays.end()
					, itCurrRay
					, distPart
					, coneAxis
					, rayAngleSigma
					)
				};
#			endif
			if (probRay.isValid())
			{
				std::pair<double, double> const dpPair
					{ probRay.likelyDistProb() };
				double const & dist = dpPair.first;
				double const & prob = dpPair.second;
				if (probBest < prob)
				{
					probBest = prob;
					modePnt = itCurrRay->pointAt(dist);
				}
			}
		}

		if (modePnt.isValid())
		{
			pnts[pntNdx] = modePnt;
		}

	}
	return pnts;
}


XRefDists
distanceTable
	( XRefRays const & rayTab
	, std::vector<ga::Vector> const & pntsForRows
	, double const & rejMagMaxTol
	)
{
	size_t const numPnts{ rayTab.pntCapacity() };
	size_t const numAcqs{ rayTab.acqCapacity() };
	cam::XRefDists distTab(numPnts, numAcqs);

	// ensure consistency of input data
	assert(numPnts == pntsForRows.size());
	assert(dat::isValid(rejMagMaxTol));

	// compute distance along each ray
	for (XRefRays::const_iterator iter{rayTab.begin()} ; iter ; ++iter)
	{
		// access corresponding input data items
		cam::XRefRays::PAItem const paRay{ iter.paItem() };
		cam::PntNdx const & pntNdx = paRay.thePntNdx;
		cam::AcqNdx const & acqNdx = paRay.theAcqNdx;
		geo::Ray const & ray = paRay.theItem;
		ga::Vector const & pnt = pntsForRows[pntNdx];

		// if point is close enough to ray, ...
		ga::Vector const rej{ ray.rejectionTo(pnt) };
		double const rejMag{ ga::magnitude(rej) };
		if (rejMag < rejMagMaxTol)
		{
			// ... then update distance along ray
			double const dist{ ray.distanceAlong(pnt) };
			distTab(pntNdx, acqNdx) = dist;
		}
	}

	return distTab;
}

std::vector<ga::Vector>
averagePoints
	( cam::XRefRays const & rayTab
	, XRefDists const & distTab
	, size_t const & minNearCount
	)
{
	std::vector<ga::Vector> avePnts;

	// ensure consistent input data
	size_t const rayPntCap{ rayTab.pntCapacity() };
	size_t const rayAcqCap{ rayTab.acqCapacity() };
	size_t const distPntCap{ distTab.pntCapacity() };
	size_t const distAcqCap{ distTab.acqCapacity() };
	assert(rayPntCap == distPntCap);
	assert(rayAcqCap == distAcqCap);

	// allocate space
	size_t const & numPnts = distPntCap;
	size_t const & numAcqs = distAcqCap;
	avePnts.resize(numPnts, ga::Vector{}); // set default to null
	std::vector<ga::Vector> eachPnts(numPnts);

	for (cam::PntNdx pntNdx{0u} ; pntNdx < numPnts ; ++pntNdx)
	{
		eachPnts.clear();
		for (cam::AcqNdx acqNdx{0u} ; acqNdx < numAcqs ; ++acqNdx)
		{
			double const & dist = distTab(pntNdx, acqNdx);
			if (dat::isValid(dist))
			{
				geo::Ray const & ray = rayTab(pntNdx, acqNdx);
				if (dat::isValid(ray))
				{
					ga::Vector const eachPnt{ ray.pointAt(dist) };
					eachPnts.emplace_back(eachPnt);
				}
			}
		}

		// check if enough candidates to be "significant"
		if (minNearCount <= eachPnts.size())
		{
			ga::Vector const avePnt
				{ geo::stats::centroidOf(eachPnts.begin(), eachPnts.end()) };
			avePnts[pntNdx] = avePnt;
		}
	}

	return avePnts;
}

} // xref
} // cam

