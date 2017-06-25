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
\brief Inline definitions for ga::constructs
*/



namespace ga
{
//======================================================================
	// Useful constructions using geometric algebra and algorithms.

namespace priv
{
	inline
	void
	antiSymPro
		( std::array<double, 3u> const & aa
		, std::array<double, 3u> const & bb
		, std::array<double, 3u> * const & ptOut
		)
	{
		std::array<double, 3u>::iterator iter(ptOut->begin());
		*iter++ = (aa[1]*bb[2] - aa[2]*bb[1]);
		*iter++ = (aa[2]*bb[0] - aa[0]*bb[2]);
		*iter++ = (aa[0]*bb[1] - aa[1]*bb[0]);
	}
}


inline
Scalar
dot
	( Vector const & v1
	, Vector const & v2
	)
{
	return Scalar
		( std::inner_product
			( v1.theValues.begin(), v1.theValues.end()
			, v2.theValues.begin()
			, double(0.)
			)
		);
}

inline
Scalar
dot
	( BiVector const & b1
	, BiVector const & b2
	)
{
	return Scalar
		( -std::inner_product
			( b1.theValues.begin(), b1.theValues.end()
			, b2.theValues.begin()
			, double(0.)
			)
		);
}

inline
Vector
dot
	( Vector const & v1
	, BiVector const & b2
	)
{
	Vector result;
	// NOTE: bivector negates the obvious
	// priv::antiSymPro(v1.theValues, b2.theValues, &(result.theValues));
	// so reverse the args
	priv::antiSymPro(b2.theValues, v1.theValues, &(result.theValues));
	return result;
}

inline
Vector
dot
	( BiVector const & b1
	, Vector const & v2
	)
{
	return { -dot(v2, b1) };
}

inline
BiVector
wedge
	( Vector const & v1
	, Vector const & v2
	)
{
	BiVector result;
	priv::antiSymPro(v1.theValues, v2.theValues, &(result.theValues));
	return result;
}

inline
Vector
cross
	( Vector const & v1
	, Vector const & v2
	)
{
	// use array members to short-cut duality (and negation) operations
	return Vector{wedge(v1, v2).theValues};
}

inline
Vector
rotated
	( Spinor const & spin
	, Vector const & vec
	)
{
	// Semi-Optimized version:
	//  - distilled to scalar ops, but with some redundant computations

	// assign shorthand
	double const & a0 = spin.theS.theValue;
	double const & a1 = spin.theB.theValues[0];
	double const & a2 = spin.theB.theValues[1];
	double const & a3 = spin.theB.theValues[2];
	double const & x1 = vec.theValues[0];
	double const & x2 = vec.theValues[1];
	double const & x3 = vec.theValues[2];

	// compute intermediate values
	double const a00(a0*a0);
	double const a01(a0*a1);
	double const a02(a0*a2);
	double const a03(a0*a3);
	double const a11(a1*a1);
	double const a12(a1*a2);
	double const a13(a1*a3);
	double const a22(a2*a2);
	double const a23(a2*a3);
	double const a33(a3*a3);

	// form elements of classic rotation matrix
	double const r11(a00 + a11 - a22 - a33);
	double const r12(2.*(a12 + a03));
	double const r13(2.*(a13 - a02));

	double const r21(2.*(a12 - a03));
	double const r22(a00 - a11 + a22 - a33);
	double const r23(2.*(a23 + a01));

	double const r31(2.*(a13 + a02));
	double const r32(2.*(a23 - a01));
	double const r33(a00 - a11 - a22 + a33);

	// assign results in correspondance with matrix multiply
	double const xp1(r11*x1 + r12*x2 + r13*x3);
	double const xp2(r21*x1 + r22*x2 + r23*x3);
	double const xp3(r31*x1 + r32*x2 + r33*x3);

	return Vector(xp1, xp2, xp3);
}

inline
Vector
rejectionFrom
	( Vector const & loc
	, Vector const & somedir
	)
{
	Vector const unitdir(unit(somedir));
	return ((wedge(loc, unitdir) * unitdir).theV);
}

inline
TriVector
rejectionFrom
	( Vector const & vec
	, BiVector const & somePlane
	)
{
	return TriVector
		( std::inner_product
			( vec.theValues.begin(), vec.theValues.end()
			, somePlane.theValues.begin()
			, double(0.)
			)
		);
}

inline
Vector
projectionOnto
	( Vector const & loc
	, Vector const & somedir
	)
{
	Vector const unitdir(unit(somedir));
	return (dot(loc, unitdir) * unitdir);
}

inline
Vector
projectionOnto
	( Vector const & vec
	, BiVector const & somePlane
	)
{
	Vector proj;
	BiVector const bv(unit(somePlane));
	if (bv.isValid())
	{
		Vector const reflected((bv * vec * bv).theV);
		proj =.5 * (vec + reflected);
	}
	return proj;
}

inline
Vector
perpendicular
	( Vector const & vec
	, BiVector const & somePlane
	)
{
	return ((vec * somePlane).theV);
}

//======================================================================
}

