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
\brief  This file contains main application program demoCylinder
*/


#include "libdat/info.h"
#include "libga/ga.h"
#include "libga/Pose.h"
#include "libgeo/LineSeg.h"
#include "libgeo/sphere.h"
#include "libio/stream.h"
#include "libtri/IsoTille.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>


namespace
{
	using PairZA = std::pair<double, double>;

	//! A simple surface shape (an oblique cone)
	struct SurfGeo
	{
		geo::LineSeg theAxis{};
		ga::BiVector theDirA{};
		ga::BiVector theAzim0{};
		ga::Vector theRad0{};

		static
		constexpr
		double
		fullDepth
			()
		{
			return { 5. };
		}

		static
		geo::LineSeg
		axis
			()
		{
			static ga::Vector const pntBot(100., 200., 300.);
			static ga::Vector const aDir{ ga::e3 };
			return geo::LineSeg(geo::Ray(pntBot, aDir), fullDepth());
		}

		static
		double
		radMag
			( double const & zeta
			)
		{
			double result{ dat::nullValue<double>() };
			if ((0. <= zeta) && (zeta < fullDepth()))
			{
				result = {2. * fullDepth() - zeta };
			}
			return result;
		}

		static
		double
		nomRadius
			()
		{
			return radMag(.5*(0. + fullDepth()));
		}

		SurfGeo
			()
			: theAxis{ axis() }
			, theDirA{ ga::unit(ga::E123 * theAxis.direction()) }
			, theAzim0{ ga::E31 }
			, theRad0{ ga::unit(ga::dot(theAxis.direction(), theAzim0)) }
		{

io::out() << dat::infoString(theAxis, "theAxis") << std::endl;
io::out() << dat::infoString(theDirA, "theDirA") << std::endl;
io::out() << dat::infoString(theAzim0, "theAzim0") << std::endl;
io::out() << dat::infoString(theRad0, "theRad0") << std::endl;

		}

	};

	//! Surface model - radius as function of zLoc,Azim
	struct SurfModel
	{
		SurfGeo theGeo;

		double
		radMagAtZA
			( PairZA const & zaPair
			) const
		{
			double const & zeta = zaPair.first;
			double const & azim = zaPair.second;
			double const dMag
				{ .50  * std::cos(azim)
				+ .125 * std::cos(4.*(azim - .5*math::qtrPi))
				};
			return (theGeo.radMag(zeta) * (1. + dMag));
		}

		ga::Vector
		hPointAtZA
			( PairZA const & zaPair
			) const
		{
		//	double const & zeta = zaPair.first;
			double const & azim = zaPair.second;
			ga::Pose const poseOfAzim(azim * theGeo.theDirA);
			ga::Vector const rHat{ poseOfAzim(theGeo.theRad0) };
			double const radMag{ radMagAtZA(zaPair) };
			return (radMag * rHat);
		}

		ga::Vector
		pointAtZA
			( PairZA const & zaPair
			) const
		{
			double const & zeta = zaPair.first;
			ga::Vector const hPnt{ hPointAtZA(zaPair) };
			ga::Vector const aPnt{ theGeo.theAxis.pointAtDistance(zeta) };
			return (aPnt + hPnt);
		}
	};

	//! A pseudo-random pair of values
	PairZA
	randomPairZA
		( double const & length
		)
	{
		static std::mt19937_64 rgen;
		static std::uniform_real_distribution<double> zDistro
			(-1., 1.25 * length);
		static std::uniform_real_distribution<double> aDistro
			(-math::pi, math::pi);
		return { zDistro(rgen), aDistro(rgen) };
	}

	void
	savePointCloud
		( SurfModel const & model
		, std::string const & fname
		, size_t const & numPnts
		)
	{
		std::ofstream ofs(fname);
		for (size_t nn{0u} ; nn < numPnts ; ++nn)
		{
			PairZA const za{ randomPairZA(model.theGeo.fullDepth()) };
			ga::Vector const pnt{ model.pointAtZA(za) };
			if (dat::isValid(pnt))
			{
				ofs << dat::infoString(pnt) << '\n';
			}
		}
	}

	struct NodePool
	{
		// --- TriTille support

		using PropType = double; // radius
		using value_type = PropType;

		PropType
		operator()
			( tri::NodeNdxType const & ndxI
			, tri::NodeNdxType const & ndxJ
			) const
		{
			tri::IsoGeo const & trigeo = theTriNet.theTileGeo;
			dat::Spot const zaLoc(trigeo.refSpotForIndices({ ndxI, ndxJ }));
			PairZA const zaPair{ zaLoc[0], zaLoc[1] };
			double const radius{ theSurfModel.radMagAtZA(zaPair) };
			return radius;
		}

		// --- Application domain

		SurfModel const theSurfModel;
		tri::IsoTille const theTriNet;

		void
		saveNodeInfo
			( std::string const & fname
			) const
		{
			std::ofstream ofs(fname);
			tri::IsoGeo const & trigeo = theTriNet.theTileGeo;
			for (tri::NodeIterator iter{theTriNet.begin()} ; iter ; ++iter)
			{
				tri::NodeNdxPair const ndxIJ{ iter.indexPair() };
				dat::Spot const zaLoc(trigeo.refSpotForIndices(ndxIJ));
				double const radius{ operator()(ndxIJ.first, ndxIJ.second) };

				ofs
					<< "z,a,Rad:"
					<< " " << dat::infoString(zaLoc)
					<< " " << dat::infoString(radius)
					<< '\n';
			}
		}
	};

}

//! Demonstrate use of an iso-tritille in cylindrical coordinantes.
int
main
	( int const // argc
	, char const * const * const // argv
	)
{
	// simulated environment
	SurfGeo const sgeo{};
	SurfModel const model{ sgeo };

	// ** Save point cloud
	constexpr size_t numPnts{ 16u * 1024u };
	savePointCloud(model, "test_pnts.dat", numPnts);

	// tritille to represent environment surface in cylindrical coordinates
	dat::Range<double> const zRange{ 0., sgeo.fullDepth() };
	dat::Range<double> const aRange{ -math::pi, math::pi };
	tri::Domain const tridom(dat::Area<double>{ zRange, aRange });

	// construct tritille
	double const dz{ .125 };
	double const da{ (1./sgeo.nomRadius()) * math::twoPi };
	tri::IsoGeo::Vec2D const zDir{{ 1., 0. }};
	tri::IsoGeo const trigeo(dz, da, zDir);;
	tri::IsoTille const trinet(trigeo, tridom);

	// ** Save iterator info
	NodePool const triPool{ model, trinet };
	triPool.saveNodeInfo("test_nodes.dat");

	return 0;
}
