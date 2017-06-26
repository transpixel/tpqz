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
\brief  This file contains main application program demoLoadBinary
*/


#include "libapp/Usage.h"

#include "build/version.h"
#include "libapp/Timer.h"
#include "libcloud/cast.h"
#include "libcloud/FixedPoint.h"
#include "libcloud/io.h"
#include "libcloud/stats.h"
#include "libio/stream.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>


namespace
{
	inline
	cloud::FixedPoint
	toFixed
		( cloud::RecordBin const & bin
		)
	{
		ga::Vector const vec(bin[0], bin[1], bin[2]);
		return cloud::cast::FixedPoint(vec);
	}
}

//! Demonstrate loading/manipulation of binary point cloud
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
		( "Load binary (full)point cloud and report statistics"
		);
	usage.addArg("InFile", "Binary processed *.bin format");
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
	std::string const inpath(argv[++argnum]);

	app::Timer t1;
	app::Timer t2;

	// load data
	t1.start("loadBinary");
	std::vector<cloud::RecordBin> const bpnts(cloud::io::loadAsBinary(inpath));

	// convert to packed format
	t1.start("pack");
	std::vector<cloud::FixedPoint> xpnts(bpnts.size());
	std::transform
		( bpnts.begin(), bpnts.end()
		, xpnts.begin()
		, toFixed
		);
	t1.stop();

	// load data -- about 2x as fast as the double loop and less memory
	t2.start("loadFixed");
	std::vector<cloud::FixedPoint> const fpnts(cloud::io::loadAsFixed(inpath));
	t2.stop();

	// get cloud statistics
	dat::Volume<double> const bounds(cloud::stats::boundingVolumeOf(fpnts));

	// report info
	io::out()
		<< "inpath: " << inpath
		<< "\n--- load large binary, then compress"
		<< "\nbpnt_size: " << bpnts.size()
		<< "\nbin_bytes: " << bpnts.size()*sizeof(cloud::RecordBin)
		<< "\nxfm_bytes: " << xpnts.size()*sizeof(cloud::FixedPoint)
		<< '\n' << dat::infoString(t1, "time:loadBin,xform:")
		<< "\n--- compress while loading"
		<< "\nfpnt_size: " << fpnts.size()
		<< "\nfix_bytes: " << fpnts.size()*sizeof(cloud::FixedPoint)
		<< '\n' << dat::infoString(t2, "time:loadFixed")
		<< std::endl;
	io::out() << dat::infoString(bounds, "bounds") << std::endl;

	return 0;
}
