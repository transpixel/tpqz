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
\brief Definitions for ga::spin
*/


#include "libga/spin.h"

#include "libga/constructs.h"
#include "libga/functions.h"
#include "libga/operators.h"
#include "libga/quantities.h"

#include <limits>


namespace ga
{
namespace spin
{
//======================================================================

BiVector
physicalAngleBetween
	( Vector const & dirFrom
	, Vector const & dirInto
	, BiVector const & planeForPi
	)
{
	ga::Spinor const spin{ between(dirFrom, dirInto, planeForPi) };
	return physicalAngleFrom(spin, planeForPi);
}

// spinor utilities

Spinor
between
	( Vector const & dirFrom
	, Vector const & dirInto
	, BiVector const & planeForPi
	)
{
	Spinor spin(unit(dirFrom * dirInto));
	if ((spin.theB).nearlyEquals(bZero))
	{
		if (spin.theS.theValue < 0.)
		{
			if (planeForPi.isValid())
			{
				// use default plane to construct spinor
				spin = Spinor{sZero, unit(.5*planeForPi)};
			}
			else
			{
				spin = Spinor{};
			}
		}
		else
		{
			spin = Spinor{one};
		}
	}
	else
	{
		spin = sqrt(spin);
	}
	// convert to passive convention
	return spin.reverse();
}

Spinor
betweenProjectionsOnto
	( Vector const & vecFrom
	, Vector const & vecInto
	, BiVector const & projPlane
	)
{
	Spinor spin;

	// project and normalize input vectors
	Vector const unitFrom(unit(vecFrom));
	Vector const unitInto(unit(vecInto));

	Vector const projFrom(projectionOnto(unitFrom, projPlane));
	Vector const projInto(projectionOnto(unitInto, projPlane));

	if (projFrom.isValid() && projInto.isValid())
	{
		double const magFrom{ magnitude(projFrom) };
		if (std::numeric_limits<double>::epsilon() < magFrom)
		{
			double const magInto{ magnitude(projInto) };
			if (std::numeric_limits<double>::epsilon() < magInto)
			{
				spin = between(projFrom, projInto, projPlane);
			}
		}
	}

	return spin;
}

BiVector
argAngleFrom
	( Spinor const & spin
	, BiVector const & planeForPi
	)
{
	BiVector angle;
	if (spin.isValid())
	{
		double const & caVal = spin.theS.theValue;
		double const saMag = magnitude(spin.theB);
		if (std::numeric_limits<double>::epsilon() < saMag)
		{
			// input has significant bivector component
			BiVector const bvDir{ (1./saMag) * spin.theB };
			if (bvDir.isValid())
			{
				double const amag(std::atan2(saMag, caVal));
				angle = amag * bvDir;
			}
		}
		else
		if (0. < std::abs(caVal))
		{
			// input has significant scalar part & little/no bivector part
			angle = bZero;
		}
		else
		{
			// input has little/no scalar part & little/no bivector part
			BiVector const piDir{ ga::unit(planeForPi) };
		//	assert(piDir.isValid());
			angle = math::pi * piDir;
		}
	}
	return angle;
}

BiVector
physicalAngleFrom
	( Spinor const & spin
	, BiVector const & planeForPi
	)
{
	return 2.*argAngleFrom(spin, planeForPi);
}

std::array<double, 4u>
components
	( Spinor const & spin
	)
{
	std::array<double, 4u> comps;
	if (spin.isValid())
	{
		comps[0] = spin.theS.theValue;
		comps[1] = spin.theB[0];
		comps[2] = spin.theB[1];
		comps[3] = spin.theB[2];
	}
	else
	{
		std::fill(comps.begin(), comps.end(), dat::nullValue<double>());
	}
	return comps;
}

//======================================================================
}
}

