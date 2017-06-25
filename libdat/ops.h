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

#ifndef dat_ops_INCL_
#define dat_ops_INCL_

/*! \file
\brief Declarations for dat::ops
*/


#include "libdat/array.h"
#include "libdat/grid.h"
#include "libdat/dat.h"

#include <cmath>


namespace dat
{

/*! \brief Functions that operate on data types.

\par Example
\dontinclude testdat/uops.cpp
\skip ExampleStart
\until ExampleEnd
*/

//! Grid with scaled individual elements
template <typename Type>
inline
dat::grid<Type>
scaled
	( Type const & scalar
	, dat::grid<Type> const & ingrid
	);

//! Upper bound of resized extents
inline
dat::Extents
scaledCeil
	( double const & scalar
	, dat::Extents const & hwSize
	);

//! Location at center of extents
inline
Spot
centerOf
	( Extents const & someSize
	);

//! Length of diagonal
inline
double
diagonalMag
	( Extents const & someSize
	);

//! Number of elements contained within Extent
inline
size_t
areaOf
	( Extents const & someSize
	);

//! Number of exterior boundary elements
inline
size_t
perimeterOf
	( Extents const & someSize
	);

//! Radius of circumscribing circle
inline
double
radiusCircumscribed
	( Extents const & someSize
	);

//! Directed (CCW-positive) angle between two locations
inline
double
angleToward
	( Spot const & toward
	, Spot const & from = Spot{{ 0., 0. }}
	);

//! Magnitude of arg (aka 'modulus')
inline
double
magnitude
	( Spot const & arg
	);

}

// Inline definitions
#include "libdat/ops.inl"

#endif // dat_ops_INCL_

