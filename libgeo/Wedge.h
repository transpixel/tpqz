//
//
// MIT License
//
// Copyright (c) 2020 Stellacore Corporation.
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

#ifndef geo_Wedge_INCL_
#define geo_Wedge_INCL_

/*! \file
\brief Declarations for geo::Wedge
*/


#include "libgeo/Triangle.h"

#include <string>
#include <utility>


namespace geo
{

/*! \brief A triangle vertex situated in space (like a pizza slice in 3D).

Wedge is a vertex location and two edges - i.e. a triangle with special
interpretation of one of the nodes as "the" vertex - and with the adjacent
edges understood to be in order (from)1 and (into)2.

\par Example
\dontinclude testgeo/uWedge.cpp
\skip ExampleStart
\until ExampleEnd
*/

struct Wedge
{
	geo::Triangle theTriangle{};

	//! default null constructor
	Wedge
		() = default;

	//! Value construction.
	explicit
	Wedge
		( ga::Vector const & vert
		, std::pair<ga::Vector, ga::Vector> const & locPair
		);

	//! True if this instance is valid
	inline
	bool
	isValid
		() const;

	//! Vertex location in space
	inline
	ga::Vector const &
	vertex
		() const;

	//! End point location for the first (from) edge
	inline
	ga::Vector const &
	point1
		() const;

	//! End point location for the second (into) edge
	inline
	ga::Vector const &
	point2
		() const;

	//! Vector edge from vert to first location
	inline
	ga::Vector
	edge1
		() const;

	//! Vector edge from vert to second location
	inline
	ga::Vector
	edge2
		() const;

	//! Descriptive information about this instance
	std::string
	infoString
		( std::string const & title = {}
		) const;

}; // Wedge

} // geo

// Inline definitions
#include "libgeo/Wedge.inl"

#endif // geo_Wedge_INCL_

