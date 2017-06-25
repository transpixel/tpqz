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


/*! \file
\brief Inline definitions for math::interp
*/


#include "libmath/math.h"


namespace math
{
namespace interp
{
//======================================================================

template <typename Type>
inline
Type
weightedCombo
	( double const & w1
	, Type const & value1
	, Type const & value2
	)
{
	// (w1*value1 + (1. - w1)*value2)
	return (w1*(value1 - value2) + value2);
}

inline
double
fractionAtValid
	( double const & value
	, std::pair<double, double> const & rangePair
	)
{
	double const mag(rangePair.second - rangePair.first);
	double const delta(value - rangePair.first);
	return (delta / mag);
}

template <typename OutType>
inline
OutType
valueAtValid
	( double const & frac
	, std::pair<OutType, OutType> const & rangePair
	)
{
	OutType const range(rangePair.second - rangePair.first);
	OutType const delta(frac * range);
	return (rangePair.first + delta);
}

template <typename OutType>
inline
OutType
valueAtValid
	( double const & frac
	, dat::Range<OutType> const & range
	)
{
	return valueAtValid(frac, range.pair());
}

template <typename OutType>
inline
OutType
linear
	( std::pair<double, double> const & fromRange
	, double const & fromValue
	, std::pair<OutType, OutType> const & intoRange
	)
{
	double const xFrac(fractionAtValid(fromValue, fromRange));
	double const & yFrac = xFrac;
	OutType const intoValue(valueAtValid<OutType>(yFrac, intoRange));
	return intoValue;
}

template <typename OutType, typename Collection>
inline
OutType
linear
	( double const & subIndex
	, Collection const & values
	)
{
	OutType result(dat::nullValue<OutType>());
	if (! values.empty())
	{
		if (0. == subIndex)
		{
			result = values[0];
		}
		else
		if (1u < values.size())
		{
			double const lastNdx(values.size() - 1u);
			if ((0. < subIndex) && (subIndex < lastNdx))
			{
				// determine bracketing indices
				double fndx(0.);
				double const frac(modf(subIndex, &fndx));
				size_t const loNdx(static_cast<size_t>(fndx));
				size_t const hiNdx(loNdx + 1u);

				// interpolate within brackets
				std::pair<double, double> const valueRange
					(values[loNdx], values[hiNdx]);
				result = valueAtValid<OutType>(frac, valueRange);
			}
		}
	}
	return result;
}

inline
dat::Spot
linear
	( dat::Area<double> const & fromArea
	, dat::Spot const & fromSpot
	, dat::Area<double> const & intoArea
	)
{
	return dat::Spot
		{{ linear(fromArea[0].pair(), fromSpot[0], intoArea[0].pair())
		 , linear(fromArea[1].pair(), fromSpot[1], intoArea[1].pair())
		}};
}

//======================================================================
}
}

