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
\brief Definitions for recon::RayConvergence
*/


#include "librecon/RayConvergence.h"

#include "libgeo/intersect.h"
#include "libgeo/stats.h"
#include "libmath/math.h"
#include "libprob/Coordinates.h"

#include <cmath>
#include <iterator>
#include <sstream>


namespace
{
	//! Robust intersection point for rays
	template <typename FwdIter>
	ga::Vector
	robustIntersectionFor
		( FwdIter const & rayBeg
		, FwdIter const & rayEnd
		, double const & maxCosMag
		, std::vector<double> * const & ptGapMags
		)
	{
		ga::Vector pnt{};

		// get valid rays
		size_t const numRays{ size_t(std::distance(rayBeg, rayEnd)) };
		if (1u < numRays)
		{
			if (ptGapMags)
			{
				// max possible number of gaps (if all solutions are not null)
				size_t const numGaps{ (numRays * (numRays - 1u)) / 2u };
				ptGapMags->reserve(numGaps);
			}

			// brute force combinatorial intersection
			size_t const numCombo{ (math::sq(numRays) - numRays)/2u };
			prob::Coordinates coords(2u*numCombo);
			for (FwdIter it1{rayBeg} ; rayEnd != it1 ; ++it1)
			{
				geo::Ray const & ray1 = *it1;
				ga::Vector const & dir1 = ray1.theDir;
				for (FwdIter it2{it1 + 1u} ; rayEnd != it2 ; ++it2)
				{
					geo::Ray const & ray2 = *it2;
					ga::Vector const & dir2 = ray2.theDir;
					// check if rays are sufficiently convergent
					double const gotCosMag
						{ std::abs(ga::dot(dir1, dir2).theValue) };
					if (gotCosMag < maxCosMag)
					{
						// compute intersection point pair
						std::pair<ga::Vector, ga::Vector> const pntPair
							(geo::intersect::pointsFor(ray1, ray2));

						// add both points
						coords.addPoint(pntPair.first);
						coords.addPoint(pntPair.second);

						// if requested, record gap magnitude
						if (ptGapMags)
						{
							ptGapMags->emplace_back
								(ga::magnitude(pntPair.second - pntPair.first));
						}
					}
				}
			}
			// use independent coordinate median as estimated robust location
			pnt = coords.pointAtMedians();
		}

		return pnt;
	}

	//! Distance along ray nearest qualLoc - if rejection less than tol
	double
	qualifiedRange
		( geo::Ray const & ray
		, ga::Vector const & qualPnt
		, double const & maxRejMag
		)
	{
		double dist{ dat::nullValue<double>() };
		if (ray.isValid() && qualPnt.isValid())
		{
			// compute vector rejection from ray to qualPnt
			ga::Vector const rejVec{ ray.rejectionTo(qualPnt) };
			assert(rejVec.isValid()); // should always be a solution
			// qualify result by distance between ray and point
			double const rejMag{ ga::magnitude(rejVec) };
			if (rejMag < maxRejMag)
			{
				dist = ray.distanceAlong(qualPnt);
			}
		}
		return dist;
	}

	//! Robust intersection point for rays
	template <typename FwdIter>
	std::vector<ga::Vector>
	nearbyPoints
		( FwdIter const & rayBeg
		, FwdIter const & rayEnd
		, ga::Vector const & evalPoint
		, double const & maxRej
		)
	{
		std::vector<ga::Vector> pnts;
		for (FwdIter iter{rayBeg} ; rayEnd != iter ; ++iter)
		{
			geo::Ray const & ray = *iter;
			double const dist{ qualifiedRange(ray, evalPoint, maxRej) };
			if (dat::isValid(dist))
			{
				pnts.emplace_back(ray.pointAt(dist));
			}
		}
		return pnts;
	}
}


namespace recon
{

// explicit
RayConvergence :: RayConvergence
	( std::vector<geo::Ray> const & rays
	)
	: theRays{ rays }
{
}

bool
RayConvergence :: isValid
	() const
{
	return (1u < theRays.size()); // need at least two for intersection
}

ga::Vector
RayConvergence :: robustPoint
	( double const minAngle
	, std::vector<double> * const & ptGapMags
	) const
{
	ga::Vector pnt;
	if (isValid())
	{
		double const maxCos(std::cos(minAngle));
		pnt = robustIntersectionFor
			(theRays.begin(), theRays.end(), maxCos, ptGapMags);
	}
	return pnt;
}

ga::Vector
RayConvergence :: meanNearTo
	( ga::Vector const & evalPoint
	, double const & maxRejTol
	) const
{
	ga::Vector pnt;
	if (isValid())
	{
		// if not provided, estimate fuzzy point robustly
		ga::Vector qualPoint{ evalPoint };
		if (! qualPoint.isValid())
		{
			qualPoint = robustPoint();
		}

		// compute points on rays proximal to fuzzy robust value
		std::vector<ga::Vector> const nearPoints
			{ nearbyPoints
				(theRays.begin(), theRays.end(), qualPoint, maxRejTol)
			};

		// return centroid of proximal points
		pnt = geo::stats::centroidOf(nearPoints.begin(), nearPoints.end());
	}
	return pnt;
}

std::string
RayConvergence :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	if (isValid())
	{
		oss << "numRays: " << dat::infoString(theRays.size());
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}


} // recon

