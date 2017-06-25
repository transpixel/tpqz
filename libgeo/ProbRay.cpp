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


namespace geo
{

// explicit
ProbRay :: ProbRay
	( geo::Ray const & ray
	, math::Partition const & probPart
	, double const & rayDirSigma
	)
	: theRay{ ray }
	, thePart{ probPart }
	, theProbs(thePart.size(), 0.) // allocate and initialize
	, theDirSigma{ rayDirSigma }
{
	assert(thePart.isValid());
	assert(std::numeric_limits<double>::epsilon() < theDirSigma);
}

bool
ProbRay :: isValid
	() const
{
	return
		(  theRay.isValid()
		&& thePart.isValid() && (0u < numSamples())
		&& dat::isValid(theDirSigma)
		);
}

void
ProbRay :: considerPoint
	( ga::Vector const & pnt
	, double const & pntSigma
	)
{
	if (isValid() && pnt.isValid())
	{
		// ray probability of observing point
		double const angleMag{ subtendedAngleFor(pnt, theRay) };
		double const probRay{ pseudoProbFor(angleMag, theDirSigma) };

		// probability of domain value being near point
		ga::Vector const projVec{ theRay.projectionOf(pnt) };
		ga::Vector const rejVec{ theRay.rejectionTo(pnt) };
		double const projMag{ ga::magnitude(projVec) };
		double const rejMag{ ga::magnitude(rejVec) };

		// evaluate probability at each sample location
		size_t const numSamps{ numSamples() };
		for (size_t nn{0u} ; nn < numSamps ; ++nn)
		{
			// sampling distance along the ray
			double const mu{ thePart.interpValueFor(double(nn)) };

			// probability of mu value relative to point
			double const deltaRay{ projMag - mu };
			double const pntDist{ math::hypot(deltaRay, rejMag) };
			double const probPnt{ pseudoProbFor(pntDist, pntSigma) };

			// use composite as probability of interest (TODO good?bad?)
			double const prob{ probRay * probPnt };
			if (dat::isValid(prob))
			{
				theProbs[nn] += prob;
			}
		}
	}
}

void
ProbRay :: considerRay
	( geo::Ray const & ray
	, double const & raySigma
	)
{
	if (isValid() && ray.isValid())
	{
		// evaluate probability at each sample location
		size_t const numSamps{ numSamples() };
		double const & uSigma = theDirSigma;
		for (size_t nn{0u} ; nn < numSamps ; ++nn)
		{
			// sampling distance along the ray
			double const mu{ thePart.interpValueFor(double(nn)) };
			ga::Vector const uPnt{ theRay.pointAt(mu) };
			ga::Vector const vPnt{ ray.projectionOf(uPnt) };

			double const uAngle{ subtendedAngleFor(vPnt, theRay) };
			double const vAngle{ subtendedAngleFor(uPnt, ray) };

			double const vSigma = raySigma;

			// compute the probability of each point being on the other ray
			double const uProb{ pseudoProbFor(uAngle, uSigma) };
			double const vProb{ pseudoProbFor(vAngle, vSigma) };

			// use composite as probability of interest (TODO good?bad?)
			double const prob{ uProb * vProb };
			if (dat::isValid(prob))
			{
				theProbs[nn] += prob;
			}
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
		ga::Vector const & vv = rayOnCone.theStart;
		ga::Vector const & rr = rayOnCone.theDir;
		ga::Vector const aa{ ga::unit(coneAxisDir) };
		assert(aa.isValid());

		// determine cones (half)apex angle
		double const coneAngle{ angleMagBetween(rr, aa) };

		// evaluate probability at each sample location
		size_t const numSamps{ numSamples() };
		for (size_t nn{0u} ; nn < numSamps ; ++nn)
		{
			// sampling distance along the ray
			double const mu{ thePart.interpValueFor(double(nn)) };

			// compute direction to sampling point on ray
			ga::Vector const uPnt{ theRay.pointAt(mu) };
			ga::Vector const uDir{ ga::unit(uPnt - vv) };
			double const muAngle{ angleMagBetween(uDir, aa) };

			// deviation angle from cone
			double const vAngle{ muAngle - coneAngle };
			double const & vSigma = apexSigma;

			// probability value at this sample
			double const prob{ pseudoProbFor(vAngle, vSigma) };
			if (dat::isValid(prob))
			{
				theProbs[nn] += prob;
			}
		}
	}
}

std::vector<ProbRay::DistProb>
ProbRay :: distProbs
	() const
{
	std::vector<DistProb> distprobs;
	if (isValid())
	{
		size_t const numSamps{ numSamples() };
		distprobs.reserve(numSamps);
		for (size_t nn{0u} ; nn < numSamps ; ++nn)
		{
			// sampling distance along the ray
			double const mu{ thePart.interpValueFor(double(nn)) };
			double const & prob = theProbs[nn];
			distprobs.emplace_back(DistProb{ mu, prob });
		}
	}
	return distprobs;
}

std::pair<double, double>
ProbRay :: likelyDistProb
	() const
{
	std::pair<double, double> dpPair
		{ dat::nullValue<double>(), dat::nullValue<double>() };
	double & dist = dpPair.first;
	double & prob = dpPair.second;

	if (isValid())
	{
		using Iter = std::vector<double>::const_iterator;
		std::pair<Iter, Iter> const itMinMax
			{ std::minmax_element(theProbs.begin(), theProbs.end()) };
		Iter const & itMin = itMinMax.first;
		Iter const & itMax = itMinMax.second;
		if (itMax != itMin) // non-const prob samples
		{
			size_t const ndxCurr{ size_t(itMax - theProbs.begin()) };
			if (0u < ndxCurr)
			{
				size_t const ndxPrev{ ndxCurr - 1u }; 
				size_t const ndxNext{ ndxCurr + 1u }; 
				if (ndxNext < theProbs.size())
				{
					// three values spaning the (assumed) peak
					double const alpha{ theProbs[ndxPrev] };
					double const beta{ theProbs[ndxCurr] };
					double const gamma{ theProbs[ndxNext] };

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
		oss << dat::infoString(theDirSigma, "theDirSigma");
	}
	else
	{
		oss << " <null>";
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


// static
double
ProbRay :: subtendedAngleFor
	( ga::Vector const & pnt
	, geo::Ray const & ray
	)
{
	double angleMag{ dat::nullValue<double>() };
	ga::Vector const projPnt{ ray.projectionOf(pnt) };
	ga::Vector const rejPnt{ ray.rejectionTo(pnt) };
	double const projMag{ ga::magnitude(projPnt) };
	double const rejMag{ ga::magnitude(rejPnt) };
	if (dat::isValid(projMag) && dat::isValid(rejMag))
	{
		angleMag = std::atan2(rejMag, projMag);
	}
	return angleMag;
}


} // geo

