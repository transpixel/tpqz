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
\brief  This file contains main application program demoRaw10
*/


#include "libapp/Usage.h"

#include "build/version.h"
#include "libio/stream.h"

#include "libdat/grid.h"
#include "libdat/info.h"
#include "libimg/cfa.h"
#include "libimg/io.h"
#include "libimg/raw10.h"
#include "libimg/stats.h"
#include "libprob/mean.h"
#include "libprob/SampleStats.h"

#include "libprob/CdfForward.h"
#include "libprob/Frac9.h"

#include "libsys/Timer.h"

#include <bitset>


namespace
{
	//! Report file save status
	void
	reportSave
		( bool const & okaySave
		, std::string const & pathOut
		)
	{
		if (okaySave)
		{
			io::out() << "Success: saved to '" << pathOut << "'\n";
		}
		else
		{
			io::err() << "ERROR: couldn't save to '" << pathOut << "'\n";
		}
	}

	//! Median values for each channel
	template <typename PixType>
	std::array<double, 4u>
	bandMediansFrom
		( std::array<dat::grid<PixType>, 4u> const bands
		)
	{
		std::array<double, 4u> bMedians{};
		for (size_t nn{0u} ; nn < bands.size() ; ++nn)
		{
			static dat::Range<double> const uRange{ 0., 255. };
			static math::Partition const uPart(uRange, 256u);
			prob::CdfForward const cdf
				{ prob::CdfForward::fromSamps
					(bands[nn].begin(), bands[nn].end(), uPart)
				};
			prob::Frac9 const bFracs(cdf);
			bMedians[nn] = bFracs.median();
			//io::out() << dat::infoString(bFracs, "bFracs") << '\n';
		}
		return bMedians;
	}

	//! Display cell contents
	std::string
	infoString
		( img::cfa::Cell<float> const & cell
		, std::string const & title = {}
		)
	{
		std::ostringstream oss;
		if (! title.empty())
		{
			oss << title << std::endl;
		}
		static std::string const pad{ "   " };
		oss
			<< pad << dat::infoString(cell.theElems[0][0], "cell[0][0]")
			<< pad << dat::infoString(cell.theElems[0][0], "cell[0][0]")
			<< std::endl
			<< pad << dat::infoString(cell.theElems[1][0], "cell[1][0]")
			<< pad << dat::infoString(cell.theElems[1][0], "cell[1][0]")
			;
		return oss.str();
	}
}

//! Read a 'raw10' formated binary (e.g. from Raspberry Pi cameras)
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
		( "Read (Raspberry Pi Camera) 'raw10' image format and save components"
		);
	usage.addArg("PathInput", "Input data file <raw10 format>");
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
	std::string const pathRaw(argv[++argnum]);
	std::string const pathOutPix{ "uGrid.png" };
	std::string const pathOutFlt{ "fGrid.pgm" };
	std::string const pathOutGray{ "gGrid.pgm" };

	sys::Timer timer;

	// load file (active) data into memory
	timer.start("load.quads");
	img::raw10::Sizes const raspi(img::raw10::RasPiV2{});
	dat::grid<img::raw10::FourPix> const rawQuads
		{ img::io::loadFourPixGrid(pathRaw, raspi) };
	timer.stop();

	// convert to uint8 image
	timer.start("decode.uint8");
	dat::grid<uint8_t> const uGrid
		{ img::convert::pixelGridFor<uint8_t>(rawQuads) };
	timer.stop();

	// convert to float image
	timer.start("decode.float");
	dat::grid<float> const fGrid
		{ img::convert::pixelGridFor<float>(rawQuads) };
	timer.stop();

	// split into individual channels
	timer.start("bands.split");
	std::array<dat::grid<uint8_t>, 4u> const bands
		{ img::cfa::channelsFromRGGB<uint8_t>(uGrid) };
	timer.stop();

	// compute statistics
	timer.start("minmax.uint8");
	dat::MinMax<uint8_t> const uMinMax
		{ img::stats::activeMinMax<uint8_t>(uGrid.begin(), uGrid.end()) };
	timer.stop();
	timer.start("minmax.float");
	dat::MinMax<float> const fMinMax
		{ img::stats::activeMinMax<float>(fGrid.begin(), fGrid.end()) };
	timer.stop();

	// save to files
	timer.start("save.uint8(png)");
	bool const okayPix{ img::io::savePng(uGrid, pathOutPix) };
	timer.start("save.float(pgm)");
	bool const okayFlt{ img::io::savePgmAutoScale(fGrid, pathOutFlt) };
	timer.stop();
	timer.start("save.bands(pgm)");
	std::array<std::string, 4u> const bandNames
		{ "band0.pgm", "band1.pgm", "band2.pgm", "band3.pgm" };
	std::array<bool, 4u> const okayBands
		{ img::io::savePgm(bands[0], bandNames[0])
		, img::io::savePgm(bands[1], bandNames[1])
		, img::io::savePgm(bands[2], bandNames[2])
		, img::io::savePgm(bands[3], bandNames[3])
		};
	timer.stop();

	// report actions
	io::out() << dat::infoString(pathRaw, "pathRaw") << '\n';
	io::out() << '\n';

	io::out() << dat::infoString(uGrid, "uGrid") << '\n';
	io::out() << dat::infoString(uMinMax, "uMinMax") << '\n';
	io::out() << '\n';

	io::out() << dat::infoString(fGrid, "fGrid") << '\n';
	io::out() << dat::infoString(fMinMax, "fMinMax") << '\n';
	io::out() << '\n';

	// Analyse invidiual band data
	std::array<double, 4u> const bMedians{ bandMediansFrom(bands) };
	double const bTgt
		{ prob::mean::geometric(bMedians.begin(), bMedians.end()) };
	img::cfa::Cell<float> cellGains
		{ static_cast<float>(bTgt/bMedians[0])
		, static_cast<float>(bTgt/bMedians[1])
		, static_cast<float>(bTgt/bMedians[2])
		, static_cast<float>(bTgt/bMedians[3])
		};

	dat::Range<double> const uRange{ 0., 256. };
	math::Partition const histPart(uRange, 256u);
	std::array<prob::SampleStats, 4u> const bStats
		{ prob::SampleStats(bands[0].begin(), bands[0].end(), histPart)
		, prob::SampleStats(bands[1].begin(), bands[1].end(), histPart)
		, prob::SampleStats(bands[2].begin(), bands[2].end(), histPart)
		, prob::SampleStats(bands[3].begin(), bands[3].end(), histPart)
		};
	std::array<double, 4u> const bEntrops
		{ bStats[0].theEntropy.perElement()
		, bStats[1].theEntropy.perElement()
		, bStats[2].theEntropy.perElement()
		, bStats[3].theEntropy.perElement()
		};

	// create grayscale image
	dat::grid<float> const gGrid{ img::cfa::grayGridFor(fGrid, cellGains) };
	std::array<dat::grid<float>, 4u> const gBands
		{ img::cfa::channelsFromRGGB<float>(gGrid) };
	std::array<double, 4u> const gMedians{ bandMediansFrom(gBands) };
	bool const okayGray{ img::io::savePgmAutoScale(gGrid, pathOutGray) };

	io::out() << dat::infoString(bMedians, "bMedians") << '\n';
	io::out() << infoString(cellGains, "cellGains") << '\n';
	io::out() << dat::infoString(bTgt, "bTgt") << '\n';
	io::out() << dat::infoString(gMedians, "gMedians") << '\n';
	io::out() << dat::infoString(bEntrops[0], "entropy[0]") << '\n';
	io::out() << dat::infoString(bEntrops[1], "entropy[1]") << '\n';
	io::out() << dat::infoString(bEntrops[2], "entropy[2]") << '\n';
	io::out() << dat::infoString(bEntrops[3], "entropy[3]") << '\n';

	io::out() << dat::infoString(timer, "processing times") << '\n';
	io::out() << std::endl;

	reportSave(okayPix, pathOutPix);
	reportSave(okayFlt, pathOutFlt);
	reportSave(okayGray, pathOutGray);
	reportSave(okayBands[0], bandNames[0]);
	reportSave(okayBands[1], bandNames[1]);
	reportSave(okayBands[2], bandNames[2]);
	reportSave(okayBands[3], bandNames[3]);
	io::out() << std::endl;

	dat::grid<uint16_t> const sGrid
		{ img::convert::pixelGridFor<uint16_t>(rawQuads) };
	dat::MinMax<uint16_t> const sMinMax
		{ img::stats::activeMinMax<uint16_t>(sGrid.begin(), sGrid.end()) };
	dat::grid<uint8_t> loGrid(sGrid.hwSize());
	dat::grid<uint8_t>::iterator itN{ loGrid.begin() };
	for (dat::grid<uint16_t>::const_iterator
		itS{ sGrid.begin() } ; sGrid.end() != itS ; ++itS, ++itN)
	{
		uint16_t const & sElem = *itS;
		uint8_t & loElem = *itN;
		std::bitset<10u> const mask
			{ 0u << 9u // lost to cast below
			| 0u << 8u // lost to cast below
			| 1u << 7u
			| 1u << 6u
			| 1u << 5u
			| 1u << 4u
			| 1u << 3u
			| 1u << 2u
			| 1u << 1u
			| 1u << 0u
			};
		loElem = static_cast<uint8_t>((sElem & mask.to_ulong()));
	}
	img::io::savePng(loGrid, "loGrid.png");
	io::out() << dat::infoString(sMinMax, "sMinMax") << '\n';

	return 0;
}

