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
#include "libmath/math.h"

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

		bool
		isValid
			() const
		{
			return dat::isValid(theABC);
		}

		double
		testValue
			() const
		{
			return (theABC[0] + theABC[1] + theABC[2]);
		}

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
				<< " " << dat::infoString(testValue())
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

		std::string
		infoString
			( std::string const & title = {}
			) const
		{
			std::ostringstream oss;
			if (! title.empty())
			{
				oss << title << std::endl;
			}
			oss << dat::infoString(theSamps.size(), "numSamp") << '\n';
			for (std::pair<KeyType, DataType> const & samp: theSamps)
			{
				oss
					<< "key,value:"
					<< " " << dat::infoString(samp.first)
					<< " " << dat::infoString(samp.second)
					<< std::endl;
			}
			return oss.str();
		}

	};

	// Planar "surface"
	DataType
	valueOnPlaneAtXY
		( dat::Spot const & xyLoc
		)
	{
		// return DataType(0., xyLoc[0], xyLoc[1]);
		constexpr double bias{ 7000. };
		double const xRamp{  10.*xyLoc[0] };
		double const yRamp{ 100.*xyLoc[1] };
		return DataType(bias, xRamp, yRamp);
	}

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
			pool.addSample(valueOnPlaneAtXY(xyLoc), key);
		}
		return pool;
	}

}

void
saveNodeStates
	( tri::IsoGeo const & trigeo
	, tri::Domain const & xyDomain
	, std::string const & fname = {"/dev/stdout"}
	)
{
	std::ofstream ofs(fname);
	for (tri::NodeIterator iter(trigeo, xyDomain) ; iter ; ++iter)
	{
		std::pair<long, long> const ndxPair{ iter.indexPair() };
		dat::Spot const xyLoc(trigeo.refSpotForFracPair(iter.fracPair()));

		bool const isIn(xyDomain.contains(xyLoc));

		ofs
			<< " " << dat::infoString(xyLoc, "xyLoc")
			<< " " << dat::infoString("isIn") << " " << isIn
			<< std::endl;
	}
}

dat::Spot
refSpotFor
	( tri::IsoGeo const & trigeo
	, std::pair<long, long> const & ndxPair
	)
{
	tri::IsoGeo::QuantPair const fracPair
		{ trigeo.fracPairForIndices(ndxPair.first, ndxPair.second) };
	return trigeo.refSpotForFracPair(fracPair);
}


void
saveNodeInterp
	( tri::IsoTille const & trinet
	, tri::NodeIterator const & itBeg
	, tri::Domain const & xyDomain
	, sample::SamplePool const & samples
	)
{
	tri::IsoGeo const & trigeo = trinet.theTileGeo;
	std::ofstream ofsHas("testNodeHas.dat");
	std::ofstream ofsBad("testNodeBad.dat");
	std::ofstream ofsTri("testNodeTri.dat");
	long prevI{ -10000L };
	for (tri::NodeIterator iter{itBeg} ; iter ; ++iter)
	{
		std::pair<long, long> const ndxPair{ iter.indexPair() };
		dat::Spot const xyLoc(trigeo.refSpotForFracPair(iter.fracPair()));

		sample::DataType const tinValue{ trinet(xyLoc, samples) };
		bool const hasValue{ tinValue.isValid() };

		std::ostringstream oss;
		long const & currI = ndxPair.first;
		if (currI != prevI)
		{
			oss << "\n\n";
			prevI = currI;
		}
		oss
			<< " " << dat::infoString(xyLoc, "xyLoc")
			<< " " << "hasValue" << " " << hasValue
			<< " " << dat::infoString(ndxPair, "ndxPair")
			<< std::endl;

		if (hasValue)
		{
			ofsHas << oss.str() << std::endl;
		}
		else
		{
			ofsBad << oss.str() << std::endl;
		}

		using NdxPair = std::pair<long, long>;
		NdxPair const & ndx00 = ndxPair;
		NdxPair const ndx10{ ndx00.first + 1L, ndx00.second + 0L };
		NdxPair const ndx11{ ndx00.first + 1L, ndx00.second + 1L };
		NdxPair const ndx01{ ndx00.first + 0L, ndx00.second + 1L };

		dat::Spot const xy00(refSpotFor(trigeo, ndx00));
		dat::Spot const xy10(refSpotFor(trigeo, ndx10));
		dat::Spot const xy11(refSpotFor(trigeo, ndx11));

		dat::Spot const xy01(refSpotFor(trigeo, ndx01));

		if ( xyDomain.contains(xy00)
		  && xyDomain.contains(xy11)
		   )
		{
			ofsTri << "\n\n";

			if (xyDomain.contains(xy10))
			{
				ofsTri
					<< dat::infoString(xy00) << " 0.000" << '\n'
					<< dat::infoString(xy10) << " 0.000" << '\n'
					<< dat::infoString(xy11) << " 0.000" << '\n'
					<< dat::infoString(xy00) << " 0.000" << '\n'
					;
			}
			if (xyDomain.contains(xy01))
			{
				ofsTri
					<< dat::infoString(xy00) << " 0.000" << '\n'
					<< dat::infoString(xy11) << " 0.000" << '\n'
					<< dat::infoString(xy01) << " 0.000" << '\n'
					<< dat::infoString(xy00) << " 0.000" << '\n'
					;
			}
		}
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
	constexpr double deltaHigh{ 1./8. };
	constexpr double deltaWide{ 1./11. };

	constexpr size_t gridHigh{ 4u * 1024u };
	constexpr size_t gridWide{ 4u * 1024u };

	constexpr std::array<double, 2u> aDir{{ .125, 1. }};
	tri::IsoGeo const trigeo(deltaHigh, deltaWide, aDir);

	// construct tritille
	tri::IsoTille const trinet(trigeo);

	dat::Range<double> xRange{ -1., 1. };
	dat::Range<double> yRange{ -1., 1. };
	dat::Area<double> xyArea{ xRange, yRange };
	tri::Domain const xyDomain{ xyArea };

	// generate samples at each tritille node
	sample::SamplePool const samples(sample::poolOfSamples(xyDomain, trigeo));

	// interpolate surface value at raster grid locations
	size_t numNull{ 0u };
	size_t numGood{ 0u };
	double sumSqDiff{ 0. };

	// check evaluation directly at sample locations
	for (tri::NodeIterator iter(trigeo, xyDomain) ; iter ; ++iter)
	{
		std::pair<long, long> const ndxPair{ iter.indexPair() };
		tri::IsoGeo::QuantPair const fracPair
			(trigeo.fracPairForIndices(ndxPair.first, ndxPair.second));
		dat::Spot const xyLoc(trigeo.refSpotForFracPair(fracPair));
		sample::DataType const expSamp{ sample::valueOnPlaneAtXY(xyLoc) };

		if (xyDomain.contains(xyLoc))
		{
			sample::DataType const gotSamp{ trinet(xyLoc, samples) };
			if (gotSamp.isValid())
			{
				++numGood;
				double const diff{ gotSamp.testValue() - expSamp.testValue() };
				sumSqDiff += math::sq(diff);
			}
			else
			{
				++numNull;
			}

			/*
			io::out()
				<< " " << dat::infoString(ndxPair, "r.ndxPair")
				<< " " << dat::infoString(fracPair, "r.fracPair")
				<< " " << dat::infoString(xyLoc, "r.xyLoc")
				<< " " << dat::infoString(expSamp, "r.exp")
				<< " " << dat::infoString(gotSamp, "r.got")
				<< std::endl;
			*/
		}
	}
	assert(numNull < numGood);
	double const diffPerNode{ std::sqrt(sumSqDiff / double(numGood)) };
	constexpr double tolPerNode{ math::eps };
	if (! (diffPerNode < tolPerNode))
	{
		std::ostringstream oss;
		oss << "Failure of interpolation test" << std::endl;
		oss << dat::infoString(numNull, "numNull") << std::endl;
		oss << dat::infoString(numGood, "numGood") << std::endl;
		oss
			<< dat::infoString(sumSqDiff, "sumSqDiff") << '\n'
			<< "diffPerNode: " << io::sprintf("%12.5e", diffPerNode) << '\n'
			<< " tolPerNode: " << io::sprintf("%12.5e", tolPerNode) << '\n'
			<< '\n';
		io::out() << oss.str() << std::endl;
	}

	constexpr bool saveGrid{ false };
	if (saveGrid)
	{
		saveNodeStates(trigeo, xyDomain, "testNodeLocs.dat");
		tri::NodeIterator const itBeg(trigeo, xyDomain);
		saveNodeInterp(trinet, itBeg, xyDomain, samples);

		// probably don't want to write file with very large rasters
		assert(gridHigh <= 1024u);
		assert(gridWide <= 1024u);
		std::ofstream ofsGrid("testNodeGrid.dat");

		// interpolate tritille samples at raster grid locations
		dat::Area<double> const rngArea{ trigeo.tileAreaForRefArea(xyDomain) };
		dat::grid<sample::DataType> gridValues(gridHigh, gridWide);
		math::MapSizeArea const map(gridValues.hwSize(), rngArea);
		for (dat::ExtentsIterator iter{gridValues.hwSize()} ; iter ; ++iter)
		{
			// get domain location for sample raster element
			dat::RowCol const & gridRowCol = *iter;
			dat::Spot const rcSpot(dat::cast::Spot(gridRowCol));
			dat::Spot const xySpot(map.xyAreaSpotFor(rcSpot));

			// interpolate surface properties
			sample::DataType const sampValue(trinet(xySpot, samples));
			if (sampValue.isValid())
			{
				gridValues(gridRowCol) = sampValue;
				ofsGrid << dat::infoString(xySpot) << "   0.000" << '\n';
			}

		}
	}

	return 0;
}
