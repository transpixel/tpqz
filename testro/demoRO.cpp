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
#include "libro/Accord.h"
#include "libro/io.h"
#include "libro/model.h"
#include "libro/PairRel.h"
#include "libro/sampcon.h"

#include <cassert>
#include <fstream>
#include <iostream>


namespace
{
	constexpr bool const sSavePlotable{ true };

	void
	saveModel
		( std::string const & fnameSta
		, std::string const & fnamePnt
		, ro::QuintSoln const & roQuintSoln
		, std::vector<ro::PairUV> const & uvPairs
		, std::vector<std::string> const & pntNames
		)
	{
		std::shared_ptr<ro::Pair> const & ptRO = roQuintSoln.theSoln.theRoPair;

		std::ofstream ofsSta(fnameSta);
		ro::io::gnuplot::drawOXYZ(ofsSta, ptRO->rigid1w0());
		ro::io::gnuplot::drawOXYZ(ofsSta, ptRO->rigid2w0());

		std::ofstream ofsPnt(fnamePnt);
		std::vector<ro::PntPair> const pntPairs
			{ ro::model::pointPairs(uvPairs, ptRO->pair()) };
		for (size_t nn{0u} ; nn < pntPairs.size() ; ++nn)
		{
			ro::PntPair const & pntPair = pntPairs[nn];
			ga::Vector const pntAve{ .5 * (pntPair.first + pntPair.second) };
			std::string const pntName{ pntNames[nn] };
			ofsPnt
				<< dat::infoString(pntAve, pntName)
				<< std::endl;
		}
	}

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
//	usage.addArg("outpath", "filename to report solutions");
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
//	std::string const outpath(argv[++argnum]);

	dat::Extents const detSize(3120u, 4160u);
	// double const estPd{ .5 * dat::diagonalMag(detSize) }; // 2040 or so
	double const pd{ 3112. };
	cam::Camera const camera(pd, detSize);

	io::out() << dat::infoString(meapath1, "meapath1") << std::endl;
	io::out() << dat::infoString(meapath2, "meapath2") << std::endl;
//	io::out() << dat::infoString(outpath, "outpath") << std::endl;

	std::vector<std::string> const meapaths{ meapath1, meapath2 };
	cam::Loader const loader(meapaths);
	cam::XRefSpots const spotTab{ loader.spotTable(camera) };
	std::vector<std::string> const pntNames{ loader.pntNames() };

	/*
	io::out() << "================" << std::endl;
	io::out() << dat::infoString(camera, "camera") << std::endl;
	io::out() << loader.infoStringDetail("loader") << std::endl;
	//io::out() << spotTab.infoStringAcqMajor("spotTab", true) << std::endl;
	io::out() << "================" << std::endl;
	*/

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
		dat::Spot const & spotU = spotTab(pntNdx, acqNdx1);
		dat::Spot const & spotV = spotTab(pntNdx, acqNdx2);
		ro::PairUV const pairUV
			{ camera.directionOf(spotU)
			, camera.directionOf(spotV)
			};
		uvPairs.emplace_back(pairUV);
		io::out()
			<< " " << dat::infoString(pntNdx) << ")"
			<< " " << io::sprintf("%12s", pntNames[pntNdx])
		//	<< " " << dat::infoString(spotU)
		//	<< " " << dat::infoString(spotV)
			<< " " << dat::infoString(pairUV.first, "dir:U")
			<< " " << dat::infoString(pairUV.second, "dir:V")
			<< std::endl;
	}

//	ga::Rigid const ori2w1{  ga::e1, ga::Pose::identity() }; // soln
//	ga::Rigid const ori2w1{ -ga::e1, ga::Pose::identity() }; // soln
//	ga::Rigid const ori2w1{  ga::e2, ga::Pose::identity() }; // x
//	ga::Rigid const ori2w1{ -ga::e2, ga::Pose::identity() }; // soln
//	ga::Rigid const ori2w1{  ga::e3, ga::Pose::identity() }; // soln
//	ga::Rigid const ori2w1{ -ga::e3, ga::Pose::identity() }; // soln

ga::Rigid const ori2w1{  ga::e2, ga::Pose::identity() }; // x

	ro::OriPair const oriPairNom{ ga::Rigid::identity(), ori2w1 };
	constexpr double const maxCondNum{ 1.e6 };
	ro::FitConfig const fitConfig{ maxCondNum };
	std::vector<ro::QuintSoln> const allQuintSolns
		{ ro::sampcon::allByCombo(uvPairs, oriPairNom, fitConfig) };

	// compute putative solutions
	constexpr size_t const numBest{ 3u };
	constexpr double const gapSig{ 10./2500. };
	std::vector<ro::QuintSoln> const roQuintSolns
		{ ro::sampcon::bestOf(allQuintSolns, uvPairs, numBest, gapSig) };

	io::out() << "================" << '\n';
		io::out() << dat::infoString(acqOvers.size(), "acqOvers.s") << '\n';
		io::out() << dat::infoString(pntNdxs.size(), "pntNdxs.s") << '\n';
		io::out() << dat::infoString(uvPairs.size(), "uvPairs.s") << '\n';
	io::out() << "================" << '\n';
		io::out() << dat::infoString(meapath1, "meapath1") << '\n';
		io::out() << dat::infoString(meapath2, "meapath2") << '\n';
	io::out() << "----" << '\n';
		ro::PairRel const roNom(oriPairNom);
		io::out() << dat::infoString(roNom, "roNom") << '\n';
	io::out() << "================" << std::endl;
	io::out() << "==== solutions: " << roQuintSolns.size() << '\n';

	size_t solnCount{ 0u };
	for (ro::QuintSoln const & roQuintSoln : roQuintSolns)
	{
		ro::FiveOf<size_t> const & fitNdxs = roQuintSoln.theFitNdxs;

	//	double const prob{ ro::Accord::probFor(roQuintSoln, uvPairs, gapSig) };
		ro::Accord const eval{ roQuintSoln.theSoln, &uvPairs };
		double const prob{ eval.probExcluding(fitNdxs, gapSig) };
		double const rmsGap{ eval.rmsGapExcluding(fitNdxs) };

		ro::FiveOf<std::string> const fitNames
			{ pntNames[ fitNdxs[0] ]
			, pntNames[ fitNdxs[1] ]
			, pntNames[ fitNdxs[2] ]
			, pntNames[ fitNdxs[3] ]
			, pntNames[ fitNdxs[4] ]
			};

		// report solns
		io::out() << "----" << std::endl;
		io::out()
			<< dat::infoString(fitNames.begin(), fitNames.end(), "fitNames")
			<< std::endl;
		io::out() << dat::infoString(roQuintSoln, "roQuintSoln") << std::endl;
		io::out() << "prob: " << io::sprintf("%9.6f", prob) << std::endl;
		io::out() << "rmsGap: " << io::sprintf("%9.6f", rmsGap) << std::endl;

		if (sSavePlotable)
		{
			// save to gnuplot file
			std::string const idStr{ io::sprintf("%03d", solnCount) };
			++solnCount;
			std::string const fnameSta{ "tmpSta_" + idStr + ".dat" };
			std::string const fnamePnt{ "tmpPnt_" + idStr + ".dat" };
			saveModel(fnameSta, fnamePnt, roQuintSoln, uvPairs, pntNames);

			// save input as gnuplot-able 'rays'
			std::ofstream ofsNom("tmpNom_" + idStr + ".dat");
			(void)ro::io::gnuplot::saveModelRays(ofsNom, oriPairNom, uvPairs);
			std::ofstream ofsFit("tmpFit_" + idStr + ".dat");
			ro::OriPair const oriPairFit{ roQuintSoln.theSoln.pair() };
			(void)ro::io::gnuplot::saveModelRays(ofsFit, oriPairFit, uvPairs);
		}
	}

	if (sSavePlotable)
	{
		using namespace ro::io::gnuplot;
		std::ofstream ofsNomA("tmpNom_imgA.dat");
		(void)saveImageRays(ofsNomA,  First, oriPairNom, uvPairs);
		std::ofstream ofsNomB("tmpNom_imgB.dat");
		(void)saveImageRays(ofsNomB, Second, oriPairNom, uvPairs);
	}
io::out() << "================" << std::endl;

	return 0;
}
