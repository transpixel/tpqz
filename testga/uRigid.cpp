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
\brief  This file contains unit test for ga::Rigid
*/


#include "libga/Rigid.h"

#include "libio/stream.h"
#include "libga/ga.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
ga_Rigid_test0
	()
{
	std::ostringstream oss;
	ga::Rigid const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check basic transformation operations
std::string
ga_Rigid_test1
	()
{
	std::ostringstream oss;

	// check identity transform
	{
		using namespace ga;

		Rigid const ident(Rigid::identity());
		if (! ident.isValid())
		{
			oss << "Failure of identity validity test" << std::endl;
			oss << ident.infoString("ident") << std::endl;
		}

		Vector const expVec(13., -14., 3.);
		Vector const gotVec(ident(expVec));
		if (! gotVec.nearlyEquals(expVec))
		{
			oss << "Failure of identity transform test" << std::endl;
		}
	}

	// check basic transformation properies
	{
		using namespace ga;

		Vector const station(-7., 5., 11.);
		BiVector const pAngle(.625*math::pi * unit(BiVector(17., -19., 23.)));
		Rigid const xForward(station, Pose(pAngle));
		Rigid const xInverse(xForward.inverse());

		Spinor const spinR(exp(.5 * pAngle));
		Vector const vecFrom(-17., 27., 5.);
		Vector const vecInto(rotated(spinR, (vecFrom - station)));
//		Vector const vecInto(spinR * (vecFrom - station) * spinR.reverse());
		Vector const gotInto(xForward(vecFrom));
		Vector const gotFrom(xInverse(vecInto));

		if (! ga::nearlyEquals(gotInto, vecInto))
		{
			oss << "Failure of xform forward test" << std::endl;
			oss << vecInto.infoString("exp") << std::endl;
			oss << gotInto.infoString("got") << std::endl;
		}
		if (! ga::nearlyEquals(gotFrom, vecFrom))
		{
			oss << "Failure of xform inverse test" << std::endl;
			oss << vecFrom.infoString("exp") << std::endl;
			oss << gotFrom.infoString("got") << std::endl;
		}
	}

	// check compositions
	{
		using namespace ga;

		Vector const tA(7., -5., 11.);
		Vector const tB(-11., 3., 2.);
		BiVector const pAngleA(-.625*math::pi * unit(BiVector( 7., -3., -5.)));
		BiVector const pAngleB( .125*math::pi * unit(BiVector(11., -2.,  3.)));
		Rigid const xBwrtA(tA, Pose(pAngleA));
		Rigid const xCwrtB(tB, Pose(pAngleB));

		Rigid const xCwrtA(xCwrtB * xBwrtA);

		Vector const vecInA(-27., 29., -17);
		Vector const expVecInC(xCwrtB(xBwrtA(vecInA)));
		Vector const gotVecInC(xCwrtA(vecInA));

		if (! ga::nearlyEquals(gotVecInC, expVecInC))
		{
			oss << "Failure of composition test" << std::endl;
			oss << expVecInC.infoString("expVecInC") << std::endl;
			oss << gotVecInC.infoString("gotVecInC") << std::endl;
		}
	}

	return oss.str();
}

//! Check composite transformations more generally
std::string
ga_Rigid_test2
	()
{
	std::ostringstream oss;
	constexpr double qt{ math::qtrTurn };

	// define a round trip with easy physical interpretations
	ga::Vector const locAwA( 0.,  0.,  0.);
	ga::Vector const locBwA( 3.,  0.,  0.);
	ga::Vector const locCwB( 0.,  5.,  0.);
	ga::Vector const locDwC( 0.,  0.,  7.);
	ga::Vector const locEwD( 4.,  0.,  5.);

	ga::BiVector const bivAwA( 0.,  0.,  0.);
	ga::BiVector const bivBwA( 0., -qt,  0.);
	ga::BiVector const bivCwB( 0.,  0., -qt);
	ga::BiVector const bivDwC( 0.,  qt,  0.);
	ga::BiVector const bivEwD(-qt,  0.,  0.);

	// form into orientations
	ga::Rigid const oriAwA( locAwA, ga::Pose(bivAwA) );
	ga::Rigid const oriBwA( locBwA, ga::Pose(bivBwA) );
	ga::Rigid const oriCwB( locCwB, ga::Pose(bivCwB) );
	ga::Rigid const oriDwC( locDwC, ga::Pose(bivDwC) );
	ga::Rigid const oriEwD( locEwD, ga::Pose(bivEwD) );

	// check some simple composition operations
	ga::Rigid const oriCwA{ oriCwB * oriBwA };
	ga::Rigid const oriDwA{ oriDwC * oriCwB * oriBwA };
	ga::Rigid const oriEwA{ oriEwD * oriDwC * oriCwB * oriBwA };
	if (! oriEwA.nearlyEquals(ga::Rigid::identity()))
	{
		oss << "Failure of orientation composition full trip test" << '\n';
		oss << dat::infoString(oriAwA, "oriAwA") << std::endl;
		oss << dat::infoString(oriBwA, "oriBwA") << std::endl;
		oss << dat::infoString(oriCwA, "oriCwA") << std::endl;
		oss << dat::infoString(oriDwA, "oriDwA") << std::endl;
		oss << dat::infoString(oriEwA, "oriEwA") << std::endl;
	}

	// check a few composition and inversion combinations
	ga::Rigid const oriDwE{ oriEwD.inverse() };
	ga::Rigid const oriAwB{ oriBwA.inverse() };
	ga::Rigid const oriDwB1{ oriDwE * oriEwA *oriAwB };
	ga::Rigid const oriDwB2{ oriDwC * oriCwB };
	if (! oriDwB2.nearlyEquals(oriDwB1))
	{
		oss << "Failure of partial inversion test" << std::endl;
	}

	return oss.str();
}


}

//! Unit test for ga::Rigid
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ga_Rigid_test0();
	oss << ga_Rigid_test1();
	oss << ga_Rigid_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
