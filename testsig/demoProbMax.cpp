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
\brief  This file contains main application program demoProbMax
*/


#include "libapp/Usage.h"

#include "build/version.h"
#include "libio/stream.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/sprintf.h"
#include "libmath/math.h"
#include "libmath/Partition.h"
#include "libprob/Histogram.h"

#include <random>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>


namespace
{
	constexpr size_t const sMaxNumObs{ 48u };
	constexpr size_t const sNumLocs{ 256u }; // possible sample values
	constexpr double const sSigma{ 5. };
	constexpr double const sMeanMin{ 5.*sSigma };
	constexpr double const sMeanMax{ double(sNumLocs) - sMeanMin };
	constexpr size_t const sNumTrials{ 1u * 2u * 1024u };
	constexpr size_t const sNumMetBins{ 1024u };

	//
	// Approximation to maxima
	// .08 / sqrt(x-.5)
	//
	//


	//! Define domain of measurements
	std::vector<double>
	sigLocs
		()
	{
		std::vector<double> locs(sNumLocs);
		std::iota(locs.begin(), locs.end(), 0.);
		return locs;
	}

	//! Probability density functor.
	struct Gaussian
	{
		double theMu{ dat::nullValue<double>() };
		double theSig{ dat::nullValue<double>() };
		double theArgCo{ dat::nullValue<double>() };
		double theNormCo{ dat::nullValue<double>() };

		Gaussian
			() = default;

		Gaussian
			( double const & mean
			, double const & sigma
			)
			: theMu{ mean }
			, theSig{ sigma }
			, theArgCo{ -1. / (2.*math::sq(theSig)) }
			, theNormCo{ std::sqrt(-theArgCo / math::pi) }
		{
			/*
			io::out() << dat::infoString(theMu, "theMu") << std::endl;
			io::out() << dat::infoString(theSig, "theSig") << std::endl;
			io::out() << dat::infoString(theArgCo, "theArgCo") << std::endl;
			io::out() << dat::infoString(theNormCo, "theNormCo") << std::endl;
			*/
		}

		double
		operator()
			( double const & loc
			) const
		{
			double const arg{ theArgCo * math::sq(loc - theMu) };
			return { theNormCo * std::exp(arg) };
		}
	};

	inline
	double
	randMean
		()
	{
		static std::random_device rdev;
		static std::mt19937_64 rgen{ rdev() };
		static std::uniform_real_distribution<> distro(sMeanMin, sMeanMax);
		return { distro(rgen) };
	}

	std::vector<Gaussian>
	randDistros
		( size_t const & numObs
		)
	{
		std::vector<Gaussian> distros;
		distros.reserve(numObs);
		for (size_t nn{0u} ; nn < numObs ; ++nn)
		{
			double const mean{ randMean() };
			Gaussian const distro(mean, sSigma);
			distros.emplace_back(distro);
		}
		return distros;
	}

	//! Compositite PDF for all distros
	std::vector<double>
	compositePdfFor
		( std::vector<double> const & locs
		, std::vector<Gaussian> const & curves
		)
	{
		std::vector<double> compProbs;
		compProbs.reserve(locs.size());
		double const normScale{ 1. / double(curves.size()) };
		for (double const & loc : locs)
		{
			double compProb{ 0. };
			for (Gaussian const & curve : curves)
			{
				compProb += curve(loc);
			}
			compProb = normScale * compProb;
			compProbs.emplace_back(compProb);
		}
		return compProbs;
	}

	//! Largest value in array (max_element)
	double
	maxValueIn
		( std::vector<double> const & probs
		)
	{
		std::vector<double>::const_iterator const itMax
			{ std::max_element(probs.begin(), probs.end()) };
		assert(probs.end() != itMax);
		return *itMax;
	}

	//! Max of composite pdf for one random trial
	double
	metricOneTrial
		( size_t const & numObs
		, std::vector<double> const & locs
		)
	{

		// random combination of observation distributions (measurements)
		std::vector<Gaussian> const distros{ randDistros(numObs) };

		// composite of all observation distros
		std::vector<double> const compPdf{ compositePdfFor(locs, distros) };
		assert(compPdf.size() == locs.size());

		// return maximum composite probability
		double const metric{ maxValueIn(compPdf) };
//io::out() << dat::infoString(metric, "metric") << std::endl;
		return metric;
	}

	//! Histogram of maximum disjoint prob values for numObs
	prob::Histogram
	metricHistFor
		( std::vector<double> const & locs
		, size_t const & numObs
		)
	{
		std::vector<double> pdfMaxProbs;

		Gaussian const gauss(0., sSigma);
		double const maxMetric{ (9./8.) * gauss(0.) };
		dat::Range<double> const maxMetRng{ 0., maxMetric };
		math::Partition const maxMetPart{ maxMetRng, sNumMetBins };
		prob::Histogram metHist(maxMetPart);
		for (size_t numTry{0u} ; numTry < sNumTrials ; ++numTry)
		{
			double const metric{ metricOneTrial(numObs, locs) };
			metHist.addSample(metric);
		}

		return metHist;
	}

}

//! Monte Carlo sim for disjunctive probability maxima in sums of Gaussians.
int
main
	( int const // argc
	, char const * const * const // argv
	)
{
	std::ofstream ofs("foo_probs.dat");
	std::vector<double> const locs{ sigLocs() };

	for (size_t numObs{1u} ; numObs <= sMaxNumObs ; ++numObs)
	{
		prob::Histogram const metHist{ metricHistFor(locs, numObs) };

		std::vector<double> const locs{ metHist.binCenters() };
		std::vector<double> const probs{ metHist.pdf() };
		assert(locs.size() == probs.size());
		for (size_t ndx{0u} ; ndx < locs.size() ; ++ndx)
		{
			double const & prob = probs[ndx];
			double const & loc = locs[ndx];
			ofs
				<< " " << io::sprintf("%9.6f", loc)
				<< " " << dat::infoString(numObs)
				<< " " << io::sprintf("%9.6f", prob)
				<< '\n';
		}
		ofs << "\n\n";
	}

	return 0;
}
