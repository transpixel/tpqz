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
\brief  This file contains unit test for ro::sampcon
*/


#include "libro/sampcon.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libgeo/intersect.h"
#include "libio/stream.h"
#include "libro/cast.h"
#include "libro/model.h"
#include "libro/SpinPQ.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace
{

//! Check for common functions
std::string
ro_sampcon_test0
	()
{
	std::ostringstream oss;
	/*
	ro::sampcon const aNull(dat::nullValue<ro::sampcon>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

	//! Save directions as rays in model space
	void
	savePntDeltas
		( std::ostream & oss
		, std::vector<ro::PntPair> const & uvPntPairs
		, ga::Vector const & sta1
		, ga::Vector const & sta2
		)
	{
		assert(sta1.isValid());
		assert(sta2.isValid());

		for (ro::PntPair const & uvPntPair : uvPntPairs)
		{
			if (dat::isValid(uvPntPair))
			{
				ga::Vector const & uPnt = uvPntPair.first;
				ga::Vector const & vPnt = uvPntPair.second;
				ga::Vector const uDelta{ uPnt - sta1 };
				ga::Vector const vDelta{ vPnt - sta2 };
				oss
					<< " " << dat::infoString(sta1)
					<< " " << dat::infoString(uDelta)
					<< " " << dat::infoString(sta2)
					<< " " << dat::infoString(vDelta)
					<< '\n';
			}
		}
	}

	void
	savePoints
		( std::ostream & oss
		, std::vector<ro::PntPair> const & uvPntPairs
		)
	{
		for (ro::PntPair const & uvPntPair : uvPntPairs)
		{
			if (dat::isValid(uvPntPair))
			{
				ga::Vector const & uPnt = uvPntPair.first;
				ga::Vector const & vPnt = uvPntPair.second;
				oss
					<< " " << dat::infoString(uPnt)
					<< " " << dat::infoString(vPnt)
					<< '\n';
			}
		}
	}

	//! Save directions as rays in model space
	void
	saveModelRays
		( std::vector<ro::PairUV> const & uvPairs
		, ga::Rigid const & ori1
		, ga::Rigid const & ori2
		, std::string const & name
		)
	{
		ro::OriPair const roPairIn1{ ori1, ori2 };
		std::vector<ro::PntPair> const uvPntPairs
			{ ro::model::pointPairs(uvPairs, roPairIn1) };
		{
			std::ofstream ofs("dirs" + name + ".dat");
			savePntDeltas(ofs, uvPntPairs, ori1.location(), ori2.location());
		}
		{
			std::ofstream ofs("pnts" + name + ".dat");
			savePoints(ofs, uvPntPairs);
		}
	}

	//! Save directions as rays in model space
	void
	saveModelRaysIn0
		( ro::Pair const & anRO
		, std::vector<ro::PairUV> const & uvPairs
		, std::string const & name
		)
	{
		ga::Rigid const ori1{ anRO.rigid1w0() };
		ga::Rigid const ori2{ anRO.rigid2w0() };
		saveModelRays(uvPairs, ori1, ori2, name);
	}

	//! Save directions as rays in model space
	void
	saveModelRaysIn1
		( ro::Pair const & anRO
		, std::vector<ro::PairUV> const & uvPairs
		, std::string const & name
		)
	{
		static ga::Rigid const ori1{ ga::Rigid::identity() };
		ga::Rigid const ori2{ anRO.rigid2w1() };
		saveModelRays(uvPairs, ori1, ori2, name);
	}

	//! Short named-function to create unit vector
	ga::Vector
	dir
		( double const & xx
		, double const & yy
		, double const & zz
		)
	{
		return ga::unit(ga::Vector(xx, yy, zz));
	}

//! Check solution for a particular test case
std::string
ro_sampcon_test1
	()
{
	std::ostringstream oss;

// #define Case200 // strong solution
// #define Case250a // in the noise (bad condition number?)
#define Case250b // just barely works

#	if defined(Case200)
	std::vector<ro::PairUV> const uvPairs
		{ { dir(0.922,  0.141,  0.361), dir(0.986,  0.156, -0.053) }
		, { dir(0.829,  0.417,  0.372), dir(0.890,  0.453, -0.053) }
		, { dir(0.656,  0.651,  0.382), dir(0.706,  0.706, -0.053) }
		, { dir(0.659, -0.659,  0.361), dir(0.706, -0.706, -0.053) }
		, { dir(0.833, -0.424,  0.354), dir(0.890, -0.453, -0.053) }
		, { dir(0.922, -0.145,  0.358), dir(0.986, -0.156, -0.053) }
		, { dir(0.969,  0.157, -0.189), dir(0.827,  0.134, -0.545) }
		, { dir(0.874,  0.445, -0.193), dir(0.742,  0.382, -0.551) }
		, { dir(0.690,  0.696, -0.198), dir(0.583,  0.588, -0.560) }
		, { dir(0.694, -0.694, -0.189), dir(0.598, -0.592, -0.540) }
		, { dir(0.877, -0.441, -0.189), dir(0.753, -0.379, -0.538) }
		, { dir(0.970, -0.153, -0.189), dir(0.832, -0.127, -0.540) }
		};
	std::pair<double, double> const zLoHi{ -.932, -.778 };
#	endif
#	if defined(Case250a)
	std::vector<ro::PairUV> const uvPairs
		{ { dir(0.970,  0.205,  0.130), dir(0.987,  0.156,  0.030) }
		, { dir(0.885,  0.422, -0.197), dir(0.872,  0.439,  0.215) }
		, { dir(0.689,  0.654,  0.314), dir(0.721,  0.650,  0.238) }
		, { dir(0.702, -0.655,  0.280), dir(0.693, -0.711,  0.119) }
		, { dir(0.985, -0.099,  0.139), dir(0.991, -0.108,  0.073) }
		, { dir(0.958,  0.151, -0.244), dir(0.812,  0.121, -0.571) }
		, { dir(0.862,  0.443, -0.244), dir(0.733,  0.365, -0.573) }
		, { dir(0.682,  0.688, -0.248), dir(0.578,  0.573, -0.580) }
		, { dir(0.685, -0.685, -0.248), dir(0.673, -0.466, -0.574) }
		, { dir(0.846, -0.482, -0.228), dir(0.793, -0.290, -0.536) }
		, { dir(0.958, -0.151, -0.244), dir(0.810, -0.135, -0.571) }
		};
	std::pair<double, double> const zLoHi{ -.520, -.362 };
#	endif
#	if defined(Case250b)
	std::vector<ro::PairUV> const uvPairs
{ { dir(0.970055,  0.204899,  0.130419), dir(0.987367,  0.155666,  0.029561) }
, { dir(0.885064,  0.421754, -0.196941), dir(0.872451,  0.439106,  0.214512) }
, { dir(0.688598,  0.653803,  0.313646), dir(0.721391,  0.650284,  0.238172) }
, { dir(0.701920, -0.655023,  0.279738), dir(0.692891, -0.711087,  0.119408) }
, { dir(0.985349, -0.099133,  0.138782), dir(0.991462, -0.108385,  0.072500) }
, { dir(0.957871,  0.151016, -0.244291), dir(0.812089,  0.120851, -0.570882) }
, { dir(0.862373,  0.443413, -0.244330), dir(0.733466,  0.365192, -0.573291) }
, { dir(0.682007,  0.687925, -0.248245), dir(0.578324,  0.573349, -0.580355) }
, { dir(0.684972, -0.684972, -0.248247), dir(0.673014, -0.466297, -0.574125) }
, { dir(0.845836, -0.482151, -0.228237), dir(0.793076, -0.289702, -0.535820) }
, { dir(0.957871, -0.151015, -0.244291), dir(0.809880, -0.134865, -0.570882) }
};
	std::pair<double, double> const zLoHi{ -.520, -.362 };
#	endif

	// configure nominal orientation info
	ga::Vector const staLo(zLoHi.first  * ga::e3);
	ga::Vector const staHi(zLoHi.second * ga::e3);
	ga::Rigid const eoLo(staLo, ga::Pose::identity());
	ga::Rigid const eoHi(staHi, ga::Pose::identity());
	ro::OriPair const roPairNom{ eoLo, eoHi };

	// compute consensus solution
	constexpr size_t numRandSamps{ 252 };
	ro::sampcon::BestSoln const roSoln
		{ ro::sampcon::bySample(uvPairs, roPairNom, numRandSamps) };
	ro::OriPair const & roPairFit = roSoln.theOriPair;

	if (! dat::isValid(roPairFit))
	{
		oss << "Failure of fit test" << std::endl;
	}

	// saveModelRaysIn1(roNom, uvPairs, "Nom");
	// saveModelRaysIn1(roFit, uvPairs, "Fit");

	/*
	// display measurement summary
	for (ro::PairUV const & uvPair : uvPairs)
	{
		ga::Vector const & uDir = uvPair.first;
		ga::Vector const & vDir = uvPair.second;

		io::out()
			<< " " << dat::infoString(staLo, "staLo")
			<< " " << dat::infoString(uDir, "uDirLo")
			<< " " << dat::infoString(staHi, "staHi")
			<< " " << dat::infoString(vDir, "vDirHi")
			<< std::endl;
	}

	// display nominal info
	io::out() << std::endl;
	io::out() << dat::infoString(eoLo, "eoLo") << std::endl;
	io::out() << dat::infoString(eoHi, "eoHi") << std::endl;

	// display ro pairs
	io::out() << std::endl;
	io::out() << roNom.infoString("roNom") << std::endl;
	io::out() << roFit.infoString("roFit") << std::endl;

	// display solution info
	io::out() << std::endl;
	io::out() << roNom.infoStringParms("roNom")
		<< " " << dat::infoString(roNom.parmRMS(), "rms")
		<< std::endl;
	io::out() << roFit.infoStringParms("roFit")
		<< " " << dat::infoString(roFit.parmRMS(), "rms")
		<< std::endl;
	io::out() << roFit.pose1w0().infoString("fit1w0") << std::endl;
	io::out() << roFit.pose2w0().infoString("fit2w0") << std::endl;
	io::out() << std::endl;
	*/

	return oss.str();
}


}

//! Unit test for ro::sampcon
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ro_sampcon_test0();
	oss << ro_sampcon_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
