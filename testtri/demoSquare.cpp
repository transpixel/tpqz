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

#include "libapp/Timer.h"
#include "libdat/cast.h"
#include "libdat/ExtentsIterator.h"
#include "libdat/grid.h"
#include "libdat/validity.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libmath/MapSizeArea.h"
#include "libmath/math.h"

#include "libdat/MinMax.h"

#include <cassert>
#include <complex>
#include <fstream>
#include <iostream>
#include <map>


namespace
{
	static dat::MinMax<long> sMinMaxI{};
	static dat::MinMax<long> sMinMaxJ{};


namespace example
{
	//! example data type (support op+() and op*()
	using DataType = std::complex<double>;

#define UseMap // 6.1 sec
// #define UseGrid

	// static DataType const sNull{ dat::nullValue<std::complex<double> >() };
	static DataType const sNull
		(dat::nullValue<double>(), dat::nullValue<double>());

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

		std::pair<long, long> theBegEndI
			{ dat::nullValue<long>(), dat::nullValue<long>() };
		std::pair<long, long> theBegEndJ
			{ dat::nullValue<long>(), dat::nullValue<long>() };

		explicit
		SamplePool
			( tri::NodeIterator::IndexLimits const & indexLimits
			)
			: theBegEndI{ indexLimits.ndxBegEndI() }
			, theBegEndJ{ indexLimits.ndxBegEndJ() }
		{ }

		bool
		keyIsValid
			( KeyType const & key
			) const
		{
			long const & keyI = key.first;
			long const & keyJ = key.second;
			return
				{  (theBegEndI.first <= keyI)
				&& (keyI < theBegEndI.second)
				&& (theBegEndJ.first <= keyJ)
				&& (keyJ < theBegEndJ.second)
				};
		}

		void
		addSample
			( DataType const & sample
			, KeyType const & key
			)
		{
			sMinMaxI = sMinMaxI.expandedWith(key.first);
			sMinMaxJ = sMinMaxJ.expandedWith(key.second);
			assert(keyIsValid(key));
			insertSample(sample, key);
		}

		DataType
		operator()
			( long const & keyI
			, long const & keyJ
			) const
		{
			DataType sample{};
			KeyType const key{ keyI, keyJ };
			if (keyIsValid(key))
			{
				sample = extractSample(keyI, keyJ);
			}
			return sample;
		}

		virtual
		void
		insertSample
			( DataType const & sample
			, KeyType const & key
			) = 0;

		virtual
		DataType
		extractSample
			( long const & keyI
			, long const & keyJ
			) const = 0;

		virtual
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
			
			oss << dat::infoString(theBegEndI, "theBegEndI") << std::endl;

			oss << std::endl;
			oss << dat::infoString(theBegEndJ, "theBegEndJ") << std::endl;

			return oss.str();
		}

	};

	//! "Database" of DataTypes keyed to tritille lattice node indices
	struct SampleMap : public SamplePool
	{
		std::map<KeyType, DataType> theSamps;

		explicit
		SampleMap
			( tri::NodeIterator::IndexLimits const & indexLimits
			)
			: SamplePool(indexLimits)
			, theSamps{}
		{ }

		virtual
		void
		insertSample
			( DataType const & sample
			, KeyType const & key
			)
		{
			theSamps[key] = sample;
		}

		virtual
		DataType
		extractSample
			( long const & keyI
			, long const & keyJ
			) const
		{
			DataType samp{ sNull };
			KeyType const key{ keyI, keyJ };
			std::map<KeyType, DataType>::const_iterator
				const itFind{ theSamps.find(key) };
			if (theSamps.end() != itFind)
			{
				samp = itFind->second;
			}
			return samp;
		}

		virtual
		std::string
		infoString
			( std::string const & title = {}
			) const
		{
			std::ostringstream oss;

			oss << SamplePool::infoString(title);

			oss << std::endl;
			oss << dat::infoString(theSamps.size(), "numSamp") << std::endl;
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

	//! "Database" of DataTypes keyed to tritille lattice node indices
	struct SampleGrid : public SamplePool
	{
		dat::grid<DataType> theSamps;

		explicit
		SampleGrid
			( tri::NodeIterator::IndexLimits const & indexLimits
			)
			: SamplePool(indexLimits)
			, theSamps
				( theBegEndI.second - theBegEndI.first
				, theBegEndJ.second - theBegEndJ.first
				)
		{
			std::fill(theSamps.begin(), theSamps.end(), sNull);
		}

		dat::RowCol
		gridRowColFor
			( long const & keyI
			, long const & keyJ
			) const
		{
			long const row{ keyI - theBegEndI.first};
			long const col{ keyJ - theBegEndJ.first};
			assert(0u <= row);
			assert(0u <= col);
			return {{ size_t(row), size_t(col) }};
		}

		virtual
		void
		insertSample
			( DataType const & sample
			, KeyType const & key
			)
		{
			long const & keyI = key.first;
			long const & keyJ = key.second;
			theSamps(gridRowColFor(keyI, keyJ)) = sample;
		}

		virtual
		DataType
		extractSample
			( long const & keyI
			, long const & keyJ
			) const
		{
			DataType samp{ sNull };
			dat::RowCol const gridRowCol(gridRowColFor(keyI, keyJ));
			if (theSamps.hwSize().includes(gridRowCol))
			{
				samp = theSamps(gridRowCol);
			}
			return samp;
		}

		virtual
		std::string
		infoString
			( std::string const & title = {}
			) const
		{
			std::ostringstream oss;

			oss << SamplePool::infoString(title);

			oss << std::endl;
			oss << dat::infoString(theSamps.size(), "numSamp") << std::endl;
			/*
			for (dat::ExtentsIterator iter(theSamps.hwSize()) ; iter ; ++iter)
			{
				dat::RowCol const & rowcol = *iter;
				oss
					<< "rowcol: " << dat::infoString(rowcol)
					<< "samp: " << dat::infoString(theSamps(rowcol))
					<< std::endl;
			}
			*/
			return oss.str();
		}

	};


	// Planar "surface"
	DataType
	valueOnSurfaceAtXY
		( dat::Spot const & xyLoc
		)
	{
		// return { xyLoc[0], xyLoc[1] }; // linear
		DataType const tmp{ xyLoc[0], xyLoc[1] };
		return math::sq(tmp);
	}

	//! Generate samples over domain - (from planar surface)
	template <typename SamplePool>
	void
	fillSamples
		( SamplePool * const & ptPool
		, tri::IsoTille const & trinet
		)
	{
		tri::IsoGeo const & trigeo = trinet.theTileGeo;
		for (tri::NodeIterator iter{trinet.begin()} ; iter ; ++iter)
		{
			SampleMap::KeyType const key(iter.indexPair());
			dat::Spot const xyLoc(trigeo.refSpotForFracPair(iter.fracPair()));
			ptPool->addSample(valueOnSurfaceAtXY(xyLoc), key);
		}
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

template <typename SamplePool>
void
saveNodeInterp
	( tri::IsoTille const & trinet
	, SamplePool const & samples
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
	// define area domain (simple square)
	dat::Range<double> xRange{ -1., 1. };
	dat::Range<double> yRange{ -1., 1. };
	dat::Area<double> xyArea{ xRange, yRange };
	tri::Domain const xyDomain{ xyArea };

	// define iso-tritille geometry
	constexpr double deltaHigh{ 1./128. };
	constexpr double deltaWide{ 1./128. };

	constexpr size_t gridHigh{ 4u * 1024u };
	constexpr size_t gridWide{ 4u * 1024u };

	app::Timer timer;

	// define tessellation geometry
	constexpr std::array<double, 2u> aDir{{ .125, 1. }};
	tri::IsoGeo const trigeo(deltaHigh, deltaWide, aDir);

	// construct tritille
	tri::IsoTille const trinet(trigeo, xyDomain);

io::out() << dat::infoString(trinet, "trinet") << std::endl;
io::out() << std::endl;

	// generate samples at each tritille node
	timer.start("pool.genSamples");
#	if defined (UseMap)
	example::SampleMap samples(trinet.begin().theNdxLimits);
#	endif
#	if defined (UseGrid)
	example::SampleGrid samples(trinet.begin().theNdxLimits);
#	endif
	example::fillSamples(&samples, trinet);
	timer.stop();

io::out() << dat::infoString(sMinMaxI, "sMinMaxI") << std::endl;
io::out() << dat::infoString(sMinMaxJ, "sMinMaxJ") << std::endl;

	// interpolate surface value at raster grid locations
	size_t numNull{ 0u };
	size_t numGridInDom{ 0u };
	size_t numDiff{ 0u };
	double sumSqDiff{ 0. };
	timer.start("trinet.nodes");
	size_t const numNodeInDom{ trinet.sizeValidNodes() };
	timer.stop();


	// check tritille interpolation at raster locations
	timer.start("raster.interp");
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
			++numGridInDom;
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
	timer.stop();

	assert(0u < numDiff);
	double const diffPerNode{ std::sqrt(sumSqDiff / double(numDiff)) };
	constexpr double tolPerNode{ math::eps };

	io::out() << "Failure of interpolation test" << std::endl;
	io::out() << dat::infoString(numNull, "numNull") << std::endl;
	io::out() << dat::infoString(numDiff, "numDiff") << std::endl;
	io::out()
		<< dat::infoString(sumSqDiff, "sumSqDiff") << '\n'
		<< "diffPerNode: " << io::sprintf("%12.5e", diffPerNode) << '\n'
		<< " tolPerNode: " << io::sprintf("%12.5e", tolPerNode) << '\n'
		<< '\n';

	// report information
	io::out() << dat::infoString(numNodeInDom, "numNodeInDom") << std::endl;
	io::out() << dat::infoString(numGridInDom, "numGridInDom") << std::endl;
	io::out() << dat::infoString(timer, "timer") << std::endl;

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
