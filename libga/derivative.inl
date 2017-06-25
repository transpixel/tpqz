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
\brief Inline definitions for ga::derivative
*/



namespace ga
{

inline
ga::Spinor
dexp
	( ga::BiVector const & bVal
	, ga::BiVector const & bDot
	)
{
	ga::Spinor result;

	std::pair<double, ga::BiVector> const bMagDir{ ga::magdir(bVal) };
	double const & bMag = bMagDir.first;
	if (math::eps < bMag)
	{
		ga::BiVector const & bDir = bMagDir.second;

		double const sinMag{ std::sin(bMag) };
		double const cosMag{ std::cos(bMag) };
		double const beta{ ga::dot(bDot, bDir).theValue };

		double const invMag{ 1. / bMag };
		ga::Scalar const t1{ beta * sinMag };
		ga::BiVector const t2{ invMag * sinMag * (bDot + beta*bDir) };
		ga::BiVector const t3{ -cosMag * beta * bDir };

		result = ga::Spinor(t1, t2+t3);
	}
	else
	{
		result = ga::Spinor(0., bDot);
	}

	return result;
}

inline
ga::Vector
dRotatedSpinArgs
	( ga::BiVector const & argSpinVal
	, ga::BiVector const & argSpinDot
	, ga::Vector const & vec
	, ga::Spinor const & eNegArgVal
	)
{
	ga::Spinor const ePosDot{ ga::dexp( argSpinVal,  argSpinDot) };
	ga::Vector const vecDot{ 2.*(ePosDot * vec * eNegArgVal).theV };
	return vecDot;
}

inline
ga::Vector
dRotatedPhysArgs
	( ga::BiVector const & physAngleVal
	, ga::BiVector const & physAngleDot
	, ga::Vector const & vec
	)
{
	ga::BiVector const argVal{ .5 * physAngleVal };
	ga::BiVector const argDot{ .5 * physAngleDot };
	ga::Spinor const eNegArgVal{ ga::exp(-argVal) };
	return dRotatedSpinArgs(argVal, argDot, vec, eNegArgVal);
}

} // ga

