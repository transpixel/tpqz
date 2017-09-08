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
\brief  This file contains main application program demoSphere
*/


#include "libtri/IsoTille.h"

#include "libdat/info.h"
#include "libga/ga.h"
#include "libgeo/sphere.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libmath/interp.h"
#include "libmath/Partition.h"
#include "libprob/Histogram.h"
#include "libprob/Stats.h"

#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>


namespace
{
	//! A pseudo-random direction
	ga::Vector
	randomDir
		()
	{
		ga::Vector dir;
		static std::mt19937_64 rgen;
		static std::uniform_real_distribution<double> rdistro(-1., 1.);
		ga::Vector vec;
		do
		{
			vec[0] = rdistro(rgen);
			vec[1] = rdistro(rgen);
			vec[2] = rdistro(rgen);
		}
		while (1. < ga::magnitude(vec));
		dir = ga::unit(vec);
		assert(dat::isValid(dir));
		return dir;
	}

	//! Collection of pseudo-random directions
	std::vector<ga::Vector>
	randomDirs
		( size_t const & numDirs = { 64u * 1024u }
		)
	{
		std::vector<ga::Vector> dirs;
		dirs.reserve(numDirs);
		while (dirs.size() < numDirs)
		{
			dirs.emplace_back(randomDir());
		}
		return dirs;
	}

	//! Record error values quantized into bins
	struct TestCase
	{
		std::string const theName;
		tri::IsoGeo const theTileGeo;
		prob::Histogram theHist;
		prob::Stats theStats;

		//! Construct named testcase
		TestCase
			( std::string const & name
			, math::Partition const & errPart
			, double const & da
			, double const & db
			, std::array<double, 2u> const & adir
			)
			: theName(name)
			, theTileGeo(da, db, adir)
			, theHist(errPart)
		{ }

		//! Incorporate observation into collection
		void
		recordResult
			( double const & gotValue
			, double const & expValue
			)
		{
			double const errValue{ gotValue - expValue };
			if (dat::isValid(errValue))
			{
				double const magValue{ std::abs(errValue) };
				theHist.addSample(magValue);
				theStats.add(magValue);
			}
		}

		//! Descriptive information about this instance.
		std::string
		infoString
			( std::string const & title = std::string()
			) const
		{
			std::ostringstream oss;
			if (! title.empty())
			{
				oss << title << std::endl;
			}
			{
				oss << dat::infoString(theName, "theName");

				oss << std::endl;
				oss << dat::infoString(theTileGeo, "theTileGeo");

				oss << std::endl;
				oss << dat::infoString(theHist, "theHist");

				oss << std::endl;
				oss << dat::infoString(theStats, "theStats");
			}
			return oss.str();
		}

	};

	// write gnuplot data
	void
	saveForGnuplot
		( std::vector<TestCase> const & testcases
		, math::Partition const & errPart
		)
	{
		std::string datname("errSphere.dat");
		std::string pltname("errSphere.plt");
		std::ofstream ofsdat(datname);
		std::ofstream ofsplt(pltname);

		// .plt file
		ofsplt << "set key top left" << '\n';
		ofsplt << "plot [-1./4.:1./128.]";
		size_t col{2u};
		std::array<int, 2u> colors = { 1, 3 };
		size_t ndxColor{ 0u };
		for (TestCase const & testcase : testcases)
		{
			if (2u == col)
			{
				ofsplt << " ";
			}
			else
			{
				ofsplt << ",";
			}
			ofsplt
				<< "'" << datname << "' u 1:" << col << " w l"
				<< " lc " << colors[ndxColor]
				<< " ti '" << testcase.theName << "'";
			++ndxColor;
			ndxColor = (ndxColor % colors.size());
			++col;
		}
		ofsplt << ';' << std::endl;
	//	ofsplt << "pause -1;" << std::endl;

		// get probs from each testcase
		using PDF = std::vector<double>;
		size_t const numCases{ testcases.size() };
		std::vector<PDF> pdfs(numCases);
		for (size_t nn{0u} ; nn < numCases ; ++nn)
		{
			pdfs[nn] = testcases[nn].theHist.pdf();
		}

		// .dat file
		double const emin{ errPart.min() };
		double const emax{ errPart.max() };
		double const delta{ 1./double(errPart.size()) };
		for (double errVal{emin} ; errVal < emax ; errVal += delta)
		{
			double const subIndex{ errPart.interpIndexFor(errVal) };

			ofsdat << dat::infoString(errVal);
			for (size_t nn{0u} ; nn < numCases ; ++nn)
			{
				double const eProb
					{ math::interp::linear<double>(subIndex, pdfs[nn]) };
				ofsdat << " " << io::sprintf("%12.9f", eProb);
			}
			ofsdat << '\n';
		}
		ofsplt << std::endl;
		ofsdat << std::endl;
	}

}

//! Demonstrate iso-tritille tessellation of a unit sphere
int
main
	()
{
	using PropType = ga::Vector;
	using PairZA = dat::Spot;

	constexpr size_t numDirs{  128u * 1024u };
	constexpr size_t numHist{    4u * 1024u };

	//! Property sampling functor (for unit sphere)
	struct PropertyStore
	{
		tri::IsoGeo theTileGeo;

		//! Define value type (radius of sphere)
		using value_type = PropType;

		//! Evaluate (radius) property - constant over sphere
		value_type
		operator()
			( size_t const & ndxI
			, size_t const & ndxJ
			) const
		{
			tri::NodeNdxPair const ndxIJ{ ndxI, ndxJ };

			// For this test example, generate properties "on the fly"
			// without concern for an actual storage structure for node data
			dat::Spot const zaSpot
				( theTileGeo.refSpotForFracPair
					( theTileGeo.fracPairForIndices(ndxIJ) )
				);
			double const & zenith = zaSpot[0];
			double const & azimuth = zaSpot[1];
			ga::Vector const dir
				{ geo::sphere::directionFromAZ(azimuth, zenith) };

			return { dir };
		}
	};

	// get (pseudo)random sampling directions
	std::vector<ga::Vector> const dirs{ randomDirs(numDirs) };

	// various test cases
	math::Partition const errPart(dat::Range<double>(-2., 2.), numHist);
	PairZA const adir{ 1., 0. }; // primary tesselation axis
	/*
	constexpr double f04{ math::qtrPi };
	constexpr double f08{ .5 * f04 };
	constexpr double f16{ .5 * f08 };
	constexpr double f32{ .5 * f16 };
	constexpr double f64{ .5 * f32 };
	constexpr double f128{ .5 * f64 };
	std::vector<TestCase> testcases
		{ { TestCase("cfg:_2.", errPart, 2.   , 2.   , adir) }
		, { TestCase("cfg:f04", errPart, f04  , f04  , adir) }
		, { TestCase("cfg:f08", errPart, f08  , f08  , adir) }
		, { TestCase("cfg:f16", errPart, f16  , f16  , adir) }
		, { TestCase("cfg:f32", errPart, f32  , f32  , adir) }
		, { TestCase("cfg:f64", errPart, f64  , f64  , adir) }
		, { TestCase("cfg:f128", errPart, f128  , f128  , adir) }
		};
	*/
	std::vector<TestCase> testcases;
	double frac{ 1. / 2. };
	for (size_t nn{0u} ; nn < 32u ; ++nn)
	{
		frac *= .75;
		double const zaDelta{ frac * math::pi };
		std::string const name{ io::sprintf("pi/%06d", nn) };
		TestCase const testcase(name, errPart, zaDelta  , zaDelta  , adir);
		testcases.emplace_back(testcase);
	}

	// for each case
	for (TestCase & testcase : testcases)
	{
		// check boundaries
		dat::Range<double> const zenithRange{ 0., math::pi };
		dat::Range<double> const azimuthRange{ -math::pi, math::pi };
		dat::Area<double> const zaArea{ zenithRange, azimuthRange };
		tri::Domain const zaDomain{ zaArea };

		// define iso-tritille
		tri::IsoGeo const & trigeo = testcase.theTileGeo;
		tri::IsoTille const trinet(trigeo, zaDomain);

		// "store" of property values at tritille nodes (generated on-the-fly)
		PropertyStore const radiusPropertyStore{ trigeo };

		// compute histogram of interpolation errors
		for (ga::Vector const & dir : dirs)
		{
			using namespace geo::sphere;
			PairZA const zaLoc{ zenithOf(dir), azimuthOf(dir) };
			PropType const gotVec
				{ trinet.linearInterpWithCheck(zaLoc, radiusPropertyStore) };
			PropType const & expVec = dir;
			testcase.recordResult
				( ga::magnitude(gotVec)
				, ga::magnitude(expVec)
				);
		}

		double const dab{ testcase.theTileGeo.delta() };
		double const dabSq{ math::sq(dab) };
		double const apxNumNodes{ zaArea.magnitude() / dabSq };

		std::string const fmt{ "%12.9f" };
		io::out()
			<< " "
		//	<< "case:"
		//	<< " " << std::setw(15u) << testcase.theName
			<< "apxTiles:"
			<< " " << io::sprintf("%12.0f.", apxNumNodes)
			<< " "
			<< "dab:"
			<< " " << io::sprintf(fmt, dab)
			<< " "
			<< "dabSq:"
			<< " " << io::sprintf(fmt, dabSq)
			<< " "
			<< "min,max,ave,med:"
			<< " " << io::sprintf(fmt, testcase.theStats.theMinMax.min())
			<< " " << io::sprintf(fmt, testcase.theStats.theMinMax.max())
			<< " " << io::sprintf(fmt, testcase.theStats.mean())
			<< " " << io::sprintf(fmt, testcase.theStats.medianValue())
			<< std::endl;
	}

	constexpr bool savePlotData{ false };
	if (savePlotData)
	{
		// write data to file for display
		saveForGnuplot(testcases, errPart);
	}

	return 0;
}
