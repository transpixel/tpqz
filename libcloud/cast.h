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

#ifndef cloud_cast_INCL_
#define cloud_cast_INCL_

/*! \file
\brief Declarations for cloud::cast
*/


#include "libcloud/FixedPoint.h"
#include "libdat/array.h"
#include "libga/ga.h"


namespace cloud
{

/*! \brief Functions for casting between cloud entities

\par Example
\dontinclude testcloud/ucast.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace cast
{
	//! encoding provides data range of approx +/- 16. double units
	constexpr double sCountPerDub{ 2. * 1024. }; // about .000488 per
	constexpr double sDubPerCount{ 1./sCountPerDub };

	//! Encode double into low 16 bits
	inline
	int16_t
	countFor
		( double const & dval
		);

	//! Decode low 16-bits into double
	inline
	double
	dubFor
		( int16_t const & count
		);

	//! Encode vector point into packed representation
	inline
	cloud::FixedPoint
	FixedPoint
		( float const & fx
		, float const & fy
		, float const & fz
		);

	//! Encode vector point into packed representation
	inline
	cloud::FixedPoint
	FixedPoint
		( ga::Vector const & vec
		);

	//! Decode packed representation into full vector
	inline
	ga::Vector
	Vector
		( cloud::FixedPoint const & fpnt
		);

}
}

// Inline definitions
#include "libcloud/cast.inl"

#endif // cloud_cast_INCL_

