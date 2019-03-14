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

#ifndef ga_functions_INCL_
#define ga_functions_INCL_

/*! \file
\brief Declarations for ga::functions
*/


#include "libga/operators.h"
#include "libga/quantities.h"
#include "libga/spin.h"
#include "libmath/math.h"


namespace ga
{
	// General purpose GA functions.

	//! Inverse of multivector if it exists (else null instance)
	template <typename Type>
	inline
	Type
	inverse
		( Type const & arg
		);

	//! Magnitude of quantity
	template <typename Type>
	inline
	double
	magnitude
		( Type const & arg
		);

	//! Parallel quantity with unit magnitude (or null)
	template <typename Type>
	inline
	Type
	unit
		( Type const & arg
		);

	//! Returns both (magnitude(), unit())
	template <typename Type>
	inline
	std::pair<double, Type>
	magdir
		( Type const & arg
		);

	//
	// ==== General functions
	//

	//! Specialization of 'exp' - most common case - can be generalized
	inline
	Spinor
	exp
		( BiVector const & arg
		);

	//! Specialization of 'exp' for G2-subspace element
	inline
	Spinor
	expG2
		( Spinor const & elemG2
		);

	//! Logarithm of general G2-subspace element
	inline
	Spinor
	logG2
		( Spinor const & elemG2
		, BiVector const & planeForPi = {}
		);

	//! Specialization of 'sqrt' - common case
	inline
	Spinor
	sqrt
		( Spinor const & spin
		);

}

// Inline definitions
#include "libga/functions.inl"

#endif // ga_functions_INCL_

