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

#ifndef dat_array_INCL_
#define dat_array_INCL_

/*! \file
\brief Declarations for dat::array
*/


#include "libdat/dat.h"
#include "libdat/template.h"

#include <algorithm>
#include <array>
#include <type_traits>


namespace dat
{
//======================================================================

// Operations on std::array data type

	//! An array with first element nullValue
	template <typename Type, size_t Dim>
	std::array<Type, Dim>
	nullValue
		();

	//! An array of AType with first element null
	template <typename AType>
	AType
	nullArray
		();

	//! True if array contains valid data
	template <typename Type>
	inline
	bool
	isValid
		( std::array<Type, 2u> const & array2d
		);

	//! True if array contains valid data
	template <typename Type>
	inline
	bool
	isValid
		( std::array<Type, 3u> const & array3d
		);

	//! True if array contains valid data
	template <typename Type, size_t Dim>
	inline
	bool
	isValid
		( std::array<Type, Dim> const & arrayNd
		);

	//! Uniary negation
	template <typename Type, size_t Dim>
	inline
	std::array<Type, Dim>
	operator-
		( std::array<Type, Dim> const & vals
		);

	//! Sum-of-squares of elements
	template <typename Type, size_t Dim>
	inline
	Type
	magSq
		( std::array<Type, Dim> const & vals
		);

	//! sqrt (of magSq)
	template <typename Type, size_t Dim>
	inline
	Type
	magnitude
		( std::array<Type, Dim> const & vals
		);

	//! Direction of vec (or null)
	template
		< typename FType, size_t Dim
		, EnableIf< std::is_floating_point<FType> >...
		>
	inline
	std::array<FType, Dim>
	unit
		( std::array<FType, Dim> const & vec
		);

	//! Array addition
	template <typename Type, size_t Dim>
	inline
	std::array<Type, Dim>
	operator+
		( std::array<Type, Dim> const & argA
		, std::array<Type, Dim> const & argB
		);

	//! Array subtraction
	template <typename Type, size_t Dim>
	inline
	std::array<Type, Dim>
	operator-
		( std::array<Type, Dim> const & argA
		, std::array<Type, Dim> const & argB
		);

	//! Array scalar multiply
	template <typename Type, size_t Dim>
	inline
	std::array<Type, Dim>
	operator*
		( Type const & lmult
		, std::array<Type, Dim> const & arg
		);

	//! Return reciprocals of members
	template
		< typename FType
		, size_t Dim
		, EnableIf< std::is_floating_point<FType> >...
		>
	inline
	std::array<FType, Dim>
	reciprocals
		( std::array<FType, Dim> const & arg
		);

	//! Inner product between two arrays (via std::inner_product)
	template <typename Type, size_t Dim>
	inline
	Type
	dot
		( std::array<Type, Dim> const & argA
		, std::array<Type, Dim> const & argB
		);

//======================================================================
}

// Inline definitions
#include "libdat/array.inl"

#endif // dat_array_INCL_

