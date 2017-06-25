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

#ifndef geo_cube_INCL_
#define geo_cube_INCL_

/*! \file
\brief Declarations for geo::cube
*/


#include "libga/ga.h"

#include <array>
#include <vector>


namespace geo
{

/*! \brief Functions related to structure of ideal cube.

\par Example
\dontinclude testgeo/ucube.cpp
\skip ExampleStart
\until ExampleEnd
*/
namespace cube
{
	//! Points on unit cube with edge in x,y,z each ranging [0.,1.]
	namespace points
	{
		//! Points at the vertices
		std::vector<ga::Vector>
		onVertices
			();

		//! Points along a single edge (x-axis) withOUT vertices
		std::vector<ga::Vector>
		onEdge
			( size_t const & numBetween //!< number per edge *SANS* vertices
			);

		//! Points along 3 orthogonal (xyz-)axes formed by rotating inputs
		std::vector<ga::Vector>
		onTriad
			( std::vector<ga::Vector> const & pnts1d
			);

		//! Points along skeleton (all edges but not vertices) of unit cube
		std::vector<ga::Vector>
		onSkeleton
			( std::vector<ga::Vector> const & triad
			);

		//! Points distributed on framework of cube (skeleton + vertices)
		std::vector<ga::Vector>
		onFrame
			( size_t const & numBetween //!< number per side *SANS* vertices
			);
	}

	//! Location of unit cube center
	ga::Vector
	center
		();

	//! Directions, from cube CENTER to points on cube framework
	std::vector<ga::Vector>
	centralDirections
		( size_t const & numBetween //!< Number *between* (excluding) corners
		);

	// Face ordering: (-z, +x, +y, -x, -y, +z)

	//! Directions to each of the 6 faces
	std::array<ga::Vector, 6u>
	faceCenterDirections
		();

	//! Index of face (-z, +x, +y, -x, -y, +z) to which dir is most normal
	inline
	size_t
	faceIndex
		( ga::Vector const & dir
		);
}

}

// Inline definitions
#include "libgeo/cube.inl"

#endif // geo_cube_INCL_

