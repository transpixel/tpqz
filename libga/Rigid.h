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

#ifndef ga_Rigid_INCL_
#define ga_Rigid_INCL_

/*! \file
\brief Declarations for ga::Rigid
*/


#include "libga/functions.h"
#include "libga/operators.h"
#include "libga/Pose.h"
#include "libga/quantities.h"

#include <string>


namespace ga
{

/*! \brief Classic rigid body transformation (offset and attitude).

\par Example
\snippet testga/uRigid.cpp ExampleCode
*/

class Rigid
{
	Vector theTrans{};
	Pose thePose{};

public: // static methods

	//! Identity transform
	inline
	static
	Rigid const &
	identity
		();

public: // methods

	//! default null constructor
	Rigid
		() = default;

	//! Value ctor
	explicit
	Rigid
		( Vector const & orig
		, Pose const & pose
		);

	//! Implicit value ctor
	explicit
	Rigid
		( Vector const & orig
		, BiVector const & physAngle
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Location of 'into' frame's origin expressed in the domain frame
	inline
	Vector const &
	location
		() const;

	//! Pose of 'into' frame w.r.t. 'from' frame
	inline
	Pose const &
	pose
		() const;

	//! Angle of 'into' frame expressed w.r.t. domain frame
	inline
	BiVector
	physicalAngle
		() const;

	//! Transform vector from domain into range
	inline
	Vector
	operator()
		( Vector const & vec
		) const;

	//! Transform a direction (apply only pose operation)
	inline
	Vector
	transformedDirection
		( Vector const & vec
		) const;

	//! Inverse of this instance
	Rigid
	inverse
		() const;

	//! Net transformation this after first
	Rigid
	operator*
		( Rigid const & first
		) const;

	//! True if same as other within tol
	bool
	nearlyEquals
		( Rigid const & other
		, double const & tolSta = math::eps
		, double const & tolAtt = math::eps
		) const;

	//! Descriptive information about this instance - on one line
	std::string
	infoStringShort
		( std::string const & title = std::string()
		, std::string const & fmtLoc = std::string{ "%8.3f" }
		, std::string const & fmtAng = std::string{ "%9.6f" }
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

	//! Descriptive information about this instance.
	std::string
	infoStringDetail
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
#include "libga/Rigid.inl"

#endif // ga_Rigid_INCL_

