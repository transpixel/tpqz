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
\brief  This file contains main application program ConvertToAscii
*/


#include "libapp/Usage.h"

#include "build/version.h"
#include "libio/stream.h"

#include "libcloud/io.h"
#include "libcloud/PointIterator.h"

#include <cassert>
#include <fstream>
#include <iostream>


namespace
{
}

//! Save point cloud data as ascii format.
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
		( "Load point cloud and save data in ascii text format."
		);
	usage.addArg("OutFile", "Path to output file <x y z> text format");
	usage.addArg("InFile", "Path to input cloud [.bin] (7xfloat) format");
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
	std::string const inpath(argv[++argnum]);

	// load as fixed point format
	std::vector<cloud::FixedPoint> const fpnts(cloud::io::loadAsFixed(inpath));
	if (fpnts.empty())
	{
		io::err()
			<< "ERROR: unable to load points from:" << '\n'
			<< dat::infoString(inpath, "inpath") << '\n'
			;
	}

	// save as ascii
	std::ofstream ofs(outpath);
	bool const okaySave{ cloud::io::saveAsAscii(ofs, fpnts, "%9.3f") };
	if (! okaySave)
	{
		io::err()
			<< "ERROR: unable to save output to:" << '\n'
			<< dat::infoString(outpath, "outpath") << '\n'
			;
	}

	return 0;
}
