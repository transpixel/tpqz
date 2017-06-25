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
\brief Definitions for ro::SpinPQ
*/


#include "libro/SpinPQ.h"

#include "libga/spin.h"
#include "libro/PairRel.h"
#include "libro/ro.h"

#include <array>
#include <string>
#include <sstream>


namespace ro
{

// static
std::pair<ga::Spinor, ga::Spinor>
SpinPQ :: pairPQ
	( ga::BiVector const & phi
	, ga::BiVector const & theta
	, ga::BiVector const & basePlane
	)
{ 
	std::pair<ga::Spinor, ga::Spinor> spinPair;

	ga::Spinor const ephP{ ga::exp( .5 * phi) };
	ga::Spinor const enhP{ ephP.reverse() };
	ga::Spinor const ephT{ ga::exp( .5 * theta) };
	ga::Spinor const enhT{ ephT.reverse() };

	ga::Spinor const P0{ -ephT * basePlane * enhP };
	ga::Spinor const Q0{ ephP * enhT };
	spinPair = { P0, Q0 };

	return spinPair;
}

// static
SpinPQ
SpinPQ :: from
	( ga::BiVector const & phi
	, ga::BiVector const & theta
	, ga::BiVector const & basePlane
	)
{
	return SpinPQ(pairPQ(phi, theta, basePlane));
}

// static
SpinPQ
SpinPQ :: from
	( std::pair<ga::Rigid, ga::Rigid> const & oriPair
	)
{
	ro::PairRel const roRel(oriPair.first, oriPair.second);
	ga::BiVector const basePrime{ ga::E123 * roRel.baseVector2w1() };
	return from(roRel.angle1w0(), roRel.angle2w0(), basePrime);
}

// explicit
SpinPQ :: SpinPQ
	( std::pair<ga::Spinor, ga::Spinor> const & pairPQ
	)
	: thePQ{ pairPQ }
{ }

// explicit
SpinPQ :: SpinPQ
	( ro::Pair const & anyRO
	)
	: thePQ{ from(anyRO.pair()).thePQ }
{ }

double
SpinPQ :: tripleProductGap
	( std::pair<ga::Vector, ga::Vector> const & uvPair
	) const
{
	ga::Spinor const & PP = thePQ.first;
	ga::Spinor const & QQ = thePQ.second;
	ga::Vector const & uu = uvPair.first;
	ga::Vector const & vv = uvPair.second;
	ga::Spinor const triPro{ PP * uu * QQ * vv };
	return triPro.theS.theValue;
}

bool
SpinPQ :: isValid
	() const
{
	return
		(  thePQ.first.isValid()
		&& thePQ.second.isValid()
		);
}

OriPair
SpinPQ :: pair
	() const
{
	// shortand names for current values
	ga::Spinor const & PP = thePQ.first;
	ga::Spinor const & QQ = thePQ.second;

	// dependent RO recovery
	static ga::BiVector const depPhi{ ga::bZero };
	ga::BiVector const depTheta{ (-2.*ga::logG2(QQ)).theB };
	ga::BiVector const basePlane{ -(QQ * PP).theB };
	ga::Vector const baseVec{ -ga::E123 * basePlane };

	// gather into orientation pair
	static ga::Rigid const ori1w0{ ga::Rigid::identity() };
	ga::Rigid const ori2w0(baseVec, ga::Pose(depTheta));

	// and return as RO object
	return { ori1w0, ori2w0 };
}

std::string
SpinPQ :: infoString
	( std::string const & title
	, std::string const & fmt
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	if (isValid())
	{
		oss
			<< thePQ.first.infoStringFmt("P", fmt)
			<< " " << thePQ.second.infoStringFmt("Q", fmt)
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

} // ro

