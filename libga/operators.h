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

#ifndef ga_operations_INCL_
#define ga_operations_INCL_

/*! \file
\brief Declarations for ga::operations
*/


#include "libga/quantities.h"


namespace ga
{
// Basic operations on geometric quantities

	//
	// ==== Uniary operations
	//

	inline
	Scalar
	operator-
		( One const & one
		);

	inline
	Scalar
	operator-
		( Scalar const & arg
		);

	inline
	Vector
	operator-
		( Vector const & arg
		);

	inline
	BiVector
	operator-
		( BiVector const & arg
		);

	inline
	TriVector
	operator-
		( TriVector const & arg
		);

	inline
	Spinor
	operator-
		( Spinor const & arg
		);

	inline
	ImSpinor
	operator-
		( ImSpinor const & arg
		);

	//
	// ==== Addition
	//

	inline
	Scalar
	operator+
		( Scalar const & q1
		, Scalar const & q2
		);

	inline
	Vector
	operator+
		( Vector const & q1
		, Vector const & q2
		);

	inline
	BiVector
	operator+
		( BiVector const & q1
		, BiVector const & q2
		);

	inline
	TriVector
	operator+
		( TriVector const & q1
		, TriVector const & q2
		);

	inline
	Spinor
	operator+
		( Spinor const & q1
		, Spinor const & q2
		);

	inline
	ImSpinor
	operator+
		( ImSpinor const & q1
		, ImSpinor const & q2
		);

	inline
	Spinor
	operator+
		( Scalar const & q1
		, BiVector const & q2
		);

	inline
	ImSpinor
	operator+
		( Vector const & q1
		, TriVector const & q2
		);

	//
	// ==== Subtraction
	//

	inline
	Scalar
	operator-
		( Scalar const & q1
		, Scalar const & q2
		);

	inline
	Vector
	operator-
		( Vector const & q1
		, Vector const & q2
		);

	inline
	BiVector
	operator-
		( BiVector const & q1
		, BiVector const & q2
		);

	inline
	TriVector
	operator-
		( TriVector const & q1
		, TriVector const & q2
		);

	inline
	Spinor
	operator-
		( Spinor const & q1
		, Spinor const & q2
		);

	inline
	ImSpinor
	operator-
		( ImSpinor const & q1
		, ImSpinor const & q2
		);

	//
	// ==== double.MV multiply
	//

	inline
	Scalar
	operator*
		( double const & lmult
		, Scalar const & arg
		);

	inline
	Vector
	operator*
		( double const & lmult
		, Vector const & arg
		);

	inline
	BiVector
	operator*
		( double const & lmult
		, BiVector const & arg
		);

	inline
	TriVector
	operator*
		( double const & lmult
		, TriVector const & arg
		);

	inline
	Spinor
	operator*
		( double const & lmult
		, Spinor const & arg
		);

	inline
	ImSpinor
	operator*
		( double const & lmult
		, ImSpinor const & arg
		);

	//
	// ==== Scalar.MV multiply
	//

		// Consider using (aScalar.theValue * MV)

	inline
	Scalar
	operator*
		( Scalar const & ss
		, Scalar const & sVal
		);

	inline
	Vector
	operator*
		( Scalar const & ss
		, Vector const & vVal
		);

	inline
	BiVector
	operator*
		( Scalar const & ss
		, BiVector const & bVal
		);

	inline
	TriVector
	operator*
		( Scalar const & ss
		, TriVector const & tVal
		);

	inline
	Spinor
	operator*
		( Scalar const & ss
		, Spinor const & spin
		);

	inline
	ImSpinor
	operator*
		( Scalar const & ss
		, ImSpinor const & ispin
		);

	//
	// ==== Vector.MV multiply
	//

	inline
	Vector
	operator*
		( Vector const & vVal
		, Scalar const & ss
		);

	//! Classic GA foundation product
	inline
	Spinor
	operator*
		( Vector const & vecA
		, Vector const & vecB
		);

	inline
	ImSpinor
	operator*
		( Vector const & vec
		, BiVector const & bv
		);

	inline
	BiVector
	operator*
		( Vector const & vec
		, TriVector const & bv
		);

	inline
	ImSpinor
	operator*
		( Vector const & vec
		, Spinor const & spin
		);

	inline
	Spinor
	operator*
		( Vector const & vec
		, ImSpinor const & ispin
		);

	//
	// ==== BiVector.MV Multiply
	//

	inline
	ImSpinor
	operator*
		( BiVector const & bv
		, Vector const & vec
		);

	inline
	Spinor
	operator*
		( BiVector const & bvA
		, BiVector const & bvB
		);

	inline
	Vector
	operator*
		( BiVector const & q1
		, TriVector const & q2
		);

	inline
	Spinor
	operator*
		( BiVector const & biv
		, Spinor const & spin
		);

	inline
	ImSpinor
	operator*
		( BiVector const & bv
		, ImSpinor const & ispin
		);

	//
	// ==== TriVector.MV Multiply
	//

	inline
	TriVector
	operator*
		( TriVector const & tv //!< commutes
		, Scalar const & sVal
		);

	inline
	BiVector
	operator*
		( TriVector const & tv //!< commutes
		, Vector const & vVal
		);

	inline
	Vector
	operator*
		( TriVector const & tv //!< commutes
		, BiVector const & bVal
		);

	inline
	Scalar
	operator*
		( TriVector const & tv //!< commutes
		, TriVector const & tVal
		);

	inline
	ImSpinor
	operator*
		( TriVector const & tv //!< commutes
		, Spinor const & spin
		);

	//
	// ==== Spinor.MV Multiply
	//

	inline
	ImSpinor
	operator*
		( Spinor const & spin
		, Vector const & vec
		);

	inline
	Spinor
	operator*
		( Spinor const & spin
		, BiVector const & biv
		);

	inline
	Spinor
	operator*
		( Spinor const & spinM
		, Spinor const & spinQ
		);

	inline
	ImSpinor
	operator*
		( Spinor const & spin
		, ImSpinor const & ispin
		);

	//
	// ==== ImSpinor.MV Multiply
	//

	inline
	Spinor
	operator*
		( ImSpinor const & ispin
		, Vector const & vec
		);

	inline
	ImSpinor
	operator*
		( ImSpinor const & ispin
		, BiVector const & bv
		);

	inline
	Spinor
	operator*
		( ImSpinor const & ispin
		, TriVector const & tv
		);

	inline
	ImSpinor
	operator*
		( ImSpinor const & ispin
		, Spinor const & spin
		);

	inline
	Spinor
	operator*
		( ImSpinor const & ispinM
		, ImSpinor const & ispinN
		);

}

// Inline definitions
#include "libga/operators.inl"

#endif // ga_operations_INCL_

