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
\brief Definitions for geo::intersect
*/


#include "libgeo/intersect.h"

#include "libga/constructs.h"


namespace geo
{
namespace intersect
{
//======================================================================

std::pair<double, double>
rangesFor
	( Ray const & ray1
	, Ray const & ray2
	)
{
	std::pair<double, double> ranges
		{ dat::nullValue<double>(), dat::nullValue<double>() };

	// shorthand notation
	ga::Vector const & pnt1 = ray1.theStart;
	ga::Vector const & pnt2 = ray2.theStart;
	ga::Vector const & dir1 = ray1.theDir;
	ga::Vector const & dir2 = ray2.theDir;

	ga::Vector const delta(pnt2 - pnt1);
	double const dotD1{ ga::dot(delta, dir1).theValue };
	double const dotD2{ ga::dot(delta, dir2).theValue };
	double const dot12{ ga::dot(dir1, dir2).theValue };

	double const det{ 1. - math::sq(dot12) };
	constexpr double tol{ std::numeric_limits<double>::epsilon() };
	if (! dat::nearlyEquals(det, tol))
	{
		// compute space intersection for infinite lines
		double const bigMu1{ (      dotD1 - dot12*dotD2) };
		double const bigMu2{ (dot12*dotD1 -       dotD2) };

		double const mu1(bigMu1 / det);
		double const mu2(bigMu2 / det);
		ranges = { mu1, mu2 };
	}

	return ranges;
}

std::pair<ga::Vector, ga::Vector>
pointsFor
	( Ray const & ray1
	, Ray const & ray2
	)
{
	std::pair<ga::Vector, ga::Vector> pnts;

	// get ranges to closest approach points for infinite lines
	std::pair<double, double> const ranges{ rangesFor(ray1, ray2) };
	if (dat::isValid(ranges))
	{
		// shorthand notation
		double const & mu1 = ranges.first;
		double const & mu2 = ranges.second;
		ga::Vector const & pnt1 = ray1.theStart;
		ga::Vector const & pnt2 = ray2.theStart;
		ga::Vector const & dir1 = ray1.theDir;
		ga::Vector const & dir2 = ray2.theDir;

		// check if intersection point is "in front of" each start point
		bool const inFront1{ (0. <= mu1) };
		bool const inFront2{ (0. <= mu2) };
		if (inFront1 && inFront2)
		{
			pnts = { {pnt1 + mu1*dir1}, {pnt2 + mu2*dir2} };

			/*
			// check solution conditions
			constexpr bool checkSoln{ true };
			if (checkSoln)
			{
				ga::Vector const rVec{ pnts.second - pnts.first };
				double const dotR1{ ga::dot(rVec, dir1).theValue };
				double const dotR2{ ga::dot(rVec, dir2).theValue };
				assert(dat::nearlyEquals(dotR1, 0., math::eps));
				assert(dat::nearlyEquals(dotR2, 0., math::eps));
			}
			*/
		}
	}

	return pnts;
}

double
gapBetween
	( Ray const & ray1
	, Ray const & ray2
	, std::pair<double, double> const & ranges12
	)
{
	double gap{ dat::nullValue<double>() };
	double const & dist1 = ranges12.first;
	double const & dist2 = ranges12.second;
	ga::Vector const pnt1{ ray1.theStart + dist1*ray1.theDir };
	ga::Vector const pnt2{ ray2.theStart + dist2*ray2.theDir };
	gap = ga::magnitude(pnt2 - pnt1);
	return gap;
}

double
rangeOnSegment
	( Ray const & ray
	, LineSeg const & seg
	, double const & tol
	)
{
	double dist{ dat::nullValue<double>() };
	// compute distances to closest approach points
	std::pair<double, double> const distSegRay{ rangesFor(seg.theRay, ray) };
	double const & distSeg = distSegRay.first;
	if (dat::isValid(distSeg) && seg.contains(distSeg, tol))
	{
		dist = distSeg;
	}
	return dist;
}

ga::Vector
pointOnSegment
	( Ray const & ray
	, LineSeg const & seg
	, double const & tol
	)
{
	ga::Vector pnt;
	double const distSeg{ rangeOnSegment(ray, seg, tol) };
	if (dat::isValid(distSeg))
	{
		pnt = seg.start() + distSeg*seg.direction();
	}
	return pnt;
}

double
gapBetween
	( Ray const & ray
	, LineSeg const & seg
	, double const & tol
	)
{
	double dist{ dat::nullValue<double>() };

	// compute distances to closest approach points
	std::pair<double, double> const distPair{ rangesFor(seg.theRay, ray) };
	if (dat::isValid(distPair))
	{
		double const & distOnRay = distPair.second;
		bool const isForward(0. <= distOnRay);
		if (isForward)
		{
			// check if intersection point is within finite span of segment
			double const & distSeg = distPair.first;
			if (seg.contains(distSeg, tol))
			{
				dist = gapBetween(seg.theRay, ray, distPair);
			}
		}
	}

	return dist;
}

ga::Vector
point
	( Ray const & ray
	, Plane const & plane
	)
{
	ga::Vector result{};

	ga::Vector const & rdir = ray.theDir;
	ga::Vector const & rsta = ray.theStart;

	// remove the if-condition to allow intersection from behind plane
	bool const rstaIsInFront{ (0. <= plane.offsetTo(rsta)) };
	if (rstaIsInFront) // ray start in front of plane
	{
		// compute cosine of angle between ray and plane
		ga::Vector const nhat(plane.unitNormal());
		double const dnDot{ ga::dot(rdir, nhat).theValue };

		// check for valid(finite) intersection point
		if (std::numeric_limits<double>::epsilon() < std::abs(dnDot))
		{
			double const dnInv{ 1. / dnDot };
			ga::Vector const rVec{ rsta - plane.origin() };
			double const rnDot{ ga::dot(rVec, nhat).theValue };
			double const lambda{ -rnDot * dnInv  };
			if (0. < lambda) // intersection is forward along ray
			{
				// compute point location along ray
				result = ray.pointAt(lambda);
			}
		}
	}

	return result;
}

namespace
{
	//! Pair with elements in op<() order recognizing validity
	std::pair<double, double>
	orderedPairFor
		( double const & valueA
		, double const & valueB
		)
	{
		static double const null{ dat::nullValue<double>() };
		std::pair<double, double> outPair{ null, null };
		bool const okayA{ dat::isValid(valueA) };
		bool const okayB{ dat::isValid(valueB) };
		if ((  okayA) && (  okayB))
		{
			if (valueA < valueB)
			{
				outPair = { valueA, valueB };
			}
			else
			{
				outPair = { valueB, valueA };
			}
		}
		else
		if ((  okayA) && (! okayB))
		{
			outPair = { valueA, null };
		}
		else
		if ((! okayA) && (  okayB))
		{
			outPair = { valueB, null };
		}
		return outPair;
	}

	//! for dev/testing...
	struct Soln
	{
		double const theNu;
		double const theRho;
		ga::Vector const theRhoB;
		double const theMu;

		explicit
		Soln
			( double const & nu
			, double const & rho
			, double const & adDot
			, double const & arDot
			, ga::Vector const & uVec
			, ga::Vector const & vVec
			)
			: theNu{ nu }
			, theRho{ rho }
			, theRhoB{ (theNu*theRho)*uVec + vVec}
			, theMu{ adDot*(theNu*theRho) + arDot }
		{ }

		double
		lambda
			() const
		{
			return (theNu * theRho);
		}

		std::string
		infoString
			( std::string const & title = {}
			) const
		{
			std::ostringstream oss;
			if (! title.empty())
			{
				oss << title << std::endl;
			}
			ga::Vector const bDir{ (1./theRho) * theRhoB };
			oss
				<< dat::infoString(theNu, "theNu") << '\n'
				<< dat::infoString(theRho, "theRho") << '\n'
				<< dat::infoString(lambda(), "lambda()") << '\n'
				<< dat::infoString(theRhoB, "theRhoB") << '\n'
				<< dat::infoString(bDir, "bDir") << '\n'
				<< dat::infoString(theMu, "theMu") << '\n'
				;
			return oss.str();
		}
	};

}

std::pair<ga::Vector, ga::Vector>
pointPair
	( Ray const & ray
	, Tube const & tube
	)
{
	std::pair<ga::Vector, ga::Vector> pntPair;

	if (ray.isValid() && tube.isValid())
	{
		using namespace ga;
		Vector const pVec(tube.theAxis.projectionOf(ga::vZero));
		Vector const & aDir = tube.theAxis.theDir;
		double const & rho = tube.theRadius;
		Vector const & sVec = ray.theStart;
		Vector const & dDir = ray.theDir;

		Vector const rVec{ sVec - pVec };
		double const adDot{ ga::dot(aDir, dDir).theValue };
		double const arDot{ ga::dot(aDir, rVec).theValue };
		Vector const uVec{ dDir - adDot*aDir };

		// check for singularities
		double const uMag{ ga::magnitude(uVec) };

		if (0. < uMag) // non-singular cases
		{
			// normalize vectors
			double const perMagU{ 1. / uMag };
			Vector const uHat{ perMagU * uVec }; // 1

			// compute unitless parameters
			Vector const vVec{ rVec - arDot*aDir };
			Vector const wVec{ (1./rho) * vVec };
			BiVector const uwBiv{ ga::wedge(uHat, wVec) }; // 1
			double const uwMag{ ga::magnitude(uwBiv) }; // 1
			double const delSq{ 1. - math::sq(uwMag) }; // 1

			if (0. <= delSq)
			{
				// compute terms of quadratic formula
				double const middle{ -ga::dot(uHat, wVec).theValue };
				double const delta{ std::sqrt(delSq) };

				// evaluate roots
				double const nuA{ perMagU*(middle + delta) };
				double const nuB{ perMagU*(middle - delta) };

				// reconstruct intersection points in order proximate/distal
				std::pair<double, double> const sortPair
					{ orderedPairFor(nuA, nuB) };
				if (dat::isValid(nuA))
				{
					double const lambda{ rho * nuA };
					if (! (lambda < 0.))
					{
						pntPair.first = sVec + lambda*dDir;
					}
				}
				if (dat::isValid(nuB))
				{
					double const lambda{ rho * nuB };
					if (! (lambda < 0.))
					{
						pntPair.second = sVec + lambda*dDir;
					}
				}
			}
			// else // imaginary solutions
		}
		// else // (0==uVec) ray parallel with tube
	}

	return pntPair;
}

//======================================================================
}
}

