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
		( tri::IsoTille const & trinet
		)
	{
		SamplePool pool;
		tri::IsoGeo const & trigeo = trinet.theTileGeo;
		for (tri::NodeIterator iter{trinet.begin()} ; iter ; ++iter)
		{
			SamplePool::KeyType const key(iter.indexPair());
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
			example::DataType const gotSamp{ trinet(xyLoc, samples) };
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
		example::DataType const expSamp{ example::valueOnPlaneAtXY(xySpot) };

		// interpolated value from tritille
		example::DataType const gotSamp(trinet(xySpot, samples));
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

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
