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
\brief  This file contains unit test for ga::operators
*/


#include "libga/operators.h"

#include "libga/constructs.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check basic operations
std::string
ga_operators_test0
	()
{
	std::ostringstream oss;

	//
	// vec*biv
	//
	{
		// Spot test - TODO - generalize this
		ga::ImSpinor const expVB(ga::vZero, ga::E123);
		ga::ImSpinor const gotVB{ ga::e1 * ga::E23 };
		if (! gotVB.nearlyEquals(expVB))
		{
			oss << "Failure of vec/biv test" << std::endl;
			oss << dat::infoString(expVB, "expVB") << std::endl;
			oss << dat::infoString(gotVB, "gotVB") << std::endl;
		}
	}

	//
	// biv*spin, spin*biv
	//

	ga::BiVector const bivA(-7., 3., 5.);
	ga::Scalar const sclB(2.);
	ga::BiVector const bivB(11., 13., 17.);

	// bivector is spinor with zero part - so convert biv/spin with spin/spin
	ga::Spinor const spinA(0., bivA);
	ga::Spinor const spinB(sclB, bivB);

	{
		// check (biv*spin)
		ga::Spinor const expAB{ spinA * spinB };
		ga::Spinor const gotAB{ bivA * spinB };
		if (! gotAB.nearlyEquals(expAB))
		{
			oss << "Failure of biv/spin test" << std::endl;
			oss << dat::infoString(expAB, "expAB") << std::endl;
			oss << dat::infoString(gotAB, "gotAB") << std::endl;
		}

		// check (spin*biv)
		ga::Spinor const expBA{ spinB * spinA };
		ga::Spinor const gotBA{ spinB * bivA };
		if (! gotBA.nearlyEquals(expBA))
		{
			oss << "Failure of spin/biv test" << std::endl;
			oss << dat::infoString(expBA, "expBA") << std::endl;
			oss << dat::infoString(gotBA, "gotBA") << std::endl;
		}
	}

	//
	// vec*spin, spin*vec
	//

	ga::Vector const vecA(-ga::E123 * bivA);
	{
		ga::ImSpinor const expAB{ vecA * spinB.theS + vecA * spinB.theB };
		ga::ImSpinor const gotAB{ vecA * spinB };
		if (! gotAB.nearlyEquals(expAB))
		{
			oss << "Failure of vec/spin test" << std::endl;
			oss << dat::infoString(expAB, "expAB") << std::endl;
			oss << dat::infoString(gotAB, "gotAB") << std::endl;
		}

		ga::ImSpinor const expBA{ vecA * spinB.theS + spinB.theB * vecA };
		ga::ImSpinor const gotBA{ spinB * vecA };
		if (! gotBA.nearlyEquals(expBA))
		{
			oss << "Failure of spin/vec test" << std::endl;
			oss << dat::infoString(expBA, "expBA") << std::endl;
			oss << dat::infoString(gotBA, "gotBA") << std::endl;
		}
	}

	//
	// vec*imsp, imsp*vec
	//

	ga::ImSpinor const imspB( ga::E123 * spinB);
	{
		ga::Spinor const expAB
			( ga::dot(vecA, imspB.theV)
			, (ga::wedge(vecA, imspB.theV) + vecA*imspB.theT)
			);
		ga::Spinor const gotAB{ vecA * imspB };
		if (! gotAB.nearlyEquals(expAB))
		{
			oss << "Failure of vec/imsp test" << std::endl;
			oss << dat::infoString(expAB, "expAB") << std::endl;
			oss << dat::infoString(gotAB, "gotAB") << std::endl;
		}

		ga::Spinor const expBA
			( ga::dot(imspB.theV, vecA)
			, (ga::wedge(imspB.theV, vecA) + vecA*imspB.theT)
			);
		ga::Spinor const gotBA{ imspB * vecA };
		if (! gotBA.nearlyEquals(expBA))
		{
			oss << "Failure of imsp/vec test" << std::endl;
			oss << dat::infoString(expBA, "expBA") << std::endl;
			oss << dat::infoString(gotBA, "gotBA") << std::endl;
		}
	}

	//
	// imsp*imsp
	//

	{
		ga::ImSpinor const imspA( ga::E123 * spinA);

		ga::Scalar const Isq{ ga::E123 * ga::E123 };
		ga::Spinor const expAB{ Isq * spinA * spinB };
		ga::Spinor const gotAB{ imspA * imspB };
		if (! gotAB.nearlyEquals(expAB))
		{
			oss << "Failure of imsp/imsp AB test" << std::endl;
			oss << dat::infoString(expAB, "expAB") << std::endl;
			oss << dat::infoString(gotAB, "gotAB") << std::endl;
		}
	}

	return oss.str();
}

//! Check TODO....
std::string
ga_operators_test1
	()
{
	std::ostringstream oss;
//	oss << "Failure: implement this test!" << std::endl;
	return oss.str();
}


}

//! Unit test for ga::operators
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ga_operators_test0();
	oss << ga_operators_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
