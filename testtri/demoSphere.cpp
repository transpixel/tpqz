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

	//! Example 
	struct Assessor
	{
		std::string const theName;
		math::Partition const theErrPart;
		std::vector<size_t> theErrCounts;
		size_t theErrSum;

		//! Construct named quality tracker
		Assessor
			( std::string const & name
			, math::Partition const & errPart
			)
			: theName(name)
			, theErrPart{ errPart }
			, theErrCounts(theErrPart.size() + 1u, 0u)
			, theErrSum{ 0u }
		{ }

		//! Incorporate observation error into quality assessment
		void
		addSample
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

		//! Probability of requested error
		double
		probabilityAt
			( double const & errValue //!< [0,1]
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
		( std::vector<Assessor> const & quals
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
		for (Assessor const & qual : quals)
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
				<< " ti '" << qual.theName << "'";
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
			for (Assessor const & qual : quals)
			{
				ofsdat << " " << dat::infoString(qual.probabilityAt(err));
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
	using PairZA = tri::Vec2D;

	// get (pseudo)random sampling directions
	std::vector<ga::Vector> const dirs{ randomDirs(32u) };

	// various test cases
	math::Partition const errPart(dat::Range<double>(-2., 2.), 256u);
	std::vector<Assessor> quals
	{
		  Assessor("cfg1", errPart)
/*
		, Assessor("cfg2", errPart)
		, Assessor("cfg3", errPart)
		, Assessor("cfg4", errPart)
		, Assessor("cfg5", errPart)
*/
	};

	//! Property sampling functor (for unit sphere)
	struct PropFunc
	{
		//! Define value type (radius of sphere)
		using value_type = PropType;

		//! Evaluate (radius) property - constant over sphere
		PropType
		operator()
			( size_t const & ndxI
			, size_t const & ndxJ
			) const
		{
io::out()
	<< "ndxI,J:"
	<< " " << dat::infoString(ndxI)
	<< " " << dat::infoString(ndxJ)
	<< std::endl;
//
// TODO - Investigate index computation/lookup in Triangle et.al.
//
io::out() << "TODO: --- fix libtri/IsoTile" << std::endl;
exit(8);

			return { 1. };
		}
	};
	PropFunc const unitSphere;

	// for each case
	for (Assessor & qual : quals)
	{
		double const da{ 1. };
		double const db{ 1. };
		PairZA const adir{ 1., 0. };
		tri::IsoTille const tin(da, db, adir);

		io::out() << dat::infoString(tin, "tin") << std::endl;

		// compute histogram of interpolation errors
		for (ga::Vector const & dir : dirs)
		{
			using namespace geo::sphere;
			PairZA const zaLoc{ zenithOf(dir), azimuthOf(dir) };
			PropType const gotRad{ tin(zaLoc, unitSphere) };

			if (dat::isValid(gotRad))
			{
				constexpr PropType expRad{ 1. };
				PropType const difRad{ gotRad - expRad };

io::out()
	<< "rad:exp,got,dif:"
	<< " " << dat::infoString(expRad)
	<< " " << dat::infoString(gotRad)
	<< " " << dat::infoString(difRad)
	<< std::endl;
/*
*/

				qual.addSample(difRad);
			}
		}

	}

	// write data to file for display
	saveForGnuplot(quals, errPart);

	return 0;
}
