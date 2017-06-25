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

#ifndef geo_Plane_INCL_
#define geo_Plane_INCL_

/*! \file
\brief Declarations for geo::Plane
*/


#include "libga/ga.h"

#include <string>
#include <utility>


namespace geo
{

/*! \brief Geometric 2D plane embedded in 3D space.

\par Example
\dontinclude testgeo/uPlane.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Plane
{

	ga::BiVector theDir;
	ga::Vector theNormVec;
	double theNormMag;

public: // methods

	//! default null constructor
	Plane
		() = default;

	//! Construct to contain point with direction dirBiv
	explicit
	Plane
		( ga::Vector const & anyPIP //!< Any Point-In-Plane
		, ga::BiVector const & dirBiv //!< must be non-zero (get's normalized)
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Point in plane closest to origin
	ga::Vector
	origin
		() const;

	//! Unit direction vector orthogonal to plane
	ga::Vector
	unitNormal
		() const;

	//! Point in plane closest to anypnt
	ga::Vector
	projectionOf
		( ga::Vector const & anypnt
		) const;

	//! Shortest vector from plane to any point (== offsetTo()*unitNormal
	ga::Vector
	rejectionTo
		( ga::Vector const & anypnt
		) const;

	//! Signed distance of point from plane (i.e. rejection(dot)unitNorm)
	double
	offsetTo
		( ga::Vector const & anypnt
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

};

}

// Inline definitions
// #include "libgeo/Plane.inl"

#endif // geo_Plane_INCL_

