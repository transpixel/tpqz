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

#ifndef map_ProjSinu_INCL_
#define map_ProjSinu_INCL_

/*! \file
\brief Declarations for map::ProjSinu
*/


#include "libmap/Proj.h"

#include "libdat/validity.h"
#include "libgeo/sphere.h"
#include "libmath/Partition.h"

#include <limits>


namespace map
{

constexpr double azimuth0{ 0. };

//! \brief Sinusoidal projection
class ProjSinu : public Proj
{
	math::Partition const thePartZ{};
	math::Partition const thePartX{};

public: // methods

	//! default null constructor
	ProjSinu
		() = default;

	//! Map sphere into this rectangle
	explicit
	ProjSinu
		( dat::Extents const & hwSize
		)
		: Proj(hwSize)
		, thePartZ
			{ std::pair<double, double>{ 0., math::pi }, hwSize.high() }
		, thePartX
			{ std::pair<double, double>{ -math::pi, math::pi }, hwSize.wide() }
	{
	}

	//! Noop dtor
	virtual
	~ProjSinu
		() = default;

	//! [Virtual] Raster location for a direction
	inline
	virtual
	dat::RowCol
	gridRowColFor
		( ga::Vector const & dir
		) const
	{
		double const azimuth{ geo::sphere::azimuthOf(dir) };
		double const zenith{ geo::sphere::zenithOf(dir) };
		double const lat{ math::halfPi - zenith };
		double const xx{ (azimuth0 - azimuth)*std::cos(lat) };
		return dat::RowCol
			{ thePartZ.binIndexFor(zenith)
			, thePartX.binIndexFor(xx)
			};
	}

	//! [Virtual] Direction recovered from raster location
	inline
	virtual
	ga::Vector
	directionFor
		( dat::RowCol const & mapRC
		) const
	{
		ga::Vector dir{};
		double const xx{ -thePartX.interpValueFor(mapRC[1]) };
		double const zenith{ thePartZ.interpValueFor(mapRC[0]) };
		double const lat{ math::halfPi - zenith };
		double const cosLat{ std::cos(lat) };
		if (std::numeric_limits<double>::epsilon() < std::abs(cosLat))
		{
			double const azimuth{ (1./cosLat)*xx + azimuth0 };
			if ((-math::pi <= azimuth) && (azimuth < math::pi))
			{
				dir = geo::sphere::directionFromAZ(azimuth, zenith);
			}
		}
		return dir;
	}


};

}

// Inline definitions
// #include "libmap/ProjSinu.inl"

#endif // map_ProjSinu_INCL_

