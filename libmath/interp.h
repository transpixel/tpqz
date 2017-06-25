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

#ifndef math_interp_INCL_
#define math_interp_INCL_

/*! \file
\brief Declarations for math::interp
*/


#include "libdat/Area.h"
#include "libdat/dat.h"
#include "libdat/Range.h"
#include "libdat/Spot.h"

#include <functional>
#include <utility>
#include <vector>


namespace math
{

/*! \brief Functions supporting interpolation of values.

\par Example
\dontinclude testmath/uinterp.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace interp
{
	//! Weighted combination of two values
	template <typename Type>
	inline
	Type
	weightedCombo
		( double const & w1 //!< assumed in range [0 <= w1 <= 1.]
		, Type const & value1 //!< incorporated with weight w1
		, Type const & value2 //!< incorporated with weight (1-w1)
		);

	/*! Fractional position of value within range (start, finish).
	 *
	 * Note: values are assumed to be valid! I.e.
	 *       rangePair.first <= value <= rangePair.second
	 *       rangePair.first < rangePair.second
	 */
	inline
	double
	fractionAtValid
		( double const & value
		, std::pair<double, double> const & rangePair
		);

	//! Value at fraction of the way into range: Ref note #fractionForValid.
	template <typename OutType>
	inline
	OutType
	valueAtValid
		( double const & frac
		, std::pair<OutType, OutType> const & rangePair
		);

	//! Value at fraction of the way into range: Ref note #fractionForValid.
	template <typename OutType>
	inline
	OutType
	valueAtValid
		( double const & frac
		, dat::Range<OutType> const & range
		);

	//! Linearly interpolated value
	template <typename OutType>
	inline
	OutType
	linear
		( std::pair<double, double> const & fromRange
		, double const & fromValue
		, std::pair<OutType, OutType> const & yRange
		);

	//! Linearly interpolated value from collection
	template <typename OutType, typename Collection>
	inline
	OutType
	linear
		( double const & subIndex
		, Collection const & values //!< supporting op[] access
		);

	//! Linearly interpolated value in each coordinate
	inline
	dat::Spot
	linear
		( dat::Area<double> const & fromArea
		, dat::Spot const & fromSpot
		, dat::Area<double> const & intoArea
		);

}

}

// Inline definitions
#include "libmath/interp.inl"

#endif // math_interp_INCL_

