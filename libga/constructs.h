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

#ifndef ga_constructs_INCL_
#define ga_constructs_INCL_

/*! \file
\brief Declarations for ga::constructs
*/


#include "libga/quantities.h"
#include "libga/functions.h"
#include "libga/operators.h"


namespace ga
{

	// Useful constructions using geometric algebra and algorithms.

	//! Symmetric part of product
	inline
	Scalar
	dot
		( Vector const & v1
		, Vector const & v2
		);

	//! Symmetric part of product
	inline
	Scalar
	dot
		( BiVector const & b1
		, BiVector const & b2
		);

	//! AntiSymmetric part of product
	inline
	Vector
	dot
		( Vector const & v1
		, BiVector const & b2
		);

	//! AntiSymmetric part of product
	inline
	Vector
	dot
		( BiVector const & b1
		, Vector const & v2
		);

	//! Asymmetric part of product
	inline
	BiVector
	wedge
		( Vector const & v1
		, Vector const & v2
		);

	//! Unit magnitude vector orthogonal to both (aligned with v1(x)v2
	inline
	Vector
	cross
		( Vector const & v1
		, Vector const & v2
		);

	//! specialized rotation operator
	inline
	Vector
	rotated
		( Spinor const & spin
		, Vector const & vec
		);

	//! Rejection: vector to loc from line associated with unitdir
	inline
	Vector
	rejectionFrom // TODO generalize as/if needed
		( Vector const & loc
		, Vector const & somedir
		);

	//! Trivector component of (dir*plane)
	inline
	TriVector
	rejectionFrom
		( Vector const & vec
		, BiVector const & somePlane
		);

	//! Projection: vector coincident with unitdir nearest to loc
	inline
	Vector
	projectionOnto // TODO generalize as/if needed
		( Vector const & loc
		, Vector const & somedir
		);

	//! Projection of vec onto plane
	inline
	Vector
	projectionOnto
		( Vector const & vec
		, BiVector const & somePlane
		);

	//! Aribtrary (unit-magnitude) direction vector perpendicular to vec
	Vector
	anyPerpendicularDir
		( Vector const & vec
		);

	//! Result of rotating vec by quarter turn in plane
	inline
	Vector
	perpendicular
		( Vector const & vec
		, BiVector const & somePlane
		);

}

// Inline definitions
#include "libga/constructs.inl"

#endif // ga_constructs_INCL_

