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

#ifndef math_angle_INCL_
#define math_angle_INCL_

/*! \file
\brief Declarations for math::angle
*/


#include "libdat/Range.h"

#include "libdat/array.h"

#include <cmath>
#include <cstddef>
#include <utility>


// Functions supporting interpolation of values.

namespace math
{
	//! Value of -i*ln(-1)
	constexpr double pi(4.*std::atan(1.));

	//! Value of 2*pi
	constexpr double twoPi(2.*pi);

	//! Value of pi/2.
	constexpr double halfPi(.5*pi);

	//! Value of pi/4.
	constexpr double qtrPi(.25*pi);

	// expressions in terms of full rotation
	constexpr double fullTurn{ math::twoPi };
	constexpr double halfTurn{ .5 * fullTurn };
	constexpr double qtrTurn{ .25 * fullTurn };

	//! Angle in principal range [-pi, +pi) (NOTE *EX*cludes +pi)
	inline
	double
	principalAngle
		( double const & anyAngle
		);

	//! Angle in non-negative range [0, +2pi) (NOTE *EX*cludes +2pi)
	inline
	double
	nonNegativeAngle
		( double const & anyAngle
		);

	//! Range of zenith angles for polar coordinate frame [0,pi)
	inline
	dat::Range<double>
	zenithRange
		();

	//! Range of zenith angles for polar coordinate frame [-pi,pi)
	inline
	dat::Range<double>
	azimuthRange
		();

	//! Arctangent that returns principal value (*EX*cludes +pi)
	inline
	double
	atan2
		( double const & yy
		, double const & xx
		);

	//! Double angle formula
	inline
	double
	cos2x
		( double const cosx
		, double const sinx
		);

	//! Double angle formula
	inline
	double
	sin2x
		( double const cosx
		, double const sinx
		);

	/*! \brief Angles represent the same rotation within tolerance.
	 *
	 * Comparison ignores both phase wrap and phase discontinuity.
	 */
	inline
	bool
	sameRotation
		( double const angle
		, double const other
		, double const cordTol //!< [unit] cord-length on unit circle
		);

	/*! \brief Fraction of rotation beg/end interval from angleBeg to angleAt.
	 *
	 * Note: the angles are adjusted (by 2*pi increments) until the
	 *       adjusted values satisfy the relationship
	 *       (adjBeg <= adjAt <= adjEnd)
	 *       The fraction is then computed as (adjAt-adjBeg)/(adjEnd-adjBeg)
	 */
	inline
	double
	angleFraction
		( double const & angleAt
		, std::pair<double, double> const & angleBegEnd
		);

	//! Like angleFraction but for EXTRAPOLATION from range
	inline
	double
	angleExFraction
		( double const & angleOutOf
		, std::pair<double, double> const & angleBegEnd
		);

	//! Index into table accouting for angle phase wrap-around
	inline
	size_t
	indexForValidAngle
		( double const & angle //!< must be principal value
		, size_t const & dataSize //!< must be: (0 < ...)
		, double const & angleAt0 = -math::pi
		);

	//! Index into table accouting for angle phase wrap-around
	inline
	size_t
	indexForAngle
		( double const & anyAngle
		, size_t const & dataSize
		, double const & angleAt0 = -math::pi
		);

	//! (principal) angle associated with index -- Inverse of indexForAngle
	inline
	double
	angleForIndexValid
		( size_t const & index //!< must be in range
		, size_t const & dataSize //!< more than one
		, double const & angleAt0 = -math::pi
		);

	//! (principal) angle associated with index -- Inverse of indexForAngle
	inline
	double
	angleForIndex
		( size_t const & index
		, size_t const & dataSize
		, double const & angleAt0 = -math::pi
		);
}

// Inline definitions
#include "libmath/angle.inl"

#endif // math_angle_INCL_

