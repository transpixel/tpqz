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
\brief Inline definitions for geo::Ray
*/


namespace geo
{
//======================================================================

inline
bool
Ray :: isValid
	() const
{
	return (theStart.isValid() && theDir.isValid());
}


inline
ga::Vector
Ray :: pointAt
	( double const & dist
	) const
{
	return (theStart + dist*theDir);
}

inline
double
Ray :: distanceAlong
	( ga::Vector const & pnt
	) const
{
	ga::Vector const delta(pnt - theStart);
	return ga::dot(delta, theDir).theValue;
}

inline
ga::Vector
Ray :: projectionOf
	( ga::Vector const & pnt
	) const
{
	return (theStart + distanceAlong(pnt) * theDir);
}

inline
ga::Vector
Ray :: rejectionTo
	( ga::Vector const & pnt
	) const
{
	return (pnt - projectionOf(pnt));
}

inline
ga::BiVector
Ray :: angleTo
	( ga::Vector const & pnt
	) const
{
	ga::Vector const & dirFrom = theDir;
	ga::Vector const dirInto{ ga::unit(pnt - theStart) };
	ga::Spinor const spin{ ga::spin::between(dirFrom, dirInto) };
	return ga::spin::physicalAngleFrom(spin);
}

//======================================================================
}

