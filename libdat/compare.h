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

#ifndef dat_compare_INCL_
#define dat_compare_INCL_

/*! \file
\brief Declarations for dat::compare
*/


#include "libdat/limits.h"
#include "libdat/validity.h"
#include "libdat/template.h"

#include <cmath>
#include <complex>
#include <iterator>
#include <type_traits>
#include <vector>


namespace dat
{

// Support for data value comparison

	//
	// Data comparisons
	//

	//! True if values are within tol of each other
	template <typename Type>
	inline
	bool
	nearlyEquals
		( Type const & valA
		, Type const & valB
		, Type const & tol = smallValue<Type>()
		);

	//! True if same sizes and *all* values are within tol of each other
	template <typename Type, typename FwdIter1, typename FwdIter2>
	inline
	bool
	nearlyEquals
		( FwdIter1 const & beg1
		, FwdIter1 const & end1
		, FwdIter2 const & beg2
		, FwdIter2 const & end2
		, Type const & tol = smallValue<Type>()
		);

/*
	//! Like nearlyEquals but for compound objects with fundamental tol type
	template <typename TolType, typename FwdIter1, typename FwdIter2>
	inline
	bool
	nearlyEqualsCompound
		( FwdIter1 const & beg1
		, FwdIter1 const & end1
		, FwdIter2 const & beg2
		, FwdIter2 const & end2
		, TolType const & tol = smallValue<TolType>()
		);
*/

	//! Test vectors
	template <typename DataType, typename TolType>
	inline
	bool
	nearlyEquals
		( std::vector<DataType> const & d1
		, std::vector<DataType> const & d2
		, TolType const & tol
		);

	//! Test arrays
	template <typename Type, size_t Dim>
	bool
	inline
	nearlyEquals
		( std::array<Type, Dim> const & testValue
		, std::array<Type, Dim> const & refValue
		, Type const & tol = smallValue<Type>()
		);

	//! True if both values are same within respective tolerances
	template <typename Type1, typename Type2>
	inline
	bool
	nearlyEquals
		( std::pair<Type1, Type2> const & pairA
		, std::pair<Type1, Type2> const & pairB
		, Type1 const & tol1 = smallValue<Type1>()
		, Type2 const & tol2 = smallValue<Type2>()
		);

	//! True if both real/imag parts are within tol of each other
	template <typename Type>
	inline
	bool
	nearlyEquals
		( std::complex<Type> const & valA
		, std::complex<Type> const & valB
		, Type const & tol = smallValue<Type>()
		);

	//! True if a) both are INvalid; b) both valid AND nearlyEqual
	template <typename ArgType, typename DataType>
	inline
	bool
	areSame
		( ArgType const & itemA //!< Test members as DataType
		, ArgType const & itemB
		);

	//! True if a) both are INvalid; b) both valid AND nearlyEqual
	template <typename Type>
	inline
	bool
	areSame
		( Type const & itemA
		, Type const & itemB
		);

	template <typename DatType, size_t Dim>
	inline
	bool
	areSame
		( std::array<DatType, Dim> const & itemA
		, std::array<DatType, Dim> const & itemB
		);

	//! Like areSame() but w/o use of (default) tolerance
	template <typename Type>
	inline
	bool
	areSameNoSmall
		( Type const & itemA
		, Type const & itemB
		);

	//! True if value is an even number
	template <typename IType, EnableIf< std::is_integral<IType> >...>
	inline
	bool
	isEven
		( IType const & value
		);

	//! True if value is an odd number
	template <typename IType, EnableIf< std::is_integral<IType> >...>
	inline
	bool
	isOdd
		( IType const & value
		);

	//! True if both values are isOdd() or both are isEven()
	template <typename IType, EnableIf< std::is_integral<IType> >...>
	inline
	bool
	isSameParity
		( IType const & value1
		, IType const & value2
		);

	//! True if (big - small) is okay - i.e. will not underflow/wrap
	template <typename UType, DisableIf< std::is_signed<UType> >...>
	inline
	bool
	canSubtract
		( UType const & big
		, UType const & small
		);

	//! True if value1 is strictly less than value2
	template <typename Type>
	inline
	bool
	isLess
		( Type const & value1
		, Type const & value2
		);

	//! True for (! isLess(value1,value2))
	template <typename Type>
	inline
	bool
	isNotLess
		( Type const & value1
		, Type const & value2
		);
	
}

// Inline definitions
#include "libdat/compare.inl"

#endif // dat_compare_INCL_

