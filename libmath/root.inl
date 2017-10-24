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
\brief Inline definitions for math::root
*/


namespace math
{
namespace root
{

//! Private utilities for math::root implementations
namespace priv
{
	// true if one is negative and the other positive
	inline
	bool
	oppositeSigns
		( double const & aa
		, double const & bb
		)
	{
		return ((aa*bb) < 0.);
	}

	// faster specialization of dat::nearlyEquals
	inline
	bool
	aboutSame
		( double const val1
		, double const val2
		, double const tol
		)
	{
		return (std::abs(val1 - val2) < tol);
	}

} // priv

template <typename Func>
inline
double
viaRidder
	( Func const & func
	, std::pair<double, double> const & bracket
	, double const & xTol
	)
{
	double bestRoot{ dat::nullValue<double>() };

	// get and check bracket values
	double x1{ bracket.first };
	double x2{ bracket.second };
	if (dat::isValid(bracket) && (x1 <= x2))
	{
		double f1{ func(x1) };
		double f2{ func(x2) };

		// if bracket spans (at least one) root, then start search
		bool keepLooking{ priv::oppositeSigns(f1, f2) };

		// check if one (or both) bracket endpoints are a root
		if (0. == f1)
		{
			bestRoot = x1;
			keepLooking = false;
		}
		else
		if (0. == f2)
		{
			bestRoot = x2;
			keepLooking = false;
		}

		// search for root
		double currRoot{ dat::nullValue<double>() };
		constexpr size_t maxIter{ 60u };
		for (size_t itTry{0u} ; (itTry < maxIter) && keepLooking ; ++itTry)
		{
			// evaluate update formula - at midpoint
			double const x3{ .5 * (x1 + x2) };
			double const f3{ func(x3) };

			// Ridder's 'magic' formula
			double const det{ math::sq(f3) - f1*f2 };
			double const rad{ std::sqrt(det) };
			if (rad < std::numeric_limits<double>::min())
			{
				bestRoot = currRoot; // may not be good!
				break;
			}
			double const delta3{ (x3-x1)*f3 / rad };
			double const x4{ (f2 < f1) ? (x3 + delta3) : (x3 - delta3) };

			// test convergence
			if (dat::isValid(currRoot) && priv::aboutSame(x4, currRoot, xTol))
			{
				bestRoot = currRoot;
				break;
			}

			// update current best estimate
			currRoot = x4;
			double const f4{ func(x4) };
			if (0. == f4)
			{
				bestRoot = currRoot;
				break;
			}

			// update bounds for next iteration
			if (priv::oppositeSigns(f3, f4))
			{
				// midpoint and most recent solution now braket the root
				x1 = x3;
				f1 = f3;
				x2 = x4;
				f2 = f4;
			}
			else
			if (priv::oppositeSigns(f1, f4))
			{
				// move upper bound inward
				x2 = x4;
				f2 = f4;
			}
			else
			if (priv::oppositeSigns(f2, f4))
			{
				// move lower bound inward
				x1 = x4;
				f1 = f4;
			}
			else
			{
				assert(! "Code error in sign test conditions");
			}

			// check for numeric convergence
			if (priv::aboutSame(x1, x2, xTol))
			{
				bestRoot = currRoot;
				break;
			}
		}

	}
	return bestRoot;
}


} // root

} // math

