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
\brief Inline Definitions for cloud::cast
*/


namespace cloud
{
namespace cast
{

inline
int16_t
countFor
	( double const & dval
	)
{
	return static_cast<int16_t>(std::floor(sCountPerDub * dval));
}

inline
double
dubFor
	( int16_t const & count
	)
{
	return (sDubPerCount * double(count));
}

inline
cloud::FixedPoint
FixedPoint
	( float const & fx
	, float const & fy
	, float const & fz
	)
{
	return cloud::FixedPoint
		{{ countFor(fx), countFor(fy), countFor(fz) }};
}

inline
cloud::FixedPoint
FixedPoint
	( ga::Vector const & vec
	)
{
	return cloud::FixedPoint
		{{ countFor(vec[0]), countFor(vec[1]), countFor(vec[2]) }};
}

inline
ga::Vector
Vector
	( cloud::FixedPoint const & fpnt
	)
{
	return ga::Vector
		(dubFor(fpnt[0]), dubFor(fpnt[1]), dubFor(fpnt[2]));
}

}
}

