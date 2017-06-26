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
\brief Inline definitions for ga::operations
*/


#include "libdat/array.h"

#include <algorithm>


namespace ga
{
	//! Functions for internal library use -- not part of public interface
	namespace priv
	{
		//! Sum of corresponding components
		inline
		double
		inner
			( std::array<double, 3u> const & as
			, std::array<double, 3u> const & bs
			)
		{
			return
				( (as[0] * bs[0])
				+ (as[1] * bs[1])
				+ (as[2] * bs[2])
				);
		}

		//! Cross terms in cylic order -- 3D ONLY!!
		inline
		std::array<double, 3u>
		outer
			( std::array<double, 3u> const & as
			, std::array<double, 3u> const & bs
			)
		{
			return std::array<double, 3u>
				{{ (as[1]*bs[2] - as[2]*bs[1])
				 , (as[2]*bs[0] - as[0]*bs[2])
				 , (as[0]*bs[1] - as[1]*bs[0])
				}};
		}
	}

//======================================================================

//
// ==== Uniary operations
//

inline
Scalar
operator-
	( One const & one
	)
{
	return -Scalar{one};
}

inline
Scalar
operator-
	( Scalar const & arg
	)
{
	return Scalar(-arg.theValue);
}

inline
Vector
operator-
	( Vector const & arg
	)
{
	using dat::operator-;
	return Vector(-arg.theValues);
}

inline
BiVector
operator-
	( BiVector const & arg
	)
{
	using dat::operator-;
	return BiVector(-arg.theValues);
}

inline
TriVector
operator-
	( TriVector const & arg
	)
{
	return TriVector(-arg.theValue);
}

inline
Spinor
operator-
	( Spinor const & arg
	)
{
	return Spinor(-arg.theS, -arg.theB);
}

inline
ImSpinor
operator-
	( ImSpinor const & arg
	)
{
	return ImSpinor(-arg.theV, -arg.theT);
}

//
// ==== Addition
//

// -- Scalar + ...
inline
Scalar
operator+
	( Scalar const & q1
	, Scalar const & q2
	)
{
	return Scalar(q1.theValue + q2.theValue);
}

inline
Spinor
operator+
	( Scalar const & q1
	, BiVector const & q2
	)
{
	return Spinor(q1, q2);
}

// -- Vector + ...
inline
Vector
operator+
	( Vector const & q1
	, Vector const & q2
	)
{
	using dat::operator+;
	return Vector(q1.theValues + q2.theValues);
}

inline
ImSpinor
operator+
	( Vector const & q1
	, TriVector const & q2
	)
{
	return ImSpinor(q1, q2);
}

inline
ImSpinor
operator+
	( Vector const & q1
	, ImSpinor const & q2
	)
{
	return ImSpinor(q1 + q2.theV, q2.theT);
}

// -- BiVector + ...
inline
BiVector
operator+
	( BiVector const & q1
	, BiVector const & q2
	)
{
	using dat::operator+;
	return BiVector(q1.theValues + q2.theValues);
}

// -- TriVector + ...
inline
TriVector
operator+
	( TriVector const & q1
	, TriVector const & q2
	)
{
	return TriVector(q1.theValue + q2.theValue);
}

inline
ImSpinor
operator+
	( TriVector const & q1
	, ImSpinor const & q2
	)
{
	return ImSpinor(q2.theV, q1 + q2.theT);
}

// -- Spinor + ...
inline
Spinor
operator+
	( Spinor const & q1
	, Spinor const & q2
	)
{
	return Spinor(q1.theS + q2.theS, q1.theB + q2.theB);
}

// -- ImSpinor + ...
inline
ImSpinor
operator+
	( ImSpinor const & q1
	, Vector const & q2
	)
{
	return (q2 + q1);
}

inline
ImSpinor
operator+
	( ImSpinor const & q1
	, TriVector const & q2
	)
{
	return (q2 + q1);
}

inline
ImSpinor
operator+
	( ImSpinor const & q1
	, ImSpinor const & q2
	)
{
	return ImSpinor(q1.theV + q2.theV, q1.theT + q2.theT);
}


//
// ==== Subtraction
//

inline
Scalar
operator-
	( Scalar const & q1
	, Scalar const & q2
	)
{
	return Scalar(q1.theValue - q2.theValue);
}

inline
Vector
operator-
	( Vector const & q1
	, Vector const & q2
	)
{
	using dat::operator-;
	return Vector(q1.theValues - q2.theValues);
}

inline
BiVector
operator-
	( BiVector const & q1
	, BiVector const & q2
	)
{
	using dat::operator-;
	return BiVector(q1.theValues - q2.theValues);
}

inline
TriVector
operator-
	( TriVector const & q1
	, TriVector const & q2
	)
{
	return TriVector(q1.theValue - q2.theValue);
}

inline
Spinor
operator-
	( Spinor const & q1
	, Spinor const & q2
	)
{
	return Spinor(q1.theS - q2.theS, q1.theB - q2.theB);
}

inline
ImSpinor
operator-
	( ImSpinor const & q1
	, ImSpinor const & q2
	)
{
	return ImSpinor(q1.theV - q2.theV, q1.theT - q2.theT);
}


//
// ==== double.MV multiply
//

inline
Scalar
operator*
	( double const & lmult
	, Scalar const & arg
	)
{
	return Scalar(lmult * arg.theValue);
}

inline
Vector
operator*
	( double const & lmult
	, Vector const & arg
	)
{
	using dat::operator*;
	return Vector(lmult * arg.theValues);
}

inline
BiVector
operator*
	( double const & lmult
	, BiVector const & arg
	)
{
	using dat::operator*;
	return BiVector(lmult * arg.theValues);
}

inline
TriVector
operator*
	( double const & lmult
	, TriVector const & arg
	)
{
	return TriVector(lmult * arg.theValue);
}

inline
Spinor
operator*
	( double const & lmult
	, Spinor const & arg
	)
{
	return Spinor(lmult * arg.theS, lmult * arg.theB);
}

inline
ImSpinor
operator*
	( double const & lmult
	, ImSpinor const & arg
	)
{
	return ImSpinor(lmult * arg.theV, lmult * arg.theT);
}


//
// ==== Scalar.MV multiply
//

inline
Scalar
operator*
	( Scalar const & ss
	, Scalar const & sVal
	)
{
	return Scalar(ss.theValue * sVal.theValue);
}

inline
BiVector
operator*
	( Scalar const & ss
	, BiVector const & bVal
	)
{
	using dat::operator*;
	return BiVector(ss.theValue * bVal.theValues);
}

inline
Vector
operator*
	( Scalar const & ss
	, Vector const & vVal
	)
{
	using dat::operator*;
	return Vector(ss.theValue * vVal.theValues);
}

inline
TriVector
operator*
	( Scalar const & ss
	, TriVector const & tVal
	)
{
	return TriVector(ss.theValue * tVal.theValue);
}

inline
Spinor
operator*
	( Scalar const & ss
	, Spinor const & spin
	)
{
	double const scl{ ss.theValue * spin.theS.theValue };
	using dat::operator*;
	ga::BiVector const biv(ss.theValue * spin.theB.theValues);
	return Spinor(scl, biv);
}

inline
ImSpinor
operator*
	( Scalar const & ss
	, ImSpinor const & ispin
	)
{
	using dat::operator*;
	ga::Vector const vec(ss.theValue * ispin.theV.theValues);
	ga::TriVector const tri(ss.theValue * ispin.theT.theValue);
	return ImSpinor(vec, tri);
}


//
// ==== Vector.MV multiply
//

inline
Vector
operator*
	( Vector const & vVal
	, Scalar const & ss
	)
{
	return (ss * vVal);
}

inline
Spinor
operator*
	( Vector const & vecA
	, Vector const & vecB
	)
{
	Scalar const s0(priv::inner(vecA.theValues, vecB.theValues));
	BiVector const bv(priv::outer(vecA.theValues, vecB.theValues));
	return Spinor(s0, bv);
}

inline
ImSpinor
operator*
	( Vector const & vec
	, BiVector const & bv
	)
{
	Vector const vv(priv::outer(bv.theValues, vec.theValues));
	TriVector const tv(priv::inner(vec.theValues, bv.theValues));
	return ImSpinor(vv, tv);
}

inline
BiVector
operator*
	( Vector const & vec
	, TriVector const & tv
	)
{
	using dat::operator*;
	return BiVector(tv.theValue * vec.theValues);
}

inline
ImSpinor
operator*
	( Vector const & vec
	, Spinor const & spin
	)
{
	// v * (s + B)
	// v*s + v*B
	// v*s + v*(Ib)
	// v*s + I*v*b
	// v*s + I*(v.b) + I*(v^b)
	using dat::operator-;
	Vector const v1(spin.theS.theValue * vec);
	Vector const v2
		(-priv::outer(vec.theValues, spin.theB.theValues)); // anti-sym
	TriVector const tv
		( priv::inner(vec.theValues, spin.theB.theValues));
	return ImSpinor(v1+v2, tv);
}

inline
Spinor
operator*
	( Vector const & vec
	, ImSpinor const & ispin
	)
{
	double const s0(priv::inner(vec.theValues, ispin.theV.theValues));
	BiVector const bv1(priv::outer(vec.theValues, ispin.theV.theValues));
	double const & tmag = ispin.theT.theValue;
	using dat::operator*;
	BiVector const bv2(tmag * vec.theValues);
	return Spinor(s0, bv1 + bv2);
}


//
// ==== BiVector.MV Multiply
//

inline
ImSpinor
operator*
	( BiVector const & bv
	, Vector const & vec
	)
{
	Vector const vv(priv::outer(vec.theValues, bv.theValues));
	TriVector const tv(priv::inner(vec.theValues, bv.theValues));
	return ImSpinor(vv, tv);
}

inline
Spinor
operator*
	( BiVector const & bvA
	, BiVector const & bvB
	)
{
	// Scalar const s0(- priv::inner(bvA.theValues, bvB.theValues));
	// BiVector const bv(- priv::outer(bvA.theValues, bvB.theValues));
	using dat::operator-;
	Scalar const s0
		(-priv::inner(bvA.theValues, bvB.theValues)); // symmetric
	BiVector const bv
		(priv::outer(bvB.theValues, bvA.theValues)); // anti-symmetric
	return Spinor(s0, bv);
}

inline
Vector
operator*
	( BiVector const & q1
	, TriVector const & q2
	)
{
	using dat::operator*;
	return Vector(-q2.theValue * q1.theValues);
}

inline
Spinor
operator*
	( BiVector const & biv
	, Spinor const & spin
	)
{
	BiVector const b1(spin.theS.theValue * biv);
	using dat::operator-;
	BiVector const b2
		(-priv::outer(biv.theValues, spin.theB.theValues));
	Scalar const s2
		(-priv::inner(biv.theValues, spin.theB.theValues));
	return Spinor(s2, b1+b2);
}

inline
ImSpinor
operator*
	( BiVector const & bv
	, ImSpinor const & ispin
	)
{
	// bv*vec + tri*bv
	ImSpinor const tmpIm(bv * ispin.theV);
	Vector const tmpVec(ispin.theT * bv);
	return ImSpinor((tmpIm.theV + tmpVec), tmpIm.theT);
}


//
// ==== TriVector.MV Multiply
//

inline
TriVector
operator*
	( TriVector const & tv
	, Scalar const & sVal
	)
{
	return TriVector(sVal.theValue * tv.theValue);
}

inline
BiVector
operator*
	( TriVector const & tv
	, Vector const & vVal
	)
{
	return (vVal * tv);
}

inline
Vector
operator*
	( TriVector const & tv
	, BiVector const & vec
	)
{
	using dat::operator*;
	return Vector(-tv.theValue * vec.theValues);
}

inline
Scalar
operator*
	( TriVector const & tv
	, TriVector const & tVal
	)
{
	using dat::operator*;
	return Scalar(-tv.theValue * tVal.theValue);
}

inline
ImSpinor
operator*
	( TriVector const & tv
	, Spinor const & spin
	)
{
	return ImSpinor(tv*spin.theB, tv*spin.theS);
}


//
// ==== Spinor.MV multiply
//

inline
ImSpinor
operator*
	( Spinor const & spin
	, Vector const & vec
	)
{
	// (s + B) * v
	// s*v + B*v
	// s*v + I*b*v
	// s*v + I(b.v) + I(b^v)
	using dat::operator-;
	Vector const v1(spin.theS.theValue * vec);
	Vector const v2
		(-priv::outer(spin.theB.theValues, vec.theValues)); // anti-sym
	TriVector const tv
		(priv::inner(vec.theValues, spin.theB.theValues));
	return ImSpinor(v1+v2, tv);
}

inline
Spinor
operator*
	( Spinor const & spin
	, BiVector const & biv
	)
{
	BiVector const b1(spin.theS.theValue * biv);
	using dat::operator-;
	BiVector const b2
		(-priv::outer(spin.theB.theValues, biv.theValues));
	Scalar const s2
		(-priv::inner(spin.theB.theValues, biv.theValues));
	return Spinor(s2, b1+b2);
}

inline
Spinor
operator*
	( Spinor const & spinM
	, Spinor const & spinQ
	)
{
	// direct implementation (should be faster)
	double const & m0 = spinM.theS.theValue;
	double const & m1 = spinM.theB.theValues[0];
	double const & m2 = spinM.theB.theValues[1];
	double const & m3 = spinM.theB.theValues[2];
	double const & q0 = spinQ.theS.theValue;
	double const & q1 = spinQ.theB.theValues[0];
	double const & q2 = spinQ.theB.theValues[1];
	double const & q3 = spinQ.theB.theValues[2];
	double const r0(m0*q0 - m1*q1 - m2*q2 - m3*q3);
	double const r1(m0*q1 + m1*q0 - m2*q3 + m3*q2);
	double const r2(m0*q2 + m1*q3 + m2*q0 - m3*q1);
	double const r3(m0*q3 - m1*q2 + m2*q1 + m3*q0);
	return Spinor(Scalar(r0), BiVector(r1, r2, r3));
}

inline
ImSpinor
operator*
	( Spinor const & spin
	, ImSpinor const & ispin
	)
{
	// direct access to argument data members
	double const & dubT = ispin.theT.theValue;
	double const & dubS = spin.theS.theValue;
	Vector const & tmpV = ispin.theV;
	BiVector const & tmpB = spin.theB;

	// form the commutative combinatorial products
	using dat::operator*;
	std::array<double, 3u> const v1( dubS * tmpV.theValues);
	std::array<double, 3u> const v2(-dubT * tmpB.theValues); // dual on-the-fly
	double const t3{ dubS * dubT };

	// incorporate the anti commutative product
	ImSpinor const i4(spin.theB * ispin.theV);
	using dat::operator+;
	ImSpinor const tmp(Vector{v1 + v2}, TriVector{t3});
	return ImSpinor(tmp + i4);
}

//
// ==== ImSpinor.MV multiply
//

inline
Spinor
operator*
	( ImSpinor const & ispin
	, Vector const & vec
	)
{
	double const s0(priv::inner(ispin.theV.theValues, vec.theValues));
	BiVector const bv1(priv::outer(ispin.theV.theValues, vec.theValues));
	double const & tmag = ispin.theT.theValue;
	using dat::operator*;
	BiVector const bv2(tmag * vec.theValues);
	return Spinor(s0, bv1 + bv2);
}

inline
ImSpinor
operator*
	( ImSpinor const & ispin
	, BiVector const & bv
	)
{
	// vec*bv + tri*bv
	ImSpinor const tmpIm(ispin.theV * bv);
	Vector const tmpVec(ispin.theT * bv);
	return ImSpinor((tmpIm.theV + tmpVec), tmpIm.theT);
}

inline
Spinor
operator*
	( ImSpinor const & ispin
	, TriVector const & tv
	)
{
	double const sTmp(-ispin.theT.theValue * tv.theValue);
	using dat::operator*;
	std::array<double, 3u> const bTmp(-tv.theValue * ispin.theV.theValues);
	return Spinor(Scalar(sTmp), BiVector(bTmp));
}

inline
ImSpinor
operator*
	( ImSpinor const & ispin
	, Spinor const & spin
	)
{
	// direct access to argument data members
	double const & dubT = ispin.theT.theValue;
	double const & dubS = spin.theS.theValue;
	Vector const & tmpV = ispin.theV;
	BiVector const & tmpB = spin.theB;

	// form the commutative combinatorial products
	using dat::operator*;
	std::array<double, 3u> const v1( dubS * tmpV.theValues);
	std::array<double, 3u> const v2(-dubT * tmpB.theValues); // dual on-the-fly
	double const t3{ dubS * dubT };

	// incorporate the anti commutative product
	ImSpinor const i4(ispin.theV * spin.theB);
	using dat::operator+;
	ImSpinor const tmp(Vector{v1 + v2}, TriVector{t3});
	return ImSpinor(tmp + i4);
}

inline
Spinor
operator*
	( ImSpinor const & ispinM
	, ImSpinor const & ispinN
	)
{
	ga::Spinor result{ ispinM.theV * ispinN.theV };
	double & r0 = result.theS.theValue;
	std::array<double, 3u> & r2 = result.theB.theValues;
	using dat::operator*;
	using dat::operator+;
	r2 = r2 + (ispinN.theT.theValue * ispinM.theV.theValues);
	r2 = r2 + (ispinM.theT.theValue * ispinN.theV.theValues);
	r0 = r0 - (ispinM.theT.theValue * ispinN.theT.theValue);
	return result;
}

//======================================================================
}


