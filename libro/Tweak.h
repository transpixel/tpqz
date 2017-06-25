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

#ifndef ro_Tweak_INCL_
#define ro_Tweak_INCL_

/*! \file
\brief Declarations for ro::Tweak
*/


#include "libdat/validity.h"
#include "libga/Pose.h"

#include <string>


namespace ro
{

/*! \brief Perturbations of symmetric RO parameters (phi[12],alpha,theta[12])

\par Example
\dontinclude testro/uTweak.cpp
\skip ExampleStart
\until ExampleEnd
*/

struct Tweak
{
	double thePhi1{ dat::nullValue<double>() };
	double thePhi2{ dat::nullValue<double>() };
	double theAlpha{ dat::nullValue<double>() };
	double theTheta1{ dat::nullValue<double>() };
	double theTheta2{ dat::nullValue<double>() };

public: // static methods

	//! Construct from decomposition of independent attitudes (RO: B w.r.t A)
	static
	Tweak
	from
		( ga::Pose const & attA
		, ga::Pose const & attB //!< NOTE: Must be symmetric!
		);

public: // methods

	//! default null constructor
	Tweak
		() = default;

	//! Value ctor
	inline
	explicit
	Tweak
		( double const & phi1
		, double const & phi2
		, double const & alpha //!< == {phi3, -theta3}
		, double const & theta1
		, double const & theta2
		);

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Angle of first sensor frame w.r.t. base-frame
	inline
	ga::BiVector
	phi
		() const;

	//! Angle of second sensor frame w.r.t. base-frame
	inline
	ga::BiVector
	theta
		() const;

	//! Attitude of first sensor frame w.r.t. base-frame
	inline
	ga::Pose
	attPhi
		() const;

	//! Attitude of second sensor frame w.r.t. base-frame
	inline
	ga::Pose
	attTheta
		() const;

	//! Fully rigorous (spinor based) coplanarity misclosure value
	double
	roFuncValue
		( ga::Vector const & uDir
		, ga::Vector const & vDir
		) const;

	//! Approximate (linearized) coplanarity misclosure value
	double
	linearFuncValue
		( ga::Vector const & uDir
		, ga::Vector const & vDir
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = {}
		) const;

}; // Tweak

} // ro

// Inline definitions
#include "libro/Tweak.inl"

#endif // ro_Tweak_INCL_

