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

#ifndef ga_multivector_INCL_
#define ga_multivector_INCL_

/*! \file
\brief Declarations for ga::multivector
*/


#include "libga/quantities.h"

#include <array>
#include <iomanip>
#include <sstream>
#include <string>


namespace ga
{

// general 3d multivector product

	//! struct of "math-notation" members
	struct MultiVec
	{
		double s0;
		double v1;
		double v2;
		double v3;
		double B1;
		double B2;
		double B3;
		double T0;

		MultiVec
			()
			: s0{0.}, v1{0.}, v2{0.}, v3{0.} , B1{0.}, B2{0.}, B3{0.} , T0{0.}
		{ }

		MultiVec
			( double const & in_s0
			, double const & in_v1
			, double const & in_v2
			, double const & in_v3
			, double const & in_B1
			, double const & in_B2
			, double const & in_B3
			, double const & in_T0
			)
			: s0{in_s0}
			, v1{in_v1}
			, v2{in_v2}
			, v3{in_v3}
			, B1{in_B1}
			, B2{in_B2}
			, B3{in_B3}
			, T0{in_T0}
		{ }

		MultiVec
			( Vector const &  vec
			)
			: s0{0.}
			, v1{vec.theValues[0]}
			, v2{vec.theValues[1]}
			, v3{vec.theValues[2]}
			, B1{0.}
			, B2{0.}
			, B3{0.}
			, T0{0.}
		{ }

		MultiVec
			( Spinor const & spin
			)
			: s0{spin.theS.theValue}
			, v1{0.}
			, v2{0.}
			, v3{0.}
			, B1{spin.theB.theValues[0]}
			, B2{spin.theB.theValues[1]}
			, B3{spin.theB.theValues[2]}
			, T0{0.}
		{ }

		std::string
		infoString
			( std::string const & title = std::string{}
			) const
		{
			std::ostringstream oss;
			oss << std::fixed << std::setprecision(3);
			using std::setw;
			oss
				<< setw(10) << title
				<< " " << setw(8) << s0
				<< "   "
				<< " " << setw(8) << v1
				<< " " << setw(8) << v2
				<< " " << setw(8) << v3
				<< "   "
				<< " " << setw(8) << B1
				<< " " << setw(8) << B2
				<< " " << setw(8) << B3
				<< "   "
				<< " " << setw(8) << T0
				;
			return oss.str();
		}
	};

	//! brute force full multivector product (e.g. for testing
	MultiVec
	operator*
		( MultiVec const & mvA
		, MultiVec const & mvB
		)
	{
		MultiVec result;

		double const & m0 = mvA.s0;
		double const & m1 = mvA.v1;
		double const & m2 = mvA.v2;
		double const & m3 = mvA.v3;
		double const & M1 = mvA.B1;
		double const & M2 = mvA.B2;
		double const & M3 = mvA.B3;
		double const & M0 = mvA.T0;

		double const & q0 = mvB.s0;
		double const & q1 = mvB.v1;
		double const & q2 = mvB.v2;
		double const & q3 = mvB.v3;
		double const & Q1 = mvB.B1;
		double const & Q2 = mvB.B2;
		double const & Q3 = mvB.B3;
		double const & Q0 = mvB.T0;

		return MultiVec
			{ m0*q0 + m1*q1 + m2*q2 + m3*q3 - M1*Q1 - M2*Q2 - M3*Q3 - M0*Q0

			, m0*q1 + m1*q0 - m2*Q3 + m3*Q2 - M1*Q0 - M2*q3 + M3*q2 - M0*Q1
			, m0*q2 + m1*Q3 + m2*q0 - m3*Q1 + M1*q3 - M2*Q0 - M3*q1 - M0*Q2
			, m0*q3 - m1*Q2 + m2*Q1 + m3*q0 - M1*q2 + M2*q1 - M3*Q0 - M0*Q3

			, m0*Q1 + m1*Q0 + m2*q3 - m3*q2 + M1*q0 - M2*Q3 + M3*Q2 + M0*q1
			, m0*Q2 - m1*q3 + m2*Q0 + m3*q1 + M1*Q3 + M2*q0 - M3*Q1 + M0*q2
			, m0*Q3 + m1*q2 - m2*q1 + m3*Q0 - M1*Q2 + M2*Q1 + M3*q0 + M0*q3

			, m0*Q0 + m1*Q1 + m2*Q2 + m3*Q3 + M1*q1 + M2*q2 + M3*q3 + M0*q0
			};
	}

}

// Inline definitions
// #include "libga/multivector.inl"

#endif // ga_multivector_INCL_

