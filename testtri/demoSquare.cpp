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


#include "libtri/IsoTille.h"

#include "libdat/cast.h"
#include "libdat/ExtentsIterator.h"
#include "libdat/grid.h"
#include "libdat/validity.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libmath/MapSizeArea.h"
#include "libmath/math.h"

#include <cassert>
#include <complex>
#include <fstream>
#include <iostream>
#include <map>


namespace
{

namespace example
{
	//! example data type (support op+() and op*()
	using DataType = std::complex<double>;

	//! Value for used to evaluate surface fit
	double
	testValue
		( DataType const & item
		)
	{
		return std::abs(item);
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
	valueOnSurfaceAtXY
		( dat::Spot const & xyLoc
		)
	{
		DataType const tmp{ xyLoc[0], xyLoc[1] };
		return math::sq(tmp);
		// return { xyLoc[0], xyLoc[1] }; // linear
	}

	//! Generate samples over domain - (from planar surface)
	SamplePool
	poolOfSamples
		( tri::IsoTille const & trinet
		)
	{
		SamplePool pool;
		tri::IsoGeo const & trigeo = trinet.theTileGeo;
		for (tri::NodeIterator iter{trinet.begin()} ; iter ; ++iter)
		{
			SamplePool::KeyType const key(iter.indexPair());
			dat::Spot const xyLoc(trigeo.refSpotForFracPair(iter.fracPair()));
			pool.addSample(valueOnSurfaceAtXY(xyLoc), key);
		}
		return pool;
	}

}

void
saveNodeStates
	( tri::IsoTille const & trinet
	, std::string const & fname = {"/dev/stdout"}
	)
{
	tri::IsoGeo const & trigeo = trinet.theTileGeo;
	tri::Domain const & xyDomain = trinet.theDomain;

	std::ofstream ofs(fname);
	for (tri::NodeIterator iter{trinet.begin()} ; iter ; ++iter)
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

namespace tmp
{
	bool
	isValid
		( std::complex<double> const value
		)
	{
		return
			{  dat::isValid(value.real())
			&& dat::isValid(value.imag())
			};
	}
}

void
saveNodeInterp
	( tri::IsoTille const & trinet
	, example::SamplePool const & samples
	)
{
	tri::IsoGeo const & trigeo = trinet.theTileGeo;
	tri::Domain const & xyDomain = trinet.theDomain;

	std::ofstream ofsHas("testNodeHas.dat");
	std::ofstream ofsBad("testNodeBad.dat");
	std::ofstream ofsTri("testNodeTri.dat");

	long prevI{ -10000L };
	for (tri::NodeIterator iter{trinet.begin()} ; iter ; ++iter)
	{
		std::pair<long, long> const ndxPair{ iter.indexPair() };
		dat::Spot const xyLoc(trigeo.refSpotForFracPair(iter.fracPair()));

		example::DataType const tinValue{ trinet(xyLoc, samples) };
		bool const hasValue{ tmp::isValid(tinValue) };

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
	constexpr double deltaHigh{ 1./80. };
	constexpr double deltaWide{ 1./110. };

	constexpr size_t gridHigh{ 4u * 1024u };
	constexpr size_t gridWide{ 4u * 1024u };

	// define tessellation geometry
	constexpr std::array<double, 2u> aDir{{ .125, 1. }};
	tri::IsoGeo const trigeo(deltaHigh, deltaWide, aDir);

	// define area domain (simple square)
	dat::Range<double> xRange{ -1., 1. };
	dat::Range<double> yRange{ -1., 1. };
	dat::Area<double> xyArea{ xRange, yRange };
	tri::Domain const xyDomain{ xyArea };

	// construct tritille
	tri::IsoTille const trinet(trigeo, xyDomain);

	// generate samples at each tritille node
	example::SamplePool const samples(example::poolOfSamples(trinet));

	// interpolate surface value at raster grid locations
	size_t numNull{ 0u };
	size_t numInDom{ 0u };
	size_t numDiff{ 0u };
	double sumSqDiff{ 0. };

	// check tritille interpolation at raster locations
	dat::Area<double> const rngArea{ trigeo.tileAreaForRefArea(xyDomain) };
	dat::Extents gridSize{ gridHigh, gridWide };
	math::MapSizeArea const map(gridSize, rngArea);
	for (dat::ExtentsIterator iter{gridSize} ; iter ; ++iter)
	{
		dat::RowCol const & gridRowCol = *iter;

		// get domain location for sample raster element
		dat::Spot const rcSpot(dat::cast::Spot(gridRowCol));
		dat::Spot const xySpot(map.xyAreaSpotFor(rcSpot));

		if (xyDomain.contains(xySpot))
		{
			++numInDom;
		}

		// expected value (from test case surface model)
		example::DataType const expSamp{ example::valueOnSurfaceAtXY(xySpot) };

		// interpolated value from tritille
		example::DataType const gotSamp(trinet(xySpot, samples));
		if (tmp::isValid(gotSamp))
		{
			// track differences
			++numDiff;
			using example::testValue;
			double const diff{ testValue(gotSamp) - testValue(expSamp) };
			sumSqDiff += math::sq(diff);
		}
		else
		{
			++numNull;
		}
	}

	assert(0u < numDiff);
	double const diffPerNode{ std::sqrt(sumSqDiff / double(numDiff)) };
	constexpr double tolPerNode{ math::eps };
//	if (! (diffPerNode < tolPerNode))
	{
		io::out() << "Failure of interpolation test" << std::endl;
		io::out() << dat::infoString(numNull, "numNull") << std::endl;
		io::out() << dat::infoString(numInDom, "numInDom") << std::endl;
		io::out() << dat::infoString(numDiff, "numDiff") << std::endl;
		io::out()
			<< dat::infoString(sumSqDiff, "sumSqDiff") << '\n'
			<< "diffPerNode: " << io::sprintf("%12.5e", diffPerNode) << '\n'
			<< " tolPerNode: " << io::sprintf("%12.5e", tolPerNode) << '\n'
			<< '\n';
	}

	constexpr bool saveGrid{ false };
	if (saveGrid)
	{
		saveNodeStates(trinet, "testNodeLocs.dat");
		tri::NodeIterator const itBeg(trigeo, xyDomain);
		saveNodeInterp(trinet, samples);

		// probably don't want to write file with very large rasters
		assert(gridHigh <= 1024u);
		assert(gridWide <= 1024u);
		std::ofstream ofsGrid("testNodeGrid.dat");

		// interpolate tritille samples at raster grid locations
		dat::Area<double> const rngArea{ trigeo.tileAreaForRefArea(xyDomain) };
		dat::grid<example::DataType> gridValues(gridHigh, gridWide);
		math::MapSizeArea const map(gridValues.hwSize(), rngArea);
		for (dat::ExtentsIterator iter{gridValues.hwSize()} ; iter ; ++iter)
		{
			// get domain location for sample raster element
			dat::RowCol const & gridRowCol = *iter;
			dat::Spot const rcSpot(dat::cast::Spot(gridRowCol));
			dat::Spot const xySpot(map.xyAreaSpotFor(rcSpot));

			// interpolate surface properties
			example::DataType const sampValue(trinet(xySpot, samples));
			if (tmp::isValid(sampValue))
			{
				gridValues(gridRowCol) = sampValue;
				ofsGrid << dat::infoString(xySpot) << "   0.000" << '\n';
			}

		}
	}

	return 0;
}
