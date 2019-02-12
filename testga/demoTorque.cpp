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
\brief  This file contains main application program demoTorque
*/


#include "libapp/Usage.h"

#include "build/version.h"
#include "libio/stream.h"

#include "libga/ga.h"
#include "libga/Pose.h"
#include "libgeo/sphere.h"
#include "libmath/math.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>


namespace
{
	constexpr double const numPerTurn{ 128. };
	constexpr double const sDeltaArc{ math::twoPi / numPerTurn };

	using Zenith = double;
	using Radius = double;
	using ZenRad = std::pair<Zenith, Radius>;

	//! Lattitude curves on sphere
	std::vector<ZenRad>
	lattitudeParms
		( double const & delZen = { sDeltaArc }
		)
	{
		std::vector<ZenRad> zrs;
		for (double zen{0.} ; zen <= math::pi ; zen += delZen)
		{
			double const rad{ std::sin(zen) };
			zrs.emplace_back(ZenRad{ zen, rad });
		}
		return zrs;
	}

	std::vector<double>
	azimuthValuesFor
		( double const & radius
		, double const & deltaArc = { sDeltaArc }
		)
	{
		std::vector<double> azims{};
		double const delAzim{ deltaArc / radius };
		for (double azim{0.} ; azim < math::twoPi ; azim += delAzim)
		{
			azims.emplace_back(azim);
		}
		return azims;
	}

	//! Directions distributed over sphere
	std::vector<ga::Vector>
	dirsOnSphere
		()
	{
		std::vector<ga::Vector> dirs{};
		std::vector<ZenRad> const lats{ lattitudeParms() };
		for (ZenRad const & lat : lats)
		{
			double const & zen = lat.first;
			double const & hRad = lat.second;
			std::vector<double> const azims{ azimuthValuesFor(hRad) };
			for (double const & azim : azims)
			{
				dirs.emplace_back(geo::sphere::directionFromAZ(azim, zen));
			}
		}
		return dirs;
	}

	//! Coefficients on partial derivatives of merit function w.r.t. x_k
	std::array<ga::Vector, 3u>
	dChiCo
		( std::vector<ga::Vector> const & mSamps
		)
	{
		std::array<ga::Vector, 3u> mOrthos{};
		std::fill(mOrthos.begin(), mOrthos.end(), ga::vZero);
		double count{ 0. };
		for (ga::Vector const & mSamp : mSamps)
		{
			double const & m1 = mSamp[0];
			double const & m2 = mSamp[1];
			double const & m3 = mSamp[2];

			using ga::Vector;
			Vector const d1((m2*m2-m3*m3),        -m1*m2,         m3*m1);
			Vector const d2(        m1*m2, (m3*m3-m1*m1),        -m2*m3);
			Vector const d3(       -m3*m1,         m2*m3, (m1*m1-m2*m2));
			mOrthos[0] = mOrthos[0] + d1;
			mOrthos[1] = mOrthos[1] + d2;
			mOrthos[2] = mOrthos[2] + d3;
			count += 1.;
		}
		mOrthos[0] = (1./count) * mOrthos[0];
		mOrthos[1] = (1./count) * mOrthos[1];
		mOrthos[2] = (1./count) * mOrthos[2];
		return mOrthos;
	}

	//! Write vector end point in gnuplot friendly format
	void
	writeVector
		( std::ostream & oss
		, ga::Vector const & vecTo
		, ga::Vector const & vecBeg = { ga::vZero }
		)
	{
		ga::Vector const delta{ vecTo - vecBeg };
		oss
			<< dat::infoString(vecBeg)
			<< " " << dat::infoString(delta)
			<< '\n';
	}

	//! Write circle representing plane perpendicular to normal direction
	void
	writeCircle
		( std::ostream & oss
		, ga::Vector const & norm
		)
	{
		ga::Vector const perp{ ga::anyPerpendicularDir(norm) };
		ga::BiVector const pHat{ ga::unit(ga::E123 * norm) };
		constexpr double const dAngMag{ (1./8.)*math::pi };
		ga::Vector const r0{ ga::magnitude(norm) * ga::unit(perp) };
		writeVector(oss, r0);
		for (double ang{0.} ; ang <= math::twoPi ; ang += dAngMag)
		{
			ga::Pose const pose(ang * pHat);
			ga::Vector const rvec{ pose(r0) };
			writeVector(oss, rvec);
		}
		writeVector(oss, r0);
	}

	//! Sum of squared torques on mSamps for (unit) force in direciton dir
	double
	responseAtDir
		( ga::Vector const & dir
		, std::vector<ga::Vector> const & mSamps
		)
	{
		double respo{ dat::nullValue<double>() };
		double sumMagSq{ 0. };
		double count{ 0. };
		for (ga::Vector const & mSamp : mSamps)
		{
			ga::BiVector const biv{ ga::wedge(dir, mSamp) };
			sumMagSq += (biv * biv.reverse()).theS.theValue;
			count += 1.;
		}
		respo = sumMagSq / count;
		return respo;
	}

	//! Collection of responseAtDir() for each dir
	std::vector<double>
	responseMagnitudes
		( std::vector<ga::Vector> const & dirs
		, std::vector<ga::Vector> const & mSamps
		)
	{
		std::vector<double> mags;
		for (ga::Vector const & dir : dirs)
		{
			mags.emplace_back(responseAtDir(dir, mSamps));
		}
		return mags;
	}

	std::pair<ga::Vector, ga::Vector>
	pairMinMaxDir
		( std::vector<ga::Vector> const & dirs
		, std::vector<double> const & respoMags
		)
	{
		std::pair<ga::Vector, ga::Vector> mmDirPair{};
		std::vector<double>::const_iterator const itMin
			{ std::min_element(respoMags.begin(), respoMags.end()) };
		assert(respoMags.end() != itMin);
		std::vector<double>::const_iterator const itMax
			{ std::max_element(respoMags.begin(), respoMags.end()) };
		assert(respoMags.end() != itMax);
		size_t const ndxMin
			{ (size_t)(std::distance(respoMags.begin(), itMin)) };
		size_t const ndxMax
			{ (size_t)(std::distance(respoMags.begin(), itMax)) };
		ga::Vector const dirMin{ dirs[ndxMin] };
		ga::Vector const dirMax{ dirs[ndxMax] };
		mmDirPair = { dirMin, dirMax };
		return mmDirPair;
	}

	ga::Vector
	dirMostAligned
		( ga::Vector const & dir
		, std::vector<ga::Vector> const & mSamps
		)
	{
		ga::Vector pmDir{ dir };
		double sumDots{ 0. };
		for (ga::Vector const & mSamp : mSamps)
		{
			double const dot{ ga::dot(dir, mSamp).theValue };
			sumDots += dot;
		}
		if (sumDots < 0.)
		{
			pmDir = -pmDir;
		}
		return pmDir;
	}

	std::array<double, 3u>
	valuePartials
		( std::array<ga::Vector, 3u> const & mOrthos
		, ga::Vector const & dirX
		)
	{
		return std::array<double, 3u>
			{ mOrthos[0][0]*dirX[0]
			+ mOrthos[0][1]*dirX[1]
			+ mOrthos[0][2]*dirX[2]
			, mOrthos[1][0]*dirX[0]
			+ mOrthos[1][1]*dirX[1]
			+ mOrthos[1][2]*dirX[2]
			, mOrthos[2][0]*dirX[0]
			+ mOrthos[2][1]*dirX[1]
			+ mOrthos[2][2]*dirX[2]
			};
	}
}

//! Exploration of force to minimize squared magnituede of torques
int
main
	( int const // argc
	, char const * const * const // argv
	)
{
	// create a collection of samples
	std::vector<ga::Vector> const mSamps
		{ ga::unit(ga::Vector( 1.,  0.,  0.))
		, ga::unit(ga::Vector( 1.,  1.,  0.))
		, ga::unit(ga::Vector( 0.,  1.,  0.))
		, ga::unit(ga::Vector( 1.,  1.,  .5))
	//	, ga::unit(ga::Vector( 1.,  1.,  1.))
	//	, ga::unit(ga::Vector(-1.,  0.,  0.))
//		, ga::unit(ga::Vector( 0., -1.,  0.))
//		, ga::unit(ga::Vector( 0.,  0.,  1.))
//		, ga::unit(ga::Vector( 0.,  0., -1.))
		};

	std::vector<ga::Vector> const dirs{ dirsOnSphere() };
	std::array<ga::Vector, 3u> const mOrthos{ dChiCo(mSamps) };

	ga::Vector const mDiag(mOrthos[0][0], mOrthos[1][1], mOrthos[2][2]);
	ga::Vector const mTriA
		( mOrthos[1][2] - mOrthos[2][1]
		, mOrthos[2][0] - mOrthos[0][2]
		, mOrthos[0][1] - mOrthos[1][0]
		);
//	ga::Vector const mTriB(mOrthos[1][2], mOrthos[2][0], mOrthos[0][1]);

	// get min/max response directions
	std::vector<double> const respoMags{ responseMagnitudes(dirs, mSamps) };
	std::pair<ga::Vector, ga::Vector> const mmDirPair
		{ pairMinMaxDir(dirs, respoMags) };
	ga::Vector const dirMin{ dirMostAligned(mmDirPair.first, mSamps) };
	ga::Vector const dirMax{ dirMostAligned(mmDirPair.second, mSamps) };
	ga::Vector const dir3rd{ ga::cross(dirMin, dirMax) };

	std::ofstream ofsS{ "foo_samps.dat" };
	for (ga::Vector const & mSamp : mSamps)
	{
		writeVector(ofsS, mSamp);
	}

	std::ofstream ofsE{ "foo_extrema.dat" };
	writeVector(ofsE, dirMin);
	writeVector(ofsE, dirMax);
	writeVector(ofsE, dir3rd);

	std::ofstream ofsR{ "foo_response.dat" };
	for (ga::Vector const & dir : dirs)
	{
		ga::Vector const respo{ responseAtDir(dir, mSamps) * dir };
		ofsR << dat::infoString(respo) << '\n';
	}

	std::ofstream ofsM{ "foo_orthos.dat" };
	writeVector(ofsM, mOrthos[0]);
	writeVector(ofsM, mOrthos[1]);
	writeVector(ofsM, mOrthos[2]);

	std::ofstream ofsP{ "foo_planes.dat" };
	writeCircle(ofsP, mOrthos[0]); ofsP << "\n\n";
	writeCircle(ofsP, mOrthos[1]); ofsP << "\n\n";
	writeCircle(ofsP, mOrthos[2]); ofsP << "\n\n";

	std::ofstream ofs1{ "foo_matrix.dat" };
	writeVector(ofs1, mDiag); ofs1 << "\n\n";
	writeVector(ofs1, mTriA); ofs1 << "\n\n";
//	writeCircle(ofs1, mTriA); ofs1 << "\n\n";
//	writeCircle(ofs1, mTriB); ofs1 << "\n\n";

	std::array<double, 3u> const dChiMin{ valuePartials(mOrthos, dirMin) };
	std::array<double, 3u> const dChiMax{ valuePartials(mOrthos, dirMax) };
	std::array<double, 3u> const dChi3rd{ valuePartials(mOrthos, dir3rd) };

	io::out() << dat::infoString(dChiMin, "dChiMin") << std::endl;
	io::out() << dat::infoString(dChiMax, "dChiMax") << std::endl;
	io::out() << dat::infoString(dChi3rd, "dChi3rd") << std::endl;

	return 0;
}
