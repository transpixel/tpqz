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
\brief  This file contains unit test for tri::IsoTille
*/


#include "libtri/IsoTille.h"

#include "libdat/array.h"
#include "libdat/ExtentsIterator.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libmath/MapSizeArea.h"
#include "libmath/math.h"
#include "libtri/NodeIterator.h"

#include <array>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
tri_IsoTille_test0
	()
{
	std::ostringstream oss;
	tri::IsoTille const aNull;
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}


namespace example
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

		bool
		nearlyEquals
			( DataType const & other
			, double const & tol = math::eps
			) const
		{
			return dat::nearlyEquals(theABC, other.theABC, tol);
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
			return operator()(std::make_pair(keyI, keyJ));
		}

		DataType
		operator()
			( std::pair<long, long> const & keyIJ
			) const
		{
			DataType samp;
			std::map<KeyType, DataType>::const_iterator
				const itFind{ theSamps.find(keyIJ) };
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
		( tri::IsoTille const & trinet
		)
	{
		SamplePool pool;
		tri::IsoGeo const & trigeo = trinet.theTileGeo;
		for (tri::NodeIterator iter{trinet.begin()} ; iter ; ++iter)
		{
			SamplePool::KeyType const key(iter.nodeKey());
			dat::Spot const xyLoc(trigeo.refSpotForFracPair(iter.fracPair()));
			pool.addSample(valueOnPlaneAtXY(xyLoc), key);
		}
		return pool;
	}
}


//! Check interpolation operations using planar test surface of DataType
std::string
tri_IsoTille_test1
	()
{
	std::ostringstream oss;

	// define iso-tritille geometry
	constexpr double deltaHigh{ 1./7. };
	constexpr double deltaWide{ 1./11. };

	constexpr size_t gridHigh{ 101u };
	constexpr size_t gridWide{ 107u };

	constexpr std::array<double, 2u> aDir{{ .125, 1. }};
	tri::IsoGeo const trigeo(deltaHigh, deltaWide, aDir);

	// define domain (simple square)
	dat::Range<double> xRange{ -1., 2. };
	dat::Range<double> yRange{ -1.5, .5 };
	dat::Area<double> xyArea{ xRange, yRange };
	tri::Domain const xyDomain{ xyArea };

	// construct tritille
	tri::IsoTille const trinet(trigeo, xyDomain);

	// generate samples at each tritille node
	example::SamplePool const samples(example::poolOfSamples(trinet));


	// interpolate a 'missing' node property at a node using neighbors
	{
		// select a node to interpolate
		tri::NodeKey const ndxGone
			{ trigeo.indicesForRefSpot(xyArea.center()) };
		example::DataType const expSamp{ samples(ndxGone) };

		// interpolate from neighbors
		double const smallEdge{ std::min(deltaHigh, deltaWide) };
		double const nearDist{ (17./16.)*smallEdge }; // adjacent
		example::DataType const gotSamp
			{ trinet.nodeValueViaInvDist(ndxGone, nearDist, samples) };

		// check if interpolated property matches expected
		if (! gotSamp.nearlyEquals(expSamp))
		{
			oss << "Failure of node property interpolation test" << std::endl;
			oss << dat::infoString(expSamp, "expSamp") << std::endl;
			oss << dat::infoString(gotSamp, "gotSamp") << std::endl;
		}

	} // node interp

	// interpolate surface value at raster grid locations
	size_t numNull{ 0u };
	size_t numInDom{ 0u };
	size_t numDiff{ 0u };
	double sumSqDiff{ 0. };
	double sumSqMags{ 0. };

	// check evaluation directly at sample locations
	for (tri::NodeIterator iter{trinet.begin()} ; iter ; ++iter)
	{
		dat::Spot const xyLoc(trigeo.refSpotForFracPair(iter.fracPair()));
		example::DataType const expSamp{ example::valueOnPlaneAtXY(xyLoc) };

		if (xyDomain.contains(xyLoc))
		{
			++numInDom;
			example::DataType const gotSamp
				{ trinet.linearInterpForValid(xyLoc, samples) };
			if (gotSamp.isValid())
			{
				++numDiff;
				double const diff{ gotSamp.testValue() - expSamp.testValue() };
				sumSqDiff += math::sq(diff);
				sumSqMags += math::sq(expSamp.testValue());
			}
			else
			{
				++numNull;
			}
		}
	}

	// check tritille interpolation at raster locations
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
			++numInDom;
		}

		// expected value (from test case surface model)
		example::DataType const expSamp{ example::valueOnPlaneAtXY(xySpot) };

		// interpolated value from tritille
		example::DataType const gotSamp
			{ trinet.linearInterpWithCheck(xySpot, samples) };
		if (gotSamp.isValid())
		{
			// track differences
			++numDiff;
			double const diff{ gotSamp.testValue() - expSamp.testValue() };
			sumSqDiff += math::sq(diff);
			sumSqMags += math::sq(expSamp.testValue());
		}
		else
		{
			++numNull;
		}
	}
	assert(0u < numDiff);
	double const diffPerNode{ std::sqrt(sumSqDiff / double(numDiff)) };
	double const magsPerNode{ std::sqrt(sumSqMags / double(numDiff)) };
	double const tolPerNode{ magsPerNode * math::eps };
	if (! (diffPerNode < tolPerNode))
	{
		oss << "Failure of interpolation test" << std::endl;
		oss << dat::infoString(numNull, "numNull") << std::endl;
		oss << dat::infoString(numInDom, "numInDom") << std::endl;
		oss << dat::infoString(numDiff, "numDiff") << std::endl;
		oss
			<< dat::infoString(sumSqDiff, "sumSqDiff") << '\n'
			<< "diffPerNode: " << io::sprintf("%12.5e", diffPerNode) << '\n'
			<< " tolPerNode: " << io::sprintf("%12.5e", tolPerNode) << '\n'
			<< '\n';
	}

	return oss.str();
}

	//! Find all nodes within radius via brute force evaluation
	std::set<tri::NodeKey>
	nodePairsInRadius
		( tri::IsoGeo const & trigeo
		, tri::Domain const & domain
		, dat::Spot const & center
		, double const & radius
		)
	{
		std::set<tri::NodeKey> nodesIn;
		for (tri::NodeIterator iter(trigeo, domain) ; iter ; ++iter)
		{
			tri::NodeKey const ndxIJ{ iter.nodeKey() };
			dat::Spot const refIJ(trigeo.refSpotForNodeKey(ndxIJ));
			if (domain.contains(refIJ))
			{
				using dat::operator-;
				double const dist{ dat::magnitude(refIJ - center) };
				if (dist <= radius)
				{
					nodesIn.insert(ndxIJ);
				}
			}
		}
		return nodesIn;
	}


//! Check proximity methods
std::string
tri_IsoTille_test2
	()
{
	std::ostringstream oss;

	// small domain area
//#define EasyData
#	if defined(EasyData)
	dat::Range<double> const xRange(- 5., 15.);
	dat::Range<double> const yRange(- 7., 13.);
	std::array<double, 2u> const refPnt{{ 10., -2. }};
	double const refDist{ 2.5 };
	std::array<double, 2u> const adir{{ 1., 0. }};
	double const deltaA{ .1 };
	double const deltaB{ .1 };
#	else
	dat::Range<double> const xRange(-11., -7.);
	dat::Range<double> const yRange(3., 5.);
	std::array<double, 2u> const refPnt{{ -9., 4.125 }};
	std::array<double, 2u> const adir{{ .4, .3 }};
	double const refDist{ 1.5 };
	double const deltaA{ .25 };
	double const deltaB{ .40 };
#	endif

	tri::IsoGeo const trigeo(deltaA, deltaB, adir);
	dat::Area<double> const refArea{ xRange, yRange };
	tri::Domain const domain(refArea);
	assert(domain.contains(refPnt));

	tri::IsoTille const trinet(trigeo, domain);

	tri::NodeKey const ndxAt{ trigeo.indicesForRefSpot(refPnt) };
	dat::Spot const refNodeLoc(trigeo.refSpotForNodeKey(ndxAt));
	assert(domain.contains(refNodeLoc));

	//
	// check all encompassing case
	//

	{
		constexpr double wayBig{ 1.e6 };

		// determine nodes via brute force evaluation
		std::set<tri::NodeKey> const expNodeKeys
			{ nodePairsInRadius(trigeo, domain, refNodeLoc, wayBig) };

		// request "near" nodes such that all are included
		std::vector<tri::IsoTille::DistKeyPair> const gotDistKeyPairs
			{ trinet.distKeysNearTo(ndxAt, wayBig) };

		/*
		std::ofstream ofsExp("test_exp.dat");
		for (tri::NodeKey const & ndxIJ : expNodeKeys)
		{
		ofsExp << dat::infoString(ndxIJ, "ndxIJ") << std::endl;
		}
		std::ofstream ofsGot("test_got.dat");
		for (tri::IsoTille::DistKeyPair
			const & gotDistKeyPair : gotDistKeyPairs)
		{
		ofsGot << dat::infoString(gotDistKeyPair.second, "ndxIJ") << std::endl;
		}
		*/

		assert(! expNodeKeys.empty());
		size_t const expNears{ expNodeKeys.size() - 1L }; // skip 'at'
		size_t const gotNears{ gotDistKeyPairs.size() };
		if (! dat::nearlyEquals(gotNears, expNears))
		{
			oss << "Failure of full coverage test" << std::endl;
			oss << dat::infoString(expNears, "expNears") << std::endl;
			oss << dat::infoString(gotNears, "gotNears") << std::endl;
		}
	}

	//
	// Check typical case
	//

	{
		// determine nodes via brute force evaluation
		std::set<tri::NodeKey> const expNodeKeys
			{ nodePairsInRadius(trigeo, domain, refNodeLoc, refDist) };

		// get neighbor node info
		std::vector<tri::IsoTille::DistKeyPair> const gotDistKeyPairs
			{ trinet.distKeysNearTo(ndxAt, refDist) };

		// check that returned data are within neighborhood
		size_t errCnt{ 0u };
		double prevDist{ 0. };
		std::set<tri::NodeKey> gotNodeKeys;
		for (tri::IsoTille::DistKeyPair
			const & gotDistKeyPair : gotDistKeyPairs)
		{
			double const & gotDist = gotDistKeyPair.first;
			tri::NodeKey const & ndxIJ = gotDistKeyPair.second;
			gotNodeKeys.insert(ndxIJ);

			assert(dat::isValid(gotDist));
			assert(dat::isValid(ndxIJ));

			// check if node indices are in neighborhood
			dat::Spot const gotAt(trigeo.refSpotForNodeKey(ndxIJ));
			using dat::operator-;
			dat::Spot const refDelta(gotAt - refNodeLoc);
			double const chkDist{ dat::magnitude(refDelta) };
			if (refDist < chkDist)
			{
				oss << "Failure of check radius test" << std::endl;
				oss << dat::infoString(ndxIJ, "ndxIJ") << std::endl;
				std::string const fmt{ "%21.18f" };
				oss << "chkDist: " << io::sprintf(fmt, chkDist) << std::endl;
				oss << "refDist: " << io::sprintf(fmt, refDist) << std::endl;
				++errCnt;
			}

			// check that radii are ordered from small to large
			double const & currDist = gotDist;
			if (! (prevDist <= currDist))
			{
				oss << "Failure of ordered distance test" << std::endl;
				oss << dat::infoString(ndxIJ, "ndxIJ") << std::endl;
				oss << dat::infoString(prevDist, "prevDist") << std::endl;
				oss << dat::infoString(currDist, "currDist") << std::endl;
				++errCnt;
			}
			prevDist = currDist;

			// check if returned radius is within neighborhood
			if (refDist < gotDist)
			{
				oss << "Failure of returned radius test" << std::endl;
				oss << dat::infoString(ndxIJ, "ndxIJ") << std::endl;
				oss << dat::infoString(gotDist, "gotDist") << std::endl;
				oss << dat::infoString(refDist, "refDist") << std::endl;
				++errCnt;
			}

			if (0u < errCnt)
			{
				break;
			}
		}
	}

	return oss.str();
}


}

//! Unit test for tri::IsoTille
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << tri_IsoTille_test0();
	oss << tri_IsoTille_test1();
	oss << tri_IsoTille_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
