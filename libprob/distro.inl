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
\brief Definitions for prob::distro
*/


#include <algorithm>
#include <random>


namespace prob
{
namespace distro
{
//======================================================================

inline
double
unitTriangle
	( double const & xFrac
	)
{
	constexpr double maxValue{1.};
	constexpr double slope{maxValue / .5};
	double const delta(slope * xFrac);
	double prob(delta);
	if (.5 < xFrac)
	{
		constexpr double twoMax{2.*maxValue};
		prob = twoMax - delta;
	}
	return prob;
}

inline
double
unitHumpProb
	( double const & xFrac
	)
{
	// Note that area under curve is 1
	// f(x) = 6 * (x - x^2)
	// F(x) = 6 * ((1/2)x^2 - (1/3)x^3) + c
	// F(x) =     (   3*x^2 -    2*x^3) + c
	// F(x)(0,1) = 3(1) - 2(1) = 1
	return 6.*xFrac*(1. - xFrac);
}

//======================================================================
}
}

