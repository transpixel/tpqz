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
\brief Inline definitions for geo::LineSeg
*/


namespace geo
{
//======================================================================

inline
ga::Vector const &
LineSeg :: start
	() const
{
	return theRay.theStart;
}

inline
ga::Vector
LineSeg :: stop
	() const
{
	return theRay.pointAt(theLength);
}

inline
ga::Vector const &
LineSeg :: direction
	() const
{
	return theRay.theDir;
}

inline
bool
LineSeg :: contains
	( double const & dist
	, double const & tol
	) const
{
	double const maxDist{ theLength + tol };
	return ((-tol <= dist) && (dist < maxDist));
}

inline
bool
LineSeg :: containsProjectionOf
	( ga::Vector const & pnt
	, double const & tol
	) const
{
	return contains(theRay.distanceAlong(pnt), tol);
}

inline
double
LineSeg :: fractionAtProjectionOf
	( ga::Vector const & pnt
	) const
{
	double frac{ dat::nullValue<double>() };
	if (isValid())
	{
		double const dist{ theRay.distanceAlong(pnt) };
		assert(std::numeric_limits<double>::min() < theLength);
		frac = dist / theLength;
	}
	return frac;
}

inline
ga::Vector
LineSeg :: pointAtDistance
	( double const & dist
	) const
{
	return theRay.pointAt(dist);
}

inline
ga::Vector
LineSeg :: pointAtFraction
	( double const & frac
	) const
{
	return pointAtDistance(frac * theLength);
}

//======================================================================
}

