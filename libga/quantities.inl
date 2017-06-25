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


/*! \file
\brief Inline definitions for ga::quantities
*/


namespace ga
{
//======================================================================

//
//----------------------------------------------------------------------
//

inline
// explicit
Scalar :: Scalar
	( Zero const & // zero
	)
	: theValue{ 0. }
{}

inline
// explicit
Scalar :: Scalar
	( One const & // one
	)
	: theValue{ 1. }
{}

inline
// explicit
Scalar :: Scalar
	( double const & value
	)
	: theValue{ value }
{}

inline
bool
Scalar :: isValid
	() const
{
	return (dat::isValid(theValue));
}

inline
double
Scalar :: magnitude
	() const
{
	return (std::abs(theValue));
}

inline
Scalar
Scalar :: reverse
	() const
{
	return Scalar(theValue);
}

inline
bool
Scalar :: nearlyEquals
	( Scalar const & other
	, double const & tol
	) const
{
	return dat::nearlyEquals(theValue, other.theValue, tol);
}

//
//----------------------------------------------------------------------
//

inline
// explicit
Vector :: Vector
	( Zero const & // zero
	)
	: theValues{{ 0., 0., 0. }}
{}

inline
// explicit
Vector :: Vector
	( double const & rx
	, double const & ry
	, double const & rz
	)
	: theValues{{ rx, ry, rz }}
{}

inline
// explicit
Vector :: Vector
	( std::array<double, 3u> const & values
	)
	: theValues(values)
{}

inline
bool
Vector :: isValid
	() const
{
	return dat::isValid(theValues[0]);
}

inline
double
Vector :: magnitude
	() const
{
	return (dat::magnitude(theValues));
}

inline
Vector
Vector :: reverse
	() const
{
	return Vector( theValues[0],  theValues[1],  theValues[2]);
}

inline
double const &
Vector :: operator[]
	( size_t const & validNdx
	) const
{
	return theValues[validNdx];
}

inline
double &
Vector :: operator[]
	( size_t const & validNdx
	)
{
	return theValues[validNdx];
}

inline
bool
Vector :: nearlyEquals
	( Vector const & other
	, double const & tol
	) const
{
	return dat::nearlyEquals(theValues, other.theValues, tol);
}

//
//----------------------------------------------------------------------
//

inline
// explicit
BiVector :: BiVector
	( Zero const & // zero
	)
	: theValues{{ 0., 0., 0. }}
{}

inline
// explicit
BiVector :: BiVector
	( double const & ix
	, double const & iy
	, double const & iz
	)
	: theValues{{ ix, iy, iz }}
{}

inline
// explicit
BiVector :: BiVector
	( std::array<double, 3u> const & values
	)
	: theValues(values)
{}

inline
bool
BiVector :: isValid
	() const
{
	return dat::isValid(theValues[0]);
}

inline
double
BiVector :: magnitude
	() const
{
	return (dat::magnitude(theValues));
}

inline
BiVector
BiVector :: reverse
	() const
{
	return BiVector{ -theValues[0], -theValues[1], -theValues[2] };
}

inline
double const &
BiVector :: operator[]
	( size_t const & validNdx
	) const
{
	return theValues[validNdx];
}

inline
double &
BiVector :: operator[]
	( size_t const & validNdx
	)
{
	return theValues[validNdx];
}

inline
bool
BiVector :: nearlyEquals
	( BiVector const & other
	, double const & tol
	) const
{
	return dat::nearlyEquals(theValues, other.theValues, tol);
}

//
//----------------------------------------------------------------------
//

inline
// explicit
TriVector :: TriVector
	( Zero const & // zero
	)
	: theValue{ 0. }
{}

inline
// explicit
TriVector :: TriVector
	( double const & value
	)
	: theValue{ value }
{}

inline
bool
TriVector :: isValid
	() const
{
	return (dat::isValid(theValue));
}

inline
double
TriVector :: magnitude
	() const
{
	return (std::abs(theValue));
}

inline
TriVector
TriVector :: reverse
	() const
{
	return TriVector(-theValue);
}

inline
bool
TriVector :: nearlyEquals
	( TriVector const & other
	, double const & tol
	) const
{
	return dat::nearlyEquals(theValue, other.theValue, tol);
}

//
//======================================================================
//

inline
// explicit
Spinor :: Spinor
	( One const & // one
	)
	: theS{ 1. }
	, theB{ 0., 0., 0. }
{
}

inline
// explicit
Spinor :: Spinor
	( Scalar const & ss
	, BiVector const & bv
	)
	: theS{ ss }
	, theB{ bv }
{
}

inline
// explicit
Spinor :: Spinor
	( double const & ss
	, BiVector const & bv
	)
	: theS{ ss }
	, theB{ bv }
{
}

inline
bool
Spinor :: isValid
	() const
{
	return (theS.isValid() && theB.isValid());
}

inline
double
Spinor :: magnitude
	() const
{
	return std::sqrt(math::sq(theS.theValue) + dat::magSq(theB.theValues));
}

inline
Spinor
Spinor :: reverse
	() const
{
	// NOTE: theS == theS.reverse()
	return Spinor{ theS, theB.reverse() };
}

inline
bool
Spinor :: nearlyEquals
	( Spinor const & other
	, double const & tol
	) const
{
	return
		(  theS.nearlyEquals(other.theS, tol)
		&& theB.nearlyEquals(other.theB, tol)
		);
}

//
//======================================================================
//

inline
// explicit
ImSpinor :: ImSpinor
	( Vector const & vec
	, TriVector const & tv
	)
	: theV{ vec }
	, theT{ tv }
{}

inline
bool
ImSpinor :: isValid
	() const
{
	return (theV.isValid() && theT.isValid());
}

inline
double
ImSpinor :: magnitude
	() const
{
	return std::sqrt(dat::magSq(theV.theValues) + math::sq(theT.theValue));
}

inline
ImSpinor
ImSpinor :: reverse
	() const
{
	// NOTE: theV == theV.reverse()
	return ImSpinor{ theV, theT.reverse() };
}

inline
bool
ImSpinor :: nearlyEquals
	( ImSpinor const & other
	, double const & tol
	) const
{
	return
		(  theV.nearlyEquals(other.theV, tol)
		&& theT.nearlyEquals(other.theT, tol)
		);
}

//======================================================================
}

