//
//
// MIT License
//
// Copyright (c) 2020 Stellacore Corporation.
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
\brief  This file contains main application program demoTheles
*/


#include "libmath/math.h"
#include "libgeo/si.h"
#include "libio/stream.h"

#include <cassert>
#include <sstream>


namespace
{
	//! Observation data
	template <typename GeoType>
	struct Obs
	{
		GeoType theGeoItem;
		double theDirSigma;
		double theStaSigma;

		//! Weight for this ray based on nominal range
		double
		weight
			( double const & nomRange
			) const
		{
			double const sigComposite
				{ math::hypot((nomRange*theDirSigma), theStaSigma) };
			assert(math::eps < sigComposite);
			return { 1. / sigComposite };
		}

		//! Descriptive information about this instance
		std::string
		infoString
			( std::string const & title = {}
			) const
		{
			std::ostringstream oss;
			oss << dat::infoString(theGeoItem, title);
			oss << std::endl;
			oss << "   Direction Uncertainty: ";
			oss << dat::infoString(theDirSigma);
			oss << std::endl;
			oss << "    Location Uncertainty: ";
			oss << dat::infoString(theStaSigma);
			return oss.str();
		}

	}; // ObsRay
}

//! Demonstrate ray/plane intersection ala Thale's ship location technique.
int
main
	()
{
	//
	// general configuration
	//

	constexpr double staH{ 150. }; // station height
	constexpr double range{ 1000. }; // horizontal distance to target
	ga::Vector const base{ 0., 300., 10. };
	ga::Vector const raySta{ base + staH * ga::e3 };
	ga::Vector const expPnt{ base -range * ga::e1 };

	//
	// measurement observations
	//

	// sea surface
	constexpr double sigSeaSta{ .000 };
	constexpr double sigSeaDir{ .001 };
	Obs<geo::Plane> const obsSea
		{ geo::Plane(ga::vZero, ga::E12), sigSeaDir, sigSeaSta };

	// ray
	constexpr double sigRaySta{ .500 };
	constexpr double sigRayDir{ .010 };
	Obs<geo::Ray> const obsRay
		{ geo::Ray::fromToward(raySta, expPnt), sigRayDir, sigRaySta };

	//
	// space intersection system
	//

	// collect observations by classification
	std::vector<geo::si::WRay> const wrays
		{ { obsRay.weight(range), obsRay.theGeoItem }
		};
	std::vector<geo::si::WPlane> const wplanes
		{ { obsSea.weight(range), obsSea.theGeoItem }
		};

	// create observation system
	geo::si::PointSystem system{};
	system.addWeightedRays(wrays);
	system.addWeightedPlanes(wplanes);

	// compute intersection of collections
	geo::si::PointSoln const pntSoln{ system.pointSolution() };

	//
	// report results
	//

	io::out() << "====" << std::endl;
	io::out() << "Configuration:" << std::endl;
	io::out() << dat::infoString(base, "base") << std::endl;
	io::out() << dat::infoString(raySta, "raySta") << std::endl;
	io::out() << "Observations:" << std::endl;
	io::out() << dat::infoString(obsSea, "obsSea") << std::endl;
	io::out() << dat::infoString(obsRay, "obsRay") << std::endl;
	io::out() << "Solution:" << std::endl;
	io::out() << dat::infoString(pntSoln, "pntSoln") << std::endl;
	io::out() << "====" << std::endl;

	return 0;
}
