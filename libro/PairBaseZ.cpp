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
\brief Definitions for ro::PairBaseZ
*/


#include "libro/PairBaseZ.h"

#include "libga/derivative.h"
#include "libga/functions.h"
#include "libga/Spin1d.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libmath/angle.h"
#include "libmath/Extreme.h"
#include "libmath/root.h"
#include "libro/ops.h"

#include <fstream>
#include <set>
#include <sstream>
#include <vector>



namespace ro
{

// explicit
PairBaseZ :: PairBaseZ
	( ga::Rigid const & ori1wRef
	, ga::Rigid const & ori2wRef
	)
	: ro::Pair("PairBaseZ")
	, theParms{ ArgsBaseZ::from(ori1wRef, ori2wRef) }
	, theBaseMag{ ro::gapBetween(OriPair{ ori1wRef, ori2wRef }) }
	, theOri0wRef{ theParms.baseFrameWrtRef(ori1wRef, ori2wRef) }
{
}

// explicit
PairBaseZ :: PairBaseZ
	( ArgsBaseZ const & parms
	, double const & baseMag
	, ga::Rigid const & ori0wRef
	)
	: ro::Pair("PairBaseZ")
	, theParms{ parms }
	, theBaseMag{ baseMag }
	, theOri0wRef{ ori0wRef }
{
}

// virtual
inline
bool
PairBaseZ :: isValid
	() const
{
	return
		(  theParms.isValid()
		&& dat::isValid(theBaseMag)
		&& (math::eps < theBaseMag)
		);
}

double
PairBaseZ :: parmRMS
	() const
{
	return std::sqrt
		( math::sq(theParms.thePhi12[0])
		+ math::sq(theParms.thePhi12[1])
		+ math::sq(theParms.theTheta12[0])
		+ math::sq(theParms.theTheta12[1])
		+ math::sq(theParms.theAlpha3)
		);
}

namespace // move somewhere?
{
	double
	scalarTripleProduct
		( ga::Vector const & aa
		, ga::Vector const & bb
		, ga::Vector const & cc
		)
	{
		return ga::dot(aa, ga::cross(bb, cc)).theValue;
	}
}

std::array<double, 5u>
PairBaseZ :: parmValues
	() const
{
	return
		{{ theParms.thePhi12[0]
		 , theParms.thePhi12[1]
		 , theParms.theTheta12[0]
		 , theParms.theTheta12[1]
		 , theParms.theAlpha3
		}};
}

std::array<double, 5u>
PairBaseZ :: jacobianRow
	( ga::Vector const & uDir
	, ga::Vector const & vDir
	) const
{
	std::array<double, 5u> dfdqs;
	std::fill(dfdqs.begin(), dfdqs.end(), dat::nullValue<double>());

	assert(dat::nearlyEquals(ga::magnitude(uDir), 1.));
	assert(dat::nearlyEquals(ga::magnitude(vDir), 1.));

	//
	// Follows geo/RO.lyx
	// TODO: condense/simplify the 'longhand' implementation below
	//

	// compute relevant quantities expressed w.r.t. base frame
	ga::BiVector const BB{ ga::unit(basePlaneDirIn0()) };
	assert(BB.nearlyEquals(theParms.basePlaneDir()));
	ga::Vector const gg{ gVec(uDir) };
	ga::Vector const hh{ hVec(vDir) };

	// compute the individual tems of the derivative components
	std::array<double, 3u> dfdps{{}};
	std::array<double, 3u> dfdts{{}};
	for (size_t kk{0u} ; kk < 3u ; ++kk)
	{
		ga::Vector const dg{ gDot(uDir, kk) };
		dfdps[kk] = -(BB * dg * hh).theS.theValue;

		ga::Vector const dh{ hDot(vDir, kk) };
		dfdts[kk] = -(BB * gg * dh).theS.theValue;
	}

	// combine terms into full derivative
	dfdqs[0] = (dfdps[0]           );
	dfdqs[1] = (dfdps[1]           );
	dfdqs[2] = (           dfdts[0]);
	dfdqs[3] = (           dfdts[1]);
	dfdqs[4] = (dfdps[2] + dfdts[2]);

	/*
	static std::string const fmt{ "%15.12f" };
	io::out() << std::endl;
	io::out() << dat::infoString(dfdqs, "dfdqs") << std::endl;
	io::out() << "df/dP1: " << io::sprintf(fmt, dfdqs[0]) << std::endl;
	io::out() << "df/dP2: " << io::sprintf(fmt, dfdqs[1]) << std::endl;
	io::out() << "df/dA3: " << io::sprintf(fmt, dfdqs[2]) << std::endl;
	io::out() << "df/dP1: " << io::sprintf(fmt, dfdqs[3]) << std::endl;
	io::out() << "df/dP2: " << io::sprintf(fmt, dfdqs[4]) << std::endl;
	io::out() << std::endl;
	*/

	return dfdqs;
}

std::array<double, 5u>
PairBaseZ :: jacobianRow
	( std::pair<ga::Vector, ga::Vector> const & uvPair
	) const
{
	return jacobianRow(uvPair.first, uvPair.second);
}

std::string
PairBaseZ :: parmString
	( std::string const & fmt
	) const
{
	std::ostringstream oss;
	if (isValid())
	{
		oss << theParms.infoString("p12,a3,t12:", fmt);
	}
	return oss.str();
}

//! Descriptive information about this instance.
// virtual

std::string
PairBaseZ:: infoStringParms
	( std::string const & title
	, std::string const & fmt
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << " ";
	}
	if (isValid())
	{
		oss << parmString(fmt);
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

// virtual
std::string
PairBaseZ :: infoStringDetail
	( std::string const & title
	) const
{
	std::ostringstream oss;
	oss << Pair::infoStringDetail(title);
	if (isValid())
	{
		oss << std::endl;
		oss << infoStringParms();
	}
	return oss.str();
}

ga::Vector
PairBaseZ :: gVec
	( ga::Vector const & uDir
	) const
{
	ga::BiVector const physAngle{ -theParms.phiBiv() };
	ga::Pose const att0w1(physAngle);
	return att0w1(uDir);
}

ga::Vector
PairBaseZ :: hVec
	( ga::Vector const & vDir
	) const
{
	ga::BiVector const physAngle{ -theParms.thetaBiv() };
	ga::Pose const att0w2(physAngle);
	return att0w2(vDir);
}

ga::Vector
PairBaseZ :: gDot
	( ga::Vector const & uDir
	, size_t const & ndxPhi
	) const
{
	ga::BiVector const physBiv{ -theParms.phiBiv() };
	ga::BiVector const physDot{ -theParms.phiDot(ndxPhi) };
	return ga::dRotatedPhysArgs(physBiv, physDot, uDir);
}

ga::Vector
PairBaseZ :: hDot
	( ga::Vector const & vDir
	, size_t const & ndxTheta
	) const
{
	ga::BiVector const physBiv{ -theParms.thetaBiv() };
	ga::BiVector const physDot{ -theParms.thetaDot(ndxTheta) };
	return ga::dRotatedPhysArgs(physBiv, physDot, vDir);
}

} // ro

