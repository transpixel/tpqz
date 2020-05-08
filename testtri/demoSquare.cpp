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

#include "libapp/Usage.h"
#include "build/version.h"

#include "libtri/IsoTille.h"

#include "libapp/Timer.h"
#include "libdat/cast.h"
#include "libdat/ExtentsIterator.h"
#include "libdat/grid.h"
#include "libdat/validity.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libio/string.h"
#include "libmath/MapSizeArea.h"
#include "libmath/math.h"

#include "libdat/MinMax.h"

#include <cassert>
#include <complex>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>


namespace
{
	static dat::MinMax<long> sMinMaxI{};
	static dat::MinMax<long> sMinMaxJ{};


namespace example
{
	//! example data type (support op+() and op*()
	using DataType = std::complex<double>;

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
				(  (theBegEndI.first <= keyI)
				&& (keyI < theBegEndI.second)
				&& (theBegEndJ.first <= keyJ)
				&& (keyJ < theBegEndJ.second)
				);
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
			( tri::NodeKey const & keyIJ
			) const
		{
			DataType sample{};
			if (keyIsValid(keyIJ))
			{
				sample = extractSample(keyIJ.first, keyIJ.second);
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
			SampleMap::KeyType const key(iter.nodeKey());
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
		std::pair<long, long> const ndxPair{ iter.nodeKey() };
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
	, tri::NodeKey const & ndxIJ
	)
{
	tri::IsoGeo::QuantPair const fracPair{ trigeo.fracPairForNodeKey(ndxIJ) };
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
			(  dat::isValid(value.real())
			&& dat::isValid(value.imag())
			);
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
		std::pair<long, long> const ndxPair{ iter.nodeKey() };
		dat::Spot const xyLoc(trigeo.refSpotForFracPair(iter.fracPair()));

		example::DataType const tinValue
			{ trinet.linearInterpWithCheck(xyLoc, samples) };
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

		tri::NodeKey const ndx00{ iter.nodeKey() };
		tri::NodeKey const ndx10{ iter.nextNodeMu() };
		tri::NodeKey const ndx01{ iter.nextNodeNu() };
		tri::NodeKey const ndx11{ iter.nextNodeDi() };

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
	( int const argc
	, char const * const * const argv
	)
{
	// check args
	app::Usage usage;
	usage.setSummary
		( "Demonstrate iso-tritille interpolation w/ timing"
		);
	usage.addArg("kiloNodes(k)", "Number of tritille nodes (in kilo-Nodes)");
	usage.addArg("megaElems(M)", "Number of grid elements (in mega-Elements)");
	usage.addArg("dataStoreType", "Sample data structure {grid|map}");
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
	std::string const strNumNodes(argv[++argnum]);
	std::string const strNumElems(argv[++argnum]);
	std::string const strPoolType(argv[++argnum]);
	size_t const numNodes
		{ io::string::from(strNumNodes, dat::nullValue<size_t>()) };
	size_t const numElems
		{ io::string::from(strNumElems, dat::nullValue<size_t>()) };
	assert(dat::isValid(numNodes));
	assert(dat::isValid(numElems));

	// define area domain (simple square)
	dat::Range<double> xRange{ -1., 1. };
	dat::Range<double> yRange{ -1., 1. };
	dat::Area<double> xyArea{ xRange, yRange };
	tri::Domain const xyDomain{ xyArea };

	// define iso-tritille geometry
	double const apxNumNodes{   double(numNodes) * 1024. };
	double const apxGridSize{   double(numElems) * 1024. * 1024. };

	double const dab{ 1. / std::sqrt(apxNumNodes / 4.) };
	double const deltaHigh{ dab };
	double const deltaWide{ dab };

	size_t const gridHW{ size_t(std::sqrt(double(apxGridSize))) };
	size_t const gridHigh{ gridHW };
	size_t const gridWide{ gridHW };

	app::Timer timer;
	app::Timer timeInterp;

	// define tessellation geometry
	constexpr std::array<double, 2u> aDir{{ .125, 1. }};
	tri::IsoGeo const trigeo(deltaHigh, deltaWide, aDir);

	// construct tritille
	tri::IsoTille const trinet(trigeo, xyDomain);

	//
	// configure based on args
	//

	// generate samples at each tritille node
	std::shared_ptr<example::SamplePool> ptSamples{};
	using io::string::upper;
	if (upper("map") == upper(strPoolType))
	{
		ptSamples = std::make_shared<example::SampleMap>
			(trinet.begin().theNdxLimits);
	}
	else
	if (upper("grid") == upper(strPoolType))
	{
		ptSamples = std::make_shared<example::SampleGrid>
			(trinet.begin().theNdxLimits);
	}

	// fill samples
	timer.start("pool.genSamples");
	example::SamplePool & samples = *ptSamples;
	example::fillSamples(&samples, trinet);
	timer.stop();

	// interpolate surface value at raster grid locations
	size_t numNullSamp{ 0u };
	size_t numElemInDom{ 0u };
	size_t numDiff{ 0u };
	double sumSqDiff{ 0. };
	timer.start("trinet.nodes");
	size_t const numNodeInDom{ trinet.sizeValidNodes() };
	timer.stop();


	// check tritille interpolation at raster locations
	timer.start("raster.interp");
	timeInterp.start("interp");
	dat::Area<double> const rngArea{ trigeo.tileAreaForDomain(xyDomain) };
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
			++numElemInDom;

			// expected value (from test case surface model)
			example::DataType const expSamp
				{ example::valueOnSurfaceAtXY(xySpot) };

			// interpolated value from tritille
			example::DataType const gotSamp
				{ trinet.linearInterpForValid(xySpot, samples) };

			if (! tmp::isValid(expSamp))
			{
				assert(! tmp::isValid(gotSamp));
			}

			if (tmp::isValid(gotSamp))
			{
				assert(tmp::isValid(expSamp));

				// track differences
				++numDiff;
				using example::testValue;
				double const diff{ testValue(gotSamp) - testValue(expSamp) };
				sumSqDiff += math::sq(diff);
			}
			else
			{
				++numNullSamp;
			}
		}
	}
	timeInterp.stop();
	timer.stop();

	assert(0u < numDiff);
	double const diffPerNode{ std::sqrt(sumSqDiff / double(numDiff)) };
	constexpr double tolPerNode{ math::eps };

	double const interpTimeTotal{ timeInterp.total() };
	double const interpTimePer{ interpTimeTotal / double(numDiff) };

	// report information
	std::ostringstream rpt;
	rpt << "#===================" << '\n';
	rpt << dat::infoString(numNodes, "numNodes(k)") << '\n';
	rpt << dat::infoString(numElems, "numElems(M)") << '\n';
	rpt << dat::infoString(strPoolType, "strPoolType") << '\n';
	rpt << dat::infoString(numNodeInDom, "numNodeInDom")
		<< "  of " << dat::infoString(static_cast<size_t>(apxNumNodes))
		<< '\n';
	rpt << dat::infoString(numElemInDom, "numElemInDom")
		<< "  of " << dat::infoString(gridSize.size())
		<< '\n';
	rpt << dat::infoString(interpTimeTotal, "gridGenTime") << '\n';
	rpt << "timePerElem: " << io::sprintf("%12.9f", interpTimePer) << '\n';
	rpt << "#-------------------" << '\n';
	rpt << dat::infoString(trinet) << '\n';
	rpt << dat::infoString(sMinMaxI, "sMinMaxI") << '\n';
	rpt << dat::infoString(sMinMaxJ, "sMinMaxJ") << '\n';
	rpt << "#-------------------" << '\n';
	rpt << dat::infoString(numNullSamp, "numNullSamp") << '\n';
	rpt << dat::infoString(numDiff, "numDiff") << '\n';
	rpt
		<< dat::infoString(sumSqDiff, "sumSqDiff") << '\n'
		<< "  sumSqDiff: " << io::sprintf("%12.9f", sumSqDiff) << '\n'
		<< "diffPerNode: " << io::sprintf("%12.5e", diffPerNode) << '\n'
		<< " tolPerNode: " << io::sprintf("%12.5e", tolPerNode) << '\n'
		;
	rpt << "#===================" << '\n';
	rpt << dat::infoString(timer) << '\n';
	rpt << "#===================" << '\n';

	// echo
	io::out() << rpt.str() << std::endl;

	// save to file
	std::string const rptName
		{ "time"
		+ strPoolType
		+ io::sprintf("_n%08d", numNodes)
		+ io::sprintf("_e%08d", numElems)
		+ std::string(".dat")
		};
	std::ofstream ofsOut(rptName);
	ofsOut << rpt.str() << std::endl;

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
		dat::Area<double> const rngArea{ trigeo.tileAreaForDomain(xyDomain) };
		dat::grid<example::DataType> gridValues(gridHigh, gridWide);
		math::MapSizeArea const map(gridValues.hwSize(), rngArea);
		for (dat::ExtentsIterator iter{gridValues.hwSize()} ; iter ; ++iter)
		{
			// get domain location for sample raster element
			dat::RowCol const & gridRowCol = *iter;
			dat::Spot const rcSpot(dat::cast::Spot(gridRowCol));
			dat::Spot const xySpot(map.xyAreaSpotFor(rcSpot));

			// interpolate surface properties
			example::DataType const sampValue
				(trinet.linearInterpWithCheck(xySpot, samples));
			if (tmp::isValid(sampValue))
			{
				gridValues(gridRowCol) = sampValue;
				ofsGrid << dat::infoString(xySpot) << "   0.000" << '\n';
			}

		}
	}

	return 0;
}
