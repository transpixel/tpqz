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


#include "libdat/algorithm.h"
#include "libimg/sample.h"
#include "libio/stream.h"


namespace img
{
//======================================================================

template <typename PixType>
inline
bool
isValid
	( PixType const & pix
	)
{
//	return (isUnder(pix) || isActive(pix) || isOver(pix));
	return ((fpixUnder <= pix) && (pix <= fpixOver)); // should be faster
}

template <typename PixType>
inline
bool
isUnder
	( PixType const & pix
	)
{
	return (fpixUnder == pix);
}

template <typename PixType, dat::EnableIf< std::is_floating_point<PixType> >...>
inline
bool
isActive
	( PixType const & pix
	)
{
	return (std::isfinite(pix));
}

inline
bool
isActive
	( half_float::half const & pix
	)
{
	return (std::isfinite(pix));
}

template <typename PixType, dat::EnableIf< std::is_integral<PixType> >...>
inline
bool
isActive
	( PixType const & pix
	)
{
	return dat::isValid(pix);
}

template <typename PixType>
inline
bool
isActive
	( std::array<PixType, 3u> const & deepPix
	)
{
	return
		(  isActive(deepPix[0])
		&& isActive(deepPix[1])
		&& isActive(deepPix[2])
		);
}

template <typename PixType>
inline
bool
isOver
	( PixType const & pix
	)
{
	return (fpixOver == pix);
}

inline
unsigned long
uClamp
	( unsigned long const & minValue
	, unsigned long const & value
	, unsigned long const & maxValue
	)
{
	return dat::clamped<unsigned long>
		(value, std::pair<unsigned long, unsigned long>(minValue, maxValue));
}

inline
double
dClamp
	( double const & minValue
	, double const & value
	, double const & maxValue
	)
{
	return dat::clamped<double>
		(value, std::pair<double, double>(minValue, maxValue));
}

template <size_t Dim>
inline
std::array<fpix_t, Dim>
operator*
	( double const & lmult
	, std::array<fpix_t, Dim> const & arg
	)
{
	std::array<fpix_t, Dim> result;
	std::transform
		( arg.begin(), arg.end()
		, result.begin()
		, [& lmult] (fpix_t const & pix) { return lmult*pix; }
		);
	return result;
}

//======================================================================
}


