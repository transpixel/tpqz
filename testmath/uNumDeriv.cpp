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
\brief  This file contains unit test for math::NumDeriv
*/


#include "libmath/NumDeriv.h"

#include "libdat/compare.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
math_NumDeriv_test0
	()
{
	std::ostringstream oss;
	math::NumDeriv<int> const hasNoNull{};
	if (! dat::isValid(hasNoNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(hasNoNull) << std::endl;
	}
	return oss.str();
}

//! Check simple case
std::string
math_NumDeriv_test1
	()
{
	std::ostringstream oss;

	// Some arbitrary function object (here a simple polynomial functor)
	using OutType = double;
	using InType = double;
	struct SomeFunc
	{
		OutType
		operator()
			( InType const & arg
			) const
		{
			return (7.*arg*arg - 3.*arg + 11.);
		}

		OutType
		expDeriv
			( InType const & arg
			) const
		{
			return (14.*arg - 3.);
		}
	};

	SomeFunc const func; // function to differentiate
	math::NumDeriv<OutType> const nDrv; // generic numeric diff object

	// domain value at which to compute derivative
	constexpr InType atVal{ 5. };

	// determine direction for numeric gradient
	// e.g. a unitary member of the domain (here a scalar)
	InType const delta{ nDrv.deltaInDirOf(1.) };

	// evaluate the derivative
	OutType const gotDeriv{ nDrv.derivative<InType>(atVal, delta, func) };

	// check result
	OutType const expDeriv{ func.expDeriv(atVal) };
	if (! dat::nearlyEquals(gotDeriv, expDeriv))
	{
		oss << "Failure of SomeFunc numeric deriv test" << std::endl;
		oss << dat::infoString(expDeriv, "expDeriv") << std::endl;
		oss << dat::infoString(gotDeriv, "gotDeriv") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for math::NumDeriv
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << math_NumDeriv_test0();
	oss << math_NumDeriv_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
