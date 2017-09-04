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
#include "libgeo/sphere.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libmath/interp.h"
#include "libmath/Partition.h"
#include "libprob/Histogram.h"

#include <cassert>
#include <fstream>
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
			double const difValue{ gotValue - expValue };
			if (dat::isValid(difValue))
			{
				theHist.addSample(difValue);
			}
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
		ofsplt << "plot";
		size_t col{2u};
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
				<< " ti '" << testcase.theName << "'";
			++col;
		}
		ofsplt << '\n';

		// get probs from each testcase
		using PDF = std::vector<double>;
		size_t const numTrackers{ testcases.size() };
		std::vector<PDF> pdfs(testcases.size());
		for (size_t nn{0u} ; nn < numTrackers ; ++nn)
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
			for (size_t nn{0u} ; nn < numTrackers ; ++nn)
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
	using PropType = double;
	using PairZA = dat::Spot;

//	constexpr size_t numDirs{ 32u * 1024u };
constexpr size_t numDirs{ 256u };

	//! Property sampling functor (for unit sphere)
	struct PropertyStore
	{
		//! Define value type (radius of sphere)
		using value_type = PropType;

		//! Evaluate (radius) property - constant over sphere
		value_type
		operator()
			( size_t const & // ndxI
			, size_t const & // ndxJ
			) const
		{
			// For this test example, generate properties "on the fly"
			// without concern for an actual storage structure for node data
			return { 1. };
		}
	};

	// get (pseudo)random sampling directions
	std::vector<ga::Vector> const dirs{ randomDirs(numDirs) };

	// various test cases
	math::Partition const errPart(dat::Range<double>(-2., 2.), 1024u);
	PairZA const adir{ 1., 0. }; // primary tesselation axis
	std::vector<TestCase> testcases
		{ { TestCase("cfg1", errPart, 1.   , 1.   , adir) }
		, { TestCase("cfg2", errPart,  .1  ,  .1  , adir) }
		, { TestCase("cfg3", errPart,  .001,  .001, adir) }
		};

std::ofstream ofsFoo("foo.dat");
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

		io::out() << dat::infoString(trinet, "trinet") << std::endl;

		// compute histogram of interpolation errors
		for (ga::Vector const & dir : dirs)
		{
			PropertyStore const radiusPropertyStore;
			using namespace geo::sphere;
			PairZA const zaLoc{ zenithOf(dir), azimuthOf(dir) };
			PropType const gotRad
				{ trinet.linearInterpWithCheck(zaLoc, radiusPropertyStore) };
			constexpr PropType expRad{ 1. };
			testcase.recordResult(gotRad, expRad);

ofsFoo
	<< "z,a,exp,got,dif:"
	<< " " << dat::infoString(zaLoc)
	<< " " << io::sprintf("%12.6f", expRad)
	<< " " << io::sprintf("%12.6f", gotRad)
	<< " " << io::sprintf("%12.6f", (gotRad-expRad))
	<< '\n';

		}

	}

	// write data to file for display
	saveForGnuplot(testcases, errPart);

	return 0;
}
