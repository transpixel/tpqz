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
\brief Inline definitions for dat::ops
*/



namespace dat
{
//======================================================================

template <typename Type>
inline
dat::grid<Type>
scaled
	( Type const & scalar
	, dat::grid<Type> const & ingrid
	)
{
	dat::grid<Type> result;
	if (ingrid.isValid())
	{
		result = dat::grid<Type>(ingrid.hwSize());
		std::transform
			( ingrid.begin(), ingrid.end()
			, result.begin()
			, [& scalar] (Type const & inval) { return (scalar*inval); }
			);
	}
	return result;
}

inline
dat::Extents
scaledCeil
	( double const & scalar
	, dat::Extents const & hwSize
	)
{
	double const fhigh(scalar * double(hwSize.high()));
	double const fwide(scalar * double(hwSize.wide()));
	size_t const ohigh{ static_cast<size_t>(std::ceil(fhigh)) };
	size_t const owide{ static_cast<size_t>(std::ceil(fwide)) };
	return dat::Extents(ohigh, owide);
}

inline
Spot
centerOf
	( Extents const & someSize
	)
{
	return Spot
		{{ .5 * static_cast<double>(someSize.high())
		 , .5 * static_cast<double>(someSize.wide())
		}};
}

inline
double
diagonalMag
	( Extents const & someSize
	)
{
	return std::hypot(someSize.high(), someSize.wide());
}

inline
size_t
areaOf
	( Extents const & someSize
	)
{
	return someSize.size();
}

inline
size_t
perimeterOf
	( Extents const & someSize
	)
{
	return
		( 2u * someSize.high() // top and bottom row
		+ 2u * someSize.wide() // left and right column
		- 4u                   // remove corners counted twice
		);
}

inline
double
radiusCircumscribed
	( Extents const & someSize
	)
{
	// return magnitude(centerOf(someSize));
	return
		{ .5 * std::hypot
			( static_cast<double>(someSize.high())
			, static_cast<double>(someSize.wide())
			)
		};
}

inline
double
angleToward
	( Spot const & toward
	, Spot const & from
	)
{
	double const dy(toward[1] - from[1]);
	double const dx(toward[0] - from[0]);
	return std::atan2(dy, dx);
}

inline
double
magnitude
	( Spot const & arg
	)
{
	// code explicitly to avoid extra operations in math::hypot
	// E.g. skip worry about infy/tests etc... - nan's propagate
	return std::sqrt(arg[0]*arg[0] + arg[1]*arg[1]);
}

//======================================================================
}

