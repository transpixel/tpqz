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
\brief  This file contains main application program demoArcQuant
*/


#include "libapp/Usage.h"

#include "build/version.h"
#include "libio/stream.h"
#include "libio/string.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libga/ga.h"
#include "libmath/Partition.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>


namespace
{
	//! Basis elements for arc coordinate frame
	struct Basis
	{
		ga::Scalar const theEaa{};
		ga::BiVector const theEab{};
		ga::BiVector const theEac{};

		//! Null instance
		Basis
			() = default;

		//! Value ctor
		explicit
		Basis
			( ga::Scalar const & Eaa
			, ga::BiVector const & Eab
			, ga::BiVector const & Eac
			)
			: theEaa{ Eaa }
			, theEab{ Eab }
			, theEac{ Eac }
		{ }

		//! True if this instance is not-null
		bool
		isValid
			() const
		{
			return
				(  dat::isValid(theEaa)
				&& dat::isValid(theEab)
				&& dat::isValid(theEac)
				);
		}

		//! Compute basis with "ea" aligned to r0 and vector in "Eab" plane
		static
		Basis
		basisFor
			( ga::Vector const & r0
			, ga::Vector const & vecInEab
			)
		{
			assert(dat::isValid(r0));
			ga::Vector const & dirA = r0;
			ga::Vector const vecB{ ga::rejectionFrom(vecInEab, dirA) };
			ga::Vector const dirB{ ga::unit(vecB) };
			ga::Vector const dirC{ -ga::E123 * ga::wedge(dirA, dirB) };
			Basis const base
				{ ga::Scalar{ ga::magnitude(r0) }
				, ga::unit(ga::wedge(dirA, dirB))
				, ga::unit(ga::wedge(dirA, dirC))
				};
			assert(base.isValid());
			return base;
		}

		//! Arc-Angle (scalar and two bivector components) for components
		inline
		ga::G2
		arcAngleFor
			( double const & alpha
			, double const & beta
			, double const & gamma
			) const
		{
			return ga::G2(alpha*theEaa, beta*theEab + gamma*theEac);
		}
	};

	//! Factory for transforming between (Arc and Cartesian) domains
	struct ArcQuant
	{
		ga::Vector const theR0{};
		Basis const theBasis{};

		//! Null instance
		ArcQuant
			() = default;

		//! Equatorial "default" case aligned
		static
		ArcQuant
		atDirA
			( ga::Vector const & r0
			, ga::Vector const & vecA
			)
		{
			ArcQuant const aq{ r0, Basis::basisFor(r0, vecA) };
			assert(aq.isValid());
			return aq;
		}

		//! Equatorial "default" case aligned
		static
		ArcQuant
		equatorial
			()
		{
			return atDirA(ga::e1, ga::e2);
		}

		//! Value ctor
		explicit
		ArcQuant
			( ga::Vector const & r0
			, Basis const & basis
			)
			: theR0{ r0 }
			, theBasis{ basis }
		{ }

		//! True if this instance is not null
		bool
		isValid
			() const
		{
			return
				(  dat::isValid(theR0)
				&& dat::isValid(theBasis)
				);
		}

		//! Location vector associated with arc-angle components
		ga::Vector
		locationFor
			( double const & alpha
			, double const & beta
			, double const & gamma
			) const
		{
			ga::Vector rVec{};
			ga::G2 const angle{ theBasis.arcAngleFor(alpha, beta, gamma) };
			ga::G2 const spin{ expG2(angle) };
			rVec = (theR0 * spin).theV;
			assert(dat::isValid(rVec));
			return rVec;
		}
	};

	//! generate locations over 2D angle space
	std::vector<ga::Vector>
	locations
		( math::Partition const & angPart
		, ArcQuant const & xf
		)
	{
		std::vector<ga::Vector> locs;

		// construct transformer
		double const alpha{ 0. };

		// allocate (more than) enough space
		size_t const numParts{ angPart.size() };
		locs.reserve(math::sq(numParts)); // cube if using alpha
		for (size_t nb{0u} ; nb < numParts ; ++nb)
		{
			double const beta{ angPart.interpValueFor(double(nb)) };
			for (size_t nc{0u} ; nc < numParts ; ++nc)
			{
				double const gamma{ angPart.interpValueFor(double(nc)) };
				// only process angles within principal domain
				double const angMag{ math::hypot(beta, gamma) };
				if (angMag < math::pi)
				{
					locs.emplace_back(xf.locationFor(alpha, beta, gamma));
				}
			}
		}
		return locs;
	}
}

//! Demonstrate log-based (arc-length) quantiation on sphere
int
main
	( int const argc
	, char const * const * const argv
	)
{
	// check args
	app::Usage usage;
	usage.setSummary
		( "Demonstrate log-based quantization of spherical shell"
		);
	usage.addArg("numParts", "in) number of intervals each dimension");
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
	std::string const strParts(argv[++argnum]);

	// configure sampling
	constexpr size_t const nullSize{ dat::nullValue<size_t>() };
	size_t const numParts{ io::string::from(strParts, nullSize) };
	assert(dat::isValid(numParts));

	static dat::Range<double> const angRange{ -math::pi, math::pi };
	math::Partition const angPart(angRange, numParts);

	//ga::Vector const r0{ ga::unit(ga::e1 + ga::e2 + ga::e3) };
	//ga::Vector const vecA{ ga::e2 + ga::e3 };
	ga::Vector const r0{ ga::e1 };
	ga::Vector const vecA{ ga::e2 };
	ArcQuant const xf{ ArcQuant::atDirA(r0, vecA) };

	// generate locations
	std::vector<ga::Vector> const locs{ locations(angPart, xf) };

	// write to file
	std::ofstream ofs("sphereLocs.dat");
	for (ga::Vector const & loc : locs)
	{
	//	if ((0. <= loc[0]) && (0. <= loc[1]) && (0. <= loc[2]))
//		if ((0. <= loc[2]))
		{
			ofs << " " << dat::infoString(loc) << '\n';
		}
	}

	return 0;
}
