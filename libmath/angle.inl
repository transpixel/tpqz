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
\brief Inline definitions for math::angle
*/


#include "libdat/array.h"
#include "libdat/dat.h"

#include <cassert>

#include <iomanip>
#include <iostream>

#include "libdat/info.h"
#include "libio/stream.h"


namespace math
{
//======================================================================


inline
double
principalAngle
	( double const & anyAngle
	)
{
	// note: atan2 can return either -pi or +pi !!
	return (math::atan2(std::sin(anyAngle), std::cos(anyAngle)));
}

inline
double
nonNegativeAngle
	( double const & anyAngle
	)
{
	double angle(principalAngle(anyAngle));
	if (angle < 0.)
	{
		angle += math::twoPi;
	}
	return angle;
}

inline
dat::Range<double>
zenithRange
	()
{
	return dat::Range<double>(0., pi);
}

inline
dat::Range<double>
azimuthRange
	()
{
	return dat::Range<double>(-pi, pi);
}

inline
double
atan2
	( double const & yy
	, double const & xx
	)
{
	double angle(std::atan2(yy, xx));
	// compare (using value computed with (hopefully) the same function
	static double const wrapPoint(std::atan2( 0., -1.)); // +pi
	///    double const wrapPoint(std::atan2(-0., -1.)); // -pi
	if (wrapPoint == angle)
	{
		angle = -wrapPoint;
	}
	return angle;
}

inline
double
cos2x
	( double const cosx
	, double const sinx
	)
{
	return ((cosx*cosx) - (sinx*sinx));
}

inline
double
sin2x
	( double const cosx
	, double const sinx
	)
{
	return (2.*cosx*sinx);
}

inline
bool
sameRotation
	( double const angle
	, double const other
	, double const cordTol //!< [unit] cord-length on unit circle
	)
{
	bool same{ false };
	if (dat::isValid(angle) && dat::isValid(other))
	{
		// covert to locations on unit circle
		std::array<double, 2u> const aVec
			{{ std::cos(angle), std::sin(angle) }};
		std::array<double, 2u> const bVec
			{{ std::cos(other), std::sin(other) }};

		// compute cord length
		using dat::operator-;
		std::array<double, 2u> const cord(bVec - aVec);

		double const cordMag{ dat::magnitude(cord) };

		same = dat::nearlyEquals(cordMag, 0., cordTol);
	}
	return same;
}

inline
double
angleFraction
	( double const & angleAt
	, std::pair<double, double> const & angleBegEnd
	)
{
	double const & angleBeg = angleBegEnd.first;
	double const & angleEnd = angleBegEnd.second;

	double frac(dat::nullValue<double>());
	double adjBeg(math::principalAngle(angleBeg));
	double adjAt(math::principalAngle(angleAt));
	double adjEnd(math::principalAngle(angleEnd));

	// first adjust angle to be greater/eq than start point
	while (adjAt < adjBeg)
	{
		adjAt += math::twoPi;
	}

	// then adjust end point to be greater/eq than angle
	while (adjEnd < adjAt)
	{
		adjEnd += math::twoPi;
	}

	// compute faction - but only if adjAt is "inside" interval
	double const mag(adjEnd - adjBeg);
	if (std::numeric_limits<double>::min() < mag)
	{
		// check for angle exterior to range
		// (if angle is exterior, above increment loops make range > 2pi)
		if (mag <= math::twoPi)
		{
			double const delta(adjAt - adjBeg);
			frac = delta / mag;
		}
	}

	return frac;
}

//! Private utilities for math::angle implementations
namespace priv
{
	using Vec2 = std::array<double, 2u>;

	//! An invalid instance
	static Vec2 const sNullVec2{{ dat::badDub, dat::badDub }};

	//! True if vec has valid components
	inline
	bool
	isValid
		( Vec2 const & vec
		)
	{
		return
			(  dat::isValid(vec[0])
			&& dat::isValid(vec[1])
			);
	}


	//! Unit magnitude vector associated with polar angle
	inline
	Vec2
	dirVec
		( double const & angle
		)
	{
		return { std::cos(angle), std::sin(angle) };
	}

	//! Unit direction of vec (or null if vec zero)
	inline
	Vec2
	unit
		( Vec2 const & vec
		)
	{
		Vec2 dir{ dat::nullValue<double>(), dat::nullValue<double>() };
		double const mag{ dat::magnitude(vec) };
		if (dat::isValid(mag))
		{
			double const scale(1. / mag);
			using dat::operator*;
			dir = scale * vec;
		}
		return dir;
	}

	//! Principal valued angle from A toward B
	inline
	double
	angleBetween
		( Vec2 const & vecA
		, Vec2 const & vecB
		)
	{
		double const wedge{ vecA[0]*vecB[1] - vecA[1]*vecB[0] };
		double const dot{ vecA[0]*vecB[0] + vecA[1]*vecB[1] };
		return math::atan2(wedge, dot);
	}

	//! Direction midway from A toward B (in positive sense)
	inline
	Vec2
	bisectionDir
		( Vec2 const & vecA
		, Vec2 const & vecB
		)
	{
		Vec2 midDir(sNullVec2);
		using dat::operator+;
		Vec2 const midVec(vecA + vecB);
		double const midMag(dat::magnitude(midVec));
		if (std::numeric_limits<double>::min() < midMag)
		{
			double const scale(1./midMag);
			using dat::operator*;
			midDir = scale * midVec;
		}
		return midDir;
	}
}

inline
double
angleExFraction
	( double const & angleOutOf
	, std::pair<double, double> const & angleBegEnd
	)
{
	double frac(dat::nullValue<double>());

	//
	// Objective:
	//  - compute fraction (from Beg to At)
	//  - that is symmetric about midAngle of Beg/End range
	// Algorithm:
	//  - interpret Beg/End in positive sense
	//  - determine bisection midAngle of positive-angle partition
	//  - reduce all three angles to expression relative to midAngle
	//    -- so that relativeAngle is in principal [-pi,pi) range
	//  - perform classic interpolation
	//    -- linear from Beg to End
	//    -- Discontinuity at +/- pi from midAngle
	//

	double const & aAt = angleOutOf;
	double const & aLo = angleBegEnd.first;
	double const & aHi = angleBegEnd.second;
	if (dat::isValid(aAt) && dat::isValid(aLo) && dat::isValid(aHi))
	{
		// construct vectors associated with angles
		priv::Vec2 const dirLo(priv::dirVec(aLo));
		priv::Vec2 const dirHi(priv::dirVec(aHi));
		priv::Vec2 const dirAt(priv::dirVec(aAt));

		// positive interval bisector (unless directions anti-parallel
		priv::Vec2 const dirMid(priv::bisectionDir(dirLo, dirHi));
		if (priv::isValid(dirMid)) // else Lo=-Hi: ambiguous positive half
		{
			// compute directed angles
			double const angleML(priv::angleBetween(dirMid, dirLo));
			double const angleMH(priv::angleBetween(dirMid, dirHi));
			double const angleMA(priv::angleBetween(dirMid, dirAt));
			assert(angleML <= angleMH);

			// perform linear interpolation symmetrically about bisection dir
			double const deltaLoHi(angleMH - angleML);
			if (std::numeric_limits<double>::min() < deltaLoHi)
			{
				frac = .5 + angleMA / deltaLoHi;
			}
		}
	}

	return frac;
}

inline
size_t
indexForValidAngle
	( double const & angle
	, size_t const & dataSize
	, double const & angleAt0
	)
{
	// express relative to start angle
	double const delta(angle - angleAt0);

	// quantize to index step-size
	assert(1u < dataSize);
	double const anglePerIndex(math::twoPi / (double)(dataSize));
	double const fndx(std::floor(delta / anglePerIndex));

	// compute index
	return ((size_t)fndx % dataSize);
}

inline
size_t
indexForAngle
	( double const & anyAngle
	, size_t const & dataSize
	, double const & angleAt0
	)
{
	size_t index(dat::nullValue<size_t>());
	// check input values
	if (1u < dataSize)
	{
		// convert to principal range
		double const angle(math::principalAngle(anyAngle));
		// lookup index
		double const pa0(principalAngle(angleAt0));
		index = indexForValidAngle(angle, dataSize, pa0);
	}
	return index;
}

inline
double
angleForIndexValid
	( size_t const & index
	, size_t const & dataSize
	, double const & angleAt0
	)
{
	assert(1u < dataSize);
	double const anglePerIndex(math::twoPi / (double)(dataSize));
	double const deltaAngle(anglePerIndex * (double)index);
	return principalAngle(angleAt0 + deltaAngle);
}

inline
double
angleForIndex
	( size_t const & index
	, size_t const & dataSize
	, double const & angleAt0
	)
{
	double angle{dat::nullValue<double>()};
	if (1u < dataSize)
	{
		double const pa0(principalAngle(angleAt0));
		angle = angleForIndexValid(index, dataSize, pa0);
	}
	return angle;
}


//======================================================================
}

