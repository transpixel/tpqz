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
\brief Inline definitions for math::Partition
*/


#include "libdat/info.h"
#include "libdat/validity.h"
#include "libmath/interp.h"

#include <cassert>
#include <cstddef>
#include <cmath>
#include <iterator>


namespace math
{
//======================================================================

template <typename FwdIter>
inline
// static
Partition
Partition :: spanning
	( FwdIter const & beg
	, FwdIter const & end
	, size_t const & numParts
	, double const & endEpsilon
	)
{
	Partition part;
	if (beg != end)
	{
		// determine min max data values
		dat::Range<double> const dataRange
			(dat::Range<double>::spanning(beg, end, endEpsilon));

		// construct partition
		part = Partition(dataRange, numParts);
	}
	return part;
}

inline
bool
Partition :: isValid
	() const
{
	return
		(  dat::isValid(theMin)
		&& dat::isValid(theDelta)
		&& (std::numeric_limits<double>::min() < theDelta)
		&& (dat::isValid(theNumParts) && (0u < theNumParts))
		);
}

inline
size_t
Partition :: size
	() const
{
	return theNumParts;
}

inline
double
Partition :: min
	() const
{
	return theMin;
}

inline
double
Partition :: max
	() const
{
	return interpValueFor(theNumParts);
}

inline
dat::Range<double>
Partition :: range
	() const
{
	dat::Range<double> rng;
	if (isValid())
	{
		rng = dat::Range<double>(min(), max());
		assert(rng.isValid());
	}
	return rng;
}

inline
dat::Range<double>
Partition :: rangeForBin
	( size_t const & ndx
	) const
{
	dat::Range<double> rng;
	if (ndx < theNumParts)
	{
		// here 'interp' is exact
		double const valLo(interpValueFor((double)ndx));
		double const valHi(valLo + theDelta);
		rng = dat::Range<double>(valLo, valHi);
	}
	return rng;
}

inline
size_t
Partition :: binIndexFor
	( double const & value
	) const
{
	size_t ndx(dat::nullValue<size_t>());
	if (dat::isValid(value))
	{
		if (theMin < value)
		{
			double const offset(value - theMin);
			double const intervals(offset / theDelta);
			size_t const binNdx((size_t)std::floor(intervals));
			if (binNdx < theNumParts)
			{
				ndx = binNdx;
			}
		}
		else
		if (theMin == value)
		{
			ndx = 0u;
		}
	}
	return ndx;
}

inline
double
Partition :: interpValueFor
	( double const & subIndex
	) const
{
	return (theMin + (subIndex * theDelta));
}

inline
double
Partition :: interpIndexFor
	( double const & value
	) const
{
	double fndx(dat::nullValue<double>());
	size_t const ndx{binIndexFor(value)};
	if (dat::isValid(ndx))
	{
		std::pair<double, double> const bMinMax(rangeForBin(ndx));
		double const frac
			(math::interp::fractionAtValid(value, bMinMax));
		fndx = (double)ndx + frac;
	}
	return fndx;
}

template<typename IType>
inline
IType
Partition :: indexFor
	( double const & value
	) const
{
	return static_cast<IType>(interpIndexFor(value));
}

//======================================================================
}

