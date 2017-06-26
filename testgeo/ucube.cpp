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
\brief  This file contains unit test for geo::cube
*/


#include "libgeo/cube.h"

#include "libdat/dat.h"
#include "libga/Pose.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

/*
//! Check for common functions
std::string
geo_cube_test0
	()
{
	std::ostringstream oss;
	geo::cube const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}
*/

	struct GotPoints
	{
		size_t theNumEdge;
		std::vector<ga::Vector> const theVert;
		std::vector<ga::Vector> const theEdge;
		std::vector<ga::Vector> const theTriad;
		std::vector<ga::Vector> const theSkeleton;
		std::vector<ga::Vector> const theFrame;

		explicit
		GotPoints
			( size_t const & numEdge
			)
			: theNumEdge(numEdge)
			, theVert(geo::cube::points::onVertices())
			, theEdge(geo::cube::points::onEdge(theNumEdge))
			, theTriad(geo::cube::points::onTriad(theEdge))
			, theSkeleton(geo::cube::points::onSkeleton(theTriad))
			, theFrame(geo::cube::points::onFrame(theNumEdge))
		{ }

		std::vector<ga::Vector>
		allPoints
			() const
		{
			std::vector<ga::Vector> all;
			all.insert(all.end(), theVert.begin(), theVert.end());
			all.insert(all.end(), theEdge.begin(), theEdge.end());
			all.insert(all.end(), theTriad.begin(), theTriad.end());
			all.insert(all.end(), theSkeleton.begin(), theSkeleton.end());
			all.insert(all.end(), theFrame.begin(), theFrame.end());
			return all;
		}

		std::pair<ga::Vector, ga::Vector>
		minmax
			() const
		{
			std::pair<ga::Vector, ga::Vector> minmax;
			ga::Vector & min = minmax.first;
			ga::Vector & max = minmax.second;
			static double const big(123456789.);
			min = ga::Vector( big,  big,  big);
			max = ga::Vector(-big, -big, -big);

			std::vector<ga::Vector> const pnts(allPoints());
			for (ga::Vector const pnt : pnts)
			{
				for (size_t kk(0u) ; kk < 3u ; ++kk)
				{
					min[kk] = std::min(min[kk], pnt[kk]);
					max[kk] = std::max(max[kk], pnt[kk]);
				}
			}
			return minmax;
		}

		inline
		std::pair<double, double>
		minComponent
			() const
		{
			std::pair<double, double> mmComp;
			double & minComp = mmComp.first;
			double & maxComp = mmComp.second;
			std::pair<ga::Vector, ga::Vector> const mmVec(minmax());
			ga::Vector const & minVec = mmVec.first;
			ga::Vector const & maxVec = mmVec.second;
			minComp = minVec[0];
			minComp = std::min(minComp, minVec[1]);
			minComp = std::min(minComp, minVec[2]);
			maxComp = maxVec[0];
			maxComp = std::max(maxComp, maxVec[1]);
			maxComp = std::max(maxComp, maxVec[2]);
			return mmComp;
		}

		inline
		bool
		isValid
			() const
		{
			size_t const expVert(8u);
			size_t const expEdge(theNumEdge);
			size_t const expTriad(3u * expEdge);
			size_t const expSkeleton(4u * expTriad);
			size_t const expFrame(expSkeleton + expVert);

			size_t const gotVert(theVert.size());
			size_t const gotEdge(theEdge.size());
			size_t const gotTriad(theTriad.size());
			size_t const gotSkeleton(theSkeleton.size());
			size_t const gotFrame(theFrame.size());

			std::pair<double, double> const minmax(minComponent());

			ga::Vector const expCenter(.5, .5, .5);
			ga::Vector const gotCenter(geo::cube::center());

			return
				(  (expVert == gotVert)
				&& (expEdge == gotEdge)
				&& (expTriad == gotTriad)
				&& (expSkeleton == gotSkeleton)
				&& (expFrame == gotFrame)

				&& dat::nearlyEquals(minmax.first, 0.)
				&& dat::nearlyEquals(minmax.second, 1.)

				&& gotCenter.nearlyEquals(expCenter)
				);
		}

		std::string
		infoString
			( std::string const & title
			) const
		{
			std::ostringstream oss;
			oss
				<< title << '\n'
				<< "numVert: " << theVert.size() << '\n'
				<< "numPerEdge: " << theNumEdge << '\n'
				<< "... numEdge: " << theEdge.size() << '\n'
				<< "... numTriad: " << theTriad.size() << '\n'
				<< "... numSkeleton: " << theSkeleton.size() << '\n'
				<< "... numFrame: " << theFrame.size()
				;
			return oss.str();
		}
	};

//! Check basic count operations
std::string
geo_cube_test1
	()
{
	std::ostringstream oss;

	if (8u != geo::cube::points::onVertices().size())
	{
		oss << "Failure of onVertices test" << std::endl;
	}

	for (size_t nn(0u) ; nn < 3u ; ++nn)
	{
		GotPoints const got(nn);
		if (! got.isValid())
		{
			oss << "Failure of " << nn << " edge point test" << std::endl;
			oss << got.infoString("got values") << std::endl;
			oss << std::endl;
		}
	}

	return oss.str();
}

//! Check direction/face relationships
std::string
geo_cube_test2
	()
{
	std::ostringstream oss;

	// face centers
	std::array<ga::Vector, 6u> const faceDirs
		(geo::cube::faceCenterDirections());

	// small perturbations
	constexpr double mag{ (1023./1024.)*math::qtrPi };
	std::vector<ga::BiVector> const deltaAngles
		{                ga::BiVector{  0.,  0.,  0. }
		, mag * ga::unit(ga::BiVector{ -1., -1., -1. })
		, mag * ga::unit(ga::BiVector{  1., -1., -1. })
		, mag * ga::unit(ga::BiVector{ -1.,  1., -1. })
		, mag * ga::unit(ga::BiVector{  1.,  1., -1. })
		, mag * ga::unit(ga::BiVector{ -1., -1.,  1. })
		, mag * ga::unit(ga::BiVector{  1., -1.,  1. })
		, mag * ga::unit(ga::BiVector{ -1.,  1.,  1. })
		, mag * ga::unit(ga::BiVector{  1.,  1.,  1. })
		};

	for (size_t expNdx(0u) ; expNdx < 6u ; ++expNdx)
	{
		ga::Vector const & fdir = faceDirs[expNdx];

		// check face extraction for various directions in face
		for (ga::BiVector const & deltaAngle : deltaAngles)
		{
			// perturb face-center direction
			ga::Pose const perturb(deltaAngle);
			ga::Vector const pdir{ perturb(fdir) };

			// check face extraction directly at center
			size_t const gotNdx{ geo::cube::faceIndex(pdir) };
			if (! dat::nearlyEquals(gotNdx, expNdx))
			{
				oss << "Failure of faceIndex pdir test" << std::endl;
				oss << dat::infoString(fdir, "fdir") << '\n';
				oss << dat::infoString(pdir, "pdir") << '\n';
				oss << dat::infoString(expNdx, "expNdx") << '\n';
				oss << dat::infoString(gotNdx, "gotNdx") << '\n';
			}
		}
	}

	return oss.str();
}


}

//! Unit test for geo::cube
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
//	oss << geo_cube_test0();
	oss << geo_cube_test1();
	oss << geo_cube_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
