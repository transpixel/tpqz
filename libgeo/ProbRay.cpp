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
\brief Definitions for geo::ProbRay
*/


#include "libgeo/ProbRay.h"

#include "libgeo/intersect.h"
#include "libio/sprintf.h"
#include "libmath/QuadEq.h"

#include <cassert>
#include <algorithm>
#include <fstream>
#include <sstream>


namespace
{
	std::vector<ga::Vector>
	pointsAlong
		( geo::Ray const & uRay
		, math::Partition const & uPart
		)
	{
		std::vector<ga::Vector> uPnts;
		if (uRay.isValid() && uPart.isValid())
		{
			size_t const numSamps{ uPart.size() };
			uPnts.reserve(numSamps);
			for (size_t nn{0u} ; nn < numSamps ; ++nn)
			{
				// sampling distance along the ray
				double const mu{ uPart.interpValueFor(double(nn)) };
				ga::Vector const uPnt{ uRay.pointAt(mu) };
				uPnts.emplace_back(uPnt);
			}
		}
		return uPnts;
	}
}

namespace geo
{

// explicit
ProbRay :: ProbRay
	( geo::Ray const & ray
	, math::Partition const & probPart
	, double const & rayAngleSigma
	)
	: theRay{ ray }
	, thePart{ probPart }
	, theAccums{} // alloc/init below
	, thePntUs{ pointsAlong(theRay, thePart) }
	, theDistroAngU{ rayAngleSigma, 0. }
{
	assert(thePart.isValid());
	assert(theDistroAngU.isValid());

	// can override in initAccumulator
	double const pSelf{ theDistroAngU(0.) };
	theAccums.resize(thePart.size(), pSelf); // allocate and initialize

	assert(thePntUs.size() == theAccums.size());
}

bool
ProbRay :: isValid
	() const
{
	return
		(  theRay.isValid()
		&& thePart.isValid() && (0u < numSamples())
		&& dat::isValid(theDistroAngU)
		);
}

void
ProbRay :: considerPoint
	( ga::Vector const & vPnt
	, double const & vPntSigma
	)
{
	if (isValid() && vPnt.isValid())
	{
		// evaluate probability at each sample location
		size_t const numSamps{ numSamples() };

		prob::Gauss const & distroVwU = theDistroAngU;
		prob::Gauss const distroUwV(vPntSigma);

		for (size_t nn{0u} ; nn < numSamps ; ++nn)
		{
			geo::Ray const & uRay = theRay;

			// prob density of point w.r.t uRay
			double const angVwU{ ga::magnitude(uRay.angleTo(vPnt)) };

			// prob density of ray w.r.t. point
			ga::Vector const & uPnt = thePntUs[nn];
			double const dstUwV{ ga::magnitude(uPnt - vPnt) };

			// compute the probability of each entity w.r.t. the other
			double const pdfVwU{ distroVwU(angVwU) };
			double const pdfUwV{ distroUwV(dstUwV) };

			// use composite as probability of interest (TODO good?bad?)
			double const density{ pdfVwU * pdfUwV };
			accumulateDensity(density, nn);
		}
	}
}

void
ProbRay :: considerRay
	( geo::Ray const & vRay
	, double const & vRaySigma
	)
{
	if (isValid() && vRay.isValid())
	{
		// evaluate probability at each sample location
		size_t const numSamps{ numSamples() };
		prob::Gauss const & distroVwU = theDistroAngU;
		prob::Gauss const distroUwV(vRaySigma);
		for (size_t nn{0u} ; nn < numSamps ; ++nn)
		{
			geo::Ray const & uRay = theRay;
			ga::Vector const & uPnt = thePntUs[nn];

			// other entity
			ga::Vector const vPnt{ vRay.projectionOf(uPnt) };

			// probability of vRay w.r.t uRay and v.v.
			double const angVwU{ ga::magnitude(uRay.angleTo(vPnt)) };
			double const angUwV{ ga::magnitude(vRay.angleTo(uPnt)) };

			// compute the probability of each point being on the other ray
			double const pdfVwU{ distroVwU(angVwU) };
			double const pdfUwV{ distroUwV(angUwV) };

			// use composite as probability of interest (TODO good?bad?)
			double const density{ pdfVwU * pdfUwV };
			accumulateDensity(density, nn);
		}
	}
}

namespace
{
	inline
	double
	angleMagBetween
		( ga::Vector const & vecA
		, ga::Vector const & vecB
		)
	{
		ga::Spinor const spinAB{ ga::spin::between(vecA, vecB) };
		ga::BiVector const angleAB{ ga::spin::physicalAngleFrom(spinAB) };
		return ga::magnitude(angleAB);
	}
}

void
ProbRay :: considerCone
	( geo::Ray const & rayOnCone
	, ga::Vector const & coneAxisDir
	, double const & apexSigma
	)
{
	if (isValid() && rayOnCone.isValid() && coneAxisDir.isValid())
	{
		// access the relevant entities
		ga::Vector const & vSta = rayOnCone.theStart;
		ga::Vector const & vDir = rayOnCone.theDir;
		ga::Vector const aDir{ ga::unit(coneAxisDir) };
		assert(aDir.isValid());

		// determine cones (half)apex angle
		double const apexAngle{ angleMagBetween(vDir, aDir) };
		prob::Gauss const distroUwV(apexSigma, apexAngle);

		// evaluate probability at each sample location
		size_t const numSamps{ numSamples() };
		for (size_t nn{0u} ; nn < numSamps ; ++nn)
		{
			ga::Vector const & uPnt = thePntUs[nn];

			// compute direction to sampling point on ray
			ga::Vector const uDir{ ga::unit(uPnt - vSta) };
			double const angUwV{ angleMagBetween(uDir, aDir) };

			// probability value at this sample
			double const density{ distroUwV(angUwV) };
			if (dat::isValid(density))
			{
				theAccums[nn] += density;
			}
		}
	}
}

double
ProbRay :: probDensityAt
	( double const & distAlong
	, std::vector<DistProb> const & distProbs
	) const
{
	double prob{ dat::nullValue<double>() };
	if (isValid() && dat::isValid(distAlong))
	{
		double const fndx{ thePart.interpIndexFor(distAlong) };
		size_t const ndxLo{ (size_t)std::floor(fndx) };
		size_t const ndxHi{ ndxLo + 1u };
		if ((0u < ndxLo) && (ndxHi < theAccums.size()))
		{
			double const frac{ fndx - double(ndxLo) };
			std::pair<double, double> const probPair
				{ distProbs[ndxLo].second, distProbs[ndxHi].second };
			prob = math::interp::valueAtValid(frac, probPair);
		}
	}
	return prob;
}

std::vector<ProbRay::DistProb>
ProbRay :: distProbs
	() const
{
	std::vector<DistProb> dps;
	if (isValid())
	{
		// compute sum (for normalization)
		double const sum
			{ std::accumulate(theAccums.begin(), theAccums.end(), 0.) };
		size_t const numSamps{ numSamples() };
		dps.resize(numSamps);
		double normCo{ 0. };
		if (std::numeric_limits<double>::min() < sum)
		{
			normCo = 1. / sum;

		}
		for (size_t nn{0u} ; nn < numSamps ; ++nn)
		{
			// sampling distance along the ray
			double const mu{ thePart.interpValueFor(double(nn)) };
			double const prob{ normCo * theAccums[nn] };
			dps[nn] = DistProb{ mu, prob };
		}
	}
	return dps;
}

std::pair<double, double>
ProbRay :: likelyDistProb
	( std::vector<DistProb> const & distProbs
	) const
{
	std::pair<double, double> dpPair
		{ dat::nullValue<double>(), dat::nullValue<double>() };
	double & dist = dpPair.first;
	double & prob = dpPair.second;

	std::vector<DistProb> const & dps = distProbs;

	if (isValid())
	{
		using Iter = std::vector<DistProb>::const_iterator;
		std::pair<Iter, Iter> const itMinMax
			{ std::minmax_element
				( dps.begin(), dps.end()
				, [] (DistProb const & dpA, DistProb const &dpB)
					{ return (dpA.second < dpB.second); }
				)
			};
		Iter const & itMin = itMinMax.first;
		Iter const & itMax = itMinMax.second;
		if (itMax != itMin) // non-const prob samples
		{
			size_t const ndxCurr{ size_t(itMax - dps.begin()) };
			if (0u < ndxCurr)
			{
				size_t const ndxPrev{ ndxCurr - 1u }; 
				size_t const ndxNext{ ndxCurr + 1u }; 
				if (ndxNext < dps.size())
				{
					// three values spaning the (assumed) peak
					double const alpha{ dps[ndxPrev].second };
					double const beta{ dps[ndxCurr].second };
					double const gamma{ dps[ndxNext].second };

					// verify a 'true' peak (not a flat-ish spot)
					if ((alpha < beta) && (gamma < beta))
					{
						// interpolated peak location
						// formula from
						//   https://www.dsprelated.com
						//   /freebooks/sasp
						//   /Quadratic_Interpolation_Spectral_Peaks.html
						double const num{ alpha - gamma };
						double const den{ alpha - 2.*beta + gamma };
						double const denMag{ std::abs(den) };
						if (std::numeric_limits<double>::min() < denMag)
						{
							// peak location - provides distance
							double const fndx{ .5 * num / den };
							double const ndx{ double(ndxCurr) + fndx };
							dist = thePart.interpValueFor(ndx);

							// peak magnitude - provides (quasi)probability
							prob = beta - .25*(alpha - gamma)*fndx;
						}
					}
				} // not at end
			} // not at start
		} // distinct min and max

	} // valid instance

	return dpPair;
}

std::pair<double, double>
ProbRay :: likelyDistProb
	() const
{
	return likelyDistProb(distProbs());
}

double
ProbRay :: likelyDistance
	() const
{
	return likelyDistProb().first;
}

ga::Vector
ProbRay :: likelyPoint
	() const
{
	ga::Vector pnt;
	if (isValid())
	{
		double const dist{ likelyDistance() };
		if (dat::isValid(dist))
		{
			pnt = theRay.pointAt(dist);
		}
	}
	return pnt;
}

std::string
ProbRay :: infoString
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
		oss << dat::infoString(theRay, "theRay");

		oss << std::endl;
		oss << dat::infoString(thePart, "thePart");

		oss << std::endl;
		oss << dat::infoString(theDistroAngU, "theDistroAngU");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

std::string
ProbRay :: infoStringPDF
	( std::string const & fmtDist
	, std::string const & fmtProb
	) const
{
	std::ostringstream oss;
	if (isValid())
	{
		std::vector<DistProb> const dps{ distProbs() };
		oss << "# dist-on-ray, probability" << '\n';
		for (DistProb const & dp : dps)
		{
			double const & dist = dp.first;
			double const & prob = dp.second;
			oss
				<< " " << io::sprintf(fmtDist, dist)
				<< " " << io::sprintf(fmtProb, prob)
				<< '\n';
		}
	}
	else
	{
		oss << "# <null>";
	}
	return oss.str();
}

void
ProbRay :: saveDistProbs
	( std::ostream & oss
	, std::string const & fmt
	) const
{
	std::vector<DistProb> const dps{ distProbs() };
	for (DistProb const & dp : dps)
	{
		oss
			<< io::sprintf(fmt, dp.first)
			<< " "
			<< io::sprintf(fmt, dp.second)
			<< '\n';
	}
}

void
ProbRay :: accumulateDensity
	( double const & nextProb
	, size_t const & ndx
	)
{
	if (dat::isValid(nextProb))
	{
		assert(ndx < theAccums.size());
		theAccums[ndx] += nextProb;
		/*
		double const & pA = theAccums[ndx];
		double const & pB = nextProb;
		theAccums[ndx] = netProb(pA, pB);
		*/
	}
}

} // geo

