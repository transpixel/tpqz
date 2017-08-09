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
\brief Definitions for ga::Rigid
*/


#include "libga/Rigid.h"

#include "libio/sprintf.h"

#include <iomanip>
#include <sstream>


namespace ga
{
//======================================================================


Rigid :: Rigid
	()
	: theTrans()
	, thePose()
{ }

// explicit
Rigid :: Rigid
	( Vector const & orig
	, Pose const & pose
	)
	: theTrans(orig)
	, thePose(pose)
{ }

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

Rigid
Rigid :: inverse
	() const
{
	// y = R*(x - t)*Rr
	// Rr*y*R + t = x
	// x = Rr*(y)*R + Rr*R*t*Rr*R
	// x = Rr*(y - [-R*t*Rr] )*R
	Vector const invTrans(-thePose(theTrans));
	return Rigid(invTrans, thePose.inverse());
}

Rigid
Rigid :: operator*
	( Rigid const & first
	) const
{
	// y = R*(x - t)*Rr
	// z = Q*(y - u)*Qr
	// z = Q*(R*x*Rr - R*t*Rr - u)*Qr
	// z = Q*R*(x - t - Rr*u*R)*Rr*Qr
	// z = Q*R*(x - [t + Rr*u*R])*Rr*Qr
	//
	// y = F(x - t)
	// z = G(y - u)
	// z = GF(x - [t + Fi(u)])

	Vector const tt = first.theTrans;
	Pose const FF = first.thePose;
	Vector const uu = theTrans;
	Pose const GG = thePose;

	Pose const Fi = FF.inverse();
	Vector const netTrans(tt + Fi(uu));
	Pose const netPose(GG * FF);
	return Rigid(netTrans, netPose);
}

bool
Rigid :: nearlyEquals
	( Rigid const & other
	, double const & tolSta
	, double const & tolAtt
	) const
{
	bool same{ location().nearlyEquals(other.location(), tolSta) };
	if (same)
	{
		same &= pose().nearlyEquals(other.pose(), tolAtt);
	}
	return same;
}

std::string
Rigid :: infoStringShort
	( std::string const & title
	, std::string const & fmtLoc
	, std::string const & fmtAng
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << "  ";
	}
	if (isValid())
	{
		Vector const & loc = theTrans;
		BiVector const biv{ thePose.physicalAngle() };
		oss
			<< "  " << "loc"
			<< " " << ::io::sprintf(fmtLoc, loc[0])
			<< " " << ::io::sprintf(fmtLoc, loc[1])
			<< " " << ::io::sprintf(fmtLoc, loc[2])
			<< "  " << "pAng"
			<< " " << ::io::sprintf(fmtAng, biv[0])
			<< " " << ::io::sprintf(fmtAng, biv[1])
			<< " " << ::io::sprintf(fmtAng, biv[2])
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

std::string
Rigid :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	if (isValid())
	{
		static bool const showG(false);
		oss
			<< theTrans.infoString(" orig", showG) << '\n'
			<< thePose.physicalAngle().infoString()
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

std::string
Rigid :: infoStringDetail
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	if (isValid())
	{
		static bool const showG(false);
		oss
			<< theTrans.infoString(" orig", showG) << '\n'
			<< thePose.infoString()
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

//======================================================================
}

