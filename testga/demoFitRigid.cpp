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
\brief  This file contains main application program demoFitRigid
*/


#include "libdat/array.h"
#include "libdat/grid.h"
#include "libdat/validity.h"
#include "libga/Rigid.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libla/eigen.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>


namespace
{
	using PointSet = std::vector<ga::Vector>;
	using PairOfSet = std::pair<PointSet, PointSet>;

	size_t
	numberIn
		( PairOfSet const & pairs
		)
	{
		assert(pairs.first.size() == pairs.second.size());
		return pairs.first.size();
	}

	/*
	std::string
	infoString
		( PairOfSet const & pairPnts
		, std::string const & title = {}
		)
	{
		std::ostringstream oss;
		if (! title.empty())
		{
			oss << title << std::endl;
		}
		for (size_t nn{0u} ; nn < numberIn(pairPnts) ; ++nn)
		{
			oss
				<< " " << dat::infoString(pairPnts.first[nn])
				<< " " << dat::infoString(pairPnts.second[nn])
				<< '\n';
		}

		return oss.str();
	}
	*/

	std::string
	infoString
		( std::vector<double> const & values
		, std::string const & title = {}
		)
	{
		std::ostringstream oss;
		if (! title.empty())
		{
			oss << title << std::endl;
		}
		for (double const & value : values)
		{
			oss << dat::infoString(value) << std::endl;
		}
		return oss.str();
	}

	PairOfSet
	points
		( ga::Rigid const & xform
		)
	{
		static std::vector<ga::Vector> const xlocs
			{ ga::Vector( 10.,   0.,   0.)
			, ga::Vector(  0.,  10.,   0.)
			, ga::Vector(  0.,   0.,  10.)
			};
		std::vector<ga::Vector> ylocs;
		ylocs.reserve(xlocs.size());
		for (ga::Vector const & xloc : xlocs)
		{
			ylocs.emplace_back(xform(xloc));
		}
		return { xlocs, ylocs };
	}

	ga::Vector
	meanFor
		( PointSet const & pnts
		)
	{
		ga::Vector sum{ ga::vZero };
		for (ga::Vector const & pnt : pnts)
		{
			sum = sum + pnt;
		}
		return (1. / double(pnts.size())) * sum;
	}

	PointSet
	shifted
		( PointSet const & pnts
		, ga::Vector const & mean
		)
	{
		PointSet rels;
		rels.reserve(pnts.size());
		for (ga::Vector const & pnt : pnts)
		{
			rels.emplace_back(pnt - mean);
		}
		return rels;
	}

	PointSet
	sum
		( PairOfSet const & uvRel
		)
	{
		PointSet result;
		size_t const num{ numberIn(uvRel) };
		result.reserve(num);
		for (size_t nn{0u} ; nn < num ; ++nn)
		{
			ga::Vector const & upnt = uvRel.first[nn];
			ga::Vector const & vpnt = uvRel.second[nn];
			result.emplace_back(vpnt + upnt);
		}
		return result;
	}

	PointSet
	diff
		( PairOfSet const & uvRel
		)
	{
		PointSet result;
		size_t const num{ numberIn(uvRel) };
		result.reserve(num);
		for (size_t nn{0u} ; nn < num ; ++nn)
		{
			ga::Vector const & upnt = uvRel.first[nn];
			ga::Vector const & vpnt = uvRel.second[nn];
			result.emplace_back(vpnt - upnt);
		}
		return result;
	}

	void
	sumInto
		( dat::grid<double> * const & ptSum
		, dat::grid<double> const & grid
		)
	{
		std::transform
			( ptSum->begin(), ptSum->end()
			, grid.begin()
			, ptSum->begin()
			, std::plus<double>()
			);
	}

	dat::grid<double>
	aCoFor
		( ga::Vector const & crel
		, ga::Vector const & drel
		)
	{
		dat::grid<double> matA(4u, 4u);

		// access elements with 'math notation'
		double const & c1 = crel[0];
		double const & c2 = crel[1];
		double const & c3 = crel[2];
		double const & d1 = drel[0];
		double const & d2 = drel[1];
		double const & d3 = drel[2];

		// repeated products
		double const c1c1{ math::sq(c1) };
		double const c2c2{ math::sq(c2) };
		double const c3c3{ math::sq(c3) };
		double const d1d1{ math::sq(d1) };
		double const d2d2{ math::sq(d2) };
		double const d3d3{ math::sq(d3) };

		// format consistent with: (G1,G2,G3,gamma)
		matA(0, 0) = c3c3 + c2c2 + d1d1;
		matA(0, 1) =  - c2*c1 + d1*d2;
		matA(0, 2) =  - c3*c1 + d1*d3;
		matA(0, 3) =  - c3*d2 + c2*d3;
		matA(1, 1) = c3c3 + c1c1 + d2d2;
		matA(1, 2) =  - c3*c2 + d2*d3;
		matA(1, 3) =  + c3*d1 - c1*d3;
		matA(2, 2) = c2c2 + c1c1 + d3d3;
		matA(2, 3) =  - c2*d1 + c1*d2;
		matA(3, 3) = d1d1 + d2d2 + d3d3;

		// complete fill symmetrically
		matA(1, 0) = matA(0, 1);
		matA(2, 0) = matA(0, 2);
		matA(2, 1) = matA(1, 2);
		matA(3, 0) = matA(0, 3);
		matA(3, 1) = matA(1, 3);
		matA(3, 2) = matA(2, 3);

		return matA;
	}

	// Extract null space from data in grid
	std::vector<double>
	nullVectorFor
		( dat::grid<double> const & matA
		)
	{
		std::vector<double> soln;
		size_t const numDim{ matA.high() };
		assert(numDim == matA.wide());

		constexpr bool debug{ false };
		constexpr bool checkKernel{ true };

		la::eigen::ConstMap<double> const eA{ la::eigen::withGrid(matA) };
		Eigen::EigenSolver<la::eigen::Matrix_t<double> > solver(eA);

		if (debug)
		{
			io::out()
				<< "evals:"
				<< "  nrows: " << solver.eigenvalues().rows()
				<< "  ncols: " << solver.eigenvalues().cols()
				<< std::endl;
			io::out()
				<< "evecs:"
				<< "  nrows: " << solver.eigenvectors().rows()
				<< "  ncols: " << solver.eigenvectors().cols()
				<< std::endl;
		}

		std::vector<std::pair<double, size_t> > magNdxs;
		magNdxs.reserve(numDim);
		for (size_t kk{0u} ; kk < numDim ; ++kk)
		{
			std::complex<double> const & eval = solver.eigenvalues()(kk,0);
			double const mag{ std::abs(eval) };
			magNdxs.emplace_back(std::make_pair(mag, kk));

			if (debug)
			{
				io::out() << "eval: " << eval;
				io::out() << "   evec: ";
				for (size_t jj{0u} ; jj < static_cast<size_t>(eA.cols()) ; ++jj)
				{
					std::complex<double> const & foo
						= solver.eigenvectors()(jj,kk);
					io::out() << "   " << foo;
				}
				io::out() << std::endl;
			}
		}

		// TODO - can be replaced with linear complexity algorithm
		std::sort(magNdxs.begin(), magNdxs.end());

		bool cleanKernel{ true };
		if (checkKernel)
		{
			assert(1u < magNdxs.size());
			double const & mag0 = magNdxs[0].first;
			double const & mag1 = magNdxs[1].first;
			double const & magN = magNdxs[numDim - 1u].first;

			constexpr double pad{ 1024. };
			double const tol
				{ magN * pad * std::numeric_limits<double>::epsilon() };
			cleanKernel = ((mag0 < tol) && (tol < mag1));
		}
		if (cleanKernel)
		{
			size_t const & minNdx = magNdxs[0].second;
			soln.resize(numDim);
			for (size_t kk{0u} ; kk < numDim ; ++kk)
			{
				soln[kk] = solver.eigenvectors().col(minNdx)(kk,0).real();
			}
			if (debug)
			{
				io::out() << infoString(soln, "soln") << '\n';
			}
		}
		else
		{
			io::out() << "ERROR: to large of null space" << std::endl;
		}

		return soln;
	}

	struct Rotor
	{
		ga::BiVector thePhysA{};
		ga::Spinor theExpHalfA{};

		static
		Rotor
		fromValues
			( std::vector<double> const & values
			)
		{
			Rotor result;
			if (4u == values.size())
			{
				ga::BiVector const biv(values[0], values[1], values[2]);
				double const scl{ values[3] };
				if (scl < 0.)
				{
					result = Rotor(ga::Spinor(-scl, -biv));
				}
				else
				{
					result = Rotor(ga::Spinor(scl, biv));
				}
			}
			return result;
		}

		Rotor
			() = default;

		explicit
		Rotor
			( ga::BiVector const & physA
			)
			: thePhysA{ physA }
			, theExpHalfA{ ga::exp(.5*thePhysA) }
		{ }

		explicit
		Rotor
			( ga::Spinor const & expHalfA
			)
			: thePhysA{ ga::spin::physicalAngleFrom(expHalfA) }
			, theExpHalfA{ expHalfA }
		{ }

		bool
		isValid
			() const
		{
			return
				(  thePhysA.isValid()
				&& theExpHalfA.isValid()
				&& (! (theExpHalfA.theS.theValue < 0.))
				);
		}

		std::string
		infoString
			( std::string const & title = {}
			) const
		{
			std::ostringstream oss;
			if (! title.empty())
			{
				oss << io::sprintf("%15s", title) << " ";
			}
			oss
				<< " " << dat::infoString(theExpHalfA, "e^A/2")
				<< " " << dat::infoString(thePhysA, "bivA")
				;
			return oss.str();
		}
	};

	//! Recover displacement vector from centroids and spinor
	ga::Vector
	sVecFor
		( std::pair<ga::Vector, ga::Vector> const & xyMean
		, Rotor const & rot
		)
	{
		ga::Vector vec;
		ga::Vector const & xbar = xyMean.first;
		ga::Vector const & ybar = xyMean.second;
		ga::Spinor const & spin = rot.theExpHalfA;
		vec = (-spin.reverse()*ybar*spin + xbar).theV;
		return vec;
	}

}

//! Experiment with rigid body transform parameter-fitting.
int
main
	( int const // argc
	, char const * const * const // argv
	)
{
	// define configuration
	ga::Vector const expVec(3., 5., 7.);
	ga::BiVector const expBiv( .2, 3.0, .4);

	Rotor const expRot(expBiv);
	ga::Rigid const expRigid(expVec, ga::Pose(expRot.thePhysA));

	// generate corresponding points
	PairOfSet const xyPairPnts{ points(expRigid) };
	size_t const numPnts{ numberIn(xyPairPnts) };

	// compute mean value
	std::pair<ga::Vector, ga::Vector> const xyMean
		{ meanFor(xyPairPnts.first)
		, meanFor(xyPairPnts.second)
		};
	// remove mean value
	PairOfSet const uvPairPnts
		{ shifted(xyPairPnts.first, xyMean.first)
		, shifted(xyPairPnts.second, xyMean.second)
		};
	// compute correspondence sum/diff relationships
	PairOfSet const cdPairPnts
		{ sum(uvPairPnts)
		, diff(uvPairPnts)
		};

	// fill LS system coefficient matrix
	dat::grid<double> matA(4u, 4u);
	std::fill(matA.begin(), matA.end(), 0.);
	for (size_t nn{0u} ; nn < numPnts ; ++nn)
	{
		ga::Vector const & crel = cdPairPnts.first[nn];
		ga::Vector const & drel = cdPairPnts.second[nn];

		dat::grid<double> const matAn{ aCoFor(crel, drel) };
		sumInto(&matA, matAn);
	}

	// find null space vector
	std::vector<double> const solnX(nullVectorFor(matA));

	// cast numeric solution to GA
	Rotor const gotRot(Rotor::fromValues(solnX));

	// check if solution is valid
	if (gotRot.isValid())
	{
		// recover location vector
		ga::Vector const gotVec{ sVecFor(xyMean, gotRot) };

		// recover entire transformation
		ga::Rigid const gotRigid(gotVec, ga::Pose(gotRot.thePhysA));

		// display results
		io::out() << std::endl;
		io::out() << dat::infoString(expRot, "expRot") << std::endl;
		io::out() << dat::infoString(gotRot, "gotRot") << std::endl;

		io::out() << std::endl;
		io::out() << expRigid.infoStringShort("expRigid") << std::endl;
		io::out() << gotRigid.infoStringShort("gotRigid") << std::endl;

		io::out() << std::endl;
	}
	else
	{
		io::err() << "ERROR: invalid solution" << std::endl;
	}

	return 0;
}
