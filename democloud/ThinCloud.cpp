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
\brief  This file contains main application program ThinCloud
*/


#include "libapp/Usage.h"

#include "build/version.h"
#include "libcloud/io.h"
#include "libcloud/PointIterator.h"
#include "libcloud/stats.h"
#include "libdat/Latice.h"
#include "libga/Aligner.h"
#include "libga/Rigid.h"
#include "libgeo/Ray.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libio/string.h"
#include "libmath/Partition.h"

#include <array>
#include <cassert>
#include <fstream>
#include <iostream>

namespace testgeo
{

::geo::Ray const &
axisInBinCloud
	()
{
	// probe height is about .606
//	constexpr double zCloudInRef{ -.765 }; // smaller: cloud to low
	// constexpr double zCloudInRef{ -.630 }; // 
	constexpr double zCloudInRef{ -.950 }; // 
//	constexpr double zCloudInRef{ -.500 }; // larger: cloud to high
//	static ga::Vector const origCloudInRef(0., 0., zCloudInRef);
	// on screen view0:
	// x: down
	// y: right
	// z: up
	static ga::Vector const origCloudInRef(-.055, -.015, zCloudInRef);
	static ::geo::Ray const axis(origCloudInRef, ga::e2);
	return axis;
}

ga::Pose const &
poseCloudWrtRef
	()
{
	// cloud data axis
	static ::geo::Ray const axisInCloud(axisInBinCloud());

	// attitude -- note convention (into:cloud, from:ref)
	// cloud : ref
	//  e1 : -e2  (-Y)
	//  e2 : -e3  (-Z)
	//  e3 : +e1  (+X)
	static ga::Vector const & cloudDown = axisInCloud.theDir;
	static ga::Vector const & refDown = -ga::e3;
	static ga::Vector const & cloudAzim = ga::e1;
	static ga::Vector const & refAzim = -ga::e2;
	// static ga::Vector const & cloudAzim = ga::e3;
	// static ga::Vector const & refAzim =  ga::e1;
	if (cloudAzim.nearlyEquals(cloudDown))
	{
		io::err() << "ERROR: cloud axis/azimth must be distinct" << std::endl;
		assert(! "check/change clound axis pair");
	}

	static ga::Aligner::ExactFit<ga::Vector> const eFit(refDown, cloudDown);
	static ga::Aligner::BestTry<ga::Vector> const  bTry(refAzim, cloudAzim);
	static ga::Aligner const align(eFit, bTry);

	static ga::Pose const att(align.pose());
	return att;
}

ga::Rigid const &
xformCloudWrtRef
	()
{
	// cloud data axis
	static ::geo::Ray const axisInCloud(axisInBinCloud());

	// offset
	static ga::Vector const orig(axisInCloud.theStart);

	// attitude -- note passive convention
	static ga::Pose const attCloudWrtRef(poseCloudWrtRef());

	static ga::Rigid xform(orig, attCloudWrtRef);
	return xform;
}

}

namespace
{
	//! An uninitialized latice spanning fpnts
	std::array<math::Partition, 3u>
	laticePartitions
		( dat::Volume<double> const & bounds
		, size_t const & numParts
		)
	{
		std::array<math::Partition, 3u> parts;
		if (dat::smallValue<double>() < bounds.magnitude())
		{
			// create a latice partition
			parts[0] = math::Partition(bounds[0].endExpanded(), numParts);
			parts[1] = math::Partition(bounds[1].endExpanded(), numParts);
			parts[2] = math::Partition(bounds[2].endExpanded(), numParts);
		}
		return parts;
	}

	//! True if point is within maxMagRej of axis and 'in front' of axis start
	bool
	isNearAxis
		( ga::Vector const & point
		, double const & maxMagRej
		, geo::Ray const axisRay
		)
	{
		bool okay{ false };
		// check that it is close to axis
		ga::Vector const rej{ axisRay.rejectionTo(point) };
		if (rej.magnitude() < maxMagRej)
		{
			// check that point is 'to the front of' ray
			double const dist{ axisRay.distanceAlong(point) };
			okay = (0. < dist);
		}
		return okay;
	}


	//! Fewer points determined by binning
	std::vector<ga::Vector>
	sparsePoints
		( std::vector<cloud::FixedPoint> const & fpnts
		, std::array<math::Partition, 3u> const & parts
		, double const & maxMagRej
		, geo::Ray const axisRay
		)
	{
		std::vector<ga::Vector> points;

		// allocate latice space which spans fpnts
		using SumWgt = std::pair<ga::Vector, double>;
		dat::Latice<SumWgt> latice
			(parts[0].size(), parts[1].size(), parts[2].size());

		// accumulate points into latice cells
		std::fill(latice.begin(), latice.end(), SumWgt{ ga::vZero, 0.});
		for (cloud::PointIterator iter{fpnts} ; iter ; ++iter)
		{
			ga::Vector const point(iter.vectorPoint());
			// compute indices into latice for this point
			size_t const xNdx{ parts[0].binIndexFor(point[0]) };
			size_t const yNdx{ parts[1].binIndexFor(point[1]) };
			size_t const zNdx{ parts[2].binIndexFor(point[2]) };
			// accumulate this point into corresponding voxel
			SumWgt & sw = latice(xNdx, yNdx, zNdx);
			sw.first = sw.first + point;
			sw.second = sw.second + 1.;
		}

		// compute voxel point location averages (in place)
		points.reserve(latice.size());
		static ga::Vector const null{};
		for (dat::Latice<SumWgt>::iterator
			iter{latice.begin()} ; latice.end() != iter ; ++iter)
		{
			SumWgt & sw = *iter;
			ga::Vector & sum = sw.first;
			double const & wgt = sw.second;
			if (0. < wgt)
			{
				ga::Vector const point((1./wgt) * sum);
				if (isNearAxis(point, maxMagRej, axisRay))
				{
					points.emplace_back(point);
				}
			}
		}

		return points;
	}

	// transform into reference frame
	std::vector<ga::Vector>
	pointsInStation
		( std::vector<ga::Vector> const & pntsInCloud
		, double const & zLocSta
		)
	{
		std::vector<ga::Vector> pntsInSta(pntsInCloud.size());

		// relationship of ref frame w.r.t. cloud frame
		static ga::Rigid const xCloudWrtRef(testgeo::xformCloudWrtRef());
		static ga::Rigid const xRefWrtCloud(xCloudWrtRef.inverse());

		// orientation of station frame w.r.t. reference frame
		ga::Rigid const xStaWrtRef(zLocSta*ga::e3, ga::Pose::identity());
		ga::Rigid const xStaWrtCloud(xStaWrtRef * xRefWrtCloud);

		// convert points into station frame
		std::transform
			( pntsInCloud.begin(), pntsInCloud.end()
			, pntsInSta.begin()
			, [&xStaWrtCloud]
				(ga::Vector const & pCloud)
				{ return xStaWrtCloud(pCloud); }
			);

		return pntsInSta;
	}

	//! Write points to fpath in standard text format
	bool
	savePoints
		( std::vector<ga::Vector> const & pntsInSta
		, std::string const & fpath
		)
	{
		std::ofstream ofs(fpath);
		for (ga::Vector const & point : pntsInSta)
		{
			ofs
				<< ' ' << io::sprintf("%9.6f", point[0])
				<< ' ' << io::sprintf("%9.6f", point[1])
				<< ' ' << io::sprintf("%9.6f", point[2])
				<< '\n';
		}
		ofs << std::endl;
		return (! ofs.fail());
	}
}

//! Read a binary cloud and output a thinned version.
int
main
	( int const argc
	, char const * const * const argv
	)
{
	// Don't mix stdio & streams (for performance)
	//std::ios::sync_with_stdio(false);

	// check args
	app::Usage usage;
	usage.setSummary
		( "Write a thinned version of binary point cloud."
		);
	usage.addArg("OutFile", "path to output file [text format]");
	usage.addArg("NumPerDim", "Number of voxel cells per dimension");
	usage.addArg("RadTol", "radial threshold tolerance [m] (e.g. 1m)");
	usage.addArg("InFile", "path to input cloud <7-float.bin>");
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
	std::string const outpath(argv[++argnum]);
	std::string const numperStr(argv[++argnum]);
	std::string const radtolStr(argv[++argnum]);
	std::string const inpath(argv[++argnum]);

	size_t const numPerDim
		{ io::string::from(numperStr, dat::nullValue<size_t>()) };
	double const radTol
		{ io::string::from(radtolStr, dat::nullValue<double>()) };
	io::out() << dat::infoString(numPerDim, "numPerDim") << std::endl;
	io::out() << dat::infoString(radTol, "radTol") << std::endl;

	// thin data if input parms are valid
	if (dat::isValid(numPerDim) && dat::isValid(radTol))
	{
		//
		// get data values and compute bounding volume
		//

		// load data -- about 2x as fast as the double loop and less memory
		std::vector<cloud::FixedPoint> const fpnts
			(cloud::io::loadAsFixed(inpath));

		// get cloud statistics
		dat::Volume<double> const bounds(cloud::stats::boundingVolumeOf(fpnts));
		io::out() << dat::infoString(bounds, "bounds") << std::endl;

		//
		// perform thinning
		//

		// determine latice to span cloud data
		std::array<math::Partition, 3u> const parts
			(laticePartitions(bounds, numPerDim));

		// extract sparse point collection
		std::vector<ga::Vector> const pntsInCloud
			(sparsePoints(fpnts, parts, radTol, testgeo::axisInBinCloud()));

		//
		// transform into "station" frame
		//

		// transform into station coordinate frame
		double const zLocSta{ -0. };
		std::vector<ga::Vector> const pntsInSta
			(pointsInStation(pntsInCloud, zLocSta));

		//
		// save data and report interesting info
		//

		// write to output file
		(void)savePoints(pntsInSta, outpath);

		// report info
		double const fracFull
			{ double(pntsInSta.size()) / double(fpnts.size()) };
		io::out()
			<< "num:in,out,fracFull:"
			<< ' ' << dat::infoString(fpnts.size())
			<< ' ' << dat::infoString(pntsInSta.size())
			<< ' ' << dat::infoString(fracFull)
			<< std::endl;
	}
	else
	{
		io::err()
			<< "Failure: unable to parse numeric args. got:" << '\n'
			<< dat::infoString(numPerDim, "numPerDim") << '\n'
			<< dat::infoString(radTol, "radTol") << '\n'
			<< std::endl;
	}

	return 0;
}
