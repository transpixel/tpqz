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
\brief Definitions for math::QuadEq
*/

#include "libmath/QuadEq.h"

#include <limits>
#include <sstream>


namespace
{
	//! Descirminant for equation
	double
	descriminant
		( double const coA
		, double const coB
		, double const coC
		)
	{
		return (math::sq(coB) - 4.*coA*coC);
	}

	//! Delta (geometric half cord)
	double
	delta
		( double const descrim
		)
	{
		double delta{ dat::nullValue<double>() };
		if (0. <= descrim)
		{
			delta = std::sqrt(descrim);
		}
		return delta;
	}
}

namespace math
{

// explicit
QuadEq :: QuadEq
	( double const coA
	, double const coB
	, double const coC
	)
	: theA(coA)
	, theB(coB)
	, theC(coC)
	, theDescrim{ descriminant(theA, theB, theC) }
	, theDelta{ delta(theDescrim) }
{ }

bool
QuadEq :: isValid
	() const
{
	return
		(  dat::isValid(theA)
		&& dat::isValid(theB)
		&& dat::isValid(theC)
		&& (std::numeric_limits<double>::epsilon() < std::abs(theA))
		);
}

double
QuadEq :: realRootMin
	() const
{
	double root{ dat::nullValue<double>() };
	if (isValid() && dat::isValid(theDelta))
	{
		if (theB < 0.)
		{
			root = 2.*theC / (theDelta - theB);
		}
		else
		{
			root = -(theB + theDelta) / (2.*theA);
		}
	}
	return root;
}

double
QuadEq :: realRootMax
	() const
{
	double root{ dat::nullValue<double>() };
	if (isValid() && dat::isValid(theDelta))
	{
		if (theB < 0.)
		{
			root = (theDelta - theB) / (2.*theA);
		}
		else
		{
			root = -2.*theC / (theB + theDelta);
		}
	}
	return root;
}

std::vector<double>
QuadEq :: realUniqueRoots
	( double const & tol
	) const
{
	std::vector<double> roots;
	if (dat::isValid(theDelta)) // real roots
	{
		roots.reserve(2u);
		double const root1{ realRootMin() };
		double const root2{ realRootMax() };
		if (dat::nearlyEquals(theDelta, 0., tol)) // repeated root
		{
			// average to mitigate any rounding error
			// (since roots may be computed with different formulae)
			double const rootAve{ .5 * (root1 + root2) };
			roots.emplace_back(rootAve);
		}
		else
		{
			if (dat::isValid(root1))
			{
				roots.emplace_back(root1);
			}
			if (dat::isValid(root2))
			{
				roots.emplace_back(root2);
			}
			if (1u < roots.size())
			{
				if (roots[1] < roots[0])
				{
					std::swap(roots[1], roots[0]);
				}
			}
		}
	}
	return roots;
}

namespace
{
	inline
	bool
	isPos
		( double const value
		)
	{
		return (dat::isValid(value) && (0. < value));
	}
}

double
QuadEq :: realRootMinPos
	() const
{
	double root{ dat::nullValue<double>() };
	double const rootA{ realRootMin() };
	double const rootB{ realRootMax() };
	if (isPos(rootA))
	{
		if (isPos(rootB))
		{
			root = std::min(rootA, rootB);
		}
		else
		{
			root = rootA;
		}
	}
	else
	{
		if (isPos(rootB))
		{
			root = rootB;
		}
	}
	return root;
}

std::string
QuadEq :: infoString
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
		double const rootMin{ realRootMin() };
		double const rootMax{ realRootMax() };
		double const midRoot{ .5 * (rootMin + rootMax) };
		oss
			<< " a,b,c:"
			<< " " << dat::infoString(theA)
			<< " " << dat::infoString(theB)
			<< " " << dat::infoString(theC)
			;
		oss << std::endl;
		oss << "descrm:"
			<< " " << dat::infoString(theDescrim);
		oss << std::endl;
		oss << "middle:"
			<< " " << dat::infoString(midRoot);
		oss << std::endl;
		oss << " delta:"
			<< " " << dat::infoString(theDelta);
		oss << std::endl;
		oss
			<< " roots:"
			<< " " << dat::infoString(rootMin)
			<< " " << dat::infoString(rootMax)
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

}

