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


// #include "libdat/info.h"
#include "libio/stream.h"

#include "libdat/grid.h"
#include "libgeo/sphere.h"
#include "libmath/interp.h"
#include "libmath/Partition.h"
#include "libprob/pdf.h"
#include "libtri/IsoTille.h"

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
	struct SampleTracker
	{
		std::string const theName;
		math::Partition const theErrPart;
		std::vector<size_t> theErrCounts;
		size_t theErrSum;

		//! Construct named tracker
		SampleTracker
			( std::string const & name
			, math::Partition const & errPart
			)
			: theName(name)
			, theErrPart{ errPart }
			, theErrCounts(theErrPart.size() + 1u, 0u)
			, theErrSum{ 0u }
		{ }

		//! Incorporate observation error into error collection
		void
		recordError
			( double const & err
			)
		{
			if (dat::isValid(err))
			{
				size_t const bin{ theErrPart.binIndexFor(err) };
				if (dat::isValid(bin))
				{
					theErrCounts[bin] += 1.;
					theErrSum += 1.;
				}
				else
				{
					io::err() << "out of range bin: " << bin << std::endl;
				}
			}
		}

		//! Probability of error of (near to) specified value
		double
		probabilityOf
			( double const & errValue
			) const
		{
			double prob{ dat::nullValue<double>() };
			assert(theErrPart.range().contains(errValue));
			if (dat::isValid(errValue))
			{
				double const subIndex{ theErrPart.interpIndexFor(errValue) };
				double const count
					{ math::interp::linear<double>(subIndex, theErrCounts) };

				if (dat::isValid(count))
				{
					prob = count / theErrSum;
				}
				else
				{
					io::err() << "badCount:evalue,subIndex:"
						<< " " << errValue
						<< " " << subIndex
						<< std::endl;
				}
			}
			return prob;
		}
	};

	// write gnuplot data
	void
	saveForGnuplot
		( std::vector<SampleTracker> const & errPots
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
		for (SampleTracker const & errPot : errPots)
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
				<< " ti '" << errPot.theName << "'";
			++col;
		}
		ofsplt << '\n';

		// .dat file
		double const emin{ errPart.min() };
		double const emax{ errPart.max() };
		double const delta{ 1./double(errPart.size()) };
		for (double err{emin} ; err < emax ; err += delta)
		{
			ofsdat << dat::infoString(err);
			for (SampleTracker const & errPot : errPots)
			{
				ofsdat << " " << dat::infoString(errPot.probabilityOf(err));
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

	//! Property sampling functor (for unit sphere)
	struct PropertyStore
	{
		//! Define value type (radius of sphere)
		using value_type = PropType;

		//! Evaluate (radius) property - constant over sphere
		PropType
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
	std::vector<ga::Vector> const dirs{ randomDirs(32u) };

	// various test cases
	math::Partition const errPart(dat::Range<double>(-2., 2.), 256u);
	std::vector<SampleTracker> errPots
	{
		  SampleTracker("cfg1", errPart)
	//	, SampleTracker("cfg2", errPart)
	//	, SampleTracker("cfg3", errPart)
	//	, SampleTracker("cfg4", errPart)
	//	, SampleTracker("cfg5", errPart)
	};

	PropertyStore const radiusPropertyStore;

	// for each case
	for (SampleTracker & errPot : errPots)
	{
		// check boundaries
		dat::Range<double> const zenithRange{ 0., math::pi };
		dat::Range<double> const azimuthRange{ 0., math::twoPi };
		dat::Area<double> const zaArea{ zenithRange, azimuthRange };
		tri::Domain const zaDomain{ zaArea };

		// define iso-tritille
		double const da{ 1. }; // azimuth tritille spacing
		double const db{ 1. }; // zenith tritille spacing
		PairZA const adir{ 1., 0. }; // primary tesselation axis
		tri::IsoGeo const trigeo(da, db, adir);
		tri::IsoTille const trinet(trigeo, zaDomain);

/*
dat::Area<double> const mnArea{ trigeo.tileAreaForRefArea(zaDomain) };
io::out() << std::endl;
io::out() << dat::infoString(zaArea, "zaArea") << std::endl;
io::out() << dat::infoString(mnArea, "mnArea") << std::endl;
io::out() << std::endl;
*/

		io::out() << dat::infoString(trinet, "trinet") << std::endl;

		// compute histogram of interpolation errors
		for (ga::Vector const & dir : dirs)
		{
			using namespace geo::sphere;
			PairZA const zaLoc{ zenithOf(dir), azimuthOf(dir) };
			PropType const gotRad
				{ trinet.linearInterpWithCheck(zaLoc, radiusPropertyStore) };

			if (dat::isValid(gotRad))
			{
				constexpr PropType expRad{ 1. };
				PropType const difRad{ gotRad - expRad };

/*
io::out() << std::endl;
io::out()
	<< "za:"
	<< " " << dat::infoString(zaLoc)
	<< "   " << "rad:exp,got,dif:"
	<< " " << dat::infoString(expRad)
	<< " " << dat::infoString(gotRad)
	<< " " << dat::infoString(difRad)
	<< std::endl;
*/

				errPot.recordError(difRad);
			}
		}

	}

	// write data to file for display
	saveForGnuplot(errPots, errPart);

	return 0;
}
