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
\brief  This file contains unit test for img::color
*/


#undef NDEBUG // assert() is integral to test paradigm


#include "libimg/color.h"

#include "libimg/pixel.h"
#include "libio/stream.h"
#include "libmath/math.h"
#include "libsys/time.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>

namespace
{
	static bool const sEnforceLimits(true);
	static bool const sDebug(false);
	static bool const sDoTiming(false);

	// NOTE: precision limit by sig-figs in mat/inv representations
	constexpr float sFeps{std::numeric_limits<img::fpix_t>::epsilon()};
	constexpr float sFuzzyOne{1.f + 32.f*sFeps};

	//! Very fugly hack for pixel conversion error assessment.
	class PixStats
	{
		std::array<double, 3u> theMaxErrYxy;
		std::array<double, 3u> theMaxErrLab;
		std::array<double, 3u> theMinXYZ;
		std::array<double, 3u> theMaxXYZ;
		std::array<double, 3u> theMinYxy;
		std::array<double, 3u> theMaxYxy;
		std::array<double, 3u> theMinLab;
		std::array<double, 3u> theMaxLab;
		std::array<double, 3u> theMinSRGB;
		std::array<double, 3u> theMaxSRGB;

	public:

		//! Start running statistics
		explicit
		PixStats
			()
			: theMaxErrYxy{{ 0., 0., 0. }}
			, theMaxErrLab{{ 0., 0., 0. }}
			, theMinXYZ{{ 1.e6, 1.e6, 1.e6 }}
			, theMaxXYZ{{ -1., -1., -1. }}
			, theMinYxy{{ 1.e6, 1.e6, 1.e6 }}
			, theMaxYxy{{ -1., -1., -1. }}
			, theMinLab{{ 1.e6, 1.e6, 1.e6 }}
			, theMaxLab{{ -1., -1., -1. }}
			, theMinSRGB{{ 1.e6, 1.e6, 1.e6 }}
			, theMaxSRGB{{ -1., -1., -1. }}
		{
		}

		//! Incorporate round trip results into stats
		void
		add
			( img::UPix8 const & lrgb
			)
		{
			// forward conversions
			img::UPix8 const expLRGB(lrgb);

			//
			// XYZ
			//

			img::FPix const gotXYZ(img::color::toXYZfromLRGB8(expLRGB));

			// track minimum/max values
			theMinXYZ[0] = std::min(theMinXYZ[0], (double)gotXYZ[0]);
			theMinXYZ[1] = std::min(theMinXYZ[1], (double)gotXYZ[1]);
			theMinXYZ[2] = std::min(theMinXYZ[2], (double)gotXYZ[2]);

			theMaxXYZ[0] = std::max(theMaxXYZ[0], (double)gotXYZ[0]);
			theMaxXYZ[1] = std::max(theMaxXYZ[1], (double)gotXYZ[1]);
			theMaxXYZ[2] = std::max(theMaxXYZ[2], (double)gotXYZ[2]);

			// enforce basic requirements
			if (sEnforceLimits)
			{
				assert(0.f <= (float)gotXYZ[0]);
				assert((float)gotXYZ[0] <= sFuzzyOne);
				assert(0.f <= (float)gotXYZ[1]);
				assert((float)gotXYZ[1] <= sFuzzyOne);
				assert(0.f <= (float)gotXYZ[2]);
				assert((float)gotXYZ[2] <= sFuzzyOne);
			}

			//
			// Yxy
			//

			{
				img::FPix const gotYxy(img::color::toYxyFromXYZ(gotXYZ));

				// track minimum/max values
				theMinYxy[0] = std::min(theMinYxy[0], (double)gotYxy[0]);
				theMinYxy[1] = std::min(theMinYxy[1], (double)gotYxy[1]);
				theMinYxy[2] = std::min(theMinYxy[2], (double)gotYxy[2]);

				theMaxYxy[0] = std::max(theMaxYxy[0], (double)gotYxy[0]);
				theMaxYxy[1] = std::max(theMaxYxy[1], (double)gotYxy[1]);
				theMaxYxy[2] = std::max(theMaxYxy[2], (double)gotYxy[2]);

				// enforce basic requirements
				if (sEnforceLimits)
				{
					assert(0.f <= (float)gotYxy[0]);
					assert((float)gotYxy[0] <= sFuzzyOne);
					assert(0.f <= (float)gotYxy[1]);
					assert((float)gotYxy[1] <= .74f);
					assert(0.f <= (float)gotYxy[2]);
					assert((float)gotYxy[2] <= .72f);
				}

				// inverse conversions
				bool okay(false);
				img::UPix8 const gotLRGB
					(img::color::toLRGB8fromYxy(gotYxy, & okay));

				// check differences
				double const dr((double)gotLRGB[0] - (double)expLRGB[0]);
				double const dg((double)gotLRGB[1] - (double)expLRGB[1]);
				double const db((double)gotLRGB[2] - (double)expLRGB[2]);

				// track roundtrip differences
				theMaxErrYxy[0] = std::max(theMaxErrYxy[0], std::abs(dr));
				theMaxErrYxy[1] = std::max(theMaxErrYxy[1], std::abs(dg));
				theMaxErrYxy[2] = std::max(theMaxErrYxy[2], std::abs(db));
			}

			//
			// Lab
			//

			{
				img::FPix const gotLab(img::color::toLabFromXYZ(gotXYZ));

				// track minimum/max values
				theMinLab[0] = std::min(theMinLab[0], (double)gotLab[0]);
				theMinLab[1] = std::min(theMinLab[1], (double)gotLab[1]);
				theMinLab[2] = std::min(theMinLab[2], (double)gotLab[2]);

				theMaxLab[0] = std::max(theMaxLab[0], (double)gotLab[0]);
				theMaxLab[1] = std::max(theMaxLab[1], (double)gotLab[1]);
				theMaxLab[2] = std::max(theMaxLab[2], (double)gotLab[2]);

				if (sEnforceLimits)
				{
					assert(0.f <= (float)gotLab[0]);
					assert((float)gotLab[0] <= sFuzzyOne);
					assert(-1.5f <= (float)gotLab[1]);
					assert((float)gotLab[1] <= 2.0f);
					assert(-1.5f <= (float)gotLab[2]);
					assert((float)gotLab[2] <= 1.9f);
				}

				// inverse conversions
				bool okay(false);
				img::UPix8 const gotLRGB
					(img::color::toLRGB8fromLab(gotLab, & okay));
				img::FPix const rtnXYZ
					(img::color::toXYZfromLab(gotLab));
				bool ok2(false);
				img::UPix8 const rtnLRGB
					(img::color::toLRGB8fromXYZ(rtnXYZ, & ok2));

				// check differences
				double const dr((double)gotLRGB[0] - (double)expLRGB[0]);
				double const dg((double)gotLRGB[1] - (double)expLRGB[1]);
				double const db((double)gotLRGB[2] - (double)expLRGB[2]);

				// track roundtrip differences
				theMaxErrLab[0] = std::max(theMaxErrLab[0], std::abs(dr));
				theMaxErrLab[1] = std::max(theMaxErrLab[1], std::abs(dg));
				theMaxErrLab[2] = std::max(theMaxErrLab[2], std::abs(db));

			}

			//
			// sRGB
			//

			{
				bool okay(false);
				img::UPix8 const gotSRGB
					(img::color::toSRGB8fromXYZ(gotXYZ, &okay));
				assert(okay);

				// track minimum/max values
				theMinSRGB[0] = std::min(theMinSRGB[0], (double)gotSRGB[0]);
				theMinSRGB[1] = std::min(theMinSRGB[1], (double)gotSRGB[1]);
				theMinSRGB[2] = std::min(theMinSRGB[2], (double)gotSRGB[2]);

				theMaxSRGB[0] = std::max(theMaxSRGB[0], (double)gotSRGB[0]);
				theMaxSRGB[1] = std::max(theMaxSRGB[1], (double)gotSRGB[1]);
				theMaxSRGB[2] = std::max(theMaxSRGB[2], (double)gotSRGB[2]);

				/* -- pointless since everything is in 8-bit range
				if (sEnforceLimits)
				{
					assert(0u <= gotSRGB[0]);
					assert(gotSRGB[0] <= 255u);
					assert(0u <= gotSRGB[1]);
					assert(gotSRGB[1] <= 255u);
					assert(0u <= gotSRGB[2]);
					assert(gotSRGB[2] <= 255u);
				}
				*/
			}
		}

		//! Report maximum error
		double
		maxError
			() const
		{
			// check/enforce that proper limits are reached
			double const tolBig(.001);

			//
			// XYZ
			//

			if (sEnforceLimits)
			{
				assert(0. == theMinXYZ[0]);
				assert(0. == theMinXYZ[1]);
				assert(0. == theMinXYZ[2]);
				assert(std::abs(1. - theMaxXYZ[0]) < tolBig);
				assert(std::abs(1. - theMaxXYZ[1]) < tolBig);
				assert(std::abs(1. - theMaxXYZ[2]) < tolBig);
			}

			//
			// Yxy
			//

			if (sEnforceLimits)
			{
				double const minCx(.0f);
				double const minCy(.0f);
				double const maxCx(0.734375); // values from this test code
				double const maxCy(0.717285);
				assert(0. == theMinYxy[0]);
				assert(std::abs(minCx - theMinYxy[1]) < tolBig);
				assert(std::abs(minCy - theMinYxy[2]) < tolBig);
				assert(std::abs(1. - theMaxYxy[0]) < tolBig);
				assert(std::abs(maxCx - theMaxYxy[1]) < tolBig);
				assert(std::abs(maxCy - theMaxYxy[2]) < tolBig);
			}

			//
			// Lab
			//

			if (sEnforceLimits)
			{
				double const minCa(-1.22363); // values from this test code
				double const minCb(-1.49707);
				double const maxCa( 1.87402);
				double const maxCb( 1.57324);
				assert(0. == theMinLab[0]);
				assert(std::abs(minCa - theMinLab[1]) < tolBig);
				assert(std::abs(minCb - theMinLab[2]) < tolBig);
				assert(std::abs(1. - theMaxLab[0]) < tolBig);
				assert(std::abs(maxCa - theMaxLab[1]) < tolBig);
				assert(std::abs(maxCb - theMaxLab[2]) < tolBig);
			}

			//
			// sRGB
			//

			if (sEnforceLimits)
			{
				assert(0u == theMinSRGB[0]);
				assert(0u == theMinSRGB[1]);
				assert(0u == theMinSRGB[2]);
				assert(theMaxSRGB[0] == 255u);
				assert(theMaxSRGB[1] == 255u);
				assert(theMaxSRGB[2] == 255u);
			}

			//
			// Output debug info
			//

			if (sDebug)
			{
				io::out() << std::endl;
				io::out() << "theMinXYZ[0]: " << theMinXYZ[0] << std::endl;
				io::out() << "theMinXYZ[1]: " << theMinXYZ[1] << std::endl;
				io::out() << "theMinXYZ[2]: " << theMinXYZ[2] << std::endl;
				io::out() << "theMaxXYZ[0]: " << theMaxXYZ[0] << std::endl;
				io::out() << "theMaxXYZ[1]: " << theMaxXYZ[1] << std::endl;
				io::out() << "theMaxXYZ[2]: " << theMaxXYZ[2] << std::endl;

				io::out() << std::endl;
				io::out() << "theMinYxy[0]: " << theMinYxy[0] << std::endl;
				io::out() << "theMinYxy[1]: " << theMinYxy[1] << std::endl;
				io::out() << "theMinYxy[2]: " << theMinYxy[2] << std::endl;
				io::out() << "theMaxYxy[0]: " << theMaxYxy[0] << std::endl;
				io::out() << "theMaxYxy[1]: " << theMaxYxy[1] << std::endl;
				io::out() << "theMaxYxy[2]: " << theMaxYxy[2] << std::endl;

				io::out() << std::endl;
				io::out() << "theMinLab[0]: " << theMinLab[0] << std::endl;
				io::out() << "theMinLab[1]: " << theMinLab[1] << std::endl;
				io::out() << "theMinLab[2]: " << theMinLab[2] << std::endl;
				io::out() << "theMaxLab[0]: " << theMaxLab[0] << std::endl;
				io::out() << "theMaxLab[1]: " << theMaxLab[1] << std::endl;
				io::out() << "theMaxLab[2]: " << theMaxLab[2] << std::endl;

				io::out() << std::endl;
				io::out() << "theMinSRGB[0]: " << theMinSRGB[0] << std::endl;
				io::out() << "theMinSRGB[1]: " << theMinSRGB[1] << std::endl;
				io::out() << "theMinSRGB[2]: " << theMinSRGB[2] << std::endl;
				io::out() << "theMaxSRGB[0]: " << theMaxSRGB[0] << std::endl;
				io::out() << "theMaxSRGB[1]: " << theMaxSRGB[1] << std::endl;
				io::out() << "theMaxSRGB[2]: " << theMaxSRGB[2] << std::endl;
			}

			// compute error functions
			double const sumSqYxy
				( std::inner_product
					( theMaxErrYxy.begin(), theMaxErrYxy.end()
					, theMaxErrYxy.begin()
					, double(0.)
					)
				);
			double const sumSqLab
				( std::inner_product
					( theMaxErrLab.begin(), theMaxErrLab.end()
					, theMaxErrLab.begin()
					, double(0.)
					)
				);

			// return worse case error
			double const sumSq(std::max(sumSqYxy, sumSqLab));
			return std::sqrt(sumSq);
		}

	};

	//! Generate 8-bit 'delta' values more dense near ends and middle
	std::vector<uint8_t>
	deltaValues
		()
	{
		std::vector<uint8_t> dvals;
		dvals.reserve(256u);

		// establish max spacing (controls number of samples)
		double const mag(8. - 1./128.); // corresponds with step size
		// set period for max density at ends and middle of range [0,255]
		double const freq(math::twoPi / 128.);
		for (size_t nn(0u) ; nn < 256u ; ++nn)
		{
			// create oscillating function
			double const xx((double)nn);
			double const cxx(std::cos(xx*freq));
			double const func(.5*(1. - cxx));
			// convert to spacing increments
			double const delta(std::abs(mag * func));
			size_t const inc((size_t)std::floor(delta) + 1u);
			// record increments for return
			dvals.push_back(inc);
		}

		return dvals;
	}

	//! Generate collection of 8-bit values - heavy on ends and middles
	std::vector<uint8_t>
	intensityValues
		()
	{
		std::vector<uint8_t> ivals;
		ivals.reserve(256u);

		// compute increments between samples to be generated
		std::vector<uint8_t> const dvals(deltaValues());
		assert(255u < dvals.size());

		// generate samples using established 'delta' increments
		ivals.push_back(0u);
		for (size_t nn(0u) ; nn < 255u ; ++nn)
		{
			size_t const ndx((size_t)ivals.back());
			if (ndx < nn)
			{
				size_t const delta((size_t)(dvals[ndx]));
				ivals.push_back(ndx + delta);
			}
		}
		ivals.push_back(255u);

		/*
		std::ofstream ofs("inten.dat");
		for (uint8_t const & ival : ivals)
		{
		ofs
			<< "ival"
			<< " " << std::setw(4) << (int)ival
			<< std::endl;
		}
		*/

		return ivals;
	}

	//! Generate a sampling of pixel values
	std::vector<img::UPix8>
	lrgbSamples
		()
	{
		std::vector<img::UPix8> rgbs;

		// get sampling pattern
		std::vector<uint8_t> const ivals(intensityValues());

		// reserve space (for number of samples)
		size_t const numEach(ivals.size());
		size_t const numCombos(math::cube(numEach));
		rgbs.reserve(numCombos);

		for (uint8_t const & red : ivals)
		{
			for (uint8_t const & grn : ivals)
			{
				for (uint8_t const & blu : ivals)
				{
					img::UPix8 const rgb
						((uint8_t)red, (uint8_t)grn, (uint8_t)blu);
					rgbs.push_back(rgb);
				}
			}
		}
		// verify that reserve size is correct
		assert(numCombos == rgbs.size());

		return rgbs;
	}

//! Check round trip color error
std::string
img_color_test1
	()
{
	std::ostringstream oss;

	// generate a spectrum of linear RGB values
	std::vector<img::UPix8> const lrgbs(lrgbSamples());

	// compute statistics
	PixStats stats;
	for (img::UPix8 const & lrgb : lrgbs)
	{
		stats.add(lrgb);
	}

	// check that max errors are within (reasonable) limits
	double const err(stats.maxError());
	if (3. < err)
	{
		oss << "Failure of max round trip error test" << std::endl;
		oss << "maxError: " << err << std::endl;
	}

	// save information to debug file
	if (sDebug)
	{
		std::ofstream ofs("labs.dat");
		for (img::UPix8 const & lrgb : lrgbs)
		{
			img::FPix const lab(img::color::toLabFromLRGB8(lrgb));
			ofs << "lrgb,Lab:"
				<< " " << lrgb.infoString()
				<< " " << lab.infoString()
				<< '\n';
		}
		ofs << std::endl;
		ofs.close();
	}

	return oss.str();
}

//! Check L*a*b* forward and inverse function components
std::string
img_color_test2
	()
{
	std::ostringstream oss;

	std::ostringstream errInfo;
	float const darg(1.f/512.f);
	for (float arg(0.f) ; arg < 1.25f ; arg += darg)
	{
		float const & expVal = arg;
		float const gotVal(img::color::fwdLab(expVal));
		float const rtnVal(img::color::invLab(gotVal));
		float const absDif(std::abs(rtnVal - expVal));
		float const tol(4.f * std::numeric_limits<img::fpix_t>::epsilon());
		if (! (absDif < tol))
		{
			errInfo << "exp,got,rtn,adiff:"
				<< std::fixed
				<< "  " << expVal
				<< "  " << gotVal
				<< "  " << rtnVal
				<< "  " << std::setprecision(15) << absDif
				<< "  " << tol
				<< std::endl;
		}
	}
	if (! errInfo.str().empty())
	{
		oss << "Failure of forward/inverse L*a*b* function test\n"
			<< errInfo.str()
			<< std::endl;
	}


	return oss.str();
}

//! Report timing
std::string
img_color_test3
	()
{
	std::ostringstream oss;

	// generate a spectrum of linear RGB values
	std::vector<img::UPix8> const pRgbIns(lrgbSamples());
	std::vector<img::FPix> pLabs
		(pRgbIns.size(), img::FPix(0.f, 0.f, 0.f));
	std::vector<img::UPix8> pRgbOuts
		(pLabs.size(), img::UPix8(0u, 0u, 0u));

	// time forward transform from L-RGB to Lab
	double const startTime(sys::time::relativeNow());
	std::vector<img::UPix8>::const_iterator itRgbIn(pRgbIns.begin());
	std::vector<img::FPix>::iterator itLabOut(pLabs.begin());
	while (pRgbIns.end() != itRgbIn)
	{
		*itLabOut++ = img::color::toLabFromLRGB8(*itRgbIn++);
	}
	double const afterLabTime(sys::time::relativeNow() - startTime);

	// time inverse transform back from Lab to sRGB
	std::vector<img::FPix>::const_iterator itLabIn(pLabs.begin());
	std::vector<img::UPix8>::iterator itRgbOut(pRgbOuts.begin());
	while (pLabs.end() != itLabIn)
	{
		*itRgbOut++ = img::color::toSRGB8fromLab(*itLabIn++, nullptr);
	}
	double const afterRgbTime(sys::time::relativeNow() - afterLabTime);

	// report results
	double const numPix((double)pRgbIns.size());
	io::out() << std::endl;
	io::out()
		<< std::fixed << std::setprecision(9)
		<< " afterLabTime: " << afterLabTime << '\n'
		<< " afterRgbTime: " << afterRgbTime << '\n'
		<< "afterLabTime@: " << afterLabTime/numPix << '\n'
		<< "afterRgbTime@: " << afterRgbTime/numPix << '\n'
		;
	io::out() << std::endl;

	return oss.str();
}


//! Check fast luminance transform
std::string
img_color_test4
	()
{
	std::ostringstream oss;

	// generate a spectrum of linear RGB values
	std::vector<img::UPix8> const pRgbs(lrgbSamples());

	// test each luminance value against full transform
	float maxAbsFracDiff(0.);
	for (img::UPix8 const & pRgb : pRgbs)
	{
		img::FPix const pLab(img::color::toLabFromLRGB8(pRgb));
		float const expL((float)pLab[0]);
		float const gotL(img::color::toLfromLRGB8(pRgb));
		static float const tol
			((float)std::numeric_limits<img::fpix_t>::epsilon());
		float const absDiff(std::abs(gotL - expL));
		if (tol < expL)
		{
			maxAbsFracDiff = std::max(maxAbsFracDiff, absDiff/expL);
		}
		if (tol < absDiff)
		{
			oss
				<< "Failure of luminance only test\n"
				<< "expL: " << expL << '\n'
				<< "gotL: " << gotL << '\n'
				;
			break;
		}
	}

	// report timing for luminance only transform
	if (sDoTiming)
	{
		std::vector<img::UPix8>::const_iterator itRgb(pRgbs.begin());
		float sumL(0.f);
		double const startTime(sys::time::relativeNow());
		while (pRgbs.end() != itRgb)
		{
			sumL += img::color::toLfromLRGB8(*itRgb++);
		}
		double const xformTime(sys::time::relativeNow() - startTime);
		double const numPix((double)pRgbs.size());
		io::out() << "maxAbsFracDiff: " << maxAbsFracDiff << std::endl;
		io::out() << " toLabXformTime: " << xformTime << std::endl;
		io::out() << "toLabXformTime@: " << xformTime/numPix << std::endl;
		// force output in case of optimization
		io::out() << "sumL: " << sumL << std::endl;
	}

	return oss.str();
}

}


//! Unit test for img::color
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << img_color_test1();
	oss << img_color_test2();
	if (sDoTiming)
	{
		oss << img_color_test3();
	}
	oss << img_color_test4();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
