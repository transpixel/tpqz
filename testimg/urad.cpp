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
\brief  This file contains unit test for img::rad
*/

// ------ debug
#include <iostream>
#include "libdat/MinMax.h"
#include "libdat/info.h"
#include "libimg/img.h"
// ------

#include "libimg/rad.h"

#include "libimg/io.h"
#include "libio/stream.h"
#include "libmath/interp.h"
#include "libmath/math.h"
#include "libprob/SampleGen.h"
#include "libprob/distro.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
img_rad_test0
	()
{
	std::ostringstream oss;
	/*
	img::rad const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	*/
	return oss.str();
}

//! Check histogram eq
std::string
img_rad_test1
	()
{
	std::ostringstream oss;

	dat::Range<double> const & dataRange = dat::Range<double>::uint8;

//#define EasyCase
#	if defined(EasyCase)
	constexpr dat::Extents const hwSize{ 8u, 1u };
	constexpr double mx{255.};
	std::vector<double> const samps
		{ 0./mx, 1./mx, 2./mx, 3./mx, 252./mx, 253./mx, 254./mx, 255./mx };
#	else
	constexpr dat::Extents const hwSize{ 1024u, 1280u };
	// generate samples from a "triangle" pdf
	size_t const lutSize(256u);
	prob::SampleGen const generator
		( prob::CdfInverse::generateFor
			(prob::distro::unitTriangle, dataRange, lutSize)
		);
	std::vector<double> const samps(generator.samples(hwSize.size()));
#	endif


	// populate image with sample values
	dat::grid<uint8_t> srcImage(hwSize);
	static double const outMax(256. * (1. - dat::smallValue<double>()));
	static std::pair<double, double> const outRange(0., outMax);
	using math::interp::valueAtValid;
	std::transform
		( samps.begin(), samps.end()
		, srcImage.begin()
		, []
			(double const & samp)
			{ return static_cast<uint8_t>(valueAtValid(samp, outRange)); }
		);

	// check results
	math::Partition const part(0., 1., 255u);
	dat::grid<uint8_t> const gotImage(img::rad::equalized(srcImage, part));
	std::array<size_t, 256u> const gotHist
		(prob::histo::countsForUint8(gotImage.begin(), gotImage.end()));
	prob::CdfForward const gotCdf
		( prob::CdfForward::fromFreqs
			(gotHist.begin(), gotHist.end(), dataRange)
		);
	
	prob::CdfForward const expCdf(prob::CdfForward::uniform(dataRange));

	math::Partition const testPart(dataRange, 57u);
	for (size_t nn(0u) ; nn < testPart.size() ; ++nn)
	{
		double const dataVal(testPart.interpValueFor(double(nn)));
		double const expFrac(expCdf(dataVal));
		double const gotFrac(gotCdf(dataVal));
		static double const tol(2./256.);
		if (! dat::nearlyEquals(gotFrac, expFrac, tol))
		{
			double const difFrac(gotFrac - expFrac);
			oss << "Failure of triangle equalization test" << std::endl;
			oss << dat::infoString(expFrac, "expFrac") << std::endl;
			oss << dat::infoString(gotFrac, "gotFrac") << std::endl;
			oss << dat::infoString(difFrac, "difFrac") << std::endl;
			oss << dat::infoString(tol, "tol") << std::endl;
		}
	}



#	if defined(EasyCase)
	io::out()
		<< "samps\n"
		<< dat::infoString(samps.begin(), samps.end())
		<< std::endl;
	io::out()
		<< "srcImage\n"
		<< dat::infoString(srcImage.begin(), srcImage.end())
		<< std::endl;
	io::out()
		<< srcImage.infoStringContents("srcImage", "%9.6f")
		<< std::endl;
	io::out()
		<< gotImage.infoStringContents("gotImage", "%9.6f")
		<< std::endl;
#	endif

	return oss.str();
}


}

//! Unit test for img::rad
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << img_rad_test0();
	oss << img_rad_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
