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

#ifndef ro_ArgsBaseZ_INCL_
#define ro_ArgsBaseZ_INCL_

/*! \file
\brief Declarations for ro::ArgsBaseZ
*/


#include "libdat/validity.h"
#include "libga/ga.h"
#include "libga/Rigid.h"

// for error reporting
#include "libio/sprintf.h"
#include "libio/stream.h"

#include <array>
#include <string>


namespace ro
{
/*! \brief Parameters for symmetric orientation.
 *
 * NOTE: Function model is (ref RO.lyx)
 *
 * \arg   Phi: Phi1*E23 + Phi2*E31 - Alpha3*E12
 * \arg Theta: Theta1*E23 + Theta2*E31 + Alpha3*E12
 *
 * Where alpha3 is the shared value, i.e. (alpha3 == Theta3 == -Phi3)
 *    
 * The function model is:
 * - f(phi,theta) = B * (epG * u * enG) * (epH * v * enH) = 0
 *
 * Where:
 * - epG = exp(-.5*Phi)
 * - epH = exp(-.5*Theta)
 * - enG = epG.reverse()
 * - enH = epH.reverse()
 *
*/

struct ArgsBaseZ
{
	std::array<double, 2u> thePhi12
		{{ dat::nullValue<double>(), dat::nullValue<double>() }};
	std::array<double, 2u> theTheta12
		{{ dat::nullValue<double>(), dat::nullValue<double>() }};
	double theAlpha3{ dat::nullValue<double>() };

	//! Shorter name for math::principalAngle()
	inline
	static
	double
	paFor
		( double const & angle
		)
	{
		return math::principalAngle(angle);
	}

	//! Create a invalid instance
	ArgsBaseZ
		() = default;

	/*! \brief Construct from independent station angles.
	 *
	 * Note that the arguments overspecify the problem. The resulting
	 * "Alpha3" parameter is set to the average magnitude of the provided
	 * +theta[2] and -phi[2] values.
	 */
	explicit
	ArgsBaseZ
		( ga::BiVector const & phi
		, ga::BiVector const & theta
		)
		: thePhi12{{ paFor(phi[0]), paFor(phi[1]) }}
		, theTheta12{{ paFor(theta[0]), paFor(theta[1]) }}
		, theAlpha3{ paFor(.5*(theta[2] - phi[2])) }
	{
		// catch potential insane conditions
		// ('average' above keeps parms consistent)
		double const fuzzyTol{ 128.*math::eps };
		double const sumA3{ phi[2] + theta[2] };
		if (! (dat::nearlyEquals(sumA3, 0., fuzzyTol)))
		{
			io::err()
				<< "sumA3 not trivial:"
				<< " " << dat::infoString(phi[2], "phi[2]")
				<< " " << dat::infoString(theta[2], "theta[2]")
				<< " " << "sumA3: " << io::sprintf("%12.5e", sumA3)
				<< std::endl;
		}
		assert(dat::nearlyEquals(sumA3, 0., fuzzyTol));
	}

	//! Direct value construction.
	explicit
	ArgsBaseZ
		( std::array<double, 5u> const & pptta
			//!< Parm values as contiguous array
		)
		: thePhi12{{ paFor(pptta[0]), paFor(pptta[1]) }}
		, theTheta12{{ paFor(pptta[2]), paFor(pptta[3]) }}
		, theAlpha3{ paFor(pptta[4]) }
	{ }

	//! Compute parameters from general orientations
	static
	ArgsBaseZ
	from
		( ga::Rigid const & ori1wRef
		, ga::Rigid const & ori2wRef
		);

	//! Fixed reference baseline for baseframe
	static
	ga::Vector
	baseVectorDir
		()
	{
		return ga::e3;
	}

	//! Fixed reference base plane (dual to baseline) for baseframe
	static
	ga::BiVector
	basePlaneDir
		()
	{
		return ga::E123 * baseVectorDir();
	}

	//! location of first station w.r.t. base frame
	static
	ga::Vector
	loc1w0
		( double const & bMag
		)
	{
		return -.5 * bMag * ga::e3;
	}

	//! location of second station w.r.t. base frame
	static
	ga::Vector
	loc2w0
		( double const & bMag
		)
	{
		return  .5 * bMag * ga::e3;
	}


	//! True if this instance is not null
	bool
	isValid
		() const
	{
		return dat::isValid(theAlpha3);
	}

	//! Orientation of base frame w.r.t. ref for current parms
	ga::Rigid
	baseFrameWrtRef
		( ga::Rigid const & ori1wRef
		, ga::Rigid const & ori2wRef
		) const;

	//! Set parameter interpretation convention
	inline
	ga::BiVector
	phiBiv
		() const
	{
		return ga::BiVector(thePhi12[0], thePhi12[1], -theAlpha3);
	}

	//! Set parameter interpretation convention
	inline
	ga::BiVector const &
	phiDot
		( size_t const & ndx
		) const
	{
		static std::array<ga::BiVector, 3u> const pDots
			{{ ga::E23, ga::E31, -ga::E12 }};
		return pDots[ndx];
	}

	//! Set parameter interpretation convention
	inline
	ga::BiVector
	thetaBiv
		() const
	{
		return ga::BiVector(theTheta12[0], theTheta12[1],  theAlpha3);
	}

	//! Set parameter interpretation convention
	inline
	ga::BiVector const &
	thetaDot
		( size_t const & ndx
		) const
	{
		static std::array<ga::BiVector, 3u> const pDots
			{{ ga::E23, ga::E31,  ga::E12 }};
		return pDots[ndx];
	}

	//! Attitude of first frame w.r.t. base frame
	ga::Pose
	pose1w0
		() const
	{
		return ga::Pose(phiBiv());
	}

	//! Attitude of second frame w.r.t. base frame
	ga::Pose
	pose2w0
		() const
	{
		return ga::Pose(thetaBiv());
	}

	//! Full orientation of first frame w.r.t. base frame
	ga::Rigid
	rigid1w0
		( double const & bMag
		) const
	{
		return ga::Rigid(loc1w0(bMag), pose1w0());
	}

	//! Full orientation of second frame w.r.t. base frame
	ga::Rigid
	rigid2w0
		( double const & bMag
		) const
	{
		return ga::Rigid(loc2w0(bMag), pose2w0());
	}

	//! True if this is same as other within tolerance
	bool
	nearlyEquals
		( ArgsBaseZ const & other
		, double const & tol = math::eps
		) const;

	//! Descriptive information about this instance
	std::string
	infoString
		( std::string const & title = {}
		, std::string const & fmt = "%12.6f"
		) const;

}; // ArgsBaseZ

} // ro

// Inline definitions
// #include "libro/ArgsBaseZ.inl"

#endif // ro_ArgsBaseZ_INCL_

