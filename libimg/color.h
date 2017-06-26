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

#ifndef img_color_INCL_
#define img_color_INCL_

/*! \file
\brief Declarations for img::color
*/

#include "libimg/pixel.h"

namespace img
{

/*! \brief Image color space operations.
 *
 * Ranges for color space values are:
 *
 * RGB8:
 *    red: [0, 255]
 *    grn: [0, 255]
 *    blu: [0, 255]
 *
 * Note: The upper limits below "not-to-exceed" values.
 *       Actual max observed values are generally a bit less than this
 *       because of quantization effects when UPix values are involved.
 *
 * XYZ -- Note "fuzzy" upper limits
 *    X: [0.f, 1.f]
 *    Y: [0.f, 1.f]
 *    Z: [0.f, 1.f]
 *
 * Yxy -- Note "fuzzy" upper limits
 *    Y: [0.f, 1.00f]
 *    x: [0.f,  .74f]
 *    y: [0.f,  .72f]
 *
 * Lab -- Note "fuzzy" upper limits
 *    L: [ 0.0f, 1. f]
 *    a: [-1.5f, 2. f]
 *    b: [-1.5f, 1.9f]
 *
 *

\par Example
\dontinclude testimg/ucolor.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace color
{
	//! Color space identifications
	enum Space
	{
		  Unknown
		, RgbLinear
		, RgbStandard
	};

	//! String associated with color space
	inline
	std::string
	string
		( Space const & space
		);

	//! Color space recovered from a string
	inline
	Space
	spaceFrom
		( std::string const & colorstr
		);

	//
	// toXYZ: CIE XYZ space tristimulus values (approximately)
	//        This nominally quantifies the response of human cone receptors.
	//

	/*! Almost: CIE 1931 XYZ color from uint8 RGB.
	 *
	 * This is a scaled version of the CIE space. For the
	 * CIE standard, a max RGB 'vector' of (255, 255, 255)
	 * scales to a value of (255/.17697).
	 *
	 * For general mathematical convenience, this color space is
	 * normalized, such that (255, 255, 255) maps to (1., 1., 1.).
	 * Input value of zero maps to zero in all cases.
	 *
	 * https://en.wikipedia.org/wiki/CIE_1931_color_space
	 */
	inline
	FPix
	toXYZfromLRGB8
		( UPix8 const & uLRGB8
		);

	//! CIE XYZ from CIE Inten/Chrom
	inline
	FPix
	toXYZfromYxy
		( FPix const & fYxy
		);

	//! CIE XYZ from CIE orthogonal L*a*b*
	inline
	FPix
	toXYZfromLab
		( FPix const & fLab
		);

	//
	// toYxy: CIE Yxy projective color space values
	//        A 3D (non-orthogonal) vector space representation of XYZ.
	//

	//! CIE Inten/Chrom Yxy color from CIE XYZ
	inline
	FPix
	toYxyFromXYZ
		( FPix const & fXYZ
		);

	//! CIE Inten/Chrom Yxy color from 8-bit RGB
	inline
	FPix
	toYxyFromLRGB8
		( UPix8 const & uLRGB8
		);

	//
	// toLab: CIE 1976 color space
	//        This is nominally 'Euclidean' in terms of perceived colors.
	//

	/*! CIE Orthogonal L*a*b* color from CIE XYZ
	 *
	 * Note: the function name includes "Lab", but the color space
	 *       is actually CIE "L*a*b*" (not Hunters "Lab").
	 *
	 * https://en.wikipedia.org/wiki/Lab_color_space
	 *
	 */
	inline
	FPix
	toLabFromXYZ
		( FPix const & fXYZ
		);

	//! CIE orthogonal L*a*b* color from 8-bit RGB
	inline
	FPix
	toLabFromLRGB8
		( UPix8 const & uLRGB8
		);

	//! SPECIALIZATION: compute only luminance (matching Lab) (faster)
	inline
	float
	toLfromLRGB8
		( UPix8 const & uLRGB8
		);

	//
	// toLRGB8: *LINEAR* CIE RGB
	//          A representation of color via additive components.
	//

	/*! uint8 linear RGB from CIE 131 XYZ color space
	 *
	 * Note: Assume input data are normalized to Ymax = 1.
	 *
	 */
	inline
	UPix8
	toLRGB8fromXYZ
		( FPix const & fXYZ
		, bool * const & ptValid // = nullptr
		);

	//! 8-bit RGB from Yxy
	inline
	UPix8
	toLRGB8fromYxy
		( FPix const & fYxy
		, bool * const & ptValid // = nullptr
		);

	//! 8-bit RGB from L*a*b*
	inline
	UPix8
	toLRGB8fromLab
		( FPix const & fLab
		, bool * const & ptValid // = nullptr
		);

	//
	// toSRGB8: Standard (8-bit) RGB for (CRT) computer monitors
	//          In practice, this is default assumed by most applications.
	//          https://en.wikipedia.org/wiki/SRGB
	//           #The_forward_transformation_.28CIE_xyY_or_CIE_XYZ_to_sRGB.29
	//

	//! 8-bit sRGB color space value from CIE XYZ
	inline
	UPix8
	toSRGB8fromXYZ
		( FPix const & fXYZ
		, bool * const & ptValid // = nullptr
		);

	//! 8-bit sRGB color space value from CIE (linear) RGB
	inline
	UPix8
	toSRGB8fromLab
		( FPix const & fLab
		, bool * const & ptValid // = nullptr
		);

	//
	// Special cases
	//

	//! sRGB color space value (in float range [0.,256.) from CIE (linear) RGB.
	inline
	std::array<float, 3u>
	toSRGB8fromLRGB
		( double const & lred
		, double const & lgrn
		, double const & lblu
		, std::pair<double, double> const & outRange
			= std::pair<double, double>
			((double)u8pixMinValid, (double)u8pixMaxValid)
		, double const outMaxLimit = 256. // normalized XYZ scaled to this
		);

}

}

// Inline definitions
#include "libimg/color.inl"

#endif // img_color_INCL_

