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
\brief Inline definitions for ga::functions
*/


#include <algorithm>


namespace ga
{
//======================================================================

template <typename Type>
inline
Type
inverse
	( Type const & arg
	)
{
	Type result{};
	if (arg.isValid())
	{
		// NOTE: could implement individual inverse() methods for speed
		// this does double work -- e.g. sq(sqrt(..))
		double const magSq{ math::sq(arg.magnitude()) };
		Type const rev{ arg.reverse() };
		if (std::numeric_limits<double>::min() < magSq)
		{
			result = (1./magSq) * rev;
		}
	}
	return result;
}

template <typename Type>
inline
double
magnitude
	( Type const & arg
	)
{
	// invoke specialized functions (faster than general multi-vec ops)
	return arg.magnitude();
}

template <typename Type>
inline
Type
unit
	( Type const & arg
	)
{
	Type result; // default to null
	double const mag(arg.magnitude());
	if (math::eps < mag)
	{
		// for non-zero arguments, normalize data
		result = Type((1./mag) * arg);
	}
	return result;
}

template <typename Type>
inline
std::pair<double, Type>
magdir
	( Type const & arg
	)
{
	std::pair<double, Type> result{ dat::nullValue<double>(), Type{} };
	if (arg.isValid())
	{
		double & mag = result.first;
		mag = arg.magnitude();
		if (math::eps < mag)
		{
			// for non-zero arguments, normalize data
			Type const dir{ ((1./mag) * arg) };
			result.second = dir;
		}
	}
	return result;
}

//
// ==== General functions
//


inline
Spinor
exp
	( BiVector const & arg
	)
{
	double const mag(arg.magnitude());
	BiVector angleDir(bZero);
	if (std::numeric_limits<double>::min() < mag)
	{
		angleDir = (1./mag) * arg;
	}
	return Spinor(Scalar(std::cos(mag)), std::sin(mag)*angleDir);
}

inline
Spinor
expG2
	( Spinor const & elemG2
	)
{
	double const expRo{ std::exp(elemG2.theS.theValue) };
	Spinor const spin{ ga::exp(elemG2.theB) };
	return Spinor{ expRo*spin };
}

inline
Spinor
logG2
	( Spinor const & elemG2
	, BiVector const & planeForPi
	)
{
	Spinor result{};
	double const spinMag{ ga::magnitude(elemG2) };
	if (std::numeric_limits<double>::epsilon() < spinMag)
	{
		Spinor const spinHat{ ga::unit(elemG2) };
		Scalar const logS{ std::log(spinMag) };
		BiVector const logB{ spin::argAngleFrom(spinHat, planeForPi) };
		if (logB.isValid())
		{
			result = Spinor{ logS, logB };
		}
	}
	return result;
}


inline
Spinor
sqrt
	( Spinor const & spin
	)
{
	Spinor result;
	double const inMag(magnitude(spin));
	if (math::eps < inMag)
	{
		double const outMag(std::sqrt(inMag));
		BiVector const outAngle(.5 * spin::argAngleFrom(spin));
		result = outMag * exp(outAngle);
	}
	else
	{
		result = Spinor{ sZero, bZero };
	}
	return result;
}

//======================================================================
}


