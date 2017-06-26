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

#ifndef img_convert_INCL_
#define img_convert_INCL_

/*! \file
\brief Declarations for img::convert
*/


#include "libdat/grid.h"
#include "libdat/MinMax.h"
#include "libdat/SubExtents.h"
#include "libimg/img.h"

#include "opencv.hpp"

#include <array>


namespace img
{
//! \brief Functions and operations for converting imagery.
namespace convert
{
	//! Convert Raw8 Bayer format to LRGB B-bit color format
	cv::Mat
	cvRgb8FromCvRaw8
		( cv::Mat const & raw8
		);

	//! OpenCV matrix from single color channel: TODO - could templatize
	cv::Mat
	cvMatFromGrid
		( dat::grid<uint8_t> const & ugrid
		);

	//! OpenCV matrix from three separate channels: TODO - could templatize
	cv::Mat
	cvMatFromBands
		( std::array<dat::grid<uint8_t>, 3u> const & bands
		);

	//! OPenCV matrix from compound pixels: TODO - could templatize
	cv::Mat
	cvMatFromComposite
		( dat::grid<std::array<uint8_t, 3u> > const & deepGrid
		);

	//! Inverse of cvMatFromBands()
	std::array<dat::grid<uint8_t>, 3u>
	bandsFromCvMat
		( cv::Mat const & mat
		);

	/*! Convert linear-RGB (CIE version) to standard-RGB (default '90s-y2k+).
	 *
	 * NOTE: The input data are assumed to be w.r.t. an arbitrary scale.
	 *       However, the SRGB/LRGB transform assumes (for function
	 *       boundaries and exp/log coefficients) specific input data
	 *       values which are based on the data range sMinMax.
	 *
	 *       The data scaling is accomplished by scaling lMinMax into
	 *       the range sMinMax, then performing the SRGB conversion
	 *       *WITH FLOATING POINT PRECISION*. Thus, the data values
	 *       returned will, in general, be a subset of the range sMinMax.
	 *
	 * Return:
	 *       Data values are scaled inside range sMinMax such that
	 *       a (hypothetical) data value of lMinMax.first maps to 
	 *       sMinMax.first and lMinMax.second maps to sMinMax.second.
	 *
	 */
	std::array<dat::grid<float>, 3u>
	sRgbFromLRgb
		( std::array<dat::grid<float>, 3u> const & lrgbs
		, dat::MinMax<float> const & lMinMax
			= dat::MinMax<float>() //!< If null, compute lrgbs min/max
		, dat::MinMax<float> const & sMinMax
			= dat::MinMax<float>
			((float)u8pixMinValid, (float)u8pixMaxValid)
		);

	//! Convert deep-grid to standard rgb space
	dat::grid<std::array<float, 3u> >
	sRgbFromLRgb
		( dat::grid<std::array<float, 3u> > const & lrgbDeep
		, dat::MinMax<float> const & lMinMax
			= dat::MinMax<float>() //!< If null, compute lrgbs min/max
		, dat::MinMax<float> const & sMinMax
			= dat::MinMax<float>
			((float)u8pixMinValid, (float)u8pixMaxValid)
		);

	//! Subgrid of intensity values extracted from fullGrid
	dat::grid<fpix_t>
	grayGridFrom
		( dat::grid<fpix_t> const & fullGrid
		, dat::SubExtents const & crop
		, std::array<fpix_t, 3u> const & rgbGains
		);

	//! Tone-map gray-scale floating point image into 8-bit
	dat::grid<uint8_t>
	downMappedLinear
		( dat::grid<float> const & inGrid
		, dat::MinMax<float> const & inMinMax = dat::MinMax<float>()
		, uint8_t const & useForNull = 0u
		);

	//! Tone-map deep color floating point image into 8-bit
	dat::grid<std::array<uint8_t, 3u> >
	downMappedLinear
		( dat::grid<std::array<float, 3u> > const & inGrid
		, dat::MinMax<float> const & inMinMax = dat::MinMax<float>()
		, std::array<uint8_t, 3u> const useForNull = { 0u, 0u, 0u }
		);

	//! Tone-map multi-band color floating point image into 8-bit
	std::array<dat::grid<uint8_t>, 3u>
	downMappedLinear
		( std::array<dat::grid<float>, 3u> const & inGrids
		, dat::MinMax<float> const & inMinMax = dat::MinMax<float>()
		, uint8_t const & useForNull = 0u
		);

	//! Convert to 8-bit with simple static cast
	template <typename OutChanType, typename InChanType>
	dat::grid<std::array<OutChanType, 3u> >
	downCast
		( dat::grid<std::array<InChanType, 3u> > const & inGrid
		);

	//! Multiplexed (RGB) data from multi-channel (R),(G),(B) data
	template <typename PixType>
	inline
	dat::grid<std::array<PixType, 3u> >
	multiplexed
		( std::array<dat::grid<PixType>, 3u> const & chans
		);

}

}

// Inline definitions
#include "libimg/convert.inl"

#endif // img_convert_INCL_

