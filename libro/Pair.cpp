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
\brief Definitions for ro::Pair
*/


#include "libro/Pair.h"


namespace ro
{

//
// Information
//

std::string
Pair :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss
			<< title
			<< " " << dat::infoString(className())
			<< std::endl;
	}
	if (isValid())
	{
		oss
			<< dat::infoString(baseVector2w1(), "baseVector2w1")
			<< " " << dat::infoString(baseMag(), "baseMag")
			;
		oss << std::endl;
		oss << dat::infoString(baseAngle2w1(), "baseAngle2w1");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

std::string
Pair :: infoStringDetail
	( std::string const & title
	) const
{
	std::ostringstream oss;
	oss << infoString(title);
	if (isValid())
	{
		oss << std::endl;
		oss << dat::infoString(baseMag(), "baseMag");
		oss << std::endl;
		oss << dat::infoString(station1w0(), "station1w0");
		oss << std::endl;
		oss << dat::infoString(station2w0(), "station2w0");
		oss << std::endl;
		oss << dat::infoString(angle1w0(), "angle1w0");
		oss << std::endl;
		oss << dat::infoString(angle2w0(), "angle2w0");
		oss << std::endl;
		oss << dat::infoString(rigid2w1(), "rigid2w1");
	}
	return oss.str();
}

//
// Environment interaction
//

std::pair<ga::Vector, ga::Vector>
Pair :: uvDirectionsFor
	( ga::Vector const & pntIn0
	) const
{
	ga::Vector const uVec{ rigid1w0()(pntIn0) };
	ga::Vector const vVec{ rigid2w0()(pntIn0) };
	return { ga::unit(uVec), ga::unit(vVec) };
}


//
// Coplanarity relationships
//

double
Pair :: tripleProductGap
	( ga::Vector const & uDirIn1
	, ga::Vector const & vDirIn2
	) const
{
	double value{ dat::nullValue<double>() };

	assert(dat::nearlyEquals(ga::magnitude(uDirIn1), 1.));
	assert(dat::nearlyEquals(ga::magnitude(vDirIn2), 1.));

	ga::Pose const pose0w1{ pose1w0().inverse() };
	ga::Pose const pose0w2{ pose2w0().inverse() };
	ga::Vector const uDirIn0{ pose0w1(uDirIn1) };
	ga::Vector const vDirIn0{ pose0w2(vDirIn2) };
	ga::BiVector const BB{ basePlaneDirIn0() };

	/*
	ga::Spinor const tripro{ -BB * uDirIn0 * vDirIn0 };
	value = tripro.theS.theValue;
	*/
	value = -ga::dot(BB, (uDirIn0 * vDirIn0).theB).theValue;

	return value;
}

bool
Pair :: nearlyEquals
	( Pair const & other
	) const
{
	return rigid2w1().nearlyEquals(other.rigid2w1());
}

} // ro

