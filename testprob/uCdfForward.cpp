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
\brief  This file contains unit test for prob::CdfForward
*/


#include "libdat/Range.h"
#include "libio/stream.h"
#include "libprob/CdfForward.h"
#include "libprob/pdf.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace
{

//! Check for common functions
std::string
prob_CdfForward_test0
	()
{
	std::ostringstream oss;
	prob::CdfForward const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

#	if defined HasBeenFixed
//! Check general functions
std::string
prob_CdfForward_test1
	()
{
	std::ostringstream oss;

	// simple histogram data
	std::vector<size_t> const hist{ 4, 5, 4 };

	dat::Range<double> const expDataRange(100., 400.);
	prob::CdfForward const cdf
		(prob::CdfForward::fromFreqs(hist.begin(), hist.end(), expDataRange));
	std::vector<std::pair<double, double> > const gotDPs(cdf.dataProbs());

//	io::out() << dat::infoString(cdf, "cdf") << std::endl;
//	io::out() << cdf.infoStringContents("cdf") << std::endl;
//	(void)cdf.writeDataProbs(io::out(), "data,probs");

	// check function values
	std::vector<std::pair<double, double> > expDPs(4u);
	double const sum((double)(hist[0] + hist[1] + hist[2]));
	expDPs[0] = std::make_pair( 100., 0.);
	expDPs[1] = std::make_pair( 200., (hist[0])/sum);
	expDPs[2] = std::make_pair( 300., (hist[1] + hist[0])/sum);
	expDPs[3] = std::make_pair( 400., 1.);
	if (! dat::nearlyEquals<double>
		(expDPs.begin(), expDPs.end(), gotDPs.begin(), gotDPs.end())
	   )
	{
		oss << "Failure of cdf simple construction/dp test" << std::endl;
		oss << dat::infoString(expDPs.begin(), expDPs.end(), "expDPs")
			<< std::endl;
		oss << dat::infoString(gotDPs.begin(), gotDPs.end(), "gotDPs")
			<< std::endl;
	}

	// check individual probabilibies
	double const got250(cdf(250.));
	double const exp250(.5 * (expDPs[1].second + expDPs[2].second));
	if (! dat::nearlyEquals(got250, exp250))
	{
		oss << "Failure of op(250) test" << std::endl;
		oss << dat::infoString(exp250, "exp250") << std::endl;
		oss << dat::infoString(got250, "got250") << std::endl;
	}

	double const got125(cdf(125.));
	double const exp125(.25 * (expDPs[0].second + expDPs[1].second));
	if (! dat::nearlyEquals(got125, exp125))
	{
		oss << "Failure of op(125) test" << std::endl;
		oss << dat::infoString(exp125, "exp125") << std::endl;
		oss << dat::infoString(got125, "got125") << std::endl;
	}

	return oss.str();
}
#	endif


#	if defined HasBeenFixed
//! Check cdf operations
std::string
prob_CdfForward_test2
	()
{
	std::ostringstream oss;

	// create simple CDF
	std::vector<double> const expCdf{ 0., .125, .750, 1. };
	std::vector<size_t> const hist{ 125u, 625u, 250u };
	dat::Range<double> const dataRange(100., 400.);
	prob::CdfForward const cdfFwd
		(prob::CdfForward::fromFreqs(hist.begin(), hist.end(), dataRange));

	double const eps(dataRange.max() * dat::smallValue<double>());
	std::vector<double> const gotCdf
		{ cdfFwd(100.), cdfFwd(200.), cdfFwd(300.), cdfFwd(400.-eps) };
	if (! dat::nearlyEquals<double>
		(gotCdf.begin(), gotCdf.end(), expCdf.begin(), expCdf.end())
	   )
	{
		oss << "Failure of value ctor test" << std::endl;
		oss << dat::infoString(expCdf.begin(), expCdf.end(), "expCdf")
			<< std::endl;
		oss << dat::infoString(gotCdf.begin(), gotCdf.end(), "gotCdf")
			<< std::endl;
	}

	return oss.str();
}
#	endif

//! Check construction with empty data
std::string
prob_CdfForward_test3
	()
{
	std::ostringstream oss;

	// configure an empty data set
	std::vector<double> const empty;

	// construct CDF
	dat::Range<double> const dataRange(-15., -5.);
	math::Partition const dataPart(dataRange, 8u);
	prob::CdfForward const cdfFwd
		(prob::CdfForward::fromSamps(empty.begin(), empty.end(), dataPart));

	// check if valid: should be valid, but all 0.s until end value of 1.
	if (! cdfFwd.isValid())
	{
		oss << "Failure of cdfFwd empty construction test" << std::endl;
	}

	return oss.str();
}

//! Check discrete case
std::string
prob_CdfForward_test4
	()
{
	std::ostringstream oss;

	// create an 'intense' PDF
	std::vector<double> const hist
		{ 1., 0., 0., 1., 0., 0., 0., 0., 0., 0. };
	std::vector<double> const pdf
		(prob::pdf::normalizedToUnitSum(hist.begin(), hist.end()));

	// construct cdf object
	dat::Range<double> const dataRange(0., 1.);
	prob::CdfForward const cdf
		(prob::CdfForward::fromFreqs(pdf.begin(), pdf.end(), dataRange));

	// get classic probability array
	std::vector<double> const gotProbs(cdf.probabilities());

	// check sizes
	size_t const gotSize(gotProbs.size());
	size_t const expSize(hist.size() + 1u);
	if (! dat::nearlyEquals(gotSize, expSize))
	{
		oss << "Failure of probabilities size test" << std::endl;
	}

	// check that CDF is "smoothly" increasing
	assert(1u < gotProbs.size());
	double const minRise(1. / double(hist.size()));
	double const run(double(hist.size()));
	double const minDelta(minRise / run);
	double prevProb(gotProbs[0]);
	for (size_t nn(1u) ; nn < gotProbs.size() ; ++nn)
	{
		double const currProb(gotProbs[nn]);
		double const delta(currProb - prevProb);
		bool okay(minDelta < delta);
		if (! okay)
		{
			oss << "Failure of probabilities size test" << std::endl;
			oss
				<< "nn,(prev,curr)Prob,delta,tol:"
				<< dat::infoString(nn)
				<< dat::infoString(prevProb)
				<< dat::infoString(currProb)
				<< dat::infoString(delta)
				<< dat::infoString(minDelta)
				<< std::endl;
		}

		prevProb = currProb;
	}

	return oss.str();
}

}

//! Unit test for prob::CdfForward
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << prob_CdfForward_test0();
#	if defined HasBeenFixed
	oss << prob_CdfForward_test1(); // * needs resolution
	oss << prob_CdfForward_test2(); // * needs resolution
#	endif
	oss << prob_CdfForward_test3();
	oss << prob_CdfForward_test4();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
