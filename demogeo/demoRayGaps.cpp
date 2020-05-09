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
\brief  This file contains main application program demoRayGaps
*/


#include "build/version.h"
#include "libio/stream.h"

#include "libdat/info.h"
#include "libga/ga.h"
#include "libgeo/intersect.h"
#include "libgeo/Ray.h"
#include "libmath/math.h"
#include "libprob/Histogram.h"


#include <cassert>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>


namespace
{
	//! Directions quasi uniformly spanning a sphere
	std::vector<ga::Vector>
	directions
		( size_t const & numDirs
		)
	{
		std::vector<ga::Vector> dirs;
		dirs.reserve(numDirs);

		std::default_random_engine engine{};
		std::uniform_real_distribution<> distro(-1., 1.);

		size_t count{ 0u };
		while (count < numDirs)
		{
			double const xx{ distro(engine) };
			double const yy{ distro(engine) };
			double const zz{ distro(engine) };
			ga::Vector const vec(xx, yy, zz);
			double const mag{ ga::magnitude(vec) };
			if ((.001 < mag) && (mag < 1.))
			{
				dirs.emplace_back(ga::unit(vec));
				++count;
			}
		}

		return dirs;
	}

}

//! Compute sampling of gaps between a pair of rays
int
main
	()
{
	constexpr size_t numDirs{ 10u * math::cube(100u) };
	std::vector<ga::Vector> const dirs{ directions(numDirs) };

	math::Partition const part(dat::Range<double>(0., 1.), 128u);
	prob::Histogram hist(part);

	// test ray
	ga::Vector const rDir{ ga::e1 };
	ga::Vector const rVec{ 10.*rDir };
	geo::Ray const ray1(ga::vZero, rVec);
	for (ga::Vector const & dir : dirs)
	{
		geo::Ray const ray2(ga::e2, dir);

		std::pair<double, double> const ranges
			{ geo::intersect::rangesFor(ray1, ray2) };
		ga::Vector const pnt1{ ray1.pointAt(ranges.first) };
		ga::Vector const pnt2{ ray1.pointAt(ranges.second) };
		ga::Vector const dVec{ pnt2 - pnt1 };
		/*
		double const gap
			{ geo::intersect::gapBetween
				(ray1, ray2, geo::intersect::rangesFor(ray1, ray2))
			};
		*/
		double const gap{ ga::dot(dVec, rDir).theValue };
		hist.addSample(gap);
	}

	std::ofstream ofs("gapProbs.dat");
	hist.writeDataProbs(ofs, "hist");

	return 0;
}
