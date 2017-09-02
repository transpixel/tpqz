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
\brief  This file contains main application program demoSquare
*/


#include "libtri/IsoGeo.h"
#include "libtri/IsoTille.h"
#include "libtri/NodeIterator.h"

#include "libdat/cast.h"
#include "libdat/ExtentsIterator.h"
#include "libdat/grid.h"
#include "libdat/validity.h"
#include "libmath/MapSizeArea.h"

#include "libio/stream.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <map>


namespace
{

namespace sample
{
	//! (fictitious) surface property
	struct DataType
	{
		std::array<double, 3u> theABC
			{{ dat::nullValue<double>()
			 , dat::nullValue<double>()
			 , dat::nullValue<double>()
			}};

		DataType
			() = default;

		DataType
			( double const & aa
			, double const & bb
			, double const & cc
			)
			: theABC{{ aa, bb, cc }}
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
				<< "abc:"
				<< " " << dat::infoString(theABC[0] + theABC[1] + theABC[2])
				/*
				<< " " << dat::infoString(theABC[0])
				<< " " << dat::infoString(theABC[1])
				<< " " << dat::infoString(theABC[2])
				*/
				;
			return oss.str();
		}
	};

	//! DataType: multiplication
	DataType
	operator*
		( double const & scale
		, DataType const & samp
		)
	{
		DataType result;
		using dat::operator*;
		result.theABC = scale * samp.theABC;
		return result;
	}

	//! DataType: addition
	DataType
	operator+
		( DataType const & samp1
		, DataType const & samp2
		)
	{
		DataType result;
		using dat::operator+;
		result.theABC = samp1.theABC + samp2.theABC;
		return result;
	}

	//! "Database" of DataTypes keyed to tritille lattice node indices
	struct SamplePool
	{
		using value_type = DataType;
		using KeyType = std::pair<long, long>;

		std::map<KeyType, DataType> theSamps;

		void
		addSample
			( DataType const & sample
			, KeyType const & key
			)
		{
			theSamps[key] = sample;
		}

		DataType
		operator()
			( long const & keyI
			, long const & keyJ
			) const
		{
			DataType samp;
			KeyType const key{ keyI, keyJ };
			std::map<KeyType, DataType>::const_iterator
				const itFind{ theSamps.find(key) };
			if (theSamps.end() != itFind)
			{
				samp = itFind->second;
			}
			return samp;
		}

	};

	// Planar "surface"
	DataType
	valueOnPlaneFor
		( dat::Spot const & xyLoc
		)
	{
		constexpr double bias{ 7000. };
		double const xRamp{  10.*xyLoc[0] };
		double const yRamp{ 100.*xyLoc[1] };
		return DataType(bias, xRamp, yRamp);
	}

	/*
	// Planar "surface"
	DataType
	valueOnPlaneFor
		( std::pair<long, long> const & keyIJ
		)
	{
		constexpr double bias{ 7000. };
		double const xRamp{  10.*double(keyIJ.first) };
		double const yRamp{ 100.*double(keyIJ.second) };
		return DataType(bias, xRamp, yRamp);
	}
	*/

	//! Generate samples over domain - (from planar surface)
	SamplePool
	poolOfSamples
		( tri::Domain const & domain
		, tri::IsoGeo const & trigeo
		)
	{
		sample::SamplePool pool;
		for (tri::NodeIterator iter(trigeo, domain) ; iter ; ++iter)
		{
			SamplePool::KeyType const key(iter.indexPair());
			tri::IsoGeo::QuantPair const fracPair
				(trigeo.fracPairForIndices(key.first, key.second));
			dat::Spot const xyLoc(trigeo.refSpotForFracPair(fracPair));
			pool.addSample(valueOnPlaneFor(xyLoc), key);
		}
		return pool;
	}
}

}

//! Demonstrate iso-tritille tiling of a square region
int
main
	( int const // argc
	, char const * const * const // argv
	)
{
	// define iso-tritille geometry
	constexpr double deltaHigh{ 1./10. };
	constexpr double deltaWide{ 1./20. };
	constexpr std::array<double, 2u> aDir{{ 0., 1. }};
	tri::IsoGeo const trigeo(deltaHigh, deltaWide, aDir);

	// construct tritille
	tri::IsoTille const trinet(trigeo);

io::out() << std::endl;
io::out() << dat::infoString(trinet, "trinet") << std::endl;
io::out() << std::endl;

	dat::Range<double> xRange{ -1., 1. };
	dat::Range<double> yRange{ -1., 1. };
	dat::Area<double> xyArea{ xRange, yRange };
	tri::Domain const xyDomain{ xyArea };

	// generate samples at each tritille node
	sample::SamplePool const samples(sample::poolOfSamples(xyDomain, trigeo));

	// check evaluation directly at sample locations
	std::ofstream ofs("testExpGot.dat");
	for (tri::NodeIterator iter(trigeo, xyDomain) ; iter ; ++iter)
	{
		std::pair<long, long> const mnNdxs{ iter.indexPair() };
		tri::IsoGeo::QuantPair const fracPair
			(trigeo.fracPairForIndices(mnNdxs.first, mnNdxs.second));
		dat::Spot const xyLoc(trigeo.refSpotForFracPair(fracPair));
		sample::DataType const expSamp{ sample::valueOnPlaneFor(xyLoc) };

		if (xyDomain.contains(xyLoc))
		{
			sample::DataType const gotSamp{ trinet(xyLoc, samples) };
			dat::Spot const mnLoc(trigeo.tileSpotForFracPair(fracPair));
			io::out()
				<< " " << dat::infoString(mnLoc, "r.mnLoc")
				<< " " << dat::infoString(xyLoc, "r.xyLoc")
				<< " " << dat::infoString(expSamp, "r.exp")
				<< " " << dat::infoString(gotSamp, "r.got")
				<< std::endl;

			ofs
				<< " " << dat::infoString(mnLoc, "mnLoc")
				<< " " << dat::infoString(xyLoc, "xyLoc")
				<< " " << dat::infoString(expSamp, "expSamp")
				<< " " << dat::infoString(gotSamp, "gotSamp")
				<< '\n';
		}
	}

	// interpolate tritille samples at raster grid locations
	dat::Area<double> const rngArea{ trigeo.tileAreaForRefArea(xyDomain) };
	dat::grid<sample::DataType> surfSamps(17u, 19u);
	math::MapSizeArea const map(surfSamps.hwSize(), rngArea);
	for (dat::ExtentsIterator iter{surfSamps.hwSize()} ; iter ; ++iter)
	{
		// get domain location for sample raster element
		dat::RowCol const & sampRowCol = *iter;
		dat::Spot const sampSpot(dat::cast::Spot(sampRowCol));
		dat::Spot const surfSpot(map.xyAreaSpotFor(sampSpot));

		// interpolate surface properties
		sample::DataType const sampValue(trinet(surfSpot, samples));
	}

	// interpolate surface value at raster grid locations

	io::out() << dat::infoString(trigeo, "trigeo") << std::endl;
	io::out() << dat::infoString(xyArea, "xyArea") << std::endl;
	io::out() << dat::infoString(rngArea, "rngArea") << std::endl;

	return 0;
}
