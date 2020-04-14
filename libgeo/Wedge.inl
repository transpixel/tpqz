//
//
// MIT License
//
// Copyright (c) 2020 Stellacore Corporation.
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
\brief Inline definitions for geo::Wedge
*/


namespace geo
{

inline
bool
Wedge :: isValid
	() const
{
	return dat::isValid(theTriangle);
}

inline
ga::Vector const &
Wedge :: vertex
	() const
{
	return theTriangle[1];
}

inline
ga::Vector const &
Wedge :: point1
	() const
{
	return theTriangle[0];
}

inline
ga::Vector const &
Wedge :: point2
	() const
{
	return theTriangle[2];
}

inline
ga::Vector
Wedge :: edge1
	() const
{
	return { point1() - vertex() };
}

inline
ga::Vector
Wedge :: edge2
	() const
{
	return { point2() - vertex() };
}

} // geo

