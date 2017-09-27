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

#ifndef geo_sphere_INCL_
#define geo_sphere_INCL_

/*! \file
\brief Declarations for geo::sphere
*/


#include "libga/ga.h"
#include "libmath/math.h"

#include <cmath>


namespace geo
{

/*! \brief Functions relevant to operations involving a sphere.

\par Example
\dontinclude testgeo/usphere.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace sphere
{
	//! Azimuth of dir
	inline
	double
	azimuthOf
		( ga::Vector const & dir
		);

	//! Zenith angle of dir
	inline
	double
	zenithOf
		( ga::Vector const & dir
		);

	//! Direction from polar coordinates
	inline
	ga::Vector
	directionFromAZ
		( double const & azim
		, double const & zenith
		);

}

}

// Inline definitions
#include "libgeo/sphere.inl"

#endif // geo_sphere_INCL_

