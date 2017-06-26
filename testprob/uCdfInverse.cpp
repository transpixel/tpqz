
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
\brief  This file contains unit test for prob::CdfInverse
*/


#include "libprob/CdfInverse.h"

#include "libdat/info.h"
#include "libio/stream.h"
#include "libio/sprintf.h"

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <set>

namespace
{

//! Check for common functions
std::string
prob_CdfInverse_test0
	()
{
	std::ostringstream oss;
	prob::CdfInverse const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

bool
isInteresting
	( std::string const & str
	, std::set<std::string> const & keys
	)
{
	bool okay(false);

	for (std::string const & key : keys)
	{
		size_t const pos(str.find(key));
		okay = (std::string::npos != pos);
		if (okay)
		{
			break;
		}
	}

	return okay;
}

std::string
memoryInfoString
	( std::string const & title = std::string()
	, std::set<std::string> const & keys = { "VmData" }
	, std::string const & sep = std::string(" ")
	)
{
	std::ostringstream oss;
	oss << title << sep;
	std::ifstream ifs("/proc/self/status");
	std::string line;
	while (ifs && (! ifs.eof()))
	{
		line.clear();
		std::getline(ifs, line);

		if (isInteresting(line, keys))
		{
			if (! oss.str().empty())
			{
				oss << sep;
			}
			oss << line;
		}
	}
	return oss.str();
}

//! Check basic operations
std::string
prob_CdfInverse_test1
	()
{
	std::ostringstream oss;

	// determines resolution of test
	// large size for test accuracy - but also to make timing more significant
//	size_t const tabSize(16u*1024u*1024u + 2u);
	size_t const tabSize(     256u*1024u + 2u);

	// required cdf data end conditions
	double const cdf0( .000);
	double const cdf1(1.000);

	std::ostringstream omem;
	omem << " " << memoryInfoString("before data");

	// create a simple CDF
	//                                 [  0      1      2     3)
	std::array<double, 4u> const probs{ cdf0,  .125,  .500,  cdf1 };
//	std::array<double, 4u> const probs{ cdf0, 1./3., 2./3.,  cdf1 };
//	std::array<double, 4u> const probs{ cdf0,  .001,  .010,  cdf1 };

	// same number of data values
	std::vector<double> const expDatas{ 100., 200., 300., 400. };
	dat::Range<double> const dataRange(expDatas.front(), expDatas.back());

	omem << " " << memoryInfoString("before ctor");

	// instantiate inverse
	prob::CdfInverse const invcdf
		(probs.begin(), probs.end(), dataRange, tabSize);
	if (! invcdf.isValid())
	{
		oss << "Failure of invcdf ctor test" << std::endl;
	}

	omem << " " << memoryInfoString("after ctor");

//	double const expKB(double(tabSize - 2u) * 8. / 1024.);
//	io::out()
//		<< dat::infoString(expKB) << " kB"
//		<< " " << omem.str()
//		<< std::endl;

	/*
	io::out() << invcdf.infoString("invcdf") << std::endl;
	(void)invcdf.writeProbIndices(io::out(), "probs,data", 1./10.);
	(void)invcdf.writeProbIndices(io::out(), "probs,data", 1./(double)tabSize);
	*/
	// std::ofstream ofs("invcdf.dat");
	// (void)invcdf.writeProbIndices(ofs, "probs,data", 1./1024.);

	// check extreme cases
	double const eps(dat::smallValue<double>());

	// Prob(0) -> min data
	double const gotData0(invcdf(0.000));
	double const expData0(dataRange.min());
	if (! dat::nearlyEquals(gotData0, expData0))
	{
		oss << "Failure of end point prob=0 test\n";
	}

	// Prob(~=1) -> max data
	double const dataMag(dataRange.max());
	double const tol0(dataMag * dat::smallValue<double>());
	double const gotDataNear1(invcdf(1.000 - eps));
	double const expDataNear1(dataRange.max() * (1. - eps));
	if (! dat::nearlyEquals(gotDataNear1, expDataNear1, tol0))
	{
		double const difDataNear1(gotDataNear1 - expDataNear1);
		oss << "Failure of end point prob NEAR 1 test\n";
		oss << dat::infoString(expDataNear1, "expDataNear1") << std::endl;
		oss << dat::infoString(gotDataNear1, "gotDataNear1") << std::endl;
		oss << "         tol: " << io::sprintf("%12.5e", tol0) << std::endl;
		oss << "difDataNear1: " << io::sprintf("%12.5e", difDataNear1)
			<< std::endl;
	}

	// Prob(==1) -> is valid
	double const gotDataAt1(invcdf(1.000));
	double const expDataAt1(dataRange.max());
	if (! dat::nearlyEquals(gotDataAt1, expDataAt1))
	{
		oss << "Failure of end point prob=1 test\n";
	}

	// check general results
	double const maxFrac(probs.back() * (1. - eps));
	std::vector<double> const gotDatas
		{ invcdf(probs[0])
		, invcdf(probs[1])
		, invcdf(probs[2])
		, invcdf(maxFrac)
		};

	// check general results at data values
	// be a bit more lenient to handle "corners" in data
	double const tolN(1.e-6 * dataMag);
	bool const okay
		( dat::nearlyEquals<double>
			( gotDatas.begin(), gotDatas.end()
			, expDatas.begin(), expDatas.end()
			, tolN
			)
		);
	if (! okay)
	{
		std::vector<double> difDatas(gotDatas.size());
		std::transform
			( gotDatas.begin(), gotDatas.end()
			, expDatas.begin()
			, difDatas.begin()
			, std::minus<double>()
			);
		double const rmse
			(math::rms<double>(difDatas.begin(), difDatas.end(), 0u));
		oss << "Failure of invcdf op() test" << std::endl;
		oss << dat::infoString(expDatas.begin(), expDatas.end(), "expDatas")
			<< std::endl;
		oss << dat::infoString(gotDatas.begin(), gotDatas.end(), "gotDatas")
			<< std::endl;
		oss << "tolN: " << io::sprintf("%12.5e", tolN) << std::endl;
		oss << "rmse: " << io::sprintf("%12.5e", rmse) << std::endl;
	}

	return oss.str();
}


}

//! Unit test for prob::CdfInverse
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << prob_CdfInverse_test0();
	oss << prob_CdfInverse_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
