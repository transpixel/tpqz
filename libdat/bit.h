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

#ifndef dat_bit_INCL_
#define dat_bit_INCL_

/*! \file
\brief Declarations for dat::bit
*/


#include <cstdint>


namespace dat
{

/*! \brief Bitwise functions

\par Example
\dontinclude testdat/ubit.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace bit
{

	//! True if the requested bit is set
	template <typename Type>
	inline
	bool
	isSet
		( Type const & pattern
		, uint8_t const & bitPos
		)
	{
		return (1 == ((pattern >> bitPos) & 1));
	}

	//! True if the requested bit is clear
	template <typename Type>
	inline
	bool
	isClear
		( Type const & pattern
		, uint8_t const & bitPos
		)
	{
		return (! isSet(pattern, bitPos));
	}

	//! Set the requested bit
	template <typename Type>
	inline
	void
	set
		( Type * const & pattern
		, uint8_t const & bitPos
		)
	{
		(*pattern) |= (1 << bitPos);
	}
		
	//! Clear the requested bit
	template <typename Type>
	inline
	void
	clear
		( Type * const & pattern
		, uint8_t const & bitPos
		)
	{
		(*pattern) &= ~(1 << bitPos);
	}


} // bit

} // dat

// Inline definitions
// #include "libdat/bit.inl"

#endif // dat_bit_INCL_

