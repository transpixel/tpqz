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
\brief  This file contains unit test for ro::ops
*/


#include "libro/ops.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <random>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
ro_ops_test0
	()
{
	std::ostringstream oss;
	/*
	ro::ops const aNull(dat::nullValue<ro::ops>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}


	inline
	double
	randVal
		( double const & mag = 1.5*math::twoPi
		)
	{
		static std::default_random_engine eng{};
		std::uniform_real_distribution<> distro(-mag, mag);
		return distro(eng);
	}

	std::vector<ro::OriPair>
	roTestCases
		( size_t const & numRand = 0u
		)
	{
		std::vector<ro::OriPair> roInRefs;
		roInRefs.reserve(1u + numRand);

//#define EasyCase
#	if defined(EasyCase)
		constexpr double halfBase{ 5. };
	//	ga::Vector const loc1wRef( 100.,  100., 100.-halfBase);
	//	ga::Vector const loc2wRef( 100.,  100,  100.+halfBase);
	//	ga::BiVector const biv1wRef( -.30, -.50, -.70);
	//	ga::BiVector const biv2wRef(  .30,  .50,  .70);
		ga::Vector const loc1wRef(   0.,    0.,   0.-halfBase);
		ga::Vector const loc2wRef(   0.,    0.,   0.+halfBase);
		ga::BiVector const biv1wRef( -.01, -.00, -.00);
		ga::BiVector const biv2wRef(  .01,  .00,  .00);
#	else
		ga::Vector const loc1wRef( -.7, -.2,  .3);
		ga::Vector const loc2wRef(  .4,  .5, -.1);
		ga::BiVector const biv1wRef( -.02, .13, -.05);
		ga::BiVector const biv2wRef(  .17,-.02, -.53);
#	endif

		ga::Rigid const ori1wRef{ loc1wRef, ga::Pose(biv1wRef) };
		ga::Rigid const ori2wRef{ loc2wRef, ga::Pose(biv2wRef) };
		roInRefs.emplace_back(ro::OriPair{ ori1wRef, ori2wRef });

		for (size_t nn{0u} ; nn < numRand ; ++nn)
		{
			ga::Vector const loc2wRef(randVal(), randVal(), randVal());
			ga::Vector const loc1wRef(randVal(), randVal(), randVal());
			ga::BiVector const biv2wRef(randVal(), randVal(), randVal());
			ga::BiVector const biv1wRef(randVal(), randVal(), randVal());
			ga::Rigid const ori1wRef{ loc1wRef, ga::Pose(biv1wRef) };
			ga::Rigid const ori2wRef{ loc2wRef, ga::Pose(biv2wRef) };
			roInRefs.emplace_back(ro::OriPair{ ori1wRef, ori2wRef });
		}

		return roInRefs;
	}

//! Check "square-root" orientation
std::string
ro_ops_test1
	()
{
	std::ostringstream oss;

	using OriPair = ro::OriPair;
	std::vector<OriPair> const roInRefs{ roTestCases(0u) };

	for (OriPair const & roInRef : roInRefs)
	{
		ga::Rigid const & ori1wRef = roInRef.first;
		ga::Rigid const & ori2wRef = roInRef.second;
		ga::Rigid const expRel{ ro::relative2w1(roInRef) };


		// compute RO in root frame
		ga::Rigid ori0wRef{};
		OriPair const gotInF{ ro::sqrtOriPairFor(roInRef, &ori0wRef) };
		ga::Rigid const & got1wFit = gotInF.first;
		ga::Rigid const & got2wFit = gotInF.second;
		ga::Rigid const gotRel{ ro::relative2w1(gotInF) };

		// check that relative condition is satisfied
		if (! gotRel.nearlyEquals(expRel))
		{
			oss << "Failure of relative test" << std::endl;
			oss << dat::infoString(expRel, "expRel") << std::endl;
			oss << dat::infoString(gotRel, "gotRel") << std::endl;
		}

		// check that orientations are symmetric
		ga::Rigid const oriFw1{ got1wFit.inverse() };
		ga::Rigid const & ori2wF = got2wFit;
		if (! ori2wF.nearlyEquals(oriFw1))
		{
			oss << "Failure of orientation square test" << std::endl;
		}

		// check 'square' condition
		ga::Rigid const & expSq = expRel;
		ga::Rigid const gotSq{ math::sq(oriFw1) };
		if (! gotSq.nearlyEquals(expSq))
		{
			oss << "Failure of square test" << std::endl;
		}

		// check main orientation
		ga::Rigid const got1wRef{ got1wFit * ori0wRef };
		if (! got1wRef.nearlyEquals(ori1wRef))
		{
			oss << "Failure of fit w.r.t. ref test 1" << std::endl;
			oss << dat::infoString(ori0wRef, "ori0wRef") << std::endl;
			oss << dat::infoString(ori1wRef, "ori1wRef") << std::endl;
			oss << dat::infoString(got1wRef, "got1wRef") << std::endl;
		}
		ga::Rigid const got2wRef{ got2wFit * ori0wRef };
		if (! got2wRef.nearlyEquals(ori2wRef))
		{
			oss << "Failure of fit w.r.t. ref test 2" << std::endl;
			oss << dat::infoString(ori0wRef, "ori0wRef") << std::endl;
			oss << dat::infoString(ori2wRef, "ori2wRef") << std::endl;
			oss << dat::infoString(got2wRef, "got2wRef") << std::endl;
		}

	} // test cases

	return oss.str();
}


}

//! Unit test for ro::ops
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ro_ops_test0();
	oss << ro_ops_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
