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
\brief  This file contains unit test for geo::ProbRay
*/


#include "libgeo/ProbRay.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libga/Rigid.h"
#include "libgeo/intersect.h"
#include "libio/sprintf.h"
#include "libio/stream.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
geo_ProbRay_test0
	()
{
	std::ostringstream oss;
	geo::ProbRay const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

	geo::Ray
	someRay
		()
	{
		ga::Vector const start( 3., -5., 2.);
		ga::Vector const dir{ ga::unit(ga::Vector(-1., .5, 3.)) };
		geo::Ray const ray(start, dir);
		return ray;
	}

	math::Partition
	somePart
		()
	{
		constexpr size_t numParts{ 1024u };
		constexpr double maxDist{ 3. };
		constexpr double delta{ maxDist / double(numParts) };
		math::Partition const part(0., delta, numParts);
		return part;
	}

	void
	saveDistProbs
		( geo::ProbRay const & probRay
		, std::string const & fname
		)
	{
		using DistProb = geo::ProbRay::DistProb;
		std::vector<DistProb> const distProbs(probRay.distProbs());

		std::ofstream ofs(fname);
		for (DistProb const & distProb : distProbs)
		{
			double const & mu = distProb.first;
			double const & prob = distProb.second;
			ofs
				<< "mu,prob:"
				<< " " << io::sprintf("%9.6f", mu)
				<< " " << io::sprintf("%9.6f", prob)
				<< std::endl;
		}
	}


//! Check fundamental ray operations std::string
std::string
geo_ProbRay_test1
	()
{
	std::ostringstream oss;

	geo::Ray const ray(ga::Vector(-0., 0., 0.), ga::e1);

	// check that lone ray has no likely distance
	constexpr double rayDirSigma{ 1. };
	geo::ProbRay probRay(ray, somePart(), rayDirSigma);

	// check for invalid probability if no additional information
	{
		double const gotDist{ probRay.likelyDistance() };
		if (  dat::isValid(gotDist))
		{
			oss << "Failure of likelyDistance empty test" << std::endl;
		}
	}

	// evaluate probability associated with a point
	double const expDist{ 1.75 };
	ga::Vector const somePnt{ probRay.theRay.pointAt(expDist) + ga::e3 };
	constexpr double pntSigma{ 1. };
	probRay.considerPoint(somePnt, pntSigma);
	double const gotDist{ probRay.likelyDistance() };
	double const tolDist{ .001 * (1. / double(probRay.numSamples())) };
	if (! dat::nearlyEquals(gotDist, expDist, tolDist))
	{
		double const delDist{ gotDist - expDist };
		oss << "Failure of likelyDistance point test" << std::endl;
		oss << dat::infoString(expDist, "expDist") << std::endl;
		oss << dat::infoString(gotDist, "gotDist") << std::endl;
		oss << "deltaDist: " << io::sprintf("%.15e", delDist) << std::endl;
	}

	// save to file e.g. for debugging
	// saveDistProbs(probRay, "prob.dat");

	return oss.str();
}

//! Check ray-ray interactions
std::string
geo_ProbRay_test2
	()
{
	std::ostringstream oss;

	// pretty simple test - but here to check general handling
	// not the underlying theory

	// a few rays
	std::vector<geo::Ray> const rays
		{ geo::Ray(ga::Vector(-1.,  0.,  0.) ,  ga::e1)
		, geo::Ray(ga::Vector( 1., -1.,  1.) ,  ga::e2)
		, geo::Ray(ga::Vector( 1.,  1., -1.) ,  ga::e3)
		};

	// compute classic intersection point (projected onto primary ray)
	using PntPair = std::pair<ga::Vector, ga::Vector>;
	PntPair const pair01{ geo::intersect::pointsFor(rays[0], rays[1]) };
	PntPair const pair02{ geo::intersect::pointsFor(rays[0], rays[2]) };
	ga::Vector const expPnt{ .5 * (pair01.first + pair02.first) };
	double const expDist{ rays[0].distanceAlong(expPnt) };

	// construct instance
	std::vector<geo::Ray>::const_iterator const itPrime{ rays.begin() };
	constexpr double dirSigma{ 1. };
	geo::ProbRay const probRay
		{ geo::ProbRay::from
			(rays.begin(), rays.end(), itPrime, somePart(), dirSigma)
		};

	// check if probability result is consistent with classic intersection
	double const gotDist{ probRay.likelyDistance() };
	double const tolDist{ .001 * (1. / double(probRay.numSamples())) };
	if (! dat::nearlyEquals(gotDist, expDist, tolDist))
	{
		oss << "Failure of multi-ray test" << std::endl;
		oss << dat::infoString(expDist, "expDist") << std::endl;
		oss << dat::infoString(gotDist, "gotDist") << std::endl;
	}

	return oss.str();
}

//! Check skew rays
std::string
geo_ProbRay_test3
	()
{
	std::ostringstream oss;

	std::vector<ga::Vector> const stations
		{ ga::Vector( 0.,  0.,  0.)
		, ga::Vector( 0.,  0.,  1.)
		, ga::Vector( 0.,  0.,  2.)
		, ga::Vector( 0.,  0.,  3.)
		, ga::Vector( 0.,  0.,  4.)
		, ga::Vector( 0.,  0.,  5.)
		};
	std::vector<ga::Vector> const targets
		{ ga::Vector( 1.,  .0,  0.) // prime ray

		// points of interest
		, ga::Vector( 1.000,  .0,  0.) // blunder
		, ga::Vector( 2.000,  .0,  0.)
		, ga::Vector( 2.500,  .0,  0.)
		, ga::Vector( 3.000,  .0,  0.)
		, ga::Vector( 5.000,  .0,  0.) // blunder
		};
	dat::Range<double> const distRange(0., 10.);
	ga::Vector const expPnt(2.575, 0., 0.); // from inspection of test case
	double const tolDist{ .01 };

	// generate rays from stations and associated target points
	size_t const numRays{ stations.size() };
	assert(numRays == targets.size());
	std::vector<geo::Ray> rays(numRays);
	for (size_t nn{0u} ; nn < numRays ; ++nn)
	{
		rays[nn] = geo::Ray::fromToward(stations[nn], targets[nn]);
	}

	// note primary ray
	geo::Ray const & primeRay = rays.front();

	// populate probability configuration
	constexpr double dirSigma{ .1 };
	constexpr size_t numParts{ 1024u };
	math::Partition const distPart(distRange, numParts);
	geo::ProbRay probRay(primeRay, distPart, dirSigma);
	for (geo::Ray const & ray : rays)
	{
		constexpr double skewSigma{ 1. };
		probRay.considerRay(ray, skewSigma);
	}

	// compute probablistic solution
	ga::Vector const gotPnt{ probRay.likelyPoint() };

	if (! gotPnt.nearlyEquals(expPnt, tolDist))
	{
		oss << "Failure of skew ray test" << std::endl;
		oss << dat::infoString(expPnt, "expPnt") << std::endl;
		oss << dat::infoString(gotPnt, "gotPnt") << std::endl;
	}

	// save to file e.g. for debugging
	// saveDistProbs(probRay, "prob.dat");

	return oss.str();
}

//! Check conical intersection
std::string
geo_ProbRay_test4
	()
{
	std::ostringstream oss;

	ga::Vector const cVertNom(0., 0., 10.);
	ga::Vector const aVecNom(0., 0., 1.);
	ga::Vector const rStartNom(1., 0., 5.);
	ga::Vector const expPntNom(-5., 0., 0.);

#define EasyData
#	if defined(EasyData)
	ga::Vector const locFrame( 0., 0., 0.);
	ga::BiVector const bivFrame( 0., 0., 0.);
#	else
	ga::Vector const locFrame( -3., 5., -7.);
	ga::BiVector const bivFrame(.5, .3, .7);
#	endif

	ga::Pose const attFrame(bivFrame);
	ga::Rigid const xform(locFrame, attFrame);

	ga::Vector const cVert(xform(cVertNom));
	ga::Vector const aVec(attFrame(ga::unit(aVecNom)));
	ga::Vector const rStart(xform(rStartNom));
	ga::Vector const expPnt(xform(expPntNom));

	double const expRange{ ga::magnitude(expPnt - rStart) };

	// generate axis - compute sign to ensure a solution
	ga::Vector const delPnt{ expPnt - cVert };
	double const da{ ga::dot(aVec, delPnt).theValue };
	ga::Vector const cAxis{ da * aVec };

	// generate primary ray
	geo::Ray const primeRay{ geo::Ray::fromToward(rStart, expPnt) };
	geo::Ray const coneRay{ geo::Ray::fromToward(cVert, expPnt) };

	// evaluate class
	// populate probability configuration
	constexpr size_t numParts{ 1024u };
	dat::Range<double> const distRange(0., 2.*expRange);
	math::Partition const distPart(distRange, numParts);
	constexpr double dirSigma{ .1 };

	geo::ProbRay probRay(primeRay, distPart, dirSigma);
	probRay.considerCone(coneRay, cAxis, dirSigma);

	ga::Vector const gotPnt{ probRay.likelyPoint() };
	double const tolDist{ 1./double(numParts) };
	if (! gotPnt.nearlyEquals(expPnt, tolDist))
	{
		oss << "Failure of considerCone test" << std::endl;
		oss << dat::infoString(expPnt, "expPnt") << std::endl;
		oss << dat::infoString(gotPnt, "gotPnt") << std::endl;
	}

	// save to file e.g. for debugging
	// saveDistProbs(probRay, "prob.dat");

	return oss.str();
}


}

//! Unit test for geo::ProbRay
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << geo_ProbRay_test0();
	oss << geo_ProbRay_test1();
	oss << geo_ProbRay_test2();
	oss << geo_ProbRay_test3();
	oss << geo_ProbRay_test4();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}