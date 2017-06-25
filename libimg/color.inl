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
\brief Inline definitions for img::color
*/


#include "libdat/dat.h"
#include "libimg/pixel.h"
#include "libio/string.h"
#include "libmath/Partition.h"

#include <cassert>
#include <cmath>


namespace img
{
namespace color
{
//======================================================================

	// LAB transformation constants
	// White balance point associated with Illuminant D65
	// https://en.wikipedia.org/wiki/Illuminant_D65
	static float const sLabXn( .95047f);
	static float const sLabYn(1.00000f);
	static float const sLabZn(1.08883f);

	//! Non-linear forward Lab conversion function
	inline
	float
	fwdLab
		( float const & arg
		)
	{
		float value;
		assert(0.f <= arg);
		static float const n6d29(6.f / 29.f);
		static float const n6d29Cb(n6d29 * n6d29 * n6d29);
		if (n6d29Cb < arg)
		{
			value = std::cbrt(arg);
		}
		else
		{
			static float const n29d6(29.f / 6.f);
			static float const n29d6Sq(n29d6 * n29d6);
			static float const slope(n29d6Sq / 3.f);
			value = slope*arg + (4./29.);
		}
		return value;
	}

	//! Non-linear inverse Lab conversion function
	inline
	float
	invLab
		( float const & arg
		)
	{
		float value;
		static float const delta(6.f / 29.f);
		if (delta < arg)
		{
			value = arg * arg * arg;
		}
		else
		{
			static float const scale(3.f * delta * delta);
			value = scale * (arg - 4./29.);
		}
		return value;
	}

	//! Standard component from linear component of SRGB
	inline
	double
	fwdSRGB
		( double const & clin //!< linear component of sRGB
		)
	{
		double cstd(dat::badFlt);
		static double const thresh(.0031308);
		if (thresh < clin)
		{
			// exponential portion
			static double const aco1(.055);
			static double const aco2(1. + aco1);
			static double const eco(1./2.4);
			cstd = (aco2 * std::pow(clin, eco) - aco1);
		}
		else
		{
			// linear portion
			cstd = 12.92 * clin;
		}
		return cstd;
	}

	//! Cache of fwdSRGB values
	constexpr size_t sLutSize{4u*1024u};
	struct CachedFwdSRGB
	{
		std::array<double, sLutSize> theValues;
		math::Partition theLinPart;

		//! Construct the LUT
		CachedFwdSRGB
			()
		{
			// configure LUT mapping
			static double const eps(1. / (1024.*1024.));
			static dat::Range<double> const lumRange
				{ -0.275559 - eps, 1.454030 + eps };
			theLinPart = math::Partition(lumRange, sLutSize);

			// fill LUT
			for (size_t nn(0u) ; nn < sLutSize ; ++nn)
			{
				double const fndx(theLinPart.interpValueFor(double(nn)));
				theValues[nn] = fwdSRGB(fndx);
			}
		}

		//! Return lut values
		inline
		double
		operator()
			( double const & linLum
			) const
		{
			double stdLum(0.);

			double const fndx(theLinPart.interpIndexFor(linLum));
			size_t const ndx((size_t)std::floor(fndx + .5));
			// clin is know (well should be) in range [0, 1.]
			if (ndx < sLutSize)
			{
				stdLum = theValues[ndx];
			}
			else
			{
				stdLum = theValues[sLutSize - 1u];
			}

			return stdLum;
		}
	};

static CachedFwdSRGB sFwdSRGB;

inline
std::string
string
	( Space const & space
	)
{
	std::string str;

	switch (space)
	{
		default:
		case Unknown:
			str = std::string("Unknown");
			break;
		case RgbLinear:
			str = std::string("RgbLinear");
			break;
		case RgbStandard:
			str = std::string("RgbStandard");
			break;
	}

	return str;
}

inline
img::color::Space
spaceFrom
	( std::string const & colorstr
	)
{
	Space space(Unknown);

	using io::string::upper;
	if (upper(string(RgbLinear)) == upper(colorstr))
	{
		space = RgbLinear;
	}
	else
	if (upper(string(RgbStandard)) == upper(colorstr))
	{
		space = RgbStandard;
	}

	return space;
}


//
// toXYZ
//

inline
img::FPix
toXYZfromLRGB8
	( UPix8 const & uLRGB8
	)
{
	FPix fXYZ;

	// cast to standard types (for speed and intermediate precision)
	double const red((double)uLRGB8[0]);
	double const grn((double)uLRGB8[1]);
	double const blu((double)uLRGB8[2]);

	// normalizing values for 8-bit RGB
	fXYZ[0] = (fpix_t)(0.001921579*red + 0.001215686*grn + 0.000784314*blu);
	fXYZ[1] = (fpix_t)(0.000694000*red + 0.003185882*grn + 0.000041686*blu);
	fXYZ[2] = (fpix_t)(0.000000000*red + 0.000039216*grn + 0.003882353*blu);

	// standard values
//	fXYZ[0] = (fpix_t)(2.76883*red + 1.75171*grn + 1.13014*blu);
//	fXYZ[1] = (fpix_t)(1.00000*red + 4.59061*grn + 0.06007*blu);
//	fXYZ[2] = (fpix_t)(0.00000*red + 0.05651*grn + 5.59417*blu);

	return fXYZ;
}

inline
img::FPix
toXYZfromYxy
	( FPix const & fYxy
	)
{
	FPix fXYZ;

	// formula is:
	//   Y = fYxy[0]
	//   X = Y * (smlx / smly)
	//   Z = Y * (smlz / smly) = Y * ((1 - smlx - smly) / smly)

	float const iY((float)fYxy[0]);
	if (0. < iY) // if true, then cy is also not zero (for valid RGB gamut)
	{
		float const cx((float)fYxy[1]);
		float const cy((float)fYxy[2]);
		if (std::numeric_limits<fpix_t>::epsilon() < cy)
		{
			float const cz(1.f - cx - cy);
			float const scale(iY / cy);
			fXYZ[0] = (fpix_t)(scale * cx);
			fXYZ[1] = fYxy[0];
			fXYZ[2] = (fpix_t)(scale * cz);
		}
	}
	else
	{
		fXYZ = FPix(0.f, 0.f, 0.f);
	}
	return fXYZ;
}

inline
img::FPix
toXYZfromLab
	( FPix const & fLab
	)
{
	FPix fXYZ;
	if (fLab.isValid())
	{
		float const LL(fLab[0]);
		float const aa(fLab[1]);
		float const bb(fLab[2]);
		float const arg1((1.f/1.16f) * (LL + .16f));
		fXYZ[0] = sLabXn * invLab(arg1 + (1.f/5.00f)*aa);
		fXYZ[1] = sLabYn * invLab(arg1   );
		fXYZ[2] = sLabZn * invLab(arg1 - (1.f/2.00)*bb);
	}
	return fXYZ;
}

//
// toYxy
//

inline
img::FPix
toYxyFromLRGB8
	( UPix8 const & uLRGB8
	)
{
	return toYxyFromXYZ(toXYZfromLRGB8(uLRGB8));
}

inline
img::FPix
toYxyFromXYZ
	( FPix const & fXYZ
	)
{
	FPix fYxy(0.f, 0.f, 0.f);
	float const sum((float)fXYZ[0] + (float)fXYZ[1] + (float)fXYZ[2]);
	if (0. < sum)
	{
		fpix_t const & fY = fXYZ[1];
		float const fx((float)fXYZ[0] / sum);
		float const fy((float)fXYZ[1] / sum);
		fYxy = FPix(fY, (fpix_t)fx, (fpix_t)fy);
	}
	return fYxy;
}

//
// toLab
//

inline
img::FPix
toLabFromXYZ
	( FPix const & fXYZ
	)
{
	FPix fLab;
	if (fXYZ.isValid())
	{
		float const fXX(fwdLab(fXYZ[0]/sLabXn));
		float const fYY(fwdLab(fXYZ[1]/sLabYn));
		float const fZZ(fwdLab(fXYZ[2]/sLabZn));
		fLab[0] = 1.16f * fYY - .16f;
		fLab[1] = 5.00f * (fXX - fYY);
		fLab[2] = 2.00f * (fYY - fZZ);
	}

	return fLab;
}

inline
img::FPix
toLabFromLRGB8
	( UPix8 const & uLRGB8
	)
{
	return toLabFromXYZ(toXYZfromLRGB8(uLRGB8));
}

inline
float
toLfromLRGB8
	( UPix8 const & uLRGB8
	)
{
	// cast to standard types (for speed and intermediate precision)
	double const red((double)uLRGB8[0]);
	double const grn((double)uLRGB8[1]);
	double const blu((double)uLRGB8[2]);

	// compute only 'Y' component of XYZ
	double const fXYZ_1(0.000694000*red + 0.003185882*grn + 0.000041686*blu);

	// compute only 'L' component of Lab
	double const fYY(fwdLab(fXYZ_1/sLabYn));
	float const fLab_0(1.16 * fYY - .16);

	return fLab_0;
}

//
// toLRGB8
//

inline
img::UPix8
toLRGB8fromXYZ
	( FPix const & fXYZ
	, bool * const & ptValid
	)
{
	UPix8 upix;
	bool okay(false);

	if (fXYZ.isValid())
	{
		// cast to standard types (for speed and intermediate precision)
		// while forcing into valid limits (to address possible round off)
		double const iX(dClamp(0., (double)fXYZ[0], 1.));
		double const iY(dClamp(0., (double)fXYZ[1], 1.));
		double const iZ(dClamp(0., (double)fXYZ[2], 1.));

		// restored values for 8-bit RGB
		double const red( 602.976531*iX - 228.617846*iY - 119.358685*iZ);
		double const grn(-131.367383*iX + 363.734066*iY +  22.633317*iZ);
		double const blu(   1.326943*iX -   3.674081*iY + 257.347138*iZ);

		// for standard input
	//	double const red(  .41847000*iX - .15866000*iY - .08283500*iZ);
	//	double const grn(- .09116900*iX + .25243000*iY + .01570800*iZ);
	//	double const blu(  .00092090*iX - .00254980*iY + .17860000*iZ);

		// ensure output values are in proper valid range
		upix[0] = (uint8_t)img::uClamp(0u, (unsigned int)red, 255u);
		upix[1] = (uint8_t)img::uClamp(0u, (unsigned int)grn, 255u);
		upix[2] = (uint8_t)img::uClamp(0u, (unsigned int)blu, 255u);

		okay = true;
	}

	if (ptValid)
	{
		*ptValid = okay;
	}
	return upix;
}

inline
img::UPix8
toLRGB8fromYxy
	( FPix const & fYxy
	, bool * const & ptValid
	)
{
	UPix8 uLRGB8;
	FPix const fXYZ(toXYZfromYxy(fYxy));
	bool okay(fXYZ.isValid());
	{
		uLRGB8 = toLRGB8fromXYZ(fXYZ, & okay);
	}
	if (ptValid)
	{
		*ptValid = okay;
	}
	return uLRGB8;
}

inline
img::UPix8
toLRGB8fromLab
	( FPix const & fLab
	, bool * const & ptValid
	)
{
	UPix8 uLRGB8;
	FPix const fXYZ(toXYZfromLab(fLab));
	bool okay(fXYZ.isValid());
	if (okay)
	{
		uLRGB8 = toLRGB8fromXYZ(fXYZ, & okay);
	}
	if (ptValid)
	{
		*ptValid = okay;
	}
	return uLRGB8;
}

//
// toSRGB8
//

inline
img::UPix8
toSRGB8fromXYZ
	( FPix const & fXYZ
	, bool * const & ptValid // = nullptr
	)
{
	UPix8 upix;
	bool okay(false);

	if (fXYZ.isValid())
	{
		// cast to standard types (for speed and intermediate precision)
		// while forcing into valid limits (to address possible round off)
		double const iX(dClamp(0., (double)fXYZ[0], 1.));
		double const iY(dClamp(0., (double)fXYZ[1], 1.));
		double const iZ(dClamp(0., (double)fXYZ[2], 1.));

		// temporary linear transform variables
		// Note difference from linear transform parms
		double const redL(+3.240600*iX -1.537200*iY  -0.498600*iZ);
		double const grnL(-0.968900*iX +1.875800*iY  +0.041500*iZ);
		double const bluL(+0.055700*iX -0.204000*iY  +1.057000*iZ);

		double const redS(sFwdSRGB(redL));
		double const grnS(sFwdSRGB(grnL));
		double const bluS(sFwdSRGB(bluL));

		// ensure output values are in proper valid range
		upix[0] = (uint8_t)img::uClamp(0u, (unsigned int)redS, 255u);
		upix[1] = (uint8_t)img::uClamp(0u, (unsigned int)grnS, 255u);
		upix[2] = (uint8_t)img::uClamp(0u, (unsigned int)bluS, 255u);

		okay = true;
	}

	if (ptValid)
	{
		*ptValid = okay;
	}
	return upix;
}

inline
img::UPix8
toSRGB8fromLab
	( FPix const & fLab
	, bool * const & ptValid // = nullptr
	)
{
	return toSRGB8fromXYZ(toXYZfromLab(fLab), ptValid);
}

//
// Special cases
//

inline
std::array<float, 3u>
toSRGB8fromLRGB
	( double const & ulred
	, double const & ulgrn
	, double const & ulblu
	, std::pair<double, double> const & outRange
	, double const outMaxLimit
	)
{
	std::array<float, 3u> srgb{{ fpixBad, fpixBad, fpixBad }};

	if ((isActive(ulred) && isActive(ulgrn) && isActive(ulblu)))
	{
		// combined transformation matrices
 		double const redL
			( 5.160252e-03*ulred -9.773389e-04*ulgrn +5.418270e-04*ulblu);
		double const grnL
			(-5.600127e-04*ulred +4.799827e-03*ulgrn -5.206096e-04*ulblu);
		double const bluL
			(-3.454405e-05*ulred -5.407549e-04*ulgrn +4.138829e-03*ulblu);

		/*
		// convert linear to XYZ
		double xx(0.001921579*ulred + 0.001215686*ulgrn + 0.000784314*ulblu);
		double yy(0.000694000*ulred + 0.003185882*ulgrn + 0.000041686*ulblu);
		double zz(0.000000000*ulred + 0.000039216*ulgrn + 0.003882353*ulblu);

		// convert XYZ to intermediate linear RGB (best fit for sRGB transform)
		double const redL(+3.240600*xx -1.537200*yy  -0.498600*zz);
		double const grnL(-0.968900*xx +1.875800*yy  +0.041500*zz);
		double const bluL(+0.055700*xx -0.204000*yy  +1.057000*zz);
		*/

		double const redS(outMaxLimit * sFwdSRGB(redL));
		double const grnS(outMaxLimit * sFwdSRGB(grnL));
		double const bluS(outMaxLimit * sFwdSRGB(bluL));

		// ensure output values are in proper valid range
		srgb[0] = (float)dClamp(outRange.first, redS, outRange.second);
		srgb[1] = (float)dClamp(outRange.first, grnS, outRange.second);
		srgb[2] = (float)dClamp(outRange.first, bluS, outRange.second);
	}
	else
	if ((isUnder(ulred) || isUnder(ulgrn) || isUnder(ulblu)))
	{
		srgb[0] = fpixUnder;
		srgb[1] = fpixUnder;
		srgb[2] = fpixUnder;
	}
	else
	if ((isOver(ulred) || isOver(ulgrn) || isOver(ulblu)))
	{
		srgb[0] = fpixOver;
		srgb[1] = fpixOver;
		srgb[2] = fpixOver;
	}

	return srgb;
}

//======================================================================
}
}
