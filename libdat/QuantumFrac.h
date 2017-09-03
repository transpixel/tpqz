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

#ifndef dat_QuantumFrac_INCL_
#define dat_QuantumFrac_INCL_

/*! \file
\brief Declarations for dat::QuantumFrac
*/


#include "libdat/compare.h"
#include "libdat/quantum.h"
#include "libdat/validity.h"

#include <cmath>
#include <string>


namespace dat
{

/*! \brief Decomposition of value into (Quantum + Fraction) parts.

The domain over which values are assumed to be defined, is implicitly
partitioned into half open intervals of the form [floor, floor+1).
The residual component expresses the fractional position within the
interval such that residual values are in the half-open range [0, 1)

NOTE: The quantization partition is consistent for negative numbers
such that it is continuous across zero (i.e. it does **NOT** quantize
toward zero like std::div or similar operations).

E.g.:
\arg +5.25:  +5, +.25
\arg -5.25:  -6, +.75


\par Example
\dontinclude testdat/uQuantumFrac.cpp
\skip ExampleStart
\until ExampleEnd
*/

class QuantumFrac
{

public: // data

	using Type = double;

	quantum::Splitter<long, Type> theQuant{ dat::nullValue<Type>() };
	long theFloor{ dat::nullValue<long>() };
	Type theResid{ dat::nullValue<Type>() };

public: // methods

	//! default null constructor
	QuantumFrac
		() = default;

	//! Construct by partitioning value
	inline
	explicit
	QuantumFrac
		( Type const & value
		, quantum::Splitter<long, Type> const & quantFunc
		);

	//! Value constructor
	inline
	explicit
	QuantumFrac
		( long const & floor
		, Type const & resid
		, quantum::Splitter<long, Type> const & quantFunc
		);

	//! True if instance is valid
	inline
	bool
	isValid
		() const;

	//! The unit of quantization
	inline
	Type const &
	delta
		() const;

	//! True if this is same as other within residual tolerance
	bool
	nearlyEquals
		( QuantumFrac const & other
		, Type const & tolResid = dat::smallValue<Type>()
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // QuantumFrac

} // dat

// Inline definitions
#include "libdat/QuantumFrac.inl"

#endif // dat_QuantumFrac_INCL_

