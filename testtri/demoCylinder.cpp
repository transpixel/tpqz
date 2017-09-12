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


#include "libdat/Extents.h"
#include "libdat/ExtentsIterator.h"
#include "libdat/grid.h"
#include "libdat/info.h"
#include "libga/ga.h"
#include "libga/Pose.h"
#include "libgeo/LineSeg.h"
#include "libgeo/sphere.h"
#include "libio/stream.h"
#include "libmath/MapSizeArea.h"
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
result = fullDepth();
			}
			return result;
		}

		double
		radMagAtZA
			( PairZA const & zaPair
			) const
		{
			double const & zeta = zaPair.first;
return radMag(zeta);
			double const & azim = zaPair.second;
			double const dMag
				{ .50  * std::cos(azim)
				+ .125 * std::cos(4.*(azim - .5*math::qtrPi))
				};
			return (radMag(zeta) * (1. + dMag));
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

		ga::Vector
		hPointAtZA
			( PairZA const & zaPair
			) const
		{
		//	double const & zeta = zaPair.first;
			double const & azim = zaPair.second;
			ga::Pose const poseOfAzim(azim * theGeo.theDirA);
			ga::Vector const rHat{ poseOfAzim(theGeo.theRad0) };
			double const radMag{ theGeo.radMagAtZA(zaPair) };
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

		using PropType = ga::Vector;
		using value_type = PropType;

		PairZA
		zaPairForIndices
			( tri::NodeNdxPair const & ndxIJ
			) const
		{
			tri::IsoGeo const & trigeo = theTriNet.theTileGeo;
			dat::Spot const zaLoc(trigeo.refSpotForIndices(ndxIJ));
			PairZA const zaPair
				{ zaLoc[0]
				, math::principalAngle(zaLoc[1])
				};
			return zaPair;
		}

		PropType
		operator()
			( tri::NodeNdxPair const & ndxIJ
			) const
		{
			return theSurfModel.pointAtZA(zaPairForIndices(ndxIJ));
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
			for (tri::NodeIterator iter{theTriNet.begin()} ; iter ; ++iter)
			{
				tri::NodeNdxPair const ndxIJ{ iter.indexPair() };
				ga::Vector const pnt{ operator()(ndxIJ) };

				PairZA const zaPair{ zaPairForIndices(ndxIJ) };
				double const radius{ theSurfModel.theGeo.radMagAtZA(zaPair) };

				ofs
					<< "z,a,Rad:"
					<< " " << dat::infoString(zaPair)
					<< " " << dat::infoString(radius)
					<< " " << dat::infoString(pnt, "pnt")
					<< '\n';
			}
		}

		void
		saveEdgeInfo
			( std::string const & fnameMu
			, std::string const & fnameNu
			, std::string const & fnameDi
			) const
		{
			std::ofstream ofsMu(fnameMu);
			std::ofstream ofsNu(fnameNu);
			std::ofstream ofsDi(fnameDi);
			for (tri::NodeIterator iter{theTriNet.begin()} ; iter ; ++iter)
			{
				using NdxPair = tri::NodeNdxPair;
				NdxPair const ndx0{ iter.indexPair() };
				NdxPair const ndx1{ ndx0.first + 1L, ndx0.second };
				NdxPair const ndx2{ ndx0.first + 1L, ndx0.second + 1L };

				ga::Vector const pnt0{ operator()(ndx0) };
				ga::Vector const pnt1{ operator()(ndx1) };
				ga::Vector const pnt2{ operator()(ndx2) };
				if (pnt0.isValid() && pnt1.isValid() && pnt2.isValid())
				{
					ofsMu
						<< dat::infoString(pnt0) << '\n'
						<< dat::infoString(pnt1) << '\n'
						<< "\n\n";
					ofsNu
						<< dat::infoString(pnt1) << '\n'
						<< dat::infoString(pnt2) << '\n'
						<< "\n\n";
					ofsDi
						<< dat::infoString(pnt2) << '\n'
						<< dat::infoString(pnt0) << '\n'
						<< "\n\n";
				}

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

	// determine mesh spacing
	double const numPartsAzim{ std::floor(sgeo.nomRadius()) + 1. };
//double const numPartsAzim{ 4. };
	double const da{ (1./numPartsAzim) * math::twoPi };
	double const dz{ da };

	// tritille to represent environment surface in cylindrical coordinates
	dat::Range<double> const zRange{ 0., sgeo.fullDepth() };
	double const aEdge{ math::pi + 3.*da }; // pad for azimuth wrap
	dat::Range<double> const aRange{ -aEdge, aEdge };
	dat::Area<double> const zaArea{ zRange, aRange };
	tri::Domain const tridom(zaArea);

	// construct tritille
	tri::IsoGeo::Vec2D const zDir{{ 1., 0. }};
	tri::IsoGeo const trigeo(dz, da, zDir);;
	tri::IsoTille const trinet(trigeo, tridom);

io::out() << dat::infoString(trigeo, "trigeo") << std::endl;

	// ** Save iterator info
	NodePool const triPool{ model, trinet };
	triPool.saveNodeInfo("test_nodes.dat");
	triPool.saveEdgeInfo
		("test_edgesMu.dat", "test_edgesNu.dat", "test_edgesDi.dat");

// TODO - add mesh export (e.g. nodes, edges, faces, ?)

	// interpolate surface on regular grid
	constexpr size_t zHigh{ 128u };
	constexpr size_t aWide{ 2u*zHigh };
	dat::Extents const gridSize{ zHigh, aWide };
	dat::grid<ga::Vector> zaGrid(gridSize);
	std::fill(zaGrid.begin(), zaGrid.end(), ga::Vector{});
	math::MapSizeArea const map(gridSize, zaArea);
	for (dat::ExtentsIterator iter{gridSize} ; iter ; ++iter)
	{
		dat::RowCol const & gridRowCol = *iter;

		// get domain location for sample raster element
		dat::Spot const rcSpot(dat::cast::Spot(gridRowCol));
		dat::Spot const zaSpot(map.xyAreaSpotFor(rcSpot));

		// interpolate point at this location
		ga::Vector const ipnt{ trinet.linearInterpWithCheck(zaSpot, triPool) };
		if (dat::isValid(ipnt))
		{
			zaGrid(gridRowCol) = ipnt;
		}
	}

	std::ofstream ofs("test_interp.dat");
	for (dat::ExtentsIterator iter{zaGrid.hwSize()} ; iter ; ++iter)
	{
		dat::RowCol const & gridRowCol = *iter;

		// interpolate point at this location
		ga::Vector const & ipnt = zaGrid(gridRowCol);
		if (dat::isValid(ipnt))
		{
			ofs << dat::infoString(ipnt) << '\n';
		}
	}

	return 0;
}
