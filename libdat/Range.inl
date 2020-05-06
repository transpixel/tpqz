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
\brief Inline definitions for dat::Range
*/


#include "libdat/compare.h"
#include "libdat/info.h"

#include <cassert>
#include <sstream>


namespace dat
{
//======================================================================

template <typename Type>
template <typename FwdIter>
inline
// static
Range<Type>
Range<Type> :: spanning
	( FwdIter const & itBeg
	, FwdIter const & itEnd
	, Type const & endEpsilon
	)
{
	Range<Type> range;
	if (itEnd != itBeg)
	{
		std::pair<FwdIter, FwdIter> const minmax
			(std::minmax_element(itBeg, itEnd));
		Type const & min = *minmax.first;
		Type const & max = *minmax.second;
		range = Range<Type>(min, (max + endEpsilon));
	}
	return range;
}

template <typename Type>
template <typename FwdIter, typename ValueFunc>
inline
// static
Range<Type>
Range<Type> :: spanning
	( FwdIter const & itBeg
	, FwdIter const & itEnd
	, ValueFunc const & valueGetter
	, Type const & endEpsilon
	)
{
	Range<Type> range;
	if (itEnd != itBeg)
	{
		std::pair<FwdIter, FwdIter> const minmax
			( std::minmax_element
				( itBeg
				, itEnd
				, [&valueGetter]
					( typename FwdIter::value_type const & valA
					, typename FwdIter::value_type const & valB
					)
					{ return (valueGetter(valA) < valueGetter(valB)); }
				)
			);
		Type const & min = valueGetter(*minmax.first);
		Type const & max = valueGetter(*minmax.second);
		range = Range<Type>(min, (max + endEpsilon));
	}
	return range;
}

//
// Delegating ctor
//

template <typename Type>
inline
// explicit
Range<Type> :: Range
	( Type const & minValue
	, Type const & maxValue
	)
	: theMinMax(minValue, maxValue)
{
	if (! (minValue <= maxValue))
	{
		theMinMax = { dat::nullValue<Type>(), dat::nullValue<Type>() };
	}
}

template <typename Type>
inline
// explicit
Range<Type> :: Range
	( std::pair<Type, Type> const & range
	)
	: Range(range.first, range.second)
{
}

template <typename Type>
inline
bool
Range<Type> :: isValid
	() const
{
	return dat::isValid(theMinMax);
}

template <typename Type>
inline
Range<Type> :: operator std::pair<Type, Type>
	() const
{
	return theMinMax;
}

template <typename Type>
inline
std::pair<Type, Type> const &
Range<Type> :: pair
	() const
{
	return theMinMax;
}

template <typename Type>
inline
Type
Range<Type> :: operator[]
	( size_t const & ndx
	) const
{
	Type value{ dat::nullValue<Type>() };
	if (0u == ndx)
	{
		value = min();
	}
	else
	if (1u == ndx)
	{
		value = max();
	}
	return value;
}

template <typename Type>
inline
Type
Range<Type> :: min
	() const
{
	return theMinMax.first;
}

template <typename Type>
inline
Type
Range<Type> :: max
	() const
{
	return theMinMax.second;
}

template <typename Type>
inline
Type
Range<Type> :: magnitude
	() const
{
	return (max() - min());
}

template <typename Type>
inline
Type
Range<Type> :: middle
	() const
{
	double const x1(static_cast<double>(min()));
	double const x2(static_cast<double>(max()));
	double const ave(.5 * (x1 + x2));
	return static_cast<Type>(ave);
}

template <typename Type>
inline
bool
Range<Type> :: contains
	( Type const & value
	) const
{
	return
		(  (min() <= value)
		&& (value < max())
		);
}

template <typename Type>
inline
Range<Type>
Range<Type> :: intersectWith
	( Range<Type> const & other
	) const
{
	Range<Type> common;
	if (isValid() && other.isValid())
	{
		Type const & minThis = theMinMax.first;
		Type const & maxThis = theMinMax.second;
		Type const & xA = other.theMinMax.first;
		Type const & xB = other.theMinMax.second;
		// deal with the various overlap cases
		if (xA < minThis) // A < [min,max)
		{
			if (xB < minThis) // [A,B) < [min,max)
			{
				// no intersection - return default null
			}
			else
			if (xB < maxThis) // A <= min <= B < max
			{
				common = Range<Type>{ minThis, xB };
			}
			else // A <= min,max <= B
			{
				common = Range<Type>{ minThis, maxThis };
			}
		}
		else
		if (xA < maxThis) // min <= A < max
		{
			if (xB < minThis) // not possible for A<=B and min<=max
			{
				assert(! "should never be here");
			}
			else
			if (xB < maxThis) // min <= [A,B) < max
			{
				common = Range<Type>{ xA, xB };
			}
			else // min <= A < max <= B
			{
				common = Range<Type>{ xA, maxThis };
			}
		}
		else // [min,max) < [A,B) -- default case
		{
			// no intersection - return default null
		}
	}
	return common;
}

template <typename Type>
inline
Range<Type>
Range<Type> :: endExpanded
	( Type const & endDelta
	) const
{
	return Range<Type>(min(), max()+endDelta);
}

template <typename Type>
inline
bool
Range<Type> :: nearlyEquals
	( Range<Type> const & other
	, Type const & tol
	) const
{
	return dat::nearlyEquals(theMinMax, other.theMinMax, tol);
}

template <typename Type>
inline
std::string
Range<Type> :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << " ";
	}

	oss
		<< dat::infoString(min(), "min")
		<< " " << dat::infoString(max(), "max")
		<< " " << dat::infoString(magnitude(), "mag")
		;

	return oss.str();
}

//======================================================================
}

