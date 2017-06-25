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

#ifndef geo_Ray_INCL_
#define geo_Ray_INCL_

/*! \file
\brief Declarations for geo::Ray
*/


#include "libga/constructs.h"
#include "libga/operators.h"
#include "libga/quantities.h"

#include <string>


namespace geo
{

/*! \brief Half-infinite directed line segment in 3D space.

\par Example
\dontinclude testgeo/uRay.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Ray
{

public: // data

	ga::Vector theStart{};
	ga::Vector theDir{};

public: // static methods

	//! An instance which is not valid
	static
	Ray
	null
		();

	//! Construct a ray from start point in direction toward aimPoint
	static
	Ray
	fromToward
		( ga::Vector const & rayStart
		, ga::Vector const & aimPoint
		);

public: // methods

	//! default null constructor
	Ray
		() = default;

	//! Value constructor
	explicit
	Ray
		( ga::Vector const & start
		, ga::Vector const & dir
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Point on ray at distance from start
	inline
	ga::Vector
	pointAt
		( double const & dist
		) const;

	//! Distance from start to projection of pnt on ray (positive is infront)
	inline
	double
	distanceAlong
		( ga::Vector const & pnt
		) const;

	//! Projection of point onto ray
	inline
	ga::Vector
	projectionOf
		( ga::Vector const & pnt
		) const;

	//! Rejection of point from ray (vector from ray toward pnt)
	inline
	ga::Vector
	rejectionTo
		( ga::Vector const & pnt
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
#include "libgeo/Ray.inl"

#endif // geo_Ray_INCL_

