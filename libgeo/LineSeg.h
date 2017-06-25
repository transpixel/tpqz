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

#ifndef geo_LineSeg_INCL_
#define geo_LineSeg_INCL_

/*! \file
\brief Declarations for geo::LineSeg
*/


#include "libgeo/Ray.h"
#include "libdat/validity.h"

#include <string>


namespace geo
{

/*! \brief A directed and finite line segment in 3D space

\par Example
\dontinclude testgeo/uLineSeg.cpp
\skip ExampleStart
\until ExampleEnd
*/

class LineSeg
{

public: // data

	Ray theRay{};
	double theLength{ dat::nullValue<double>() };

public: // static methods

	//! A line segment between two points
	static
	LineSeg
	between
		( ga::Vector const & begPoint
		, ga::Vector const & endPoint
		);

public: // methods

	//! default null constructor
	LineSeg
		() = default;

	//! Value constructor
	explicit
	LineSeg
		( Ray const & ray
		, double const & length
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Start point of this edge
	inline
	ga::Vector const &
	start
		() const;

	//! End point of this segment
	inline
	ga::Vector
	stop
		() const;

	//! Direction of this edge
	inline
	ga::Vector const &
	direction
		() const;

	//! True if distance is within segment (-tol <= dist) && (dist < len+tol)
	inline
	bool
	contains
		( double const & dist
		, double const & tol = 0.
		) const;

	//! True if point projects onto axis between start and end
	inline
	bool
	containsProjectionOf
		( ga::Vector const & pnt
		, double const & tol = 0.
		) const;

	//! Fraction into segment for projection of point
	inline
	double
	fractionAtProjectionOf
		( ga::Vector const & pnt
		) const;

	//! Point distance from start point
	inline
	ga::Vector
	pointAtDistance
		( double const & dist
		) const;

	//! Point fraction of way along the edge (0:begin, 1:end)
	inline
	ga::Vector
	pointAtFraction
		( double const & frac
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
#include "libgeo/LineSeg.inl"

#endif // geo_LineSeg_INCL_

