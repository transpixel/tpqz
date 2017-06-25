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

#ifndef geo_intersect_INCL_
#define geo_intersect_INCL_

/*! \file
\brief Declarations for geo::intersect
*/



#include "libga/quantities.h"
#include "libgeo/LineSeg.h"
#include "libgeo/Plane.h"
#include "libgeo/Ray.h"
#include "libgeo/Tube.h"
#include "libmath/math.h"

#include <utility>


namespace geo
{
/*! \brief Geometry constructions

\par Example
\dontinclude testgeo/uintersect.cpp
\skip ExampleStart
\until ExampleEnd
*/
namespace intersect
{
	//! Classic space intersect - distance *ALONG* each ray to closest approach
	std::pair<double, double>
	rangesFor
		( Ray const & ray1
		, Ray const & ray2
		);

	//! Classic space intersect - points *ON* each ray at closest approach
	std::pair<ga::Vector, ga::Vector>
	pointsFor
		( Ray const & ray1
		, Ray const & ray2
		);

	//! Separation distance between rays at closest approach
	double
	gapBetween
		( Ray const & ray1
		, Ray const & ray2
		, std::pair<double, double> const & ranges12
		);

	//! Distance along segment *IF* closest approach projects onto segment
	double
	rangeOnSegment
		( Ray const & ray
		, LineSeg const & seg
		, double const & tol = math::eps
		);

	//! Point on segment *IF* closest approach projects onto segment
	ga::Vector
	pointOnSegment
		( Ray const & ray
		, LineSeg const & seg
		, double const & tol = math::eps
		);

	//! Distance to segment *IF* closest approach projects onto segment
	double
	gapBetween
		( Ray const & ray
		, LineSeg const & seg
		, double const & tol = math::eps
		);

	//! Point in common for ray and plane
	ga::Vector
	point
		( Ray const & ray
		, Plane const & plane
		);

	/*! Points in common for ray and tube.
	 * Note:
	 *	- one or both may be null.
	 *	- only forward intersections are considered valid
	 *	- if both are valid, .first is proximal, .second is distal
	 */
	std::pair<ga::Vector, ga::Vector>
	pointPair
		( Ray const & ray
		, Tube const & tube
		);

}

}

// Inline definitions
// #include "libgeo/intersect.inl"

#endif // geo_intersect_INCL_

