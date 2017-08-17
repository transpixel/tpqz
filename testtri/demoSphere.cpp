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
		size_t const theSize;
		std::vector<size_t> theErrCounts;
		size_t theErrSum;
		math::Partition const theErrPart;

		//! Construct named quality tracker
		Assessor
			( std::string const & name
			)
			: theName(name)
			, theSize{ 256u }
			, theErrCounts(theSize, 0u)
			, theErrSum{ 0u }
			, theErrPart(0., 1./(double(theSize)), theSize)
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
io::out() << "err: " << err << std::endl;
io::out() << "bin: " << bin << std::endl;
				assert(bin < theErrCounts.size());
				theErrCounts[bin] += 1.;
				theErrSum += 1.;
			}
		}

		//! Probability of requested error
		double
		probabilityAt
			( double const & errValue //!< [0,1]
			) const
		{
			double prob{ dat::nullValue<double>() };
			assert(0. <= errValue);
			assert(errValue <= 1.);
			double const subIndex{ errValue * double(theErrPart.size()) };
			double const count
				{ math::interp::linear<double>(subIndex, theErrCounts) };
			if (dat::isValid(count))
			{
				prob = count / theErrSum;
			}
			return prob;
		}
	};

	// write gnuplot data
	void
	saveForGnuplot
		( std::vector<Assessor> const & quals
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
		for (double err{0.} ; err <= 1. ; err += 1./256.)
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
	std::vector<Assessor> quals
	{
		  Assessor("cfg1")
		, Assessor("cfg2")
		, Assessor("cfg3")
		, Assessor("cfg4")
		, Assessor("cfg5")
	};

//	using PropGrid = dat::grid<PropType>;
//	PropGrid const radGrid;
	struct PropFunc
	{
		using value_type = PropType;

		PropType
		operator()
			( size_t const & // ndxI
			, size_t const & // ndxJ
			) const
		{
			return {};
		}
	};

	PropFunc const radGrid;

	// for each case
	for (Assessor & qual : quals)
	{
		double const da{ 1. };
		double const db{ 1. };
		PairZA const adir{ 1., 0. };
		tri::IsoTille const tin(da, db, adir);

		// compute histogram of interpolation errors
		for (ga::Vector const & dir : dirs)
		{
			using namespace geo::sphere;
			PairZA const zaLoc{ zenithOf(dir), azimuthOf(dir) };
			PropType const gotRad{ tin(zaLoc, radGrid) };
			if (dat::isValid(gotRad))
			{
				constexpr PropType expRad{ 1. };
				PropType const err{ gotRad - expRad };
assert(0. < err);
				qual.addSample(err);
			}
		}

	}

	// write data to file for display
	saveForGnuplot(quals);

	return 0;
}
