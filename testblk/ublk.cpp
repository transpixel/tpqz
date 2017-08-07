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
\brief  This file contains unit test for blk::blk
*/


#include "libblk/blk.h"
#include "libblk/info.h"

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
blk_blk_test0
	()
{
	std::ostringstream oss;
	/*
	blk::blk const aNull(dat::nullValue<blk::blk>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

//! Check basic operations
std::string
blk_blk_test1
	()
{
	std::ostringstream oss;

	std::vector<ga::Rigid> eosIn1;
	eosIn1.reserve(10u);

	using namespace ga;

	ga::Vector const off( 10., 0., 0. );
	constexpr double qtr{ math::qtrTurn };

	eosIn1.emplace_back(Rigid(off +e1   , Pose(BiVector(0., 0., qtr))));
	eosIn1.emplace_back(Rigid(off +e1+e2, Pose(BiVector(0., 0., 2.*qtr))));
	eosIn1.emplace_back(Rigid(off    +e2, Pose(BiVector(0., 0., 3.*qtr))));
	eosIn1.emplace_back(Rigid(off       , Pose(BiVector(0., 0., .0))));

	std::vector<ga::Rigid> eosIn2;
	std::vector<ga::Rigid> eosIn3;

	// transform all orientations into arbitrary frame
	{
		size_t const ndxFit{ 2u };
		ga::Rigid const & oriIn1 = eosIn1[ndxFit];
		ga::Rigid const & oriIn2 = ga::Rigid::identity();
		eosIn2 = blk::transformed(eosIn1, oriIn2, oriIn1);
	}

	// transform back to source frame
	{
		size_t const ndxFit{ 1u };
		ga::Rigid const & oriIn2 = eosIn2[ndxFit];
		ga::Rigid const & oriIn3 = eosIn1[ndxFit];
		eosIn3 = blk::transformed(eosIn2, oriIn3, oriIn2);
	}

	// check that round-trip transformed EO's match source
	std::vector<ga::Rigid> const & expEOs = eosIn1;
	std::vector<ga::Rigid> const & gotEOs = eosIn3;
	if (! (expEOs.size() == gotEOs.size()))
	{
		oss << "Failure of size test" << std::endl;
	}
	else
	{
		for (size_t nn{0u} ; nn < expEOs.size() ; ++nn)
		{
			ga::Rigid const & expEO = expEOs[nn];
			ga::Rigid const & gotEO = gotEOs[nn];
			if (! gotEO.nearlyEquals(expEO))
			{
				oss << "Failure of EO compare test" << std::endl;
				oss << dat::infoString(expEO, "expEO") << std::endl;
				oss << dat::infoString(gotEO, "gotEO") << std::endl;
				break;
			}
		}
	}

	/*
	io::out() << "== Results" << std::endl;
	io::out() << blk::infoString(eosIn1, "eosIn1") << std::endl;
	io::out() << std::endl;
	io::out() << blk::infoString(eosIn2, "eosIn2") << std::endl;
	io::out() << std::endl;
	io::out() << blk::infoString(eosIn3, "eosIn3") << std::endl;
	io::out() << std::endl;
	*/

	return oss.str();
}


}

//! Unit test for blk::blk
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << blk_blk_test0();
	oss << blk_blk_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
