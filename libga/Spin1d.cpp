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
\brief Definitions for ga::Spin1d
*/


#include "libga/Spin1d.h"

#include <array>
#include <cassert>
#include <sstream>


namespace
{

	//! Outer product (e_k^dDir) with largest magnitude
	ga::BiVector
	maxWedgeFor
		( ga::Vector const & dDir
		)
	{
		// define independent candidates
		std::array<ga::BiVector, 3u> edProds
			{ ga::wedge(ga::e1, dDir)
			, ga::wedge(ga::e2, dDir)
			, ga::wedge(ga::e3, dDir)
			};
		std::array<double, 3u> mags
			{ ga::magnitude(edProds[0])
			, ga::magnitude(edProds[1])
			, ga::magnitude(edProds[2])
			};

		// select one with largest magnitude
		size_t ndx{ 0u };
		if (mags[0] < mags[1])
		{
			ndx = 1u;
		}
		if (mags[ndx] < mags[2])
		{
			ndx = 2u;
		}

		return edProds[ndx];
	}

	//! Unitary bivector in direction of (sd);
	inline
	ga::BiVector
	dirA0For
		( std::pair<double, ga::Vector> const & sMagDir
		, std::pair<double, ga::Vector> const & dMagDir
		)
	{
		ga::BiVector dirA0{};

		// shorthand notations
		ga::Vector const & sDir = sMagDir.second;
		ga::Vector const & dDir = dMagDir.second;
		double const & sMag = sMagDir.first;
		// double const & dMag = dMagDir.first;

		// check for singular case
		if (sMag < math::eps) // half rotation
		{
			// select arbitrary plan that contains dDir
			dirA0 = ga::unit( maxWedgeFor(dDir) );
		}
		else // general case
		{
			// select plane containing both sum and diff directions
			dirA0 = ga::unit( ga::wedge(sDir, dDir));
		}

		return dirA0;
	}
}

namespace ga
{

// explicit
Spin1d :: Spin1d
	( ga::Vector const & xx
	, ga::Vector const & yy
	)
	: sVec{ yy + xx }
	, dVec{ yy - xx }
	//
	, sMagDir{ ga::magdir(sVec) }
	, dMagDir{ ga::magdir(dVec) }
	, dirD{ ga::E123 * dMagDir.second }
	, dirA0{ dirA0For(sMagDir, dMagDir) } // null for zero rotation
{
	// ensure inputs are commensurate with a pure rotation (not dilation)
	assert(dat::nearlyEquals(ga::magnitude(xx), ga::magnitude(yy)));
}

// explicit
Spin1d :: Spin1d
	( ga::BiVector const & bivX
	, ga::BiVector const & bivY
	)
	: Spin1d(-ga::E123*bivX, -ga::E123*bivY)
{ }

bool
Spin1d :: isValid
	() const
{
	return
		(  sVec.isValid()
		&& dVec.isValid()
		);
}

ga::BiVector
Spin1d :: argFor
	( double const & ww
	) const
{
	ga::BiVector lnR{};
	if (isValid())
	{
		if (! dirA0.isValid())
		{
			// identity rotation
			lnR = ga::bZero;
		}
		else
		{
			// general rotation - determine mag & dir for ww
			double const magA{ argMagFor(ww) };
			BiVector const dirA{ argDirFor(ww) };
			lnR = magA * dirA;
		}
	}
	return lnR;
}

std::string
Spin1d :: infoString
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
		oss << "sMagDir:"
			<< " " << dat::infoString(sMagDir.first)
			<< " " << dat::infoString(sMagDir.second)
			;

		oss << std::endl;
		oss << "dMagDir:"
			<< " " << dat::infoString(dMagDir.first)
			<< " " << dat::infoString(dMagDir.second)
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

double
Spin1d :: argMagFor
	( double const & ww
	) const
{
	double const num{ -dMagDir.first };
	double const den{ sMagDir.first * std::cos(ww) };
	double alpha{ math::atan2(num, den) };
	return alpha;
}

ga::BiVector
Spin1d :: argDirFor
	( double const & ww
	) const
{
	ga::Spinor const expwD{ ga::exp(ww * dirD) };
	ga::BiVector const dirA{ (expwD * dirA0).theB };
	return dirA;
}


} // ga

