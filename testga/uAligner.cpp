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
\brief  This file contains unit test for ga::Aligner
*/


#include "libga/Aligner.h"

#include "libga/Pose.h"
#include "libio/stream.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
ga_Aligner_test0
	()
{
	std::ostringstream oss;
	ga::Aligner const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check basic construction operations
std::string
ga_Aligner_test1
	()
{
	std::ostringstream oss;

	//! Check basic construction conventions
	{
		using namespace ga;

		// setup simple rotation case
		Vector const & vecA = e1;
		Vector const & vecB = e2;
		Spinor const expSpin(sqrt(vecA * vecB));

		// NOTE: expectation based on PASSIVE (frame motion) convention
		Vector const invec(7.*vecA - 3.*vecB);
		Vector const expvec((expSpin * invec * expSpin.reverse()).theV);

		// check conventions for spinor creation from vectors
		{
			Spinor const gotSpin(Aligner::passiveSpinorFor(vecA, vecB));
			Vector const gotvec((gotSpin * invec * gotSpin.reverse()).theV);
			if (! nearlyEquals(gotvec, expvec))
			{
				oss << "Failure of passiveSpinorFor() vector test" << std::endl;
				oss << expvec.infoString("expvec") << std::endl;
				oss << gotvec.infoString("gotvec") << std::endl;
			}
		}
		// check conventions for creation with bivectors - identity case
		{
			BiVector const abiv(3., 11., -7.);
			Spinor const gotSpin(Aligner::passiveSpinorFor(abiv, abiv));
			if ( (! nearlyEquals(gotSpin.theS, Scalar(one)))
			  || (! nearlyEquals(gotSpin.theB, bZero))
			   )
			{
				oss << "Failure of passiveSpinorFor() bv ident test"
					<< std::endl;
				oss << Spinor(one).infoString("expSpin") << std::endl;
				oss << gotSpin.infoString("gotSpin") << std::endl;
			}
		}
		// check conventions for creation with bivectors
		{
			BiVector const & bivA = E123 * vecA;
			BiVector const & bivB = E123 * vecB;
			Spinor const gotSpin(Aligner::passiveSpinorFor(bivA, bivB));
			Vector const gotvec((gotSpin * invec * gotSpin.reverse()).theV);
			if (! nearlyEquals(gotvec, expvec))
			{
				oss << "Failure of passiveSpinorFor() bivector test"
					<< std::endl;
				oss << expvec.infoString("expvec") << std::endl;
				oss << gotvec.infoString("gotvec") << std::endl;
			}
		}

	}
	return oss.str();
}

//! Check spinor operations
std::string
ga_Aligner_test2
	()
{
	std::ostringstream oss;

	// basic spinor computation
	{
		using namespace ga;

		// check direction and amount
		{
			BiVector const pAngle(.5*math::pi*E12);
			Spinor const expSpin(exp(.5*pAngle));
			Spinor const gotSpin(Aligner::passiveSpinorFor(e1, e2));
			if (! nearlyEquals(gotSpin, expSpin))
			{
				oss << "Failure of passiveSpinorFor dir test" << std::endl;
				oss << pAngle.infoString("pAngle",false,true) << std::endl;
				oss << expSpin.infoString("expSpin",false,true) << std::endl;
				oss << gotSpin.infoString("gotSpin",false,true) << std::endl;
			}

			Vector const expVec(-e2);
			Vector const gotVec((gotSpin * e1 * gotSpin.reverse()).theV);
			if (! nearlyEquals(gotVec, expVec))
			{
				oss << "Failure of passiveSpinorFor dir/vec test" << std::endl;
				oss << pAngle.infoString("pAngle",false,true) << std::endl;
				oss << expSpin.infoString("expSpin",false,true) << std::endl;
				oss << gotSpin.infoString("gotSpin",false,true) << std::endl;
				oss << expVec.infoString("expVec",false,true) << std::endl;
				oss << gotVec.infoString("gotVec",false,true) << std::endl;
			}
		}

		// check if sensitive to magnitude
		{
			Spinor const expSpin{exp(.25*math::pi*E12)};
			Spinor const gotSpin(Aligner::passiveSpinorFor(7.*e1, 3.*e2));
			if (! nearlyEquals(gotSpin, expSpin))
			{
				oss << "Failure of passiveSpinorFor mag test" << std::endl;
				oss << expSpin.infoString("expSpin",false,true) << std::endl;
				oss << gotSpin.infoString("gotSpin",false,true) << std::endl;
			}
		}

		// check half-rotation case
		{
			Spinor const gotSpin(Aligner::passiveSpinorFor(7.*e1, -3.*e1));
			if (gotSpin.isValid())
			{
				oss << "Failure of half-rotation test (ambiguous spinor)"
					<< std::endl;
			}
		}
	}
	return oss.str();
}

//! Check exact alignment operations
std::string
ga_Aligner_test3
	()
{
	std::ostringstream oss;

	// check convention
	{
		using namespace ga;

		// Select two vectors expressed in the reference frame system
		Vector const vec_xInRef(e1);
		Vector const vec_yInRef(e2);

		// Select the corresponding vectors in the body frame
		Vector const vec_xInBody(e1);
		Vector const vec_yInBody(-e3);

		Pose const expPose(math::halfPi * E23);

		Aligner::ExactFit<Vector> const pairA(vec_xInRef, vec_xInBody);
		Aligner::BestTry<Vector> const pairB(vec_yInRef, vec_yInBody);
		Aligner const align(pairA, pairB);
		Pose const gotPose(align.pose());

		if (! (nearlyEquals(gotPose, expPose)))
		{
			oss << "Failure of exact convention test" << std::endl;
			oss << expPose.infoString("expPose") << std::endl;
			oss << gotPose.infoString("gotPose") << std::endl;
		}
	}

	// check exact alignment
	{
		using namespace ga;

		// check easy case

		// generate vector samples for some arbitary transform
		Pose const expPose(BiVector(.2, .6, .1));
		Vector const vec_aInRef(-5., 4., 2.);
		Vector const vec_bInRef( 3., 7., 11.);

		Vector const exp_aInBody(expPose(vec_aInRef));
		Vector const exp_bInBody(expPose(vec_bInRef));

		// attempt to reconstruct alignment
		Aligner::ExactFit<Vector> const pairA(vec_aInRef, exp_aInBody);
		Aligner::BestTry<Vector> const pairB(vec_bInRef, exp_bInBody);
		Aligner const align(pairA, pairB);
		Pose const gotPose(align.pose());

		// check if alignment is correct
		Vector const got_aInBody(gotPose(vec_aInRef));
		Vector const got_bInBody(gotPose(vec_bInRef));
		if ( (! nearlyEquals(got_aInBody, exp_aInBody))
		  || (! nearlyEquals(got_bInBody, exp_bInBody))
		   )
		{
			oss << "Failure of exact alignment test" << std::endl;
			oss << expPose.infoString("expPose") << std::endl;
			oss << gotPose.infoString("gotPose") << std::endl;
			oss << "details:" << std::endl;
			oss << vec_aInRef.infoString("... vec_aInRef") << std::endl;
			oss << exp_aInBody.infoString("... exp_aInBody") << std::endl;
			oss << got_aInBody.infoString("... got_aInBody") << std::endl;
			oss << vec_bInRef.infoString("... vec_bInRef") << std::endl;
			oss << exp_bInBody.infoString("... exp_bInBody") << std::endl;
			oss << got_bInBody.infoString("... got_bInBody") << std::endl;
		}
	}
	return oss.str();
}

//! Check best fit alignment operations
std::string
ga_Aligner_test4
	()
{
	std::ostringstream oss;

	// trivial best fit case
	{
		using namespace ga;

		Vector const vec_aInA(1., 2., 3.);
		Vector const vec_aInC(1., 2., 3.);

		Vector const vec_bInA(5., 7., 11.);
		Vector const vec_bInC(5., 7., 11.);

		Pose const expPose(Pose::identity());

		Aligner::ExactFit<Vector> const pairA(vec_aInA, vec_aInC);
		Aligner::BestTry<Vector> const pairB(vec_bInA, vec_bInC);
		Aligner const align(pairA, pairB);

		Pose const gotPose(align.pose());
		if (! nearlyEquals(align.pose(), expPose))
		{
			oss << "Failure of best-fit identity test" << std::endl;
			oss << expPose.infoString("expPose") << std::endl;
			oss << gotPose.infoString("gotPose") << std::endl;
		}
	}

	// simple best fit case
	{
		using namespace ga;

		// Select two vectors expressed in the reference frame system
		// pair in frame 'A' -- make this fit exact
		Vector const vec_aInA(e3);
		Vector const vec_bInA(e2);

		// Select the corresponding vectors in the body frame
		// pair in frame 'B' -- make fit such that 'ab' plane matches
		Vector const vec_aInC(e3);
		Vector const vec_bInC(e1 + e2 + e3);

		Pose const expPose(BiVector{0., 0., math::qtrPi});

		Aligner::ExactFit<Vector> const pairA(vec_aInA, vec_aInC);
		Aligner::BestTry<Vector> const pairB(vec_bInA, vec_bInC);
		Aligner const align(pairA, pairB);

		Pose const gotPose(align.pose());
		if (! nearlyEquals(align.pose(), expPose))
		{
			oss << "Failure of best-fit 1/4 turn test" << std::endl;
			oss << expPose.infoString("expPose") << std::endl;
			oss << gotPose.infoString("gotPose") << std::endl;
		}
	}

	// best fit alignment
	{
		using namespace ga;

		BiVector const expAngle(math::qtrPi * E23);
		Vector const fromA(e1);
		Vector const fromB(e2);
		Vector const expIntoA(e1);
		Vector const expIntoB(4.,  1., -1.);
		Aligner::ExactFit<Vector> const pairA(fromA, expIntoA);
		Aligner::BestTry<Vector> const pairB(fromB, expIntoB);
		Pose const gotPose(Aligner(pairA, pairB).pose());
		BiVector const gotAngle(gotPose.physicalAngle());

		if (! nearlyEquals(gotAngle, expAngle))
		{
			oss << "Failure of best fit alignment test" << std::endl;
			oss << expAngle.infoString("expAngle") << std::endl;
			oss << gotAngle.infoString("gotAngle") << std::endl;
		}
	}

	return oss.str();
}

//! Check zero operations
std::string
ga_Aligner_test5
	()
{
	std::ostringstream oss;

	// zero inputs
	{
		using namespace ga;

		Vector const fromA(7., 4., -2.);
		Vector const intoA(5., 2.,  7.);
		Vector const fromB(-5., 8., 3.);
		Vector const intoB( 2., 3., 5.);
		Aligner const align0111
			{ Aligner::ExactFit<Vector>(vZero, intoA)
			, Aligner::BestTry<Vector>(fromB, intoB)
			};
		Aligner const align1011
			{ Aligner::ExactFit<Vector>(fromA, vZero)
			, Aligner::BestTry<Vector>(fromB, intoB)
			};
		Aligner const align1101
			{ Aligner::ExactFit<Vector>(fromA, intoA)
			, Aligner::BestTry<Vector>(vZero, intoB)
			};
		Aligner const align1110
			{ Aligner::ExactFit<Vector>(fromA, intoA)
			, Aligner::BestTry<Vector>(fromB, vZero)
			};
		if (align0111.isValid())
		{
			oss << "Failure of 0111 test" << std::endl;
		}
		if (align1011.isValid())
		{
			oss << "Failure of 1011 test" << std::endl;
		}
		if (align1101.isValid())
		{
			oss << "Failure of 1101 test" << std::endl;
		}
		if (align1110.isValid())
		{
			oss << "Failure of 1110 test" << std::endl;
		}
	}
	return oss.str();
}

//! Check identity operations
std::string
ga_Aligner_test6
	()
{
	std::ostringstream oss;

	// identity
	{
		using namespace ga;

		Vector const sameA(7., 4., -2.);
		Vector const sameB(-5., 8., 3.);
		Aligner::ExactFit<Vector> const pairA(sameA, sameA);
		Aligner::BestTry<Vector> const pairB(sameB, sameB);
		Aligner const align(pairA, pairB);
		Pose const gotPose(align.pose());
		Pose const expPose(Pose::identity());
		if (! nearlyEquals(gotPose, expPose))
		{
			oss << "Failure of identity test" << std::endl;
		}
	}
	return oss.str();
}

//! Check half turn operations
std::string
ga_Aligner_test7
	()
{
	std::ostringstream oss;

	// pi rotation
	{
		using namespace ga;

		// configure for half-turn rotation
		BiVector const pAngle(math::pi * unit(BiVector{-8., 5., 7.}));
		Pose const poseCwrtA(pAngle);
		Vector const aInA{ 2.*e1};
		Vector const bInA{ 3.*e2};
		Vector const aInC{poseCwrtA(aInA)};
		Vector const bInC{poseCwrtA(bInA)};

		// instantiate aligner
		Aligner::ExactFit<Vector> const pairA(aInA, aInC);
		Aligner::BestTry<Vector> const pairC(bInA, bInC);
		Aligner const align(pairA, pairC);

		// check results
		Pose const expPose(pAngle);
		Pose const gotPose(align.pose());
		if (! nearlyEquals(gotPose, expPose))
		{
			oss << "Failure of pi-flip test" << std::endl;
			oss << expPose.infoString("expPose") << std::endl;
			oss << gotPose.infoString("gotPose") << std::endl;
		}
	}

	return oss.str();
}


}

//! Unit test for ga::Aligner
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ga_Aligner_test0();
	oss << ga_Aligner_test1();
	oss << ga_Aligner_test2();
	oss << ga_Aligner_test3();
	oss << ga_Aligner_test4();
	oss << ga_Aligner_test5();
	oss << ga_Aligner_test6();
	oss << ga_Aligner_test7();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
io::err() << std::endl;
io::err() << std::endl;
io::err() << "ERROR-ERROR-ERROR-ERROR-ERROR-ERROR" << std::endl;
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
