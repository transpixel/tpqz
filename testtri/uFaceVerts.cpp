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
\brief  This file contains unit test for tri::FaceVerts
*/


#include "libtri/FaceVerts.h"

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
tri_FaceVerts_test0
	()
{
	std::ostringstream oss;
	tri::FaceVerts const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

	static tri::NodeKey const sKeyA{ 0u, 0u };
	static tri::NodeKey const sKeyB{ 0u, 1u };
	static tri::NodeKey const sKeyC{ 1u, 1u };

	struct PropSampFunc
	{
		using value_type = float;

		inline
		value_type
		expValue
			( double const & wgtA
			, double const & wgtB
			, double const & wgtC
			) const
		{
			value_type const vtA{ static_cast<value_type>(wgtA) };
			value_type const vtB{ static_cast<value_type>(wgtB) };
			value_type const vtC{ static_cast<value_type>(wgtC) };
			return { vtA*10.f + vtB*200.f + vtC*3000.f };
		}

		inline
		value_type
		operator()
			( tri::NodeKey const & keyIJ
			) const
		{
			value_type value{ dat::nullValue<value_type>() };
			if (dat::nearlyEquals(sKeyA, keyIJ))
			{
				value = 10.f;
			}
			else
			if (dat::nearlyEquals(sKeyB, keyIJ))
			{
				value = 200.f;
			}
			else
			if (dat::nearlyEquals(sKeyC, keyIJ))
			{
				value = 3000.f;
			}
			else
			{
				assert(! "bad test case configuration");
			}
			return value;
		}
	};

//! Check face basic geometry operations
std::string
tri_FaceVerts_test1
	()
{
	std::ostringstream oss;

	double const wgtA{ 2. };
	double const wgtB{ 3. };
	double const wgtC{ 1. };

	// populate test face
	tri::FaceVerts fverts;
	fverts.theVerts[0].theNdxIJ = sKeyA;
	fverts.theVerts[0].theW = wgtA;
	fverts.theVerts[1].theNdxIJ = sKeyB;
	fverts.theVerts[1].theW = wgtB;
	fverts.theVerts[2].theNdxIJ = sKeyC;
	fverts.theVerts[2].theW = wgtC;

	// compute weighted average
	PropSampFunc const sampFunc{};
	PropSampFunc::value_type const gotVal{ fverts.valueFrom(sampFunc) };
	PropSampFunc::value_type const expVal
		{ sampFunc.expValue(wgtA, wgtB, wgtC) };
	if (! dat::nearlyEquals(gotVal, expVal))
	{
		oss << "Failure of valueFrom() test" << std::endl;
		oss << dat::infoString(expVal, "expVal") << std::endl;
		oss << dat::infoString(gotVal, "gotVal") << std::endl;
	}

	// get index associated with largest weight (e.g. nearest node in IsoTille)
	tri::NodeKey const gotNearKey{ fverts.nodeKeyMaxW() };
	tri::NodeKey const expNearKey{ sKeyB };
	if (! dat::nearlyEquals(gotNearKey, expNearKey))
	{
		oss << "Failure of nodeKeyMaxW() test" << std::endl;
		oss << dat::infoString(expNearKey, "expNearKey") << std::endl;
		oss << dat::infoString(gotNearKey, "gotNearKey") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for tri::FaceVerts
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << tri_FaceVerts_test0();
	oss << tri_FaceVerts_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
