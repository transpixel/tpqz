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
\brief  This file contains unit test for ro::model
*/


#include "libro/model.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"
#include "libro/PairRel.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
ro_model_test0
	()
{
	std::ostringstream oss;
	/*
	ro::model const aNull(dat::nullValue<ro::model>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

//! Check basic intersection conditions
std::string
ro_model_test1
	()
{
	std::ostringstream oss;

	constexpr double dist{ 2.75 };
	std::vector<ga::Vector> const expPnts
	//	{  ga::Vector( 2., -0., -0.)
	//	};
		{  dist * ga::Vector(-1., -1., -1.)
		,  dist * ga::Vector( 1., -1., -1.)
		,  dist * ga::Vector(-1.,  1., -1.)
		,  dist * ga::Vector( 1.,  1., -1.)
		,  dist * ga::Vector(-1., -1.,  1.)
		,  dist * ga::Vector( 1., -1.,  1.)
		,  dist * ga::Vector(-1.,  1.,  1.)
		,  dist * ga::Vector( 1.,  1.,  1.)
		};

	ga::Rigid const eo1(-ga::e3, ga::Pose(ga::BiVector(0., 0., 0.)));
	ga::Rigid const eo2( ga::e3, ga::Pose(ga::BiVector(0., 0., 0.)));
	ro::PairRel const anRO(eo1, eo2);

	// check with individual correspondences
	for (ga::Vector const & expPnt : expPnts)
	{
		ro::PairUV const uvPair{ ro::model::uvPair(expPnt, anRO.pair()) };
		ro::PntPair const gotPntPair
			{ ro::model::pointPair(uvPair, anRO.pair()) };
		if (! dat::isValid(gotPntPair))
		{
			oss << "Failure of valid pnt pair test" << std::endl;
		}
		else
		{
			ga::Vector const & gotPnt1 = gotPntPair.first;
			ga::Vector const & gotPnt2 = gotPntPair.second;
			if (! gotPnt1.nearlyEquals(expPnt))
			{
				oss << "Failure of pair gotPnt1 test" << std::endl;
				oss << dat::infoString(expPnt, "expPnt") << std::endl;
				oss << dat::infoString(gotPnt1, "gotPnt") << std::endl;
			}
			if (! gotPnt2.nearlyEquals(expPnt))
			{
				oss << "Failure of pair gotPnt2 test" << std::endl;
				oss << dat::infoString(expPnt, "expPnt") << std::endl;
				oss << dat::infoString(gotPnt2, "gotPnt") << std::endl;
			}
		}
	}

	// check with collections
	std::vector<ro::PairUV> const uvPairs
		{ ro::model::uvPairs(expPnts, anRO.pair()) };
	std::vector<ro::PntPair> const gotPnts
		{ ro::model::pointPairs(uvPairs, anRO.pair()) };
	
	if (! (gotPnts.size() == expPnts.size()))
	{
		oss << "Failure of collection size test" << std::endl;
	}
	else
	{
		for (size_t nn{0u} ; nn < expPnts.size() ; ++nn)
		{
			ga::Vector const & expPnt = expPnts[nn];
			ga::Vector const & gotPnt1 = gotPnts[nn].first;
			ga::Vector const & gotPnt2 = gotPnts[nn].second;
			if (! gotPnt1.nearlyEquals(expPnt))
			{
				oss << "Failure of collection gotPnt1 test" << std::endl;
				oss << dat::infoString(expPnt, "expPnt") << std::endl;
				oss << dat::infoString(gotPnt1, "gotPnt") << std::endl;
			}
			if (! gotPnt2.nearlyEquals(expPnt))
			{
				oss << "Failure of collection gotPnt2 test" << std::endl;
				oss << dat::infoString(expPnt, "expPnt") << std::endl;
				oss << dat::infoString(gotPnt2, "gotPnt") << std::endl;
			}
		}
	}

	return oss.str();
}


}

//! Unit test for ro::model
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ro_model_test0();
	oss << ro_model_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
