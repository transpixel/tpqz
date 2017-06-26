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
\brief  This file contains unit test for ga::ga
*/


#include "libga/ga.h"

#include "libdat/compare.h"
#include "libdat/array.h"
#include "libga/multivector.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
ga_ga_test0
	()
{
	std::ostringstream oss;

	// check test functions
	ga::Vector const vNull;
	ga::Vector const vGood(1., 2., 3.);
	ga::Vector const vSame(1., 2., 3.);
	ga::Vector const vDiff(2., 2., 3.);
	if (   ga::nearlyEquals(vNull, vNull)
	    || ga::nearlyEquals(vNull, vGood)
	   )
	{
		oss << "Failure of nearlyEquals null test" << std::endl;
	}
	if (! ga::nearlyEquals(vGood, vSame))
	{
		oss << "Failure of nearlyEquals good/same test" << std::endl;
	}
	if (  ga::nearlyEquals(vGood, vDiff))
	{
		oss << "Failure of nearlyEquals good/diff test" << std::endl;
	}

	// check for null and infoString
	{
		ga::Scalar const aNull;
		if (aNull.isValid())
		{
			oss << "Failure of Scalar null value test" << std::endl;
			oss << "infoString: " << aNull.infoString("aNull") << std::endl;
		}
	}
	{
		ga::Vector const aNull;
		if (aNull.isValid())
		{
			oss << "Failure of Vector null value test" << std::endl;
			oss << "infoString: " << aNull.infoString("aNull") << std::endl;
		}
	}
	{
		ga::BiVector const aNull;
		if (aNull.isValid())
		{
			oss << "Failure of BiVector null value test" << std::endl;
			oss << "infoString: " << aNull.infoString("aNull") << std::endl;
		}
	}
	{
		ga::TriVector const aNull;
		if (aNull.isValid())
		{
			oss << "Failure of TriVector null value test" << std::endl;
			oss << "infoString: " << aNull.infoString("aNull") << std::endl;
		}
	}
	{
		ga::Spinor const aNull;
		if (aNull.isValid())
		{
			oss << "Failure of Spinor null value test" << std::endl;
			oss << "infoString: " << aNull.infoString("aNull") << std::endl;
		}
	}

	// check for valid constructions
	ga::Scalar const okS(10.);
	if (! okS.isValid())
	{
		oss << "Failure of Scalar valid value test" << std::endl;
		oss << okS.infoString("okS") << std::endl;
	}
	ga::Vector const okV(1., 2., 3.);
	if (! okV.isValid())
	{
		oss << "Failure of Vector valid value test" << std::endl;
		oss << okV.infoString("okV") << std::endl;
	}
	ga::BiVector const okB(11., 22., 33.);
	if (! okB.isValid())
	{
		oss << "Failure of BiVector valid value test" << std::endl;
		oss << okB.infoString("okB") << std::endl;
	}
	ga::TriVector const okT(100);
	if (! okT.isValid())
	{
		oss << "Failure of TriVector valid value test" << std::endl;
		oss << okT.infoString("okT") << std::endl;
	}
	ga::Spinor const okR(okS, okB);
	if (! okR.isValid())
	{
		oss << "Failure of Spinor valid value test" << std::endl;
		oss << okR.infoString("okR") << std::endl;
	}

	// check revision (and subtraction)
	// Use exact compare since reversal is entirely sign-bit operations
	ga::Scalar const dS(okS - okS.reverse());
	if (! ga::nearlyEquals(dS, ga::sZero))
	{
		oss << "Failure of Scalar reverse test" << std::endl;
		oss << okS.infoString("fwd") << std::endl;
		oss << okS.reverse().infoString("rev") << std::endl;
		oss << dS.infoString("dS") << std::endl;
	}
	ga::Vector const dV(okV - okV.reverse());
	if (! ga::nearlyEquals(dV, ga::vZero))
	{
		oss << "Failure of Vector reverse test" << std::endl;
		oss << okV.infoString("fwd") << std::endl;
		oss << okV.reverse().infoString("rev") << std::endl;
		oss << dV.infoString("dV") << std::endl;
	}
	ga::BiVector const dB(okB + okB.reverse());
	if (! ga::nearlyEquals(dB, ga::bZero))
	{
		oss << "Failure of BiVector reverse test" << std::endl;
		oss << okB.infoString("fwd") << std::endl;
		oss << okB.reverse().infoString("rev") << std::endl;
		oss << dB.infoString("dB") << std::endl;
	}
	ga::TriVector const dT(okT + okT.reverse());
	if (! ga::nearlyEquals(dT, ga::tZero))
	{
		oss << "Failure of TriVector reverse test" << std::endl;
		oss << okT.infoString("fwd") << std::endl;
		oss << okT.reverse().infoString("rev") << std::endl;
		oss << dT.infoString("dT") << std::endl;
	}
	ga::Scalar const dRS((okR - okR.reverse()).theS);
	ga::BiVector const dRB((okR + okR.reverse()).theB);
	if (! ( ga::nearlyEquals(dRS, ga::sZero)
	     && ga::nearlyEquals(dRB, ga::bZero)
	   ) )
	{
		oss << "Failure of Spinor reverse test" << std::endl;
		oss << okR.infoString("fwd") << std::endl;
		oss << okR.reverse().infoString("rev") << std::endl;
	}

	// check zero constructors
	{
		using namespace ga;

		Scalar const s0(zero);
		Vector const v0(zero);
		BiVector const b0(zero);
		TriVector const t0(zero);
		Spinor const sp0a(s0 + b0);
		ImSpinor const im0b(v0 + t0);
		Spinor const sp0b(im0b * TriVector(1.));
		if (! sp0b.nearlyEquals(sp0a))
		{
			oss << "Failure of zero ctor test" << std::endl;
		}
	}

	return oss.str();
}

//! Check dual operations
std::string
ga_ga_test0a
	()
{
	std::ostringstream oss;


	{
		using namespace ga;

		TriVector const unitI(1.);
		Scalar const gotISq(unitI * unitI);
		// Scalar const gotISq(math::sq(unitI));
		Scalar const expISq(-1.);
		if (! gotISq.nearlyEquals(expISq))
		{
			oss << "Failure of I^2 test" << std::endl;
		}
	}

	{
		using namespace ga;

		Vector const vec(-3., 7., 5.);
		BiVector const exp(vec.theValues);
		BiVector const gotVI(vec * E123);
		BiVector const gotIV(E123 * vec);
		if ( (! gotVI.nearlyEquals(exp))
		  || (! gotIV.nearlyEquals(exp))
		   )
		{
			oss << "Failure of vector dual test" << std::endl;
		}
	}

	{
		using namespace ga;

		BiVector const biv(-11., 5., -7.);
		using dat::operator-;
		Vector const exp(-biv.theValues);
		Vector const gotBI(biv * E123);
		Vector const gotIB(E123 * biv);
		if ( (! gotBI.nearlyEquals(exp))
		  || (! gotIB.nearlyEquals(exp))
		   )
		{
			oss << "Failure of bivector dual test" << std::endl;
		}
	}

	// spinor/bivector multiply
	{
		using namespace ga;

		// use duality to check rotation case
		Spinor const spin{exp(BiVector{5., 7., 11.})};
		Vector const vecA{-7., 5., 11.};
		BiVector const bivA(E123 * vecA);
		Vector const vecRot((spin * vecA * spin.reverse()).theV);
		BiVector const expRot(E123 * vecRot);

		BiVector const gotRot((spin * bivA * spin.reverse()).theB);
		if (! nearlyEquals(gotRot, expRot))
		{
			oss << "Failure of bivector rotation test" << std::endl;
		}
	}


	return oss.str();
}

//! Check consistency of basic constants
std::string
ga_ga_test0b
	()
{
	std::ostringstream oss;
	{
		using namespace ga;

		// squared unit scalar
		Scalar const scalePosOne(one);
		if (! nearlyEquals(e0*e0, scalePosOne))
		{
			oss << "Failure of e0^2 test" << std::endl;
		}

		// squared unit vectors
		Spinor const spinPosOne(one);
		if (! nearlyEquals(e1*e1, spinPosOne))
		{
			oss << "Failure of e1^2 test" << std::endl;
		}
		if (! nearlyEquals(e2*e2, spinPosOne))
		{
			oss << "Failure of e2^2 test" << std::endl;
		}
		if (! nearlyEquals(e3*e3, spinPosOne))
		{
			oss << "Failure of e3^2 test" << std::endl;
		}

		// squred unit bivectors
		Spinor const spinNegOne(-one, bZero);
		if (! nearlyEquals(E12*E12, spinNegOne))
		{
			oss << "Failure of E12^2 test" << std::endl;
			oss << spinNegOne.infoString("exp") << std::endl;
			oss << (E12*E12).infoString("got") << std::endl;
		}
		if (! nearlyEquals(E23*E23, spinNegOne))
		{
			oss << "Failure of E23^2 test" << std::endl;
		}
		if (! nearlyEquals(E31*E31, spinNegOne))
		{
			oss << "Failure of E31^2 test" << std::endl;
		}

		// squared unit trivector
		Scalar const scaleNegOne(-one);
		if (! nearlyEquals(E123*E123, scaleNegOne))
		{
			oss << "Failure of E123^2 test" << std::endl;
		}

		// dextral product tests
		if (! nearlyEquals(e1*e2, Spinor(sZero, E12)))
		{
			oss << "Failure of e1e2,E12 test" << std::endl;
		}
		if (! nearlyEquals(e2*e3, Spinor(sZero, E23)))
		{
			oss << "Failure of e2e3,E23 test" << std::endl;
		}
		if (! nearlyEquals(e3*e1, Spinor(sZero, E31)))
		{
			oss << "Failure of e3e1,E31 test" << std::endl;
		}

		// vector/bivector unit tests
		if (! nearlyEquals(e1*E12, ImSpinor(e2, tZero)))
		{
			oss << "Failure of e1*E12 test" << std::endl;
		}
		if (! nearlyEquals(e2*E23, ImSpinor(e3, tZero)))
		{
			oss << "Failure of e2*E23 test" << std::endl;
		}
		if (! nearlyEquals(e3*E31, ImSpinor(e1, tZero)))
		{
			oss << "Failure of e3*E31 test" << std::endl;
		}
		if (! nearlyEquals(E12*e1, ImSpinor(-e2, tZero)))
		{
			oss << "Failure of E12*e1 test" << std::endl;
		}
		if (! nearlyEquals(E23*e2, ImSpinor(-e3, tZero)))
		{
			oss << "Failure of E23*eK test" << std::endl;
		}
		if (! nearlyEquals(E31*e3, ImSpinor(-e1, tZero)))
		{
			oss << "Failure of E31*eK test" << std::endl;
		}

	}

	return oss.str();
}

//! Check operators
std::string
ga_ga_test0c
	()
{
	std::ostringstream oss;

	ga::Vector const vec(1., 2., 3.);
	bool const vec0(vec.theValues[0] == vec[0]);
	bool const vec1(vec.theValues[1] == vec[1]);
	bool const vec2(vec.theValues[2] == vec[2]);
	if (! (vec0 && vec1 && vec2))
	{
		oss << "Failure of vector op[] test" << std::endl;
	}

	ga::Vector const biv(11., 12., 13.);
	bool const biv0(biv.theValues[0] == biv[0]);
	bool const biv1(biv.theValues[1] == biv[1]);
	bool const biv2(biv.theValues[2] == biv[2]);
	if (! (biv0 && biv1 && biv2))
	{
		oss << "Failure of bivector op[] test" << std::endl;
	}

	return oss.str();
}

//! Check various functions useful for transformations
std::string
ga_ga_test1
	()
{
	std::ostringstream oss;

	// check identity spinor
	ga::Spinor const spinIdent(ga::one);
	if (! spinIdent.isValid())
	{
		oss << "Failure of spinor identity validity test" << std::endl;
	}
	{
		ga::Vector const expVec(1./4., 3./8., -5./2.);
		ga::Vector const gotVec((spinIdent * expVec).theV);
		if (! gotVec.nearlyEquals(expVec))
		{
			oss << "Failure of spinor identity multiply test" << std::endl;
		}
	}

	// generate a 1/3 turn spinor
	ga::BiVector const pAngle
		((2./3.)*math::pi * ga::unit(ga::BiVector(-3., 2., -5.)));
	ga::Spinor const spinR(ga::exp(.5*pAngle));

	// check spinor properties
	double const magSpinR(magnitude(spinR));
	if (! dat::nearlyEquals(1., magSpinR))
	{
		oss << "Failure of unitary spinor exp test" << std::endl;
		oss << spinR.infoString("spinR") << std::endl;
	}

	// compute a vector in the plane of angle
	ga::Vector const tmp0(11., 17., 5.);
	ga::BiVector const pHat(ga::unit(pAngle));
	ga::ImSpinor const tmpP0P(pHat * tmp0 * pHat);
	ga::Vector const vec0(.5 * (tmp0 + tmpP0P.theV));

	// check BV/Vec multiplies
	ga::ImSpinor const gotImB((pHat * tmp0) * pHat);
	if (! ga::nearlyEquals(gotImB, tmpP0P))
	{
		oss << "Failure of Im*B test" << std::endl;
		oss << tmpP0P.infoString("exp") << std::endl;
		oss << gotImB.infoString("got") << std::endl;
	}
	ga::ImSpinor const gotBIm(pHat * (tmp0 * pHat));
	if (! ga::nearlyEquals(gotBIm, tmpP0P))
	{
		oss << "Failure of B*Im test" << std::endl;
		oss << tmpP0P.infoString("exp") << std::endl;
		oss << gotBIm.infoString("got") << std::endl;
	}
	ga::ImSpinor const gotBImT(.5 * (vec0*pHat - pHat*vec0));
	if (! ga::nearlyEquals(gotBImT.theT, ga::tZero))
	{
		oss << "Failure of vec0 in-plane test" << std::endl;
	}

	// check that vec0 is in plane of spinor
	ga::ImSpinor const chk0(spinR * vec0);
	if (! ga::nearlyEquals(chk0.theT, ga::tZero))
	{
		oss << "Failure of vec0 in-plane test" << std::endl;
		oss << ga::tZero.infoString("exp") << std::endl;
		oss << chk0.theT.infoString("got") << std::endl;
	}

	// rotate vector by 1/3 turn 3 times,
	ga::Vector const vec1(ga::rotated(spinR, vec0));
	ga::Vector const vec2(ga::rotated(spinR, vec1));
	ga::Vector const vec3(ga::rotated(spinR, vec2));

	// results should match original
	if (! ga::nearlyEquals(vec3, vec0))
	{
		oss << "Failure of 3x rotation test" << std::endl;
		oss << vec0.infoString("vec0") << std::endl;
		oss << vec1.infoString("vec1") << std::endl;
		oss << vec2.infoString("vec2") << std::endl;
		oss << vec3.infoString("vec3") << std::endl;
	}

	// check basic vector product
	ga::Vector const vecA(ga::unit(vec0));
	ga::Vector const vecB(ga::unit(vec0 + vec1));
	ga::Spinor const gotR(ga::unit(ga::sqrt(vec1 * vec0)));
	if (! ga::nearlyEquals(gotR, spinR))
	{
		oss << "Failure of vector product test" << std::endl;
		oss << spinR.infoString("expR") << std::endl;
		oss << gotR.infoString("gotR") << std::endl;
	}

	ga::Spinor const s11(ga::e1 * ga::e1);
	ga::Spinor const s22(ga::e2 * ga::e2);
	ga::Spinor const s33(ga::e3 * ga::e3);

	ga::Spinor const s12(ga::e1 * ga::e2);
	ga::Spinor const s23(ga::e2 * ga::e3);
	ga::Spinor const s31(ga::e3 * ga::e1);

	return oss.str();
}

//! Check basic operations
std::string
ga_ga_test2
	()
{
	std::ostringstream oss;

	// check fundamental vec*vec
	{
		using namespace ga;

		double const a1(11.);
		double const a2(13.);
		double const a3(17.);
		double const b1( 7.);
		double const b2( 5.);
		double const b3( 3.);
		Vector const vecA(a1*e1 + a2*e2 + a3*e3);
		Vector const vecB(b1*e1 + b2*e2 + b3*e3);

		Spinor const spinAB(vecA * vecB);
		Spinor const spinA1B((a1*e1) * vecB);
		Spinor const spinA2B((a2*e2) * vecB);
		Spinor const spinA3B((a3*e3) * vecB);
		Spinor const gotABx(spinA1B + spinA2B + spinA3B);
		Spinor const spinAB1(vecA * (b1*e1));
		Spinor const spinAB2(vecA * (b2*e2));
		Spinor const spinAB3(vecA * (b3*e3));
		Spinor const gotABy(spinAB1 + spinAB2 + spinAB3);
		if (! ga::nearlyEquals(gotABx, spinAB))
		{
			oss << "Failure of gotABx test" << std::endl;
			oss << spinAB.infoString("expABx") << std::endl;
			oss << gotABx.infoString("gotABx") << std::endl;
		}
		if (! ga::nearlyEquals(gotABy, spinAB))
		{
			oss << "Failure of gotABy test" << std::endl;
			oss << spinAB.infoString("expABy") << std::endl;
			oss << gotABy.infoString("gotABy") << std::endl;
		}
	}

	// check squares
	{
		using namespace ga;

		Scalar const ss(3.);
		Vector const vv(2., 5., 7.);
		BiVector const BB(11., 13., 17.);
		TriVector const TT(19.);

		Scalar const sq_s((ss*ss).theValue);
		Scalar const sq_v((vv*vv).theS);
		Scalar const sq_B((BB*BB).theS);
		Scalar const sq_T((TT*TT).theValue);

		if (! (0. < sq_s.theValue))
		{
			oss << "Failure of vector scalar test" << std::endl;
			oss << sq_s.infoString("sq_s") << std::endl;

		}
		if (! (0. < sq_v.theValue))
		{
			oss << "Failure of vector square test" << std::endl;
			oss << sq_v.infoString("sq_v") << std::endl;
		}
		if (! (sq_B.theValue < 0.))
		{
			oss << "Failure of bivector square test" << std::endl;
			oss << sq_B.infoString("sq_B") << std::endl;
		}
		if (! (sq_T.theValue < 0.))
		{
			oss << "Failure of trivector square test" << std::endl;
			oss << sq_T.infoString("sq_T") << std::endl;
		}
	}

	// check B*v and v*B
	{
		using namespace ga;

		Vector const vv(-11., 5., 7.);
		BiVector const BB(2., -17., 11.);

		ImSpinor const got_vB(vv * BB);
		ImSpinor const got_Bv(BB * vv);

		// v*B in detail
		double const & m1 = vv.theValues[0];
		double const & m2 = vv.theValues[1];
		double const & m3 = vv.theValues[2];
		double const & Q1 = BB.theValues[0];
		double const & Q2 = BB.theValues[1];
		double const & Q3 = BB.theValues[2];

		double const mq1(-(m2*Q3 - m3*Q2));
		double const mq2(-(m3*Q1 - m1*Q3));
		double const mq3(-(m1*Q2 - m2*Q1));
		double const mqT(m1*Q1 + m2*Q2 + m3*Q3);

		ImSpinor const exp_vB(Vector(mq1, mq2, mq3), TriVector(mqT));

		if (! ga::nearlyEquals(got_vB, exp_vB))
		{
			oss << "Failure of v*B (via v*v) test" << std::endl;
			oss << exp_vB.infoString("exp_vB") << std::endl;
			oss << got_vB.infoString("got_vB") << std::endl;
		}

		// B*v in detail
		double const & M1 = BB.theValues[0];
		double const & M2 = BB.theValues[1];
		double const & M3 = BB.theValues[2];
		double const & q1 = vv.theValues[0];
		double const & q2 = vv.theValues[1];
		double const & q3 = vv.theValues[2];

		double const qm1(-(M2*q3 - M3*q2));
		double const qm2(-(M3*q1 - M1*q3));
		double const qm3(-(M1*q2 - M2*q1));
		double const qmT(M1*q1 + M2*q2 + M3*q3);

		ImSpinor const exp_Bv(Vector(qm1, qm2, qm3), TriVector(qmT));

		if (! ga::nearlyEquals(got_Bv, exp_Bv))
		{
			oss << "Failure of B*v (via v*v) test" << std::endl;
			oss << exp_Bv.infoString("exp_Bv") << std::endl;
			oss << got_Bv.infoString("got_Bv") << std::endl;
		}


		// Check v*B and B*v products against other means to compute

		// use v*v product to establish expectation
		// Since B = I*b
		//    a*B = a*(I*b) = I*(a*b)
		//    B*a = (I*a)*b = I*(a*b)
		{
			Vector const bb(-E123 * BB); // v = -IB
			ImSpinor const exp_vB( E123 * (vv * bb));
			ImSpinor const exp_Bv( E123 * (bb * vv));

			if (! ga::nearlyEquals(got_vB, exp_vB))
			{
				oss << "Failure of v*B (via v*v) test" << std::endl;
				oss << exp_vB.infoString("exp_vB") << std::endl;
				oss << got_vB.infoString("got_vB") << std::endl;
			}
			if (! ga::nearlyEquals(got_Bv, exp_Bv))
			{
				oss << "Failure of B*v (via v*v) test" << std::endl;
				oss << exp_Bv.infoString("exp_Bv") << std::endl;
				oss << got_Bv.infoString("got_Bv") << std::endl;
			}
		}

		// use B*B product to establish expectation
		//    a*B = (-I*A)*B = -I*(A*B)
		//    B*a = B*(-I*A) = -I*(B*A)
		{
			BiVector const AA(E123 * vv); // B = I*v
			ImSpinor const exp_vB(-E123 * (AA * BB));
			ImSpinor const exp_Bv(-E123 * (BB * AA));
			if (! ga::nearlyEquals(got_vB, exp_vB))
			{
				oss << "Failure of v*B (via B*B) test" << std::endl;
				oss << exp_vB.infoString("exp_vB") << std::endl;
				oss << got_vB.infoString("got_vB") << std::endl;
			}
			if (! ga::nearlyEquals(got_Bv, exp_Bv))
			{
				oss << "Failure of B*v (via B*B) test" << std::endl;
				oss << exp_Bv.infoString("exp_Bv") << std::endl;
				oss << got_Bv.infoString("got_Bv") << std::endl;
			}
		}

	}

	// check Im*Sp and Sp*Im
	{
		using namespace ga;

		Vector const vA( -7,   3.,  5.);
		TriVector const tA(.5);
		Scalar const sB(.125);
		BiVector const bB(  2., -5., 11.);

		// (vA + tA) * (sB + bB)
		ImSpinor const expImSp(vA*sB + vA*bB + tA*sB + tA*bB);
		ImSpinor const gotImSp((vA + tA) * (sB + bB));

		if (! gotImSp.nearlyEquals(expImSp))
		{
			oss << "Failure of Im*Sp test" << std::endl;
			oss << expImSp.infoString("exp") << std::endl;
			oss << gotImSp.infoString("got") << std::endl;
		}

		// (sB * bB) * (vA + tA)
		ImSpinor const expSpIm(sB*vA + sB*tA + bB*vA + bB*tA);
		ImSpinor const gotSpIm((sB + bB) * (vA + tA));

		if (! gotSpIm.nearlyEquals(expSpIm))
		{
			oss << "Failure of Im*Sp test" << std::endl;
			oss << expSpIm.infoString("exp") << std::endl;
			oss << gotSpIm.infoString("got") << std::endl;
		}
	}

	// check various operations
	{
		using namespace ga;

		// some vectors
		Vector const vecA( 2,  3., -5);
		Vector const vecB(-7, 11., 5.);
		Vector const vecM(vecA + vecB);
		Vector const hatA(unit(vecA));
		Vector const hatM(unit(vecA));
		Vector const hatB(unit(vecB));

		// double spinors
		Spinor const expSpinRf(vecA * vecB);
		Spinor const expSpinRr(vecB * vecA);
		Spinor const gotSpinRr(expSpinRf.reverse());
		if (! ga::nearlyEquals(gotSpinRr, expSpinRr))
		{
			oss << "Failiure of spinor reverse test" << std::endl;
		}

		// check exp/specialized-log
		{
			BiVector const expAngle(-.125, 2., 1.5);
			Spinor const spinR(ga::exp(expAngle));
			BiVector const gotAngle(ga::spin::argAngleFrom(spinR));
			if (! ga::nearlyEquals(gotAngle, expAngle))
			{
				oss << "Failure of exp/log(angle) test" << std::endl;
				oss << expAngle.infoString("expAngle") << std::endl;
				oss << gotAngle.infoString("gotAngle") << std::endl;
			}
		}

		// check square root
		Spinor const gotHalfRf(ga::sqrt(expSpinRf));
		Spinor const gotSpinRf(gotHalfRf * gotHalfRf);
		if (! ga::nearlyEquals(gotSpinRf, expSpinRf))
		{
			oss << "Failure of spinor sqrt test" << std::endl;
			oss << expSpinRf.infoString("expSpinRf") << std::endl;
			oss << gotSpinRf.infoString("gotSpinRf") << std::endl;
		}
	}

	return oss.str();
}

//! Check rotation operations (debug case)
std::string
ga_ga_test3
	()
{
	std::ostringstream oss;

	// check simple case
	{
		using namespace ga;

		Vector const vecA(1., 0., 0.);
		Spinor const spinF(exp(.5 * BiVector{0., 0., math::halfPi}));
		Spinor const spinR(spinF.reverse());

		// CCW rotation of *IN-PLANE* vector
		{
			ImSpinor const expB(unit(Vector{1., 1., 0.}), tZero);
			{
				ImSpinor const gotB(vecA * spinF);
				if (! nearlyEquals(gotB, expB))
				{
					oss << "Failure of RH-side Fwd rotation test" << std::endl;
					oss << expB.infoString("exp") << std::endl;
					oss << gotB.infoString("got") << std::endl;
				}
			}
			{
				ImSpinor const gotB(spinR * vecA);
				if (! nearlyEquals(gotB, expB))
				{
					oss << "Failure of LH-side Rev rotation test" << std::endl;
					oss << expB.infoString("exp") << std::endl;
					oss << gotB.infoString("got") << std::endl;
				}
			}
		}

		// CW rotation of *IN-PLANE* vector
		{
			ImSpinor const expB(unit(Vector{1., -1., 0.}), tZero);
			{
				ImSpinor const gotB(spinF * vecA);
				if (! nearlyEquals(gotB, expB))
				{
					oss << "Failure of LH-side Fwd rotation test" << std::endl;
					oss << expB.infoString("exp") << std::endl;
					oss << gotB.infoString("got") << std::endl;
				}
			}
			{
				ImSpinor const gotB(vecA * spinR);
				if (! nearlyEquals(gotB, expB))
				{
					oss << "Failure of RH-side Rev rotation test" << std::endl;
					oss << expB.infoString("exp") << std::endl;
					oss << gotB.infoString("got") << std::endl;
				}
			}
		}

		// complete operation
		{
			Vector const gotB((spinF * vecA * spinR).theV);
			Vector const expB(0., -1., 0.);
			if (! nearlyEquals(gotB, expB))
			{
				oss << "Failure of simple rotation test" << std::endl;
				oss << expB.infoString("exp") << std::endl;
				oss << gotB.infoString("got") << std::endl;
			}
		}

	}

	return oss.str();
}

//! Check scalar operations
/*
std::string
ga_ga_test4
	()
{
	std::ostringstream oss;

	{
		using namespace ga;
	}

	return oss.str();
}
*/


}

//! Unit test for ga::ga
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ga_ga_test0();
	oss << ga_ga_test0a();
	oss << ga_ga_test0b();
	oss << ga_ga_test0c();
	oss << ga_ga_test1();
	oss << ga_ga_test2();
	oss << ga_ga_test3();
//	oss << ga_ga_test4();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
