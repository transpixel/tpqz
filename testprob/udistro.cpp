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
\brief  This file contains unit test for prob::distro
*/


#include "libprob/distro.h"

#include "libdat/array.h"
#include "libdat/info.h"
#include "libio/stream.h"
#include "libmath/math.h"
#include "libprob/SampleGen.h"
#include "libprob/CdfInverse.h"
#include "libprob/histo.h"
#include "libprob/pdf.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace
{

//! Check for common functions
std::string
prob_distro_test0
	()
{
	std::ostringstream oss;
	/*
	prob::distro const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	*/
	return oss.str();
}


	//! Functionally generated PDF
	std::array<double, 256u>
	pdfWithHighEnds
		()
	{
		std::array<double, 256u> pdf;
		for (size_t nn(0u) ; nn < pdf.size() ; ++nn)
		{
			double const frac
				{ static_cast<double>(nn) / static_cast<double>(pdf.size()) };
			pdf[nn] = 2. - prob::distro::unitHumpProb(frac);
		}
		pdf = prob::pdf::normalized(pdf);
		return pdf;
	}

	//! Pseudo-randomly generated PDF
	std::array<double, 256u>
	simulatedPdf
		( prob::CdfInverse const & cdfInv
		, size_t const & numSamps
		)
	{
		std::array<double, 256u> pdf;
		// accumulate probabilities into pdf
		std::fill(pdf.begin(), pdf.end(), 0.);

		// generate PDF via simulation
		prob::SampleGen const generator(cdfInv);
		math::Partition const & dataPart = cdfInv.theDataPart;
		for (size_t nn(0u) ; nn < numSamps ; ++nn)
		{
			double const samp(generator());
			double const fndx(dataPart.interpIndexFor(samp));
			assert(0. <= fndx);
			assert(fndx < 256.);
			size_t const ndx{ static_cast<size_t>(std::floor(fndx)) };
			pdf[ndx] += 1.;
		}

		// ensure result is normalized
		pdf = prob::pdf::normalized(pdf);
		return pdf;
	}

//! Check simulated sample generation
std::string
prob_distro_test1
	()
{
	std::ostringstream oss;

	// test with simple 'hump' distribution
	std::array<double, 256u> const expPdf(pdfWithHighEnds());

	// use inverse CDF as basis for filling distribution
	dat::Range<double> const & dataRange = dat::Range<double>::uint8;
	size_t const lutSize(1024u);
	prob::CdfInverse const cdfInv
		( prob::CdfInverse::fromFrequencies
			(expPdf.begin(), expPdf.end(), dataRange, lutSize)
		);

	// test a simple distribution
	constexpr size_t const numSamps(1024u * 1024u);
	std::array<double, 256u> const gotPdf(simulatedPdf(cdfInv, numSamps));

	// compare results
	// skip end values which are not well defined for inversion/interpolation
	constexpr double const tol(1./1024.);
	size_t const ndxBeg(2u);
	size_t const ndxEnd(256u - 2u);
	double const * const expBeg(&(expPdf[ndxBeg]));
	double const * const expEnd(&(expPdf[ndxEnd]));
	double const * const gotBeg(&(gotPdf[ndxBeg]));
	double const * const gotEnd(&(gotPdf[ndxEnd]));
	if (! dat::nearlyEquals(expBeg, expEnd, gotBeg, gotEnd, tol))
	{
		using dat::operator-;
		std::array<double, 256u> const difPdf(gotPdf - expPdf);
		double const maxAbsDif
			(math::maxAbs<double>(&(difPdf[ndxBeg]), &(difPdf[ndxEnd])));
		oss << "Failure of simulated PDF generation test" << std::endl;
		oss << dat::infoString(expPdf, "expPdf") << std::endl;
		oss << dat::infoString(gotPdf, "gotPdf") << std::endl;
		oss << dat::infoString(difPdf, "difPdf") << std::endl;
		oss << "tol: " << tol << std::endl;
		oss << "maxAbsDif: " << maxAbsDif << std::endl;
	}


	return oss.str();
}


}

//! Unit test for prob::distro
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << prob_distro_test0();
	oss << prob_distro_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
