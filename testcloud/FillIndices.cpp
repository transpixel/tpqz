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
\brief  This file contains main application program FillIndices
*/


#include "libapp/Usage.h"

#include "build/version.h"
#include "libdat/info.h"
#include "libio/stream.h"

#include "libapp/Timer.h"
#include "libcloud/cloud.h"
#include "libimg/dilate.h"

#include "libdat/io.h"
#include "libimg/io.h"
#include "libio/string.h"

#include <cassert>
#include <fstream>
#include <iostream>


namespace
{
	using PixVal = cloud::NdxType;
	constexpr bool sShowTiming{ true };
	constexpr bool sSaveTmpGrids{ true };


	//! An "on" value for pixels that are valid - else off value
	inline
	uint8_t
	outValue
		( PixVal const & pix
		)
	{
		uint8_t outVal{ 100u };
		if (dat::isValid(pix))
		{
			outVal = 200u;
		}
		return outVal;
	}

	//! Save visualization grid
	void
	saveGridViz
		( dat::grid<PixVal> const & ndxGrid
		, std::string const & fpath
		)
	{
		dat::grid<uint8_t> ugrid(ndxGrid.hwSize());
		std::fill(ugrid.begin(), ugrid.end(), 0u);
		std::transform
			( ndxGrid.begin(), ndxGrid.end()
			, ugrid.begin()
			, [] (PixVal const & pix) { return outValue(pix); }
			);
		(void)img::io::saveViaOpenCV(ugrid, fpath);
	}
}

//! Fill sparsely populated index map
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
		( "Fill missing index values with nearest index"
		);
	usage.addArg("OutPath", "path to output index [.u32 format]");
	usage.addArg("InPath", "path to input index [.u32 format]");
	usage.addArg("InHigh", "height of input .u32 grid");
	usage.addArg("InWide", "width of input .u32 grid");
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
	std::string const strhigh(argv[++argnum]);
	std::string const strwide(argv[++argnum]);

	constexpr size_t nullSize{ dat::nullValue<size_t>() };
	size_t const inHigh{ io::string::from(strhigh, nullSize) };
	size_t const inWide{ io::string::from(strwide, nullSize) };
	dat::Extents const hwSize(inHigh, inWide);
	if (! dat::isValid(hwSize))
	{
		io::err() << "ERROR: Bad input size specification."
			<< " " << dat::infoString(hwSize, "hwSize")
			<< std::endl;
		return 1;
	}

	// load input cloud
	dat::grid<PixVal> origGrid(hwSize);
	std::ifstream ifs(inpath);
	std::streamsize gotFileSize{};
	bool const okayLoad
		( dat::io::loadBinary
			(ifs, origGrid.begin(), origGrid.end(), &gotFileSize)
		);
	if (okayLoad)
	{
		// fill indices
		app::Timer timer;
		dat::grid<PixVal> const fullGrid{ img::dilate::floodFilled(origGrid) };
		timer.stop();

		if (sShowTiming)
		{
			io::out() << dat::infoString(timer) << std::endl;
		}

		// save visualization
		if (sSaveTmpGrids)
		{
			saveGridViz(fullGrid, "fullGrid.png");
		}

		std::ofstream ofs(outpath);
		bool const okaySave
			(dat::io::saveBinary(ofs, fullGrid.begin(), fullGrid.end()));
		if (okaySave)
		{
			io::out() << "Success" << std::endl;
		}
		else
		{
			io::err() << "FAILURE: Can't load save to file:"
				<< " " << outpath << std::endl;
		}
	}
	else
	{
		size_t const expFileSize(hwSize.size() * sizeof(uint32_t));
		io::err() << "FAILURE: Can't load index map from:"
			<< " '" << inpath << "'\n"
			<< " Specified size: " << dat::infoString(expFileSize) << '\n'
			<< "Found byte size: " << dat::infoString(gotFileSize) << '\n'
			<< std::endl;
	}

	return 0;
}
