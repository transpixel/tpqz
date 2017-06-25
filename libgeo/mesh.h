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

#ifndef geo_mesh_INCL_
#define geo_mesh_INCL_

/*! \file
\brief Declarations for geo::mesh
*/


#include "libio/sprintf.h"
#include "libgeo/Triangle.h"

#include <string>
#include <sstream>


namespace geo
{

/*! \brief 3D mesh representations for various geometric forms
*/
/*
\par Example
\dontinclude testgeo/umesh.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace mesh
{
	using Quadrangle = std::array<ga::Vector, 4u>;

	//! Quad vertices with first triangle vertex repeated
	Quadrangle
	quadFor
		( geo::Triangle const & triangle
		);

	//! String of vertices
	template <typename FwdIter>
	inline
	std::string
	stringFor
		( FwdIter const & vectorBeg
		, FwdIter const & vectorEnd
		, std::string const & fmt = { "%9.3f" }
		, char const & sep = { '\n' }
		);

	//! String containing triangle vertices with first one repeated)
	std::string
	quadString
		( geo::Triangle const & triangle
		, std::string const & fmt = { "%9.3f" }
		, char const & sep = { '\n' }
		);
}

}

// Inline definitions
#include "libgeo/mesh.inl"

#endif // geo_mesh_INCL_

