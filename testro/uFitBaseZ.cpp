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
\brief  This file contains unit test for ro::FitBaseZ
*/


#include "libro/FitBaseZ.h"
#include "libro/sampcon.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libmath/math.h"
#include "libro/Accord.h"
#include "libro/QuadForm.h"
#include "libsys/Timer.h"

#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <string>


namespace
{
//#define TimeIt

//! Check for common functions
std::string
ro_FitBaseZ_test0
	()
{
	std::ostringstream oss;
	ro::FitBaseZ const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//
// Simulate test case
//
	using PairUV = ro::FitBaseZ::PairUV;
	using PtrPairUV = ro::FitBaseZ::PtrPairUV;

	//! Corresponding directions (generated from ideal case)
	std::vector<PairUV>
	uvPairsFor
		( ro::Pair const & ro
		)
	{
		std::vector<PairUV> uvs;

		// push directions away from baseline
		static ga::BiVector const anyBiv(.4, .5, .25);
		static ga::Pose const anyAtt(anyBiv);

		constexpr double dist{ 2. };
		uvs.emplace_back(ro.uvDirectionsFor( anyAtt( dist * ga::e1) ));
		uvs.emplace_back(ro.uvDirectionsFor( anyAtt(-dist * ga::e1) ));
		uvs.emplace_back(ro.uvDirectionsFor( anyAtt( dist * ga::e2) ));
		uvs.emplace_back(ro.uvDirectionsFor( anyAtt(-dist * ga::e2) ));
		uvs.emplace_back(ro.uvDirectionsFor( anyAtt( dist * ga::e3) ));

		return uvs;
	}

//! Check RO improvement (single step linear iteration)
std::string
ro_FitBaseZ_test1a
	()
{
	std::ostringstream oss;

	// simulate test configuration
	ga::Vector const base2w1( 0., 0., 1. );
	ga::BiVector const angle2w1( .0, .0, .0 );
	ro::PairBaseZ const roExp
		( ga::Rigid::identity()
		, ga::Rigid(base2w1, ga::Pose(angle2w1))
		);

	// create a perturbed initial estimate
	ga::Vector const baseDel( .01, .02, .00 );
	ga::BiVector const angleDel( .30, .20, .40 );
	ro::PairBaseZ const roSim
		( ga::Rigid::identity()
		, ga::Rigid(base2w1 + baseDel, ga::Pose(angle2w1 + angleDel))
		);

	// generate measurements (using ideal test data)
	std::vector<PairUV> const uvPairs{ uvPairsFor(roExp) };

	// construct fitter starting at perturbed value
	std::vector<PairUV> const & uvs = uvPairs;
	std::array<PtrPairUV, 5u> const uvPtrs
		{{ &(uvs[0]), &(uvs[1]), &(uvs[2]), &(uvs[3]), &(uvs[4]) }};
	ro::FitBaseZ const fitter(uvPtrs);

	ga::Rigid const ori2w1{  ga::e3, ga::Pose::identity() };
	ro::PairBaseZ const roNom{ ga::Rigid::identity(), ori2w1 };
	ro::Solution const roSoln{ fitter.roSolution(roNom) };

	if (! roSoln.isValid())
	{
		oss << "Failure of valid solution test" << std::endl;
	}
	else
	{
		ro::Accord const solnFit{ roSoln, &uvPairs };

		// check that *no* RMS is available - for min ray test case
		if ( dat::isValid(solnFit.rmsGapAll()))
		{
			oss << "Failure of (nan)rmsGap test for min uv" << std::endl;
		}

		// check full solution for zero gap
		for (size_t nn{0u} ; nn < uvPairs.size() ; ++nn)
		{
			double const expGap{ 0. };
			double const gotGap{ solnFit.gapForNdx(nn) };
			if (! dat::nearlyEquals(gotGap, expGap))
			{
				oss << "Failure of solution 1a gap test" << std::endl;
				oss << "gotGap: " << io::sprintf("%12.5e", gotGap) << std::endl;
			}
		}

		// check solution's RMS statistic - for more measurements
		std::vector<PairUV> morePairs(uvPairs);
		morePairs.insert(morePairs.end(), uvPairs.begin(), uvPairs.end());
		ro::Accord const overFit{ roSoln, &morePairs };
		double const expGapRMS{ 0. };
		double const gotGapRMS{ overFit.rmsGapAll() };
		if (! dat::nearlyEquals(gotGapRMS, expGapRMS))
		{
			oss << "Failure of rmsGapAll test" << std::endl;
			oss << dat::infoString(expGapRMS, "expGapRMS") << std::endl;
			oss << dat::infoString(gotGapRMS, "gotGapRMS") << std::endl;
		}

		// check residual analysis
		std::vector<PairUV> const uvFit{ uvPairsFor(roExp) };
		std::vector<PairUV> const uvRes{ uvPairsFor(roSim) };
		assert(5u == uvFit.size());
		assert(5u == uvRes.size());
		std::vector<PairUV> uvMany
			{ uvFit[0], uvRes[0] // 0,1
			, uvFit[1], uvRes[1] // 2,3
			, uvFit[2], uvRes[2] // 4,5
			, uvFit[3], uvRes[3] // 6,7
			, uvFit[4], uvRes[4] // 8,9
			};
		ro::FiveOf<size_t> const ndxFit{{ 0u, 2u, 4u, 6u, 8u }};
		ro::FiveOf<size_t> const ndxRes{{ 1u, 3u, 5u, 7u, 9u }};

		ro::Accord const manyFit{ roSoln, &uvMany };

		// check that gaps excluding noisy measurements are near zero
		double const gotGapFit{ manyFit.sumSqGapExcluding(ndxRes) };
		double const expGapFit{ 0. };
		if (! dat::nearlyEquals(gotGapFit, expGapFit))
		{
			oss << "Failure of gapFit=0 test" << std::endl;
		}

		// check that gaps excluding perfect fit measurements are non-zero
		double const gotGapRes{ manyFit.sumSqGapExcluding(ndxFit) };
		double const someResMag{ 1./256. }; // non-trival value for this test
		bool const bigResids{ (someResMag < gotGapRes) };
		if (! bigResids)
		{
			oss << "Failure of non-trivial gapRes test" << std::endl;
		}
	}

	return oss.str();
}

//! Check RO improvement (single step linear iteration)
std::string
ro_FitBaseZ_test1b
	()
{
	std::ostringstream oss;

	// simulate test configuration
	ga::Vector const base2w1( 0., 0., 1. );
	ga::BiVector const angle2w1( .0, .0, .0 );
	ro::PairBaseZ const roExp
		( ga::Rigid::identity()
		, ga::Rigid(base2w1, ga::Pose(angle2w1))
		);

	// create a perturbed initial estimate
	ga::Vector const baseDel( .01, .02, .00 );
	ga::BiVector const angleDel( .30, .20, .40 );
	ro::PairBaseZ const roSim
		( ga::Rigid::identity()
		, ga::Rigid(base2w1 + baseDel, ga::Pose(angle2w1 + angleDel))
		);

	// generate measurements (using ideal test data)
	std::vector<PairUV> const uvs{ uvPairsFor(roExp) };

	// construct fitter starting at perturbed value
	std::array<PtrPairUV, 5u> const uvPtrs
		{{ &(uvs[0]), &(uvs[1]), &(uvs[2]), &(uvs[3]), &(uvs[4]) }};
	ro::FitBaseZ const fitter(uvPtrs);

	// perform a single iteration
	ro::PairBaseZ const roFit{ fitter.improvedNear(roSim) };

	double rmsGap{ fitter.rmsGapFor(roSim) };
	// io::out() << dat::infoString(rmsGap, "rmsGap.0") << std::endl;

	// check rms error before/after the a single linear step
	double const rmsInit{ fitter.rmsGapFor(roSim) };
	double const rmsNext{ fitter.rmsGapFor(roFit) };
	if (! (rmsNext < rmsInit))
	{
		oss << "Failure of one step improvement test" << std::endl;
		oss << dat::infoString(rmsInit, "rmsInit") << std::endl;
		oss << dat::infoString(rmsNext, "rmsNext") << std::endl;
	}

	// continue iterating until convergence
	ro::PairBaseZ roCurr{ roFit };
	constexpr size_t maxIt{ 25u };
	size_t numIt{ 0u };
	rmsGap = fitter.rmsGapFor(roCurr);
	// io::out() << dat::infoString(rmsGap, "rmsGap.1") << std::endl;
	while ((math::eps < rmsGap) && (numIt++ < maxIt))
	{
		roCurr = fitter.improvedNear(roCurr);
		rmsGap = fitter.rmsGapFor(roCurr);
		// io::out() << "rmsGap.it: " << io::sprintf("%21.18f", rmsGap) << '\n';
	}

	// check that loop converged
	if (! (numIt < maxIt))
	{
		oss << "Failure of iteration convergence" << std::endl;
		oss << dat::infoString(numIt, "numIt") << std::endl;
		oss << dat::infoString(maxIt, "maxIt") << std::endl;
	}
	else
	{
		// check if computed solution satisifes coplanarity condition
		ro::PairBaseZ const roGot = roCurr;
		for (PairUV const & uv : uvs)
		{
			double const gotGap{ roGot.tripleProductGap(uv) };
			if (! dat::nearlyEquals(gotGap, 0.))
			{
				oss << "Failure of improvedNear gap test" << std::endl;
				oss << "gotGap: " << io::sprintf("%12.5e", gotGap) << std::endl;
				oss << dat::infoString(roExp, "roExp") << std::endl;
				oss << dat::infoString(roGot, "roGot") << std::endl;
				break;
			}
		}
	}

	// Check full solution (internal iterations
	ro::Solution const roSoln{ fitter.roSolution(roSim) };

	//io::out() << dat::infoString(roSoln, "roSoln") << std::endl;

	ro::Accord const eval{ roSoln, &uvs };
	double const gotGapSq{ eval.sumSqGapAll() };
	if (! dat::nearlyEquals(gotGapSq, 0.))
	{
		oss << "Failure of solution 1b gap test" << std::endl;
		oss << "gotGapSq: " << io::sprintf("%12.5e", gotGapSq) << std::endl;
	}

	return oss.str();
}


static std::default_random_engine engine{};
//
inline
double
randValue
	( double const & maxMag
	)
{
	std::uniform_real_distribution<> distro(-maxMag, maxMag);
	return distro(engine);
}
//
inline
ga::Vector
randPoint
	( double const & maxMag
	)
{
	return ga::Vector(randValue(maxMag), randValue(maxMag), randValue(maxMag));
}

	//! Corresponding directions for ro
	std::vector<PairUV>
	simUVs
		( ro::Pair const & ro
		, size_t const & numMea
		)
	{
		std::vector<PairUV> uvs;
		uvs.reserve(numMea);
		for (size_t nn{0u} ; nn < numMea ; ++nn)
		{
			constexpr double mag{ 5. };
			uvs.emplace_back(ro.uvDirectionsFor(randPoint(mag)));
		}
		return uvs;
	}


//! Check RO sample consensus fitting
std::string
ro_FitBaseZ_test2
	()
{
	std::ostringstream oss;

	// define test configuration
	ga::Vector const base2w1{ ga::unit(ga::Vector( 0.1, -.2, .75)) };
	ga::BiVector const angle2w1( -.03, .02, .05);
	ro::PairBaseZ const roExp
		( ga::Rigid::identity()
		, ga::Rigid(base2w1, ga::Pose(angle2w1))
		);

	ro::OriPair const roNom
		{ ga::Rigid::identity()
		, ga::Rigid(ga::e3, ga::Pose::identity())
		};

	// create a "large" (relative to combinatorial) number of measurements
	std::vector<PairUV> const uvs{ simUVs(roExp, 9u) };

	sys::Timer timer;

	// get sample-consensus solution - by brute force combos
	{
		timer.start("consenusByCombo");
		ro::QuintSoln const roQuintSoln
			{ ro::sampcon::byCombo(uvs, roNom) };
		ro::PairBaseZ const roGot(roQuintSoln.theSoln.pair());
		timer.stop();
		if (! roGot.nearlyEquals(roExp))
		{
			oss << "Failure of byCombo() test" << std::endl;
			oss << roExp.infoString("roExp") << std::endl;
			oss << roGot.infoString("roGot") << std::endl;
		}
	}

	// get sample-consensus solution - by (pseudo) random sampling
	{
		timer.start("consenusBySamples");
		ro::QuintSoln const roQuintSoln
			{ ro::sampcon::bySample(uvs, roNom) };
		ro::PairBaseZ const roGot(roQuintSoln.theSoln.pair());
		timer.stop();
		if (! roGot.nearlyEquals(roExp))
		{
			oss << "Failure of bySample() test" << std::endl;
			oss << roExp.infoString("roExp") << std::endl;
			oss << roGot.infoString("roGot") << std::endl;
		}
	}

#	if defined(TimeIt)
	io::out() << "========================" << std::endl;
	{
		// larger sample for timing tests
		timer.start("consenusBySamples.1k");
		std::vector<PairUV> const uvPairs{ simUVs(roExp, 1024u) };
		(void)ro::sampcon::bySample(uvPairs, roNom);
		timer.stop();

		timer.start("ro.tripleProductGap.1M");
		double sumGaps{ 0. };
		constexpr size_t numGaps{ 1000u * 1000u };
		for (size_t nn{0u} ; nn < numGaps ; ++nn)
		{
			size_t const ndx{ nn % uvPairs.size() };
			PairUV const & uvPair = uvPairs[ndx];
			sumGaps += roExp.tripleProductGap(uvPair);
		}
		timer.stop();
		io::out() << dat::infoString(sumGaps, "sumGaps") << std::endl;

		timer.start("quad.tripleProductGap.1M");
		sumGaps = 0.;
		ro::QuadForm const quad(roExp);
		for (size_t nn{0u} ; nn < numGaps ; ++nn)
		{
			size_t const ndx{ nn % uvPairs.size() };
			PairUV const & uvPair = uvPairs[ndx];
			sumGaps += quad.tripleProductGap(uvPair);
		}
		timer.stop();
		io::out() << dat::infoString(sumGaps, "sumGaps") << std::endl;
	}
	io::out() << "========================" << std::endl;
#	endif

#	if defined(TimeIt)
	io::out() << dat::infoString(timer) << std::endl;
#	endif

	return oss.str();
}


}

//! Unit test for ro::FitBaseZ
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ro_FitBaseZ_test0();
	oss << ro_FitBaseZ_test1a();
	oss << ro_FitBaseZ_test1b();
	oss << ro_FitBaseZ_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
