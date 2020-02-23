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
\brief Definitions for geo::si
*/


#include "libgeo/si.h"

#include "libdat/info.h"
#include "libla/eigen.h"
#include "libmath/math.h"

#include <algorithm>
#include <sstream>


namespace geo
{
namespace si
{

ga::Vector
SemiAxis :: asVector
	() const
{
	return { theMag * theDir };
}

std::string
SemiAxis :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << " ";
	}
	oss << "Mag: " << dat::infoString(theMag);
	oss << "  ";
	oss << "Dir: " << dat::infoString(theDir);
	return oss.str();
}

SemiAxis
PointSoln :: kthLargestSemiAxis
	( size_t const & ndx
	) const
{
	SemiAxis axis{};
	if (ndx < theSemiAxes.size())
	{
		size_t const last{ theSemiAxes.size() - 1u };
		size_t const kthNdx{ last - ndx };
		axis = theSemiAxes[kthNdx];
	}
	return axis;
}

double
PointSoln :: rmsAxisMagnitude
	() const
{
	double rms{ dat::nullValue<double>() };
	double sumSqs{ 0. };
	double count{ 0. };
	for (size_t kk{0u} ; kk < 3u ; ++kk)
	{
		double const & mag = theSemiAxes[kk].theMag;
		if (dat::isValid(mag))
		{
			sumSqs += math::sq(mag);
			count += 1.;
		}
	}
	if (0. < count)
	{
		rms = std::sqrt(sumSqs / count);
	}
	return rms;
}

std::vector<ga::Vector>
PointSoln :: ellipsoidTips
	() const
{
	ga::Vector const & mid = theLoc;
	return std::vector<ga::Vector>
		{ mid + theSemiAxes[0].asVector()
		, mid - theSemiAxes[0].asVector()
		, mid + theSemiAxes[1].asVector()
		, mid - theSemiAxes[1].asVector()
		, mid + theSemiAxes[2].asVector()
		, mid - theSemiAxes[2].asVector()
		};
}

std::string
PointSoln :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	oss << dat::infoString(theLoc, "theLoc");
	oss << std::endl;
	oss << dat::infoString(theSemiAxes[0], "EllipA");
	oss << std::endl;
	oss << dat::infoString(theSemiAxes[1], "EllipB");
	oss << std::endl;
	oss << dat::infoString(theSemiAxes[2], "EllipC");
	return oss.str();
}

PointSystem :: PointSystem
	()
	: theNormCo{}
	, theNormRhs{}
	, theNumRays{ 0u }
	, theNumPlanes{ 0u }
{
	std::fill(std::begin(theNormCo), std::end(theNormCo), 0.);
	std::fill(std::begin(theNormRhs), std::end(theNormRhs), 0.);
}

void
PointSystem :: addWeightedRays
	( std::vector<WRay> const & wrays
	)
{
	for (WRay const & wray : wrays)
	{
		addWeightedRay(wray);
		/*
		double const weightSq{ math::sq(wray.first) };
		geo::Ray const & ray = wray.second;
		Dyadic const obsDyadic{ rayDyadicFor(ray.theDir) };
		addWeightedDyadic(weightSq, obsDyadic);
		addWeightedRhs(weightSq, obsDyadic, ray.theStart);
		++theNumRays;
		*/
	}
}

void
PointSystem :: addWeightedPlanes
	( std::vector<WPlane> const & wplanes
	)
{
	for (WPlane const & wplane : wplanes)
	{
		addWeightedPlane(wplane);
		/*
		double const weightSq{ math::sq(wplane.first) };
		geo::Plane const & plane = wplane.second;
		Dyadic const obsDyadic{ planeDyadicFor(plane.unitNormal()) };
		addWeightedDyadic(weightSq, obsDyadic);
		addWeightedRhs(weightSq, obsDyadic, plane.origin());
		++theNumPlanes;
		*/
	}
}

namespace
{
	//! Extract ellipsoid axes from SVD decomposition of normal matrix
	inline
	std::array<SemiAxis, 3u>
	inverseEllipsoidFrom
		( Eigen::BDCSVD<la::eigen::Matrix_t<double> > const & svd
		)
	{
		// NOTE: Eigen return sVals sorted in decreasing order
		la::eigen::Matrix_t<double> const vecS{ svd.singularValues() };
		la::eigen::Matrix_t<double> const matV{ svd.matrixU() };

		// weights are eigenValues which are sqrt(SingularValues)
		std::array<double, 3u> const wVals
			{ std::sqrt(vecS(0, 0))
			, std::sqrt(vecS(1, 0))
			, std::sqrt(vecS(2, 0))
			};

		/*
		io::out() << "======" << '\n';
		io::out() << "matU:\n" << svd.matrixU() << '\n';
		io::out() << "vecS:\n" << vecS << '\n';
		io::out() << "wVal:" << dat::infoString(wVals) << '\n';
		io::out() << "matV:\n" << svd.matrixV() << '\n';
		io::out() << "======" << '\n';
		*/

		// eigen vectors are columns of 'V' matrix
		ga::Vector const eVec0{ matV(0, 0), matV(1, 0), matV(2, 0) };
		ga::Vector const eVec1{ matV(0, 1), matV(1, 1), matV(2, 1) };
		ga::Vector const eVec2{ matV(0, 2), matV(1, 2), matV(2, 2) };

		// return as inverse ellipsoid
		return
			{ SemiAxis{ wVals[0], eVec0 }
			, SemiAxis{ wVals[1], eVec1 }
			, SemiAxis{ wVals[2], eVec2 }
			};
	}

	//! Inverse of each axis
	std::array<SemiAxis, 3u>
	standardEllipsoid
		( Eigen::BDCSVD<la::eigen::Matrix_t<double> > const & svd
		)
	{
		std::array<SemiAxis, 3u> invAxes;
		std::array<SemiAxis, 3u> const fwdAxes{ inverseEllipsoidFrom(svd) };
		size_t const numAxes{ fwdAxes.size() };
		for (size_t kk{0u} ; kk < numAxes ; ++kk)
		{
			SemiAxis const & fwdAxis = fwdAxes[kk];
			double const & wSqMag = fwdAxis.theMag;
			ga::Vector const & dir = fwdAxis.theDir;
			double invSqMag{ dat::nullValue<double>() };
			if (math::eps < wSqMag)
			{
				invSqMag = 1. / wSqMag;
			}
			invAxes[kk] = SemiAxis{ invSqMag, dir };
		}
		return invAxes;
	}

	//! Solve point location and uncertainty ellipsoid using SVD
	PointSoln
	pointSolnSVD
		( Dyadic const & normCo
		, RhsVec const & normRhs
		)
	{
		// Interpret input structures as Eigen data
		la::eigen::ConstMap<double> const coMat
			(normCo.begin(), 3u, 3u);
		la::eigen::ConstMap<double> const rhsMat
			(normRhs.begin(), normRhs.size(), 1u);

		// allocate output solution space
		std::array<double, 3u> soln{ dat::nullValue<double>() };
		la::eigen::WriteMap<double> solnOut(soln.begin(), soln.size(), 1u);

		// construct SVD
		auto const eFlags{ Eigen::ComputeThinU + Eigen::ComputeThinV };
		Eigen::BDCSVD<la::eigen::Matrix_t<double> > const svd(coMat, eFlags);

		// use SVD to solve system
		solnOut = svd.solve(rhsMat);

		// extract uncertainties
		std::array<SemiAxis, 3u> const covarAxes{ standardEllipsoid(svd) };

		// pack results into PointSoln
		ga::Vector const pntLoc{ soln[0], soln[1], soln[2] };
		return PointSoln{ pntLoc, covarAxes };
	}

} // [annon]

PointSoln
PointSystem :: pointSolution
	() const
{
	PointSoln const tmp{ pointSolnSVD(theNormCo, theNormRhs) };

	// adjust for statistical degrees of freedom (on average)
	// unclear if required
	constexpr double scale{ 1. };
	/* 
	double const domPlanes{ 1. * (double)theNumPlanes };
	double const domRays{ 2. * (double)theNumRays };
	double const dofPoint{ 3. };
	double const redundancy{ domRays + domPlanes - dofPoint };
	double scale{ dat::nullValue<double>() };
	if (0. < redundancy)
	{
		scale = 1. / redundancy;
	}
	if (0. == redundancy)
	{
		scale = 1.;
	}

	io::out() << dat::infoString(domPlanes, "domPlanes") << '\n';
	io::out() << dat::infoString(domRays, "domRays") << '\n';
	io::out() << dat::infoString(dofPoint, "dofPoint") << '\n';
	io::out() << dat::infoString(redundancy, "redundancy") << '\n';
	io::out() << dat::infoString(scale, "scale") << '\n';
	*/

	// adjust axes magnitudes for return values
	std::array<SemiAxis, 3u> const & tmpAxes = tmp.theSemiAxes;
	std::array<SemiAxis, 3u> const  stdEllipAxes
		{ SemiAxis{ scale*tmpAxes[0].theMag, tmpAxes[0].theDir }
		, SemiAxis{ scale*tmpAxes[1].theMag, tmpAxes[1].theDir }
		, SemiAxis{ scale*tmpAxes[2].theMag, tmpAxes[2].theDir }
		};
	return PointSoln{ tmp.theLoc, stdEllipAxes };
}

std::string
PointSystem :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	oss << priv::infoMatrix(theNormCo.begin(), "theNormCo", 3u);
	oss << std::endl;
	oss << priv::infoMatrix(theNormRhs.begin(), "theNormRhs", 1u);
	return oss.str();
}


} // si

} // geo

