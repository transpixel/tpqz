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
\brief Definitions for ro::Tweak
*/


#include "libro/Tweak.h"

#include "libio/sprintf.h"

#include <sstream>


namespace ro
{

// explicit
Tweak
Tweak :: from
	( ga::Pose const & attA
	, ga::Pose const & attB
	)
{
	Tweak twk{};

	ga::BiVector const bivA{ attA.physicalAngle() };
	ga::BiVector const bivB{ attB.physicalAngle() };
	if (bivA.isValid() && bivB.isValid())
	{
		assert(dat::nearlyEquals((bivA[2]+bivB[2]), 0.));

		twk.thePhi1 = bivA[0];
		twk.thePhi2 = bivA[1];
		twk.theAlpha = bivA[2];
		twk.theTheta1 = bivB[0];
		twk.theTheta2 = bivB[1];

io::out()
	<< dat::infoString(twk.theta(), "twk.theta()") << std::endl
	<< dat::infoString(twk.phi(), "twk.phi()") << std::endl
	<< std::endl;

	}

	return twk;
}

double
Tweak :: roFuncValue
	( ga::Vector const & uDir
	, ga::Vector const & vDir
	) const
{
	double value{ dat::nullValue<double>() };

	ga::Spinor const uSpin{ ga::exp(.5*phi()) };
	ga::Spinor const vSpin{ ga::exp(.5*theta()) };

	ga::Vector const uInRef{ (uSpin * uDir * uSpin.reverse()).theV };
	ga::Vector const vInRef{ (vSpin * vDir * vSpin.reverse()).theV };

	static ga::Vector const bInRef{ ga::e3 };
	ga::ImSpinor const spinPro{ bInRef * uInRef * vInRef };
	value = spinPro.theT.theValue;

	return value;
}

double
Tweak :: linearFuncValue
	( ga::Vector const & uDir
	, ga::Vector const & vDir
	) const
{
	double err{ dat::nullValue<double>() };
	double const & u1 = uDir[0];
	double const & u2 = uDir[1];
	double const & u3 = uDir[2];
	double const & v1 = vDir[0];
	double const & v2 = vDir[1];
	double const & v3 = vDir[2];
	double const p1{ -u3*v1 };
	double const p2{ -u3*v2 };
	double const a3{  2.*(u1*v1 + u2*v2) };
	double const t1{  v3*u1 };
	double const t2{  v3*u2 };
	double const ff{ .5*(u2*v1 - u1*v2) };
	double const gotFunc
		{ p1*thePhi1 + p2*thePhi2
		+ a3*theAlpha
		+ t1*theTheta1 + t2*theTheta2
		};
	double const expFunc{ ff };
	err = gotFunc - expFunc;
	return err;
}

std::string
Tweak :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << std::setw(15u) << title << " ";
	}
	double const magSq
		{ math::sq(thePhi1)
		+ math::sq(thePhi2)
		+ math::sq(theAlpha)
		+ math::sq(theTheta1)
		+ math::sq(theTheta2)
		};
	double const mag{ std::sqrt(magSq) };
	oss 
		<< " " << io::sprintf("%9.6f", thePhi1)
		<< " " << io::sprintf("%9.6f", thePhi2)
		<< " " << io::sprintf("%9.6f", theAlpha)
		<< " " << io::sprintf("%9.6f", theTheta1)
		<< " " << io::sprintf("%9.6f", theTheta2)
		<< " mag:"
		<< " " << io::sprintf("%9.6f", mag)
		;
	return oss.str();
}

} // ro

