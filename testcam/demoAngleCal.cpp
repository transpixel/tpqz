//
//
// MIT License
//
// Copyright (c) 2020 Stellacore Corporation.
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
\brief  This file contains main application program demoAngleCal
*/


#include "libapp/Usage.h"

#include "build/version.h"
#include "libio/stream.h"


#include "libcam/fit.h"

#include "libdat/array.h"
#include "libdat/info.h"
#include "libdat/Spot.h"
#include "libmath/math.h"
#include "libprob/Histogram.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <random>


namespace
{

	std::random_device sRandDev{};
	std::mt19937_64 sRandGen{ sRandDev() };

	dat::Spot
	detMeaErr
		( double const & pixSigma
		)
	{
		std::normal_distribution<double> distro{ 0., pixSigma };
		return dat::Spot{ distro(sRandGen), distro(sRandGen) };
	}

	double
	distMeaErr
		( double const & distSigma
		)
	{
		std::normal_distribution<double> distro{ 0., distSigma };
		return { distro(sRandGen) };
	}

	struct Uncertainty
	{
		double const theSigmaPix;
		double const theSigmaL1;
		double const theSigmaL2;
		double const theSigmaD;
	};

	dat::Extents const sDetSize{ 3120u, 4160u };

	struct MeaData
	{
		dat::Spot const theM1;
		dat::Spot const theM2;

		double const theL1;
		double const theL2;
		double const theD;
	};

	struct Observations
	{
		MeaData const theMea;
		Uncertainty const theUnc;

		Observations
			( MeaData const & mea
			, Uncertainty const & unc
			)
			: theMea{ mea }
			, theUnc{ unc }
		{
		}

		inline
		dat::Spot
		m1
			( dat::Spot const & delM1 = { 0., 0, }
			) const
		{
			using dat::operator+;
			return { theMea.theM1 + delM1 };
		}

		inline
		dat::Spot
		m2
			( dat::Spot const & delM2 = { 0., 0, }
			) const
		{
			using dat::operator+;
			return { theMea.theM2 + delM2 };
		}

		double
		angleMag
			( std::array<double, 3u> const & delLLD = { 0., 0., 0. }
			) const
		{
			double const lam1{ theMea.theL1 + delLLD[0] };
			double const lam2{ theMea.theL2 + delLLD[1] };
			double const dist{ theMea.theD + delLLD[2] };
			double const num
				{ math::sq(lam1) + math::sq(lam2) - math::sq(dist) };
			double const den{ 2. * lam1 * lam2 };
			return std::acos(num / den);
		}
	};

	std::vector<double>
	pdBestFrom
		( Observations const & mea
		)
	{
		return cam::fit::principalDistanceFor
			( std::make_pair(mea.m1(), mea.m2())
			, mea.angleMag()
			, sDetSize
			);
	}

	prob::Histogram
	pdsFrom
		( Observations const & mea
		, math::Partition const & pdPart
		, size_t const & numTrials
		)
	{
		prob::Histogram histo(pdPart);
		for (size_t nn{0u} ; nn < numTrials ; ++nn)
		{
			dat::Spot const delM1{ detMeaErr(mea.theUnc.theSigmaPix) };
			dat::Spot const delM2{ detMeaErr(mea.theUnc.theSigmaPix) };
			std::array<double, 3u> const delLLD
				{ distMeaErr(mea.theUnc.theSigmaL1)
				, distMeaErr(mea.theUnc.theSigmaL2)
				, distMeaErr(mea.theUnc.theSigmaD)
				};
			std::vector<double> const pds
				{ cam::fit::principalDistanceFor
					( std::make_pair(mea.m1(delM1), mea.m2(delM2))
					, mea.angleMag(delLLD)
					, sDetSize
					)
				};
			for (double const & pd : pds)
			{
				histo.addSample(pd);
			}
		}
		return histo;
	}

}

//! Compute (sample distribution of) principal distance from measured values
int
main
	( int const argc
	, char const * const * const argv
	)
{
	// check args
	app::Usage usage;
	usage.setSummary
		( "Estimate principal distance value using (hardcoded) measurements"
		);
	usage.addArg("histOutFile", "Histogram of PD values <.txt>");
	// ...
	if (usage.argStatus(argc, argv) != app::Usage::Valid)
	{
		std::string const fname(argv[0]);
		io::err()
			<< std::endl << build::version::buildInfo(argv[0]) << std::endl
			<< usage.infoString(fname) << std::endl;
		return 1;
	}

	// parse input argument
	int argnum(0);
	std::string const histPath(argv[++argnum]);


	// use hardcoded measurement values here
	Uncertainty const meaUnc{ 5., .50, .50, .50 }; // Real
//	Uncertainty const meaUnc{ 1., .20, .20, .20 }; // Ideal
	MeaData const meaTLBR
		{ dat::Spot{  162., 2683. }
		, dat::Spot{ 2542., 1739. }
		, 238.5
		, 328.5
		, 230.
		};

	Observations const meas{ meaTLBR, meaUnc };

	// compute best estimate (zero error case)
	std::vector<double> const bestPDs{ pdBestFrom(meas) };
	if (! bestPDs.empty())
	{
		// configure histogram for adding result variants
		double const nomPD{ bestPDs.front() };
		double const halfSpan{ 150. };
		double const histMin{ nomPD - halfSpan };
		double const histMax{ nomPD + halfSpan };
		dat::Range<double> histRange{ histMin, histMax };
		size_t const numParts{ (size_t)(histRange.magnitude()) };
		math::Partition const pdPart{ histRange, numParts };

		// compute variants using pseudo-random perturbation of measurements
		size_t const numTrials{ 1u * 1024u*1024u };
		prob::Histogram const histo{ pdsFrom(meas, pdPart, numTrials) };

		io::out()
			<< dat::infoString(bestPDs.begin(), bestPDs.end(), "best") << '\n';
		std::ofstream ofs(histPath);
		histo.writeDataProbs(ofs, "hist");
	}

	return 0;
}
