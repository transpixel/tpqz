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
\brief  This file contains unit test for prob::SampleStats
*/


#include "libprob/SampleStats.h"

#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
prob_SampleStats_test0
	()
{
	std::ostringstream oss;
	prob::SampleStats const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check basic sample properties
std::string
prob_SampleStats_test1
	()
{
	std::ostringstream oss;

	// create some simple data
	std::vector<double> const samps
		{  10.,  10.,  10.,  10.
		,  20.,  20.,  20.,  20.
		,  50.,  50.,  50.,  50.
		,  80.,  80.,  80.,  80.
		,  90.,  90.,  90.,  90.
		};
	dat::Range<double> const expDataRange(0., 100.);
	math::Partition const expDataPart(expDataRange, 100u);

	// construct stats
	prob::SampleStats const stats(samps.begin(), samps.end(), expDataPart);

	// data partition
	math::Partition const gotDataPart = stats.theDataPart;
	if (! gotDataPart.nearlyEquals(expDataPart))
	{
		oss << "Failure of dataPart test" << std::endl;
	}

	// size
	size_t const expSize(samps.size());
	size_t const & gotSize = stats.theCount;
	if (! (gotSize == expSize))
	{
		oss << "Failure of count test" << std::endl;
	}

	// Hist
	std::vector<size_t> expHist(expDataPart.size(), 0.);
	expHist[10] = 4u;
	expHist[20] = 4u;
	expHist[50] = 4u;
	expHist[80] = 4u;
	expHist[90] = 4u;
	std::vector<size_t> const & gotHist = stats.theHist;
	if (! dat::nearlyEquals<size_t>
		(expHist.begin(), expHist.end(), gotHist.begin(), gotHist.end())
	   )
	{
		oss << "Failure of Hist test" << std::endl;
		oss << dat::infoString(expHist.begin(), expHist.end(), "expHist")
			<< std::endl;
		oss << dat::infoString(gotHist.begin(), gotHist.end(), "gotHist")
			<< std::endl;
	}


	// PDF
	std::vector<double> expPdf(expDataPart.size(), 0.);
	double const norm(1. / double(samps.size()));
	std::transform
		( expHist.begin(), expHist.end()
		, expPdf.begin()
		, [&norm] (size_t const & hist) { return norm*(double)hist; }
		);
	std::vector<double> const & gotPdf = stats.thePdf;
	if (! dat::nearlyEquals<double>
		(expPdf.begin(), expPdf.end(), gotPdf.begin(), gotPdf.end())
	   )
	{
		oss << "Failure of Pdf test" << std::endl;
	}

	// Entropy
	double const expEntropy
		( -5. * expPdf[10] * std::log(expPdf[10]) / std::log(2.)
		);
	double const gotEntropy(stats.theEntropy.perElement());
	if (! dat::nearlyEquals(gotEntropy, expEntropy))
	{
		oss << "Failure of entropy test" << std::endl;
		oss << dat::infoString(expEntropy, "expEntropy") << std::endl;
		oss << dat::infoString(gotEntropy, "gotEntropy") << std::endl;
	}

	// CDF (frac9 test covers this since frac values are computed from CDFs)
	if (! stats.theCdfFwd.isValid())
	{
		oss << "Failure of CdfFwd validity test" << std::endl;
	}
	
	// Frac9
	prob::Frac9 const expFrac9 // range: [0., 100)
		{{   0.00 ,   1.05 ,  10.50 ,  18.00
		 ,  50.50
		 ,  83.00 ,  90.50 ,  99.05 , 100.00
		}};
	prob::Frac9 const & gotFrac9 = stats.theFrac9;
	std::array<double, 9u> const & gotVals9 = gotFrac9.theValues;
	std::array<double, 9u> const & expVals9 = expFrac9.theValues;
	if (! dat::nearlyEquals<size_t>
		(expVals9.begin(), expVals9.end(), gotVals9.begin(), gotVals9.end())
	   )
	{
		oss << "Failure of Frac9 test" << std::endl;
		oss << dat::infoString(expFrac9, "expFrac9") << std::endl;
		oss << dat::infoString(gotFrac9, "gotFrac9") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for prob::SampleStats
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << prob_SampleStats_test0();
	oss << prob_SampleStats_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
