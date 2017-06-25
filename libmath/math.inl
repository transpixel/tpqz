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
\brief Inline definitions for math::math
*/


#include "libdat/dat.h"


namespace math
{
// ======================================================================

template <typename SType, dat::EnableIf<std::is_signed<SType> >...>
inline
SType
separation
	( SType const & valA
	, SType const & valB
	)
{
	SType mag(dat::nullValue<SType>());
	if (dat::isValid(valA) && dat::isValid(valB))
	{
		// note which is/are negative
		bool const negA(valA < SType(0));
		bool const negB(valB < SType(0));

		// get absolute value of each argument
		SType const posA((negA) ? (-valA) : (valA));
		SType const posB((negB) ? (-valB) : (valB));

		if (negA == negB)
		{
			if (posB < posA)
			{
				mag = posA - posB;
			}
			else
			{
				mag = posB - posA;
			}
		}
		else
		{
			// TODO - this is an overly conservative test - could be replaced
			static constexpr SType two(2);
			static constexpr SType halfMax
				(std::numeric_limits<SType>::max() / two);
			if ((posA < halfMax) && (posB < halfMax))
			{
				mag = posA + posB;
			}
			// else - leave as null
		}
	}
	return mag;
}

template <typename UType, dat::EnableIf< std::is_unsigned<UType> >...>
inline
UType
separation
	( UType const & valA
	, UType const & valB
	)
{
	UType diff(dat::nullValue<UType>());
	if (dat::isValid(valA) && dat::isValid(valB))
	{
		diff = 0;
		if (valA < valB)
		{
			diff = valB - valA;
		}
		else
		if (valB < valA)
		{
			diff = valA - valB;
		}
	}
	return diff;
}

template <typename Type>
inline
constexpr
Type
sq
	( Type const & val
	)
{
	return (val * val);
}

template <typename Type>
inline
Type
hypot
	( Type const & xx
	, Type const & yy
	)
{
	return std::sqrt(math::sq(xx) + math::sq(yy));
}

template <typename Type>
inline
constexpr
Type
cube
	( Type const & val
	)
{
	return (val * sq(val));
}

template <typename Type, typename FwdIter>
inline
Type
maxAbs
	( FwdIter const & beg
	, FwdIter const & end
	)
{
	Type maxval(dat::nullValue<Type>());
	std::pair<FwdIter, FwdIter> const itMinMax(std::minmax_element(beg, end));
	FwdIter const & itMin = itMinMax.first;
	FwdIter const & itMax = itMinMax.second;
	if (end != itMin)
	{
		maxval = std::max( std::abs(*itMin), std::abs(*itMax) );
	}
	return maxval;
}

template <typename Type, typename FwdIter>
inline
Type
sumSqs
	( FwdIter const & beg
	, FwdIter const & end
	)
{
	Type const sse(std::inner_product(beg, end, beg, static_cast<Type>(0)));
	return sse;
}

template
	< typename Type
	, typename TmpType = double
	, typename FwdIter
	>
inline
Type
rms
	( FwdIter const & beg
	, FwdIter const & end
	, size_t const & dofUsed
	)
{
	Type result(dat::nullValue<Type>());
	// promote everything for intermediate computations
	TmpType const sse{ static_cast<TmpType>(sumSqs<Type>(beg, end)) };
	size_t const size{ static_cast<size_t>(std::distance(beg, end)) };
	if (dofUsed < size)
	{
		TmpType const dof{(TmpType)size - (TmpType)dofUsed};
		TmpType const tmp(std::sqrt(sse / dof));
		result = static_cast<Type>(tmp);
	}
	return result;
}

// ======================================================================
}

