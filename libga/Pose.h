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

#ifndef ga_Pose_INCL_
#define ga_Pose_INCL_

/*! \file
\brief Declarations for ga::Pose
*/


#include "libga/ga.h"
#include "libga/spin.h"

#include <array>
#include <string>


namespace ga
{

/*! \brief Representation of rigid body attitude.

\par Example
\dontinclude testga/uPose.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Pose
{
public:

	Spinor theSpin; //!< Ops spinor: i.e. exp( .5 * physicalAngle) (NOTE 1/2)

public: // static methods

	//! identity attitude (e.g. body alignment identical to ref frame)
	inline
	static
	Pose const &
	identity
		();

public: // methods

	//! default null constructor
	inline
	Pose
		();

	//! Construct such that body frame has this angle w.r.t. some ref frame
	inline
	explicit
	Pose
		( BiVector const & physAngle
		);

	//! value constructor
	inline
	explicit
	Pose
		( Spinor const & halfSpin // =exp(.5*physAngle)
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Physical angle associated with this instance
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

	//! Transform bi-vector from domain into range
	inline
	BiVector
	operator()
		( BiVector const & biv
		) const;

	//! Inverse of this instance
	inline
	Pose
	inverse
		() const;

	//! Pose associated with spinor square root
	inline
	Pose
	sqrt
		() const;

	//! Net transformation this after first
	inline
	Pose
	operator*
		( Pose const & first
		) const;

	//! True if same as other within tol
	bool
	nearlyEquals
		( Pose const & other
		, double const & tol = math::eps
		) const;

	//! Direction of ndx-th range basis vector expressed in domain frame
	ga::Vector
	bodyAxisInRef
		( size_t const & ndx
		) const;

	//! Numeric components of associated spinor
	std::array<double, 4u>
	spinComponents
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
#include "libga/Pose.inl"

#endif // ga_Pose_INCL_

