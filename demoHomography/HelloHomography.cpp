


#include "libdat/info.h"
#include "libio/stream.h"

#include "libla/eigen.h"

#include <eigen3/Eigen/Dense>

#include <iostream>


namespace
{
	dat::grid<double>
	iGrid
		( size_t const & dim = 8u
		)
	{
		dat::grid<double> matrix(dim, dim);
		std::fill(std::begin(matrix), std::end(matrix), 0.);
		for (size_t kk{0u} ; kk < dim ; ++kk)
		{
			matrix(kk, kk) = 9.;
		}
		return matrix;
	}
}


int main()
{
	io::out() << "Hello Homography!!" << std::endl;

	dat::grid<double> const srcGrid{ iGrid() };
	dat::grid<double> invGrid(srcGrid.hwSize());
	std::fill(std::begin(invGrid), std::end(invGrid), dat::nullValue<double>());

	la::eigen::ConstMap<double> const srcMat{ la::eigen::withGrid(srcGrid) };
	la::eigen::WriteMap<double> invMat{ la::eigen::withGrid(&invGrid) };

	invMat = Eigen::Inverse<la::eigen::Matrix_t<double> >(srcMat);

	io::out() << srcGrid.infoStringContents("srcGrid", "%9.3f") << std::endl;
	io::out() << invGrid.infoStringContents("invGrid", "%9.3f") << std::endl;

	return 0;
}

