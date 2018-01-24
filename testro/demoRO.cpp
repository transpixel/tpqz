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
\brief  This file contains main application program demoRO
*/


#include "libapp/Usage.h"

#include "build/version.h"
#include "libio/stream.h"

#include "libcam/Camera.h"
#include "libcam/io.h"
#include "libcam/Loader.h"
#include "libdat/info.h"
#include "libdat/ops.h"
#include "libro/PairRel.h"
#include "libro/sampcon.h"

#include <cassert>
#include <iostream>


namespace
{
}

//! Compute/report RO values given pair of .meapoint files
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
		( "Compute/Report RO solution for pair of *.meapoint measurement files"
		);
	usage.addArg("mea1", "measurements for first(ref) image (*.meapoint)");
	usage.addArg("mea2", "measurements for second image (*.meapoint)");
	usage.addArg("outpath", "filename to report solutions");
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
	std::string const meapath1(argv[++argnum]);
	std::string const meapath2(argv[++argnum]);
	std::string const outpath(argv[++argnum]);

	dat::Extents const detSize(2448u, 3264u);
double const extpd{ .5 * dat::diagonalMag(detSize) };
io::out() << dat::infoString(extpd, "extpd") << std::endl;
double const pd{ 2550. };
io::out() << dat::infoString(pd, "pd") << std::endl;
	cam::Camera const camera(pd, detSize);

	io::out() << dat::infoString(meapath1, "meapath1") << std::endl;
	io::out() << dat::infoString(meapath2, "meapath2") << std::endl;
	io::out() << dat::infoString(outpath, "outpath") << std::endl;

	std::vector<std::string> const meapaths{ meapath1, meapath2 };
//	cam::XRefSpots const spotTab{ cam::Loader::spotTableFor(meapaths) };

	cam::Loader const loader(meapaths);
	cam::XRefSpots const spotTab{ loader.spotTable() };

io::out() << "================" << std::endl;
io::out() << loader.infoStringDetail("loader") << std::endl;
io::out() << spotTab.infoStringAcqMajor("spotTab", true) << std::endl;
io::out() << dat::infoString(camera, "camera") << std::endl;
io::out() << "================" << std::endl;

	std::vector<cam::XRefSpots::AcqOverlap> const acqOvers
		{ spotTab.acqPairsWithOverlap(5u) };
	assert(1u == acqOvers.size());
	cam::AcqNdx const & acqNdx1 = acqOvers[0].theAcqNdx1;
	cam::AcqNdx const & acqNdx2 = acqOvers[0].theAcqNdx2;
	std::vector<cam::PntNdx> const & pntNdxs = acqOvers[0].thePntNdxs;
	std::vector<ro::PairUV> uvPairs;
	uvPairs.reserve(pntNdxs.size());
	for (cam::PntNdx const & pntNdx : pntNdxs)
	{
		dat::Spot const & spotU = spotTab(acqNdx1, pntNdx);
		dat::Spot const & spotV = spotTab(acqNdx2, pntNdx);
		ro::PairUV const pairUV
			{ camera.directionOf(spotU)
			, camera.directionOf(spotV)
			};
		uvPairs.emplace_back(pairUV);
		io::out()
			<< dat::infoString(pntNdx, "pntNdx")
			<< " " << "spot:U,V:"
		//	<< " " << dat::infoString(spotU)
		//	<< " " << dat::infoString(spotV)
			<< " " << dat::infoString(pairUV.first)
			<< " " << dat::infoString(pairUV.second)
			<< std::endl;
	}

//	ga::Rigid const ori2w1{  ga::e1, ga::Pose::identity() }; // soln
//	ga::Rigid const ori2w1{ -ga::e1, ga::Pose::identity() }; // soln
//	ga::Rigid const ori2w1{  ga::e2, ga::Pose::identity() }; // x
//	ga::Rigid const ori2w1{ -ga::e2, ga::Pose::identity() }; // soln
//	ga::Rigid const ori2w1{  ga::e3, ga::Pose::identity() }; // soln
//	ga::Rigid const ori2w1{ -ga::e3, ga::Pose::identity() }; // soln

	ga::Rigid const ori2w1
		{ ga::unit(ga::e1+ga::e2), ga::Pose::identity() }; // soln
	ro::PairRel const roNom{ ga::Rigid::identity(), ori2w1 };
	ro::Solution const roSoln{ ro::sampcon::byCombo(uvPairs, roNom.pair()) };
	ro::PairRel const roBest(roSoln.pair());

io::out() << "================" << std::endl;
	io::out() << dat::infoString(acqOvers.size(), "acqOvers.s") << std::endl;
	io::out() << dat::infoString(pntNdxs.size(), "pntNdxs.s") << std::endl;
	io::out() << dat::infoString(uvPairs.size(), "uvPairs.s") << std::endl;
io::out() << "================" << std::endl;
	io::out() << dat::infoString(meapath1, "meapath1") << std::endl;
	io::out() << dat::infoString(meapath2, "meapath2") << std::endl;
io::out() << "----" << std::endl;
	io::out() << dat::infoString(roNom, "roNom") << std::endl;
io::out() << "----" << std::endl;
	io::out() << dat::infoString(roBest, "roBest") << std::endl;
io::out() << "================" << std::endl;

	return 0;
}
