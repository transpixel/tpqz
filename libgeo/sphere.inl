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
\brief Inline definitions for geo::sphere
*/


namespace geo
{
namespace sphere
{
//======================================================================

inline
double
azimuthOf
	( ga::Vector const & cubedir
	)
{
	return math::atan2(cubedir[1], cubedir[0]);
}

inline
double
zenithOf
	( ga::Vector const & cubedir
	)
{
	return std::acos(cubedir[2]);
}

inline
ga::Vector
directionFromAZ
	( double const & azim
	, double const & zenith
	)
{
	double const sZ{ std::sin(zenith) };
	return ga::Vector
		{ sZ * std::cos(azim)
		, sZ * std::sin(azim)
		, std::cos(zenith)
		};
}


//======================================================================
}
}

