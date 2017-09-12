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

//#define UsePlane
#define UseRadius

	using PairZA = std::pair<double, double>;

	//! Cast operator
	static
	PairZA
	zaPairForSpot
		( dat::Spot const & zaSpot
		)
	{
		return PairZA
			{ zaSpot[0]
			, math::principalAngle(zaSpot[1])
			};
	}

	//! A simple surface shape (an oblique cone)
	struct SurfGeo
	{
		geo::LineSeg theAxis{};
		ga::BiVector theDirA{};
		ga::BiVector theAzim0{};
		ga::Vector theRad0{};

	private:

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
				static double const halfDepth{ .5 * fullDepth() };
				if (halfDepth <= zeta)
				{
					result = {2. * fullDepth() - zeta };
				}
				else
				{
					result = fullDepth(); // vertical profile
				}
			}
			return result;
		}

		static
		double
		wiggleFrac
			( double const & azim
			)
		{
			return
				{ .50  * std::cos(azim)
				+ .125 * std::cos(4.*(azim - .5*math::qtrPi))
				};
		}

		static
		double
		wiggleRadius
			( double const & zeta
			, double const & azim
			)
		{
			return radMag(zeta) * (1. + wiggleFrac(azim));
		}

	public:

		static
		constexpr
		double
		fullDepth
			()
		{
			return { 5. };
		}

		double
		radMagAtZA
			( PairZA const & zaPair
			) const
		{
			double rad{ dat::nullValue<double>() };
			double const & zeta = zaPair.first;
			// return radMag(zeta); // circular section
			double const & azim = zaPair.second;
			if ((0. <= azim) && (azim < math::halfPi))
			{
				double const rho0{ wiggleRadius(zeta, 0.) };
				double const rho1{ wiggleRadius(zeta, math::halfPi) };
				double const beta{ std::atan2(rho1, rho0) };
				if (azim < beta)
				{
					rad = rho0 / std::cos(azim);
				}
				else
				{
					rad = rho1 / std::cos(math::halfPi - azim);
				}
			}
			else
			{
				rad = wiggleRadius(zeta, azim);
			}
			return rad;
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
		{ }

	}; // SurfGeo

	struct PropType
	{
		//! Radius in cylindrical coordinates
		double theRad{ dat::nullValue<double>() };

		//! 3D location in Cartesian frame
		ga::Vector thePnt{};

		PropType
			() = default;

		explicit
		PropType
			( double const & rad
			, ga::Vector const & pnt
			)
			: theRad{ rad }
			, thePnt{ pnt }
		{ }

		std::string
		infoString
			( std::string const & title = {}
			) const
		{
			std::ostringstream oss;
			if (! title.empty())
			{
				oss << title << " ";
			}
			oss
				<< dat::infoString(theRad, "theRad")
				<< " " << dat::infoString(thePnt, "thePnt")
				;
			return oss.str();
		}

	}; // PropType

	PropType
	operator+
		( PropType const & propA
		, PropType const & propB
		)
	{
		return PropType
			( propA.theRad + propB.theRad
			, propA.thePnt + propB.thePnt
			);
	}

	PropType
	operator*
		( double const & scale
		, PropType const & propB
		)
	{
		return PropType
			( scale * propB.theRad
			, scale * propB.thePnt
			);
	}

	//! Surface model - radius as function of zLoc,Azim
	class SurfModel
	{
		SurfGeo theGeo;

		double
		radiusAtZA
			( PairZA const & zaPair
			) const
		{
			return theGeo.radMagAtZA(zaPair);
		}

		ga::Vector
		hDirAtZA
			( PairZA const & zaPair
			) const
		{
			double const & azim = zaPair.second;
			ga::Pose const poseOfAzim(-azim * theGeo.theDirA);
			ga::Vector const rHat{ poseOfAzim(theGeo.theRad0) };
			return rHat;
		}


		ga::Vector
		hVectorAtZA
			( PairZA const & zaPair
			) const
		{
			return (radiusAtZA(zaPair) * hDirAtZA(zaPair));
		}

		ga::Vector
		axialPointAtZA
			( PairZA const & zaPair
			) const
		{
			double const & zeta = zaPair.first;
			ga::Vector const aPnt{ theGeo.theAxis.pointAtDistance(zeta) };
			return aPnt;
		}

	public:

		SurfModel
			( SurfGeo const & geo
			)
			: theGeo{ geo }
		{ }

		//! A pseudo-random pair of values
		PairZA
		randomPairZA
			() const
		{
			static std::mt19937_64 rgen;
			static std::uniform_real_distribution<double> zDistro
				(-1., 1.25 * theGeo.fullDepth());
			static std::uniform_real_distribution<double> aDistro
				(-math::pi, math::pi);
			return { zDistro(rgen), aDistro(rgen) };
		}

		//! Simulation/evaluation support
		ga::Vector
		idealPointAtZA
			( PairZA const & zaPair
			) const
		{
			return (axialPointAtZA(zaPair) + hVectorAtZA(zaPair));
		}

		//! Fundamental surface representation property
		PropType
		propertyAtZA
			( PairZA const & zaPair
			) const
		{
			return PropType(radiusAtZA(zaPair), idealPointAtZA(zaPair));
		}

		//! 3D point reconstructed from surface property
#		if defined(UseRadius)
		ga::Vector
		pointFromProperty
			( dat::Spot const & zaSpot
			, PropType const & sample
			) const
		{
			PairZA const zaPair{ zaPairForSpot(zaSpot) };
			ga::Vector const hDir{ hDirAtZA(zaPair) };
			return (axialPointAtZA(zaPair) + sample.theRad*hDir);
		}
#		endif
#		if defined(UsePlane)
		ga::Vector
		pointFromProperty
			( dat::Spot const & // zaSpot
			, PropType const & sample
			) const
		{
			return sample.thePnt;
		}
#		endif

	};

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
			PairZA const za{ model.randomPairZA() };
			ga::Vector const pnt{ model.idealPointAtZA(za) };
			if (dat::isValid(pnt))
			{
				ofs << dat::infoString(pnt) << '\n';
			}
		}
	}

	struct NodePool
	{
		//
		// --- TriTille interaction
		//

		using value_type = PropType;

		PairZA
		zaPairForNodeKey
			( tri::NodeKey const & ndxIJ
			) const
		{
			tri::IsoGeo const & trigeo = theTriNet.theTileGeo;
			dat::Spot const zaLoc(trigeo.refSpotForNodeKey(ndxIJ));
			return zaPairForSpot(zaLoc);;
		}

		PropType
		operator()
			( tri::NodeKey const & ndxIJ
			) const
		{
			PropType const sample
				{ theSurfModel.propertyAtZA(zaPairForNodeKey(ndxIJ)) };
			return sample;
		}

		//
		// --- Application interaction
		//

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
				tri::NodeKey const ndxIJ{ iter.indexPair() };
				PropType const sample{ operator()(ndxIJ) };
				ga::Vector const & pnt = sample.thePnt;

				PairZA const zaPair{ zaPairForNodeKey(ndxIJ) };
				double const & radius = sample.theRad;
//				theSurfModel.pointAt(zaPair);
//				theSurfModel.pointAt(zaPair);

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
			, std::string const & fnameAll
			) const
		{
			std::ofstream ofsMu(fnameMu);
			std::ofstream ofsNu(fnameNu);
			std::ofstream ofsDi(fnameDi);
			std::ofstream ofsAll(fnameAll);
			for (tri::NodeIterator iter{theTriNet.begin()} ; iter ; ++iter)
			{
				using NdxPair = tri::NodeKey;
				NdxPair const ndx0{ iter.indexPair() };
				NdxPair const ndx1{ ndx0.first + 1L, ndx0.second };
				NdxPair const ndx2{ ndx0.first + 1L, ndx0.second + 1L };

				ga::Vector const pnt0{ operator()(ndx0).thePnt };
				ga::Vector const pnt1{ operator()(ndx1).thePnt };
				ga::Vector const pnt2{ operator()(ndx2).thePnt };
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

					ofsAll
						<< dat::infoString(pnt0) << '\n'
						<< dat::infoString(pnt1) << '\n'
						<< dat::infoString(pnt2) << '\n'
						<< dat::infoString(pnt0) << '\n'
						<< "\n\n";
				}

			}
		}

	}; // NodePool

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
	SurfModel const model(sgeo);

	// ** Save point cloud
	constexpr size_t numPnts{ 16u * 1024u };
	savePointCloud(model, "test_pnts.dat", numPnts);

	// determine mesh spacing
	double const numAzim{ 4.* (std::floor(sgeo.nomRadius()) + 1.) };
//double const numAzim{ .5* (std::floor(sgeo.nomRadius()) + 1.) };
	double const da{ (1./numAzim) * math::twoPi };
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
		("test_edgesMu.dat", "test_edgesNu.dat", "test_edgesDi.dat"
		, "test_edgesAll.dat");

	// interpolate surface on regular grid
	constexpr size_t zHigh{ 100u };
	constexpr size_t aWide{ 2u*zHigh };
	dat::Extents const gridSize{ zHigh, aWide };
	dat::grid<ga::Vector> zaGrid(gridSize);
	std::fill(zaGrid.begin(), zaGrid.end(), ga::Vector{});
	math::MapSizeArea const map(gridSize, zaArea);
	std::ofstream ofsGrid("test_grid.dat");
	for (dat::ExtentsIterator iter{gridSize} ; iter ; ++iter)
	{
		dat::RowCol const & gridRowCol = *iter;

		// get domain location for sample raster element
		dat::Spot const rcSpot(dat::cast::Spot(gridRowCol));
		dat::Spot const zaSpot(map.xyAreaSpotFor(rcSpot));

		// interpolate point at this location
		PropType const isamp
			{ trinet.linearInterpWithCheck(zaSpot, triPool) };
		ga::Vector const ipnt{ model.pointFromProperty(zaSpot, isamp) };
		if (dat::isValid(ipnt))
		{
			zaGrid(gridRowCol) = ipnt;
			ofsGrid
				<< "z,a,GridPnt:"
				<< " " << dat::infoString(zaSpot)
				<< " " << dat::infoString(ipnt)
				<< '\n';
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
