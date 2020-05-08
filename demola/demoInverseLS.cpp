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
\brief  This file contains main application program HelloHomography
*/


#include "libdat/info.h"
#include "libio/stream.h"

#include "libla/eigen.h"

#include <eigen3/Eigen/Dense>

#include <iostream>


namespace
{
	//! Create a matrix proportional to identity
	dat::grid<double>
	normCoefficients
		( double const & diagValue = 9.
		, size_t const & dim = 8u
		)
	{
		dat::grid<double> matrix(dim, dim);
		std::fill(std::begin(matrix), std::end(matrix), 0.);
		for (size_t kk{0u} ; kk < dim ; ++kk)
		{
			matrix(kk, kk) = diagValue;
		}
		return matrix;
	}

	//! Simulate right-hand-side vector
	dat::grid<double>
	rhsValues
		( size_t const & dim = 8u
		)
	{
		dat::grid<double> rhs(dim, 1u);
		std::iota(std::begin(rhs), std::end(rhs), 1.);
		return rhs;
	}

	//! Grid of requested size filled with null data values
	dat::grid<double>
	nullGrid
		( dat::Extents const & hwSize = {}
		)
	{
		dat::grid<double> grid(hwSize);
		std::fill(std::begin(grid), std::end(grid), dat::nullValue<double>());
		return grid;
	}

	//! Grid version of matrix inverse
	dat::grid<double>
	inverseGrid
		( dat::grid<double> const & srcGrid
		)
	{
		// allocate input/output space
		dat::grid<double> invGrid{ nullGrid(srcGrid.hwSize()) };

		// utilize la::eigen to map grid data structures into Eigen operations
		using la::eigen::withGrid;
		la::eigen::ConstMap<double> const srcMat{ withGrid(srcGrid) };
		la::eigen::WriteMap<double> invMat{ withGrid(&invGrid) };

		// Eigen matrix operation (here vanilla matrix inversion)
		invMat = Eigen::Inverse<la::eigen::Matrix_t<double> >(srcMat);

		return invGrid;
	}

	//! Least squares solution
	dat::grid<double>
	solutionFor
		( dat::grid<double> const & normGrid
		, dat::grid<double> const & rhsGrid
		)
	{
		// allocate space
		size_t const numParms{ normGrid.high() };
		assert(numParms == normGrid.wide());
		assert(normGrid.high() == rhsGrid.high());
		dat::grid<double> solnGrid{ nullGrid(dat::Extents{ numParms, 1u }) };

		// utilize la::eigen to map grid data structures into Eigen operations
		using la::eigen::withGrid;
		la::eigen::ConstMap<double> const normMat{ withGrid(normGrid) };
		la::eigen::WriteMap<double> solnMat{ withGrid(&solnGrid) };

		la::eigen::ConstMap<double> const rhs{ la::eigen::withGrid(rhsGrid) };
		solnMat = Eigen::BDCSVD<la::eigen::Matrix_t<double> >
			(normMat, (Eigen::ComputeThinU | Eigen::ComputeThinV)).solve(rhs);
		return solnGrid;
	}
}


//! Program that demonstrates use of Eigen to operate on dat::grid data
int main()
{
	// Create a simple coefficient matrix
	dat::grid<double> const normGrid{ normCoefficients() };
	dat::grid<double> const rhsGrid{ rhsValues() };
	io::out() << normGrid.infoStringContents("normGrid", "%9.3f") << std::endl;
	io::out() << rhsGrid.infoStringContents("rhsGrid", "%9.3f") << std::endl;

	// Demonstrate matrix inversion
	dat::grid<double> invGrid{ inverseGrid(normGrid) };
	io::out() << invGrid.infoStringContents("invGrid", "%9.3f") << std::endl;

	// compute least squares solution
	dat::grid<double> const solnGrid{ solutionFor(normGrid, rhsGrid) };
	io::out() << solnGrid.infoStringContents("solnGrid", "%9.3f") << std::endl;

	return 0;
}

