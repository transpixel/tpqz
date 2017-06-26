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
\brief  This file contains unit test for ro::Pair
*/


#include "libro/cast.h"
#include "libro/ops.h"
#include "libro/PairAbs.h"
#include "libro/PairBaseZ.h"
#include "libro/PairRel.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/sprintf.h"
#include "libio/stream.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>


namespace
{

//! Check for presence of common functions
std::string
ro_Pair_test0
	()
{
	std::ostringstream oss;

	ro::PairRel const relNull{};
	if (dat::isValid(relNull))
	{
		oss << "Failure of rel.null value test" << std::endl;
		oss << "infoString: " << dat::infoString(relNull) << std::endl;
	}
	ro::PairAbs const absNull{};
	if (dat::isValid(absNull))
	{
		oss << "Failure of abs.null value test" << std::endl;
		oss << "infoString: " << dat::infoString(absNull) << std::endl;
	}
	ro::PairBaseZ const zbaNull{};
	if (dat::isValid(zbaNull))
	{
		oss << "Failure of zba.null value test" << std::endl;
		oss << "infoString: " << dat::infoString(zbaNull) << std::endl;
	}

	ro::PairAbs const absrel{ ro::cast::intoPairAbs(relNull) };
	ro::PairAbs const absabs{ ro::cast::intoPairAbs(absNull) };
	ro::PairAbs const abszba{ ro::cast::intoPairAbs(zbaNull) };

	ro::PairRel const relrel{ ro::cast::intoPairRel(relNull) };
	ro::PairRel const relabs{ ro::cast::intoPairRel(absNull) };
	ro::PairRel const relzba{ ro::cast::intoPairRel(zbaNull) };

	ro::PairBaseZ const zbarel{ ro::cast::intoPairBaseZ(relNull) };
	ro::PairBaseZ const zbaabs{ ro::cast::intoPairBaseZ(absNull) };
	ro::PairBaseZ const zbazba{ ro::cast::intoPairBaseZ(zbaNull) };

	// check comparison
	if (  absrel.nearlyEquals(absNull))
	{
		oss << "Failure of nearly equals(same) test" << std::endl;
	}
	ro::PairAbs const roGood
		(ga::Rigid::identity(), ga::Rigid(ga::e1, ga::Pose::identity()));
	if (! roGood.nearlyEquals(roGood))
	{
		oss << "Failure of nearly equals(same) test" << std::endl;
		oss << dat::infoString(roGood, "roGood") << std::endl;
	}

	return oss.str();
}

//! Check for presence of common functions
std::string
ro_Pair_test0a
	()
{
	std::ostringstream oss;

	{ // example 1: x-baseline
		ga::Vector const uDir{ -ga::e3 };
		ga::Vector const vDir{ -ga::e3 };
		ga::Rigid const ori1wRef(ga::Rigid::identity());
		ga::Rigid const ori2wRef(ga::e1, ga::Pose(.1 * ga::E23));
		ro::PairAbs const roPair(ori1wRef, ori2wRef);
		double const gotGap{ roPair.tripleProductGap(uDir, vDir) };
		if (! (0. < gotGap))
		{
			oss << "Failure of tripleProductGap() chirality test 1" << '\n';
		}
	}

	{ // example 2: z-baseline
		ga::Vector const uDir{ ga::e2 };
		ga::Vector const vDir{ ga::e2 };
		ga::Rigid const ori1wRef(ga::Rigid::identity());
		ga::Rigid const ori2wRef(ga::e3, ga::Pose(.1 * ga::E12));
		ro::PairAbs const roPair(ori1wRef, ori2wRef);
		double const gotGap{ roPair.tripleProductGap(uDir, vDir) };
		if (! (0. < gotGap))
		{
			oss << "Failure of tripleProductGap() chirality test 2" << '\n';
		}
	}

	return oss.str();
}


bool
isUnitMag
	( ga::Vector const & vec
	)
{
	double const mag{ ga::magnitude(vec) };
	bool const isUnit{ dat::nearlyEquals(mag, 1.) };
	return isUnit;
}

//! Check basic coplanarity
std::string
ro_Pair_test1
	()
{
	std::ostringstream oss;

#define EasyCase
#	if defined(EasyCase)
	ga::Vector const loc2wRef( 100.,  0.,  1.);
	ga::Vector const loc1wRef( 100.,  0., -1.);
	ga::BiVector const biv2wRef(  .50,  .25,  .125);
	ga::BiVector const biv1wRef(  .00,  -.70,  .125);
#	else
	ga::Vector const loc2wRef(  1.,  .5, -.1);
	ga::Vector const loc1wRef( -7., -.2,  .3);
	ga::BiVector const biv2wRef(  .07,-.02, -.03);
	ga::BiVector const biv1wRef( -.02, .03, -.05);
#	endif

	ga::Rigid const ori1wRef{ loc1wRef, ga::Pose(biv1wRef) };
	ga::Rigid const ori2wRef{ loc2wRef, ga::Pose(biv2wRef) };

	std::vector<std::shared_ptr<ro::Pair> > const roPairs
		{ std::make_shared<ro::PairRel>(ori1wRef, ori2wRef)
		, std::make_shared<ro::PairAbs>(ori1wRef, ori2wRef)
		, std::make_shared<ro::PairBaseZ>(ori1wRef, ori2wRef)
		};

	// cube of points surrounding model
	std::vector<ga::Vector> const pntInRefs
		{ ga::Vector( 7.,  7.,  7.)
		, ga::Vector( 7.,  7., -7.)
		, ga::Vector( 7., -7.,  7.)
		, ga::Vector( 7., -7., -7.)
		, ga::Vector(-7.,  7.,  7.)
		, ga::Vector(-7.,  7., -7.)
		, ga::Vector(-7., -7.,  7.)
		, ga::Vector(-7., -7., -7.)
		};

	// check each type of orientation pairing
	for (std::shared_ptr<ro::Pair> const & roPair : roPairs)
	{
		for (ga::Vector const & pntInRef : pntInRefs)
		{
			ga::Vector const & pntInRO = pntInRef; // pnts arbitrary to start

			// determine directions to point
			using DirPair = std::pair<ga::Vector, ga::Vector>;
			DirPair const uvDirs{ roPair->uvDirectionsFor(pntInRO) };

			// check directions for unit magnitude
			if ( (! isUnitMag(uvDirs.first)) || (! isUnitMag(uvDirs.second)) )
			{
				oss << "Failure of unit uvDir test" << std::endl;
				oss << dat::infoString(pntInRO, "pntInRO") << std::endl;
				oss << dat::infoString(*roPair, "roPair") << std::endl;
			}

			// check directions for coplanarity condition
			constexpr double expGap{ 0. };
			double const gotGap{ roPair->tripleProductGap(uvDirs) };
			if (! dat::nearlyEquals(gotGap, expGap))
			{
				oss << "Failure of tripleProductGap test" << std::endl;
				oss << dat::infoString(expGap, "expGap") << std::endl;
				oss << dat::infoString(gotGap, "gotGap") << std::endl;
				goto EndTest;
			}

			// check reconstruction of model
			ga::Rigid const gotOri0wRef{ roPair->rigid0wRef() };
			ga::Rigid const gotOri1w0{ roPair->rigid1w0() };
			ga::Rigid const gotOri2w0{ roPair->rigid2w0() };
			ga::Rigid const gotOri1wRef{ gotOri1w0 * gotOri0wRef };
			ga::Rigid const gotOri2wRef{ gotOri2w0 * gotOri0wRef };

			ga::Rigid const & expOri1wRef = ori1wRef;
			ga::Rigid const & expOri2wRef = ori2wRef;

			if (! gotOri1wRef.nearlyEquals(expOri1wRef))
			{
				oss << "Failure of reconstruction ori1wRef test" << std::endl;
				oss << "roClass: " << roPair->className() << std::endl;
				oss << dat::infoString(expOri1wRef, "expOri1wRef") << '\n';
				oss << dat::infoString(gotOri1wRef, "gotOri1wRef") << '\n';
				goto EndTest;
			}
			if (! gotOri2wRef.nearlyEquals(expOri2wRef))
			{
				oss << "Failure of reconstruction ori2wRef test" << std::endl;
				oss << "roClass: " << roPair->className() << std::endl;
				oss << dat::infoString(expOri2wRef, "expOri2wRef") << '\n';
				oss << dat::infoString(gotOri2wRef, "gotOri2wRef") << '\n';
				goto EndTest;
			}
		}
	}
EndTest:

	return oss.str();
}


}

//! Unit test for ro::Pair
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ro_Pair_test0();
	oss << ro_Pair_test0a();
	oss << ro_Pair_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
