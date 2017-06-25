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
\brief Inline definitions for ga::Pose
*/


namespace ga
{
//======================================================================

namespace priv
{
	inline
	ga::Spinor
	spinPos0
		( ga::Spinor const & anySpin
		)
	{
		ga::Spinor posSpin{ anySpin };
		if (anySpin.theS.theValue < 0.)
		{
			// posSpin = -anySpin;
			posSpin = ga::Spinor(-anySpin.theS, -anySpin.theB);
		}
		return posSpin;
	}
}

inline
// static
Pose const &
Pose :: identity
	()
{
static Pose const ident{bZero};
	return ident;
}

inline
Pose :: Pose
	()
	: theSpin{}
{
}

// explicit
inline
Pose :: Pose
	( BiVector const & physAngle
	)
	: theSpin{ priv::spinPos0(exp(.5 * physAngle)) }
{
}

inline
// explicit
Pose :: Pose
	( Spinor const & halfSpin
	)
	: theSpin{ priv::spinPos0(unit(halfSpin)) }
{
}


// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

inline
bool
Pose :: isValid
	() const
{
	return theSpin.isValid();
}

inline
BiVector
Pose :: physicalAngle
	() const
{
	return spin::physicalAngleFrom(theSpin);
}

inline
Vector
Pose :: operator()
	( Vector const & vec
	) const
{
	return rotated(theSpin, vec);
}

inline
BiVector
Pose :: operator()
	( BiVector const & biv
	) const
{
	// if speed required, could use expanded rotated() as does vector
	// for here, exploit duality - use array copy to avoid extra multiplies
	Vector const veca{biv.theValues};
	Vector const vecb(rotated(theSpin, veca));
	return BiVector{vecb.theValues};
}

inline
Pose
Pose :: inverse
	() const
{
	return Pose{theSpin.reverse()};
}

inline
Pose
Pose :: sqrt
	() const
{
	return Pose(ga::sqrt(theSpin));
}

inline
Pose
Pose :: operator*
	( Pose const & first
	) const
{
	// ((*this) * first)
	return Pose{theSpin * first.theSpin};
}

//======================================================================
}

