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
\brief Definitions for ga::Aligner
*/


#include "libga/Aligner.h"

#include "libga/constructs.h"
#include "libga/functions.h"
#include "libga/Pose.h"
#include "libga/spin.h"
#include "libio/stream.h"

#include <sstream>


namespace ga
{
	//! Compute physical attitude angle from two direction pairs
	BiVector
	pAngleFrom
		( Aligner::ExactFit<Vector> const & exact
		, Aligner::BestTry<Vector> const & best
		)
	{
		BiVector pAngle;

		// ensure normalized values
		Vector const vec_xInRef(unit(exact.theFromInto.first));
		Vector const vec_xInB(unit(exact.theFromInto.second));
		Vector const vec_tInRef(unit(best.theFromInto.first));
		Vector const vec_tInB(unit(best.theFromInto.second));


		// by design, this direction is unaffected by second rotation
		Vector const & vec_xInA = vec_xInB;

bool const debug(false);
if (debug)
{
io::out() << "------------------" << std::endl;
io::out() << vec_xInRef.infoString("vec_xInRef") << std::endl;
io::out() << vec_xInB.infoString("vec_xIn[AB]") << std::endl;
io::out() << vec_tInRef.infoString("vec_tInRef") << std::endl;
io::out() << vec_tInB.infoString("vec_tInB") << std::endl;
}

		// Frames:
		//  Ref: Starting frame
		//    A: first rotation
		//    B: second rotation

		// check recovery of first rotation
		// In case the vectors are anti-parallel, use any rotation to resolve
		// (it will be 'cleaned-up' during the second stage
		BiVector const anyPlane(E123 * anyPerpendicularDir(vec_xInRef));
		Spinor const spin1(spin::between(vec_xInRef, vec_xInA, anyPlane));
		if (spin1.isValid())
		{
			// define plane for secondary rotation
			// dual to newly aligned xInA==xInB
			BiVector const planeFor2(E123 * vec_xInA);

			// transform secondary vector to best fit
			Vector const tmp_yInA((spin1 * vec_tInRef * spin1.reverse()).theV);
			// project secondary vectors onto plane for second rotation
			Vector const vec_yInA(unit(projectionOnto(tmp_yInA, planeFor2)));
			Vector const vec_yInB(unit(projectionOnto(vec_tInB, planeFor2)));

if (debug)
{
io::out() << "------------------" << std::endl;
io::out() << vec_yInA.infoString("vec_yInA") << std::endl;
io::out() << vec_yInB.infoString("vec_yInB") << std::endl;
io::out() << "..." << std::endl;
io::out() << spin1.infoString("spin1") << std::endl;
}

			// check recover of second rotation
			Spinor const spin2
				(spin::betweenProjectionsOnto(vec_yInA, vec_yInB, planeFor2));
			if (spin2.isValid())
			{
				Spinor const spinNet(spin2 * spin1);
				// extract angle
				pAngle = spin::physicalAngleFrom(spinNet);
if (debug)
{
io::out() << "..." << std::endl;
io::out() << spin2.infoString("spin2") << std::endl;
io::out() << spinNet.infoString("spinNet") << std::endl;
io::out() << pAngle.infoString("pAngle") << std::endl;
io::out() << "------------------" << std::endl;
}
			}
		}



if (debug)
{
io::out() << "------------------" << std::endl;
}
		return pAngle;
	}


//======================================================================

// static
Spinor
Aligner :: passiveSpinorFor
	( Vector const & from
	, Vector const & into
	)
{
	Spinor spin;
	// validity is checked after normalization
	// if (from.isValid() && into.isValid())
	Vector const unitFrom(unit(from));
	Vector const unitInto(unit(into));
	if (unitFrom.isValid() && unitInto.isValid()) // is spin defined?
	{
		// Note CONVENTION: e.g. R*v*R.rev() !
		// therefore, multiply in "reverse order" from active convention
		Spinor const spinSq(unitFrom * unitInto);
		if (nearlyEquals(spinSq.theB, bZero))
		{
			if (0. < spinSq.theS.theValue) // parallel from/into
			{
				spin = Spinor{one};
			}
			// else // anti-parallel from/into
			// needs more information to solve uniquely
		}
		else
		{
			// retains unit magnitude since spinSq is unit mag!
			spin = sqrt(spinSq);
		}
	}
	// else // pose undefined
	return spin;
}

// static
Spinor
Aligner :: passiveSpinorFor
	( BiVector const & from
	, BiVector const & into
	)
{
	// exploit 3D dual relationship and code for vector manipulations
	Vector const vFrom(E123 * from);
	Vector const vInto(E123 * into);
	return passiveSpinorFor(vFrom, vInto);
}


Aligner :: Aligner
	()
	: thePhysAngle()
{
}

// explicit
Aligner :: Aligner
	( ExactFit<Vector> const & exactFromInto
	, BestTry<Vector> const & bestFromInto
	)
	: thePhysAngle(pAngleFrom(exactFromInto, bestFromInto))
{
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

bool
Aligner :: isValid
	() const
{
	return thePhysAngle.isValid();
}

Pose
Aligner :: pose
	() const
{
	return Pose{thePhysAngle};
}

std::string
Aligner :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << " ";
	}

	oss << thePhysAngle.infoString("thePhysAngle");

	return oss.str();
}

//======================================================================
}

