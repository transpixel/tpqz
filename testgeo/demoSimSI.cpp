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
\brief  This file contains demonstration program demoSimSI
*/


#include "libgeo/si.h"

#include "libdat/info.h"
#include "libio/stream.h"

#include <fstream>
#include <iostream>


namespace
{

namespace
{
	struct TestCase
	{
		ga::Vector const theSta1;
		ga::Vector const theSta2;
		ga::Vector const theExpPnt;
		ga::Vector const theMidPnt;
		double const theRange;
		double const theSigSta{ 1./16. };
		double const theSigDir{ 1./32. };

		explicit
		TestCase
			( ga::Vector const & sta1
			, ga::Vector const & sta2
			, ga::Vector const & expPnt
			, double const & sigSta
			, double const & sigDir
			)
			: theSta1{ sta1 }
			, theSta2{ sta2 }
			, theExpPnt{ expPnt }
			, theMidPnt{ .5 * (theSta2 + theSta1) }
			, theRange{ ga::magnitude(theExpPnt - theMidPnt) }
			, theSigSta{ sigSta }
			, theSigDir{ sigDir }
		{ }

		geo::si::PointSoln
		pointSolnFor
			( ga::Vector const & sta1
			, ga::Vector const & sta2
			, ga::Vector const & dir1
			, ga::Vector const & dir2
			) const
		{
			geo::Ray const ray1(sta1, dir1);
			geo::Ray const ray2(sta2, dir2);
			geo::si::Obs<geo::Ray> const obsRay1{ ray1, theSigDir, theSigSta };
			geo::si::Obs<geo::Ray> const obsRay2{ ray2, theSigDir, theSigSta };

			// collect observations of same classifications
			std::vector<geo::si::WRay> const wrays
				{ { obsRay1.obsWeight(theRange), ray1 }
				, { obsRay2.obsWeight(theRange), ray2 }
				};

			// compute intersection of collections
			geo::si::PointSystem system{};
			system.addWeightedRays(wrays);

			// io::out() << "\n\n";
			// io::out() << dat::infoString(system, "system") << std::endl;

			geo::si::PointSoln const pntSoln{ system.pointSolution() };
			return pntSoln;
		}

	}; // TestCase

	//! Collection of 3d perturbations
	std::vector<ga::Vector>
	pointDeltas
		( double const & mag
		)
	{
		std::vector<ga::Vector> const deltas
			{  ga::vZero
			, -mag * ga::e1
			,  mag * ga::e1
			, -mag * ga::e2
			,  mag * ga::e2
			, -mag * ga::e3
			,  mag * ga::e3
			};
		return deltas;
	}

	//! Collection of perturbations about dir0
	std::vector<ga::Vector>
	perpDeltas
		( ga::Vector const & dir0
		, double const & mag
		)
	{
		ga::Vector const perp1{ ga::anyPerpendicularDir(dir0) };
		ga::Vector const perp2{ (-ga::E123 * ga::wedge(dir0, perp1)) };
		std::vector<ga::Vector> const deltas
			{  ga::vZero
			, -mag * perp1
			,  mag * perp1
			, -mag * perp2
			,  mag * perp2
			};
		return deltas;
	}

	//! Pairs of combinations of deltas
	std::vector<std::pair<ga::Vector, ga::Vector> >
	deltaPairs
		( std::vector<ga::Vector> const & delta1s
		, std::vector<ga::Vector> const & delta2s
		)
	{
		std::vector<std::pair<ga::Vector, ga::Vector> > staPairs;
		for (ga::Vector const & delta1 : delta1s)
		{
			for (ga::Vector const & delta2 : delta2s)
			{
				staPairs.emplace_back(std::make_pair(delta1, delta2));
			}
		}

		return staPairs;
	}

	//! All pairs of direction perturbation combinations
	std::vector<std::pair<ga::Vector, ga::Vector> >
	staDeltaPairs
		( double const & mag1
		, double const & mag2
		)
	{
		std::vector<ga::Vector> const delta1s{ pointDeltas(mag1) };
		std::vector<ga::Vector> const delta2s{ pointDeltas(mag2) };
		return deltaPairs(delta1s, delta2s);
	}

	//! All pairs of direction perturbation combinations
	std::vector<std::pair<ga::Vector, ga::Vector> >
	dirDeltaPairs
		( ga::Vector const & dir1
		, double const & mag1
		, ga::Vector const & dir2
		, double const & mag2
		)
	{
		std::vector<ga::Vector> const delta1s{ perpDeltas(dir1, mag1) };
		std::vector<ga::Vector> const delta2s{ perpDeltas(dir2, mag2) };
		return deltaPairs(delta1s, delta2s);
	}

	//! Write point coordiantes to output file
	void
	savePoints
		( std::vector<ga::Vector> const & pnts
		, std::string const & fpath
		)
	{
		std::ofstream ofs(fpath);
		for (ga::Vector const & pnt : pnts)
		{
			ofs << dat::infoString(pnt) << '\n';
		}
	}

	//! Trace outline of ellipsoid from tips
	std::vector<ga::Vector>
	polyLineFrom
		( std::vector<ga::Vector> const & tips
		)
	{
		std::vector<ga::Vector> poly;
		poly.reserve(4 * tips.size());
		ga::Vector const & an = tips[0];
		ga::Vector const & ap = tips[1];
		ga::Vector const & bn = tips[2];
		ga::Vector const & bp = tips[3];
		ga::Vector const & cn = tips[4];
		ga::Vector const & cp = tips[5];
		poly.emplace_back(ap);
		poly.emplace_back(bp);
		poly.emplace_back(an);
		poly.emplace_back(bn);
		poly.emplace_back(ap); // close
		poly.emplace_back(cp);
		poly.emplace_back(an);
		poly.emplace_back(cn);
		poly.emplace_back(ap); // close
		poly.emplace_back(cn); // backtrack
		poly.emplace_back(bn);
		poly.emplace_back(cp);
		poly.emplace_back(bp);
		poly.emplace_back(cn); // close
		return poly;
	}
}

//! Check uncertainty estimate vs ray perturbations
std::string
geo_si_demo
	( std::string const & pathGotLocs
	, std::string const & pathGotTips
	, std::string const & pathExpStas
	, std::string const & pathExpLocs
	)
{
	std::ostringstream oss;

	// test case configuration
	ga::Vector const expSta1{ -.5, .0,  0. };
	ga::Vector const expSta2{  .5, .0,  0. };
	ga::Vector const expPnt{ 0., 10., 0. };
	double const sigSta{ 1./16. };
	double const sigDir{ 1./32. };
	TestCase const tcase(expSta1, expSta2, expPnt, sigSta, sigDir);

	// principal ray-ray intersection
	ga::Vector const expDir1{ ga::unit(expPnt - expSta1) };
	ga::Vector const expDir2{ ga::unit(expPnt - expSta2) };
	geo::si::PointSoln const expPntSoln
		{ tcase.pointSolnFor(expSta1, expSta2, expDir1, expDir2) };

	io::out() << dat::infoString(expPntSoln, "expPntSoln") << std::endl;

	// perturb ray directions and stations
	std::vector<std::pair<ga::Vector, ga::Vector> > const delDirPairs
		{ dirDeltaPairs( expDir1, tcase.theSigDir, expDir2, tcase.theSigDir) };
	std::vector<std::pair<ga::Vector, ga::Vector> > const delStaPairs
		{ staDeltaPairs(tcase.theSigSta, tcase.theSigSta) };

	// compute locations for all combinations of perturbations
	std::vector<ga::Vector> gotLocs;
	gotLocs.reserve(delDirPairs.size() * delStaPairs.size());
	for (std::pair<ga::Vector, ga::Vector> const & delDirPair : delDirPairs)
	{
		for (std::pair<ga::Vector, ga::Vector> const & delStaPair : delStaPairs)
		{
			ga::Vector const dir1{ expDir1 + delDirPair.first };
			ga::Vector const dir2{ expDir2 + delDirPair.second };
			ga::Vector const sta1{ expSta1 + delStaPair.first };
			ga::Vector const sta2{ expSta2 + delStaPair.second };
			geo::si::PointSoln const pntSoln
				{ tcase.pointSolnFor(sta1, sta2, dir1, dir2) };
			gotLocs.emplace_back(pntSoln.theLoc);
		}
	}
	io::out() << dat::infoString(gotLocs.size(), "gotLocs.size()") << '\n';

	constexpr bool saveToFile{ true };
	if (saveToFile)
	{
		std::vector<ga::Vector> const expStas{ expSta1, expSta2 };
		std::vector<ga::Vector> const expLocs{ expPnt };
		std::vector<ga::Vector> const gotTips
			{ polyLineFrom(expPntSoln.ellipsoidTips()) };
		savePoints(gotLocs, pathGotLocs);
		savePoints(gotTips, pathGotTips);
		savePoints(expStas, pathExpStas);
		savePoints(expLocs, pathExpLocs);
	}

	return oss.str();
}

}

//! Unit test for geo::si
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	std::string const pathGotLocs("usi_gotLocs.dat");
	std::string const pathGotTips("usi_gotTips.dat");
	std::string const pathExpStaS("usi_expStas.dat");
	std::string const pathExpLocs("usi_expLocs.dat");

	// run tests
	oss << geo_si_demo(pathGotLocs, pathGotTips, pathExpStaS, pathExpLocs);

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
