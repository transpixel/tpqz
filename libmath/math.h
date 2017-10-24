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

#ifndef math_math_INCL_
#define math_math_INCL_

/*! \file
\brief Declarations for namespace math
*/


#include "libmath/angle.h"
#include "libdat/template.h"

#include <cmath>
#include <cstddef>
#include <limits>
#include <utility>


/*! \brief Declarations and Definitions for libmath.

\par General Concept:

Extends standard library operations with useful special case capabilities.

\par Special Notes:

+ package specific values - e.g. numerical tolerances
+ angle-related operations - e.g. interplation in presence of phase-wrap

\par Example
\dontinclude testmath/umath.cpp
\skip ExampleStart
\until ExampleEnd

*/
namespace math
{
	//! Factor to allow for numeric round-off
	constexpr double eps(256.*std::numeric_limits<double>::epsilon());

	/*! \brief Separation (aka distance, magnitude) between two values.
	 *
	 * NOTE: if separation is too large to represent in type
	 *       the return type is nullValue<SType>().
	 */
	template <typename SType, dat::EnableIf< std::is_signed<SType> >...>
	inline
	SType
	separation
		( SType const & valA
		, SType const & valB
		);

	//! Separation for unsigned type
	template <typename UType, dat::EnableIf< std::is_unsigned<UType> >...>
	inline
	UType
	separation
		( UType const & valA
		, UType const & valB
		);

	//! Square of an instance == (val*val)
	template <typename Type>
	inline
	constexpr
	Type
	sq
		( Type const & val
		);

	//! Faster version of std::hypot() -- does NOT check validity/nan/etc
	template <typename Type>
	inline
	Type
	hypot
		( Type const & xx
		, Type const & yy
		);

	//! Cube of an instance == (val*val*val)
	template <typename Type>
	inline
	constexpr
	Type
	cube
		( Type const & val
		);

	//! Maximum absolute value
	template <typename Type, typename FwdIter>
	inline
	Type
	maxAbs
		( FwdIter const & beg
		, FwdIter const & end
		);

	//! Sum of squares (std::inner_product() with repeated/known args)
	template <typename Type, typename FwdIter>
	inline
	Type
	sumSqs // NOTE: Watch for overflow for "small" types
		( FwdIter const & beg
		, FwdIter const & end
		);

	//! Root-Sum-Squares - specifically sqrt(sumSq / (size - dofUsed))
	template
		< typename Type
		, typename TmpType = double //!< type for intermediate computations
		, typename FwdIter
		>
	inline
	Type
	rms // NOTE: Watch for overflow for "small" types
		( FwdIter const & beg
		, FwdIter const & end
		, size_t const & dofUsed
		);

}

// Inline definitions
#include "libmath/math.inl"

#endif // math_math_INCL_

