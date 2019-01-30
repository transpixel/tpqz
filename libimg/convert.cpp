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
\brief Definitions for img::convert
*/


#include "libimg/convert.h"

#include "libimg/cfa.h"
#include "libimg/color.h"
#include "libimg/geo.h"
#include "libimg/img.h"
#include "libimg/pixel.h"
#include "libimg/stats.h"
#include "libmath/interp.h"

#include <cassert>
#include <limits>
#include <sstream>


namespace img
{
namespace convert
{
namespace
{
	//! MinMax for 8-bit output
	static dat::MinMax<float> const sOutMinMax8
		((float)u8pixMinValid, (float)u8pixMaxValid);

	//! True if span is large enough for meaningful division (and positive)
	bool
	validRange
		( std::pair<float, float> const & range
		)
	{
		float const span(range.second - range.first);
		return (std::numeric_limits<float>::min() < span);
	}

	//! Input output value ranges
	struct OutPerIn
	{
		std::pair<float, float> theIn{ 0.f, 0.f };
		std::pair<float, float> theOut{ 0.f, 0.f };
		float theOutMean{ dat::nullValue<float>() };
		bool theIsValid{ false };

		//! Min/Max from data values
		inline
		static
		dat::MinMax<float>
		gridMinMax
			( dat::grid<float> const & fromGrid
			)
		{
			return img::stats::activeMinMax<float>
				(fromGrid.begin(), fromGrid.end());
		}

		//! Min/Max from data values
		inline
		static
		dat::MinMax<float>
		gridMinMax
			( dat::grid<std::array<float, 3u>> const & fromDeep
			)
		{
			return img::stats::pixelMinMax<float>(fromDeep);
		}

		//! Min/Max from data values
		inline
		static
		dat::MinMax<float>
		gridMinMax
			( std::array<dat::grid<float>, 3u> const & fromChans
			)
		{
			return img::stats::pixelMinMax<float>(fromChans);
		}

		//! input range from arg or (if null arg, then) data
		template <typename GridType>
		static
		dat::MinMax<float>
		fromRange
			( dat::MinMax<float> const & fromMinMax
			, GridType const & fromGrid
			)
		{
			dat::MinMax<float> minmax(fromMinMax);
			if (! minmax.isValid())
			{
				// if no explicit input range, compute one from data
				minmax = gridMinMax(fromGrid);
			}
			return minmax;
		}

		//! Value ctor
		explicit
		OutPerIn
			( dat::MinMax<float> const & fromMinMax
			, dat::MinMax<float> const & intoMinMax
			)
			: theIn{ fromMinMax.pair() }
			, theOut{ intoMinMax.pair() }
			, theOutMean{ .5f * (theOut.second + theOut.first) }
			, theIsValid{ validRange(theIn) }
		{
			// confirm valid ranges
			assert(theIn.first <= theIn.second);
			assert(0.f <= theOut.first);
			assert(theOut.first <= theOut.second);
		}

		//! Construct valid remapping range from inputs and/or data
		template <typename GridType>
		inline
		explicit
		OutPerIn
			( dat::MinMax<float> const & fromMinMax
			, dat::MinMax<float> const & intoMinMax
			, GridType const & grid
			)
			: OutPerIn
				(fromRange(fromMinMax, grid), intoMinMax)
		{ }

		//! True if any of pixel values are less then input min
		inline
		bool
		under
			( float const & inPix
			) const
		{
			return
				(  (inPix < theIn.first)
				|| img::isUnder(inPix)
				);
		}

		//! True if any of pixel values are less then input min
		inline
		bool
		under
			( float const & inPix0
			, float const & inPix1
			, float const & inPix2
			) const
		{
			return (under(inPix0) || under(inPix1) || under(inPix2));
		}

		//! True if any of pixel values is more than input max
		inline
		bool
		over
			( float const & inPix
			) const
		{
			return
				(  (theIn.second < inPix)
				|| img::isOver(inPix)
				);
		}


		//! True if any of pixel values is more than input max
		inline
		bool
		over
			( float const & inPix0
			, float const & inPix1
			, float const & inPix2
			) const
		{
			return (over(inPix0) || over(inPix1) || over(inPix2));
		}

		//! Output value associated with input value
		inline
		double
		operator()
			( float const & fIn
			) const
		{
			double outVal(theOutMean);
			if (theIsValid)
			{
				outVal = math::interp::linear(theIn, (double)fIn, theOut);
			}
			return outVal;
		}

		//! Descriptive information about this instance
		std::string
		infoString
			( std::string const & title = std::string()
			) const
		{
			std::ostringstream oss;
			if (! title.empty())
			{
				oss << title << std::endl;
			}

			oss << dat::infoString(theIn, "theIn");

			oss << std::endl;
			oss << dat::infoString(theOut, "theOut");

			oss << std::endl;
			oss << dat::infoString(theOutMean, "theOutMean");

			oss << std::endl;
			oss << dat::infoString(theIsValid, "theIsValid");

			return oss.str();
		}
	};
}

//======================================================================

dat::grid<std::array<uint8_t, 3> >
gridRgb8From
	( size_t const & high
	, size_t const & wide
	, uint8_t const * const rgb8
	)
{
	dat::grid<std::array<uint8_t, 3> > grid;
	if ((0 < high) && (0 < wide))
	{
		grid = dat::grid<std::array<uint8_t, 3> >(high, wide);

		// copy individual pixels
		uint8_t const * ptIn{ rgb8 };
		dat::grid<std::array<uint8_t, 3> >::iterator itOut{ grid.begin() };
		for (size_t nn{0u} ; nn < grid.size() ; ++nn)
		{
			uint8_t const red{ *ptIn++ };
			uint8_t const grn{ *ptIn++ };
			uint8_t const blu{ *ptIn++ };
			*itOut++ = std::array<uint8_t, 3>{ red, grn, blu };
		}
	}

	return grid;
}

cv::Mat
cvRgb8FromCvRaw8
	( cv::Mat const & raw8
	)
{
	// create color image
	cv::Mat rgb8(raw8.rows, raw8.cols, CV_8UC1);

	// demosaic into it
	cv::cvtColor(raw8, rgb8, CV_BayerBG2BGR);

	// return color image
	return rgb8;
}

namespace
{
	template <typename Type>
	cv::Mat
	cvMatFromGrid
		( dat::grid<Type> const & grid
		, int const & cvTypeCode
		)
	{
		cv::Mat cvm;
		if (grid.isValid())
		{
			// construct opencv header for data
			unsigned char * const ptData((unsigned char * const)grid.begin());
			int const ihigh(grid.high());
			int const iwide(grid.wide());
			cv::Mat const tmp(ihigh, iwide , cvTypeCode , ptData);

			// deep copy data for return
			cvm = tmp.clone();
		}
		return cvm;
	}
}


cv::Mat
cvMatFromGrid
	( dat::grid<uint8_t> const & ugrid
	)
{
	return cvMatFromGrid<uint8_t>(ugrid, CV_8UC1);
}

namespace
{
	//! Swap values between chan0 and chan2 (e.g. red/blu)
	void
	swap02
		( dat::grid<std::array<uint8_t, 3u> > * const & pt012
		)
	{
		for (dat::grid<std::array<uint8_t, 3u> >::iterator
			iter(pt012->begin()) ; pt012->end() != iter ; ++iter)
		{
			std::swap((*iter)[0], (*iter)[2]);
		}
	}

	//! Swap values between chan0 and chan2 (e.g. red/blu)
	void
	swap02_u8
		( cv::Mat * const & ptMat
		)
	{
		for (int row(0u) ; row < ptMat->rows ; ++row)
		{
			for (int col(0u) ; col < ptMat->cols ; ++col)
			{
				std::swap
					( ptMat->at<cv::Vec3b>(row, col)[0]
					, ptMat->at<cv::Vec3b>(row, col)[2]
					);
			}
		}
	}
}

cv::Mat
cvMatFromBands
	( std::array<dat::grid<uint8_t>, 3u> const & bands
	)
{
	// multiplex into an interleaved structure
	dat::grid<std::array<uint8_t, 3u> > chans
		(multiplexed<uint8_t>(bands));

	// swap blue and red pixels
	swap02(&chans);

	return cvMatFromGrid<std::array<uint8_t, 3u> >(chans, CV_8UC3);
}

cv::Mat
cvMatFromComposite
	( dat::grid<std::array<uint8_t, 3u> > const & deepGrid
	)
{
	cv::Mat cvmat(cvMatFromGrid<std::array<uint8_t, 3u> >(deepGrid, CV_8UC3));

	// swap blue and red pixels
	swap02_u8(&cvmat);

	return cvmat;
}

std::array<dat::grid<uint8_t>, 3u>
bandsFromCvMat
	( cv::Mat const & mat
	)
{
	std::array<dat::grid<uint8_t>, 3u> rgb;
	dat::grid<uint8_t> & red = rgb[0];
	dat::grid<uint8_t> & grn = rgb[1];
	dat::grid<uint8_t> & blu = rgb[2];

	// get useful information
	size_t const high(mat.rows);
	size_t const wide(mat.cols);
	size_t const numPix(high * wide);

	// allocate output space
	red = dat::grid<uint8_t>(high, wide);
	grn = dat::grid<uint8_t>(high, wide);
	blu = dat::grid<uint8_t>(high, wide);

	// copy pixels
	uint8_t const * itInPix = mat.data;
	dat::grid<uint8_t>::iterator itRed(red.begin());
	dat::grid<uint8_t>::iterator itGrn(grn.begin());
	dat::grid<uint8_t>::iterator itBlu(blu.begin());
	for (size_t nn(0u) ; nn < numPix ; ++nn)
	{
		// Note OpenCV order (b,g,r)
		*itBlu++ = *itInPix++;
		*itGrn++ = *itInPix++;
		*itRed++ = *itInPix++;
	}

	return rgb;
}

std::array<dat::grid<float>, 3u>
sRgbFromLRgb
	( std::array<dat::grid<float>, 3u> const & lrgbs
	, dat::MinMax<float> const & lMinMax
	, dat::MinMax<float> const & sMinMax
	)
{
	std::array<dat::grid<float>, 3u> srgbs;
	if (lrgbs[0].isValid() && lrgbs[1].isValid() && lrgbs[2].isValid())
	{
		OutPerIn const xform(lMinMax, sMinMax, lrgbs);
		
		// allocate output space
		dat::Extents const inSize(lrgbs[0].hwSize());
		assert(inSize == lrgbs[1].hwSize());
		assert(inSize == lrgbs[2].hwSize());
		srgbs[0] = dat::grid<float>(inSize);
		srgbs[1] = dat::grid<float>(inSize);
		srgbs[2] = dat::grid<float>(inSize);

		// process each pixel in turn
		dat::grid<float>::const_iterator itIn_R(lrgbs[0].begin());
		dat::grid<float>::const_iterator itIn_G(lrgbs[1].begin());
		dat::grid<float>::const_iterator itIn_B(lrgbs[2].begin());
		dat::grid<float>::iterator itOut_R(srgbs[0].begin());
		dat::grid<float>::iterator itOut_G(srgbs[1].begin());
		dat::grid<float>::iterator itOut_B(srgbs[2].begin());

		// process each pixel
		while (lrgbs[0].end() != itIn_R)
		{
			// read input and remap to range which is valid for xform
			double const lred(xform(*itIn_R++));
			double const lgrn(xform(*itIn_G++));
			double const lblu(xform(*itIn_B++));

			// convert from linear to standard RGB
			std::array<float, 3u> const srgb
				(img::color::toSRGB8fromLRGB(lred, lgrn, lblu, xform.theOut));

			// cast and return
			*itOut_R++ = srgb[0];
			*itOut_G++ = srgb[1];
			*itOut_B++ = srgb[2];
		}
	}
	return srgbs;
}

dat::grid<std::array<float, 3u> >
sRgbFromLRgb
	( dat::grid<std::array<float, 3u> > const & lrgbGrid
	, dat::MinMax<float> const & lMinMax
	, dat::MinMax<float> const & sMinMax
	)
{
	dat::grid<std::array<float, 3u> > srgbGrid;
	if (lrgbGrid.isValid())
	{
		OutPerIn const xform(lMinMax, sMinMax, lrgbGrid);
		
		// allocate output space
		dat::Extents const inSize(lrgbGrid.hwSize());
		srgbGrid = dat::grid<std::array<float, 3u> >(inSize);

		// setup input and output iterators
		dat::grid<std::array<float, 3u> >::const_iterator itInRgb
			(lrgbGrid.begin());
		dat::grid<std::array<float, 3u> >::iterator itOutRgb
			(srgbGrid.begin());

		// process each pixel in turn
		while (lrgbGrid.end() != itInRgb)
		{
			// read input and remap to range which is valid for xform
			std::array<float, 3u> const & lrgb = *itInRgb;
			double const lred(xform(lrgb[0]));
			double const lgrn(xform(lrgb[1]));
			double const lblu(xform(lrgb[2]));

			// convert from linear to standard RGB
			std::array<float, 3u> const srgb
				(img::color::toSRGB8fromLRGB(lred, lgrn, lblu, xform.theOut));

			// return output and update iterators
			*itOutRgb++ = srgb;
			++itInRgb;
		}
	}
	return srgbGrid;
}

dat::grid<fpix_t>
grayGridFrom
	( dat::grid<fpix_t> const & fullGrid
	, dat::SubExtents const & crop
	, std::array<fpix_t, 3u> const & rgbGains
	)
{
	dat::grid<fpix_t> cfaGrid
		(img::geo::croppedGrid<fpix_t>(fullGrid, crop));
	constexpr bool setEdgeToNull{ true };
	return img::cfa::grayFastFrom2x2(cfaGrid, rgbGains, setEdgeToNull);
}

dat::grid<uint8_t>
downMappedLinear
	( dat::grid<float> const & inGrid
	, dat::MinMax<float> const & inMinMax
	, uint8_t const & useForNull
	)
{
	dat::grid<uint8_t> outGrid;
	if (inGrid.isValid())
	{
		// input and output data ranges
		OutPerIn const xform(inMinMax, sOutMinMax8, inGrid);

		// allocate output space
		outGrid = dat::grid<uint8_t>(inGrid.hwSize());
		std::fill(outGrid.begin(), outGrid.end(), useForNull);

		uint8_t const & minOut = xform.theOut.first;
		uint8_t const & maxOut = xform.theOut.second;

		// transform input values via linear
		dat::grid<float>::const_iterator itIn(inGrid.begin());
		dat::grid<uint8_t>::iterator itOut(outGrid.begin());

		uint8_t oPix(127u); // always overwritten

		while (inGrid.end() != itIn)
		{
			// get current input pixel value
			float const inPix(*itIn++);

			// select output pixel values based on state of input

			// prefer saturated values vs under
			if (xform.over(inPix))
			{
				// input overflow
				oPix = maxOut;
			}
			else
			if (xform.under(inPix))
			{
				// input underflow
				oPix = minOut;
			}
			else
			if (img::isActive(inPix))
			{
				oPix = (uint8_t)(std::floor(xform(inPix)));
			}
			else
			{
				// use last pixel?
				oPix = useForNull;
			}

			// set output to pixel value
			*itOut++ = oPix;
		}
	}
	return outGrid;
}

dat::grid<std::array<uint8_t, 3u> >
downMappedLinear
	( dat::grid<std::array<float, 3u> > const & inGrid
	, dat::MinMax<float> const & inMinMax
	, std::array<uint8_t, 3u> const useForNull
	)
{
	dat::grid<std::array<uint8_t, 3u> > outGrid;

	if (inGrid.isValid())
	{
		// input and output data ranges
		OutPerIn const xform(inMinMax, sOutMinMax8, inGrid);

		// allocate output space
		outGrid = dat::grid<std::array<uint8_t, 3u> >(inGrid.hwSize());

		// fill with default background value
		std::fill(outGrid.begin(), outGrid.end(), useForNull);

		uint8_t const & minOut = xform.theOut.first;
		uint8_t const & maxOut = xform.theOut.second;

		// transform input values via linear
		dat::grid<std::array<float, 3u> >::const_iterator itIn
			(inGrid.begin());
		dat::grid<std::array<uint8_t, 3u> >::iterator itOut
			(outGrid.begin());

		std::array<uint8_t, 3u> oPix{{}}; // always overwritten
		uint8_t & oPix0 = oPix[0];
		uint8_t & oPix1 = oPix[1];
		uint8_t & oPix2 = oPix[2];
		while (inGrid.end() != itIn)
		{
			// get current input pixel value
			std::array<float, 3u> const inPix(*itIn++);
			float const & inPix0 = inPix[0];
			float const & inPix1 = inPix[1];
			float const & inPix2 = inPix[2];

			// prefer saturated values vs under
			if (xform.over(inPix0, inPix1, inPix2))
			{
				// input overflow
				oPix0 = maxOut;
				oPix1 = maxOut;
				oPix2 = maxOut;
			}
			else
			if (xform.under(inPix0, inPix1, inPix2))
			{
				// input underflow
				oPix0 = minOut;
				oPix1 = minOut;
				oPix2 = minOut;
			}
			else
			if ( img::isActive(inPix0)
			  && img::isActive(inPix1)
			  && img::isActive(inPix2)
			   )
			{
				// valid input
				oPix0 = (uint8_t)(std::floor(xform(inPix0)));
				oPix1 = (uint8_t)(std::floor(xform(inPix1)));
				oPix2 = (uint8_t)(std::floor(xform(inPix2)));
			}
			else
			{
				// use last pixel?
				oPix0 = useForNull[0];
				oPix1 = useForNull[1];
				oPix2 = useForNull[2];
			}


			// set output to pixel value
			*itOut++ = { oPix0, oPix1, oPix2 };
		}
	}
	return outGrid;
}

std::array<dat::grid<uint8_t>, 3u>
downMappedLinear
	( std::array<dat::grid<float>, 3u> const & inGrids
	, dat::MinMax<float> const & inMinMax
	, uint8_t const & useForNull
	)
{
	std::array<dat::grid<uint8_t>, 3u> outGrids;
	if (inGrids[0].isValid() && inGrids[1].isValid() && inGrids[2].isValid())
	{
		// shorthand
		dat::grid<float> const & inGrid0 = inGrids[0];
		dat::grid<float> const & inGrid1 = inGrids[1];
		dat::grid<float> const & inGrid2 = inGrids[2];

		dat::grid<uint8_t> & outGrid0 = outGrids[0];
		dat::grid<uint8_t> & outGrid1 = outGrids[1];
		dat::grid<uint8_t> & outGrid2 = outGrids[2];

		// check sizes
		dat::Extents const inSize(inGrid0.hwSize());
		assert(inSize == inGrid1.hwSize());
		assert(inSize == inGrid2.hwSize());

		// input and output data ranges
		OutPerIn const xform(inMinMax, sOutMinMax8, inGrids);

		// allocate output space
		outGrid0 = dat::grid<uint8_t>(inSize);
		outGrid1 = dat::grid<uint8_t>(inSize);
		outGrid2 = dat::grid<uint8_t>(inSize);
		std::fill(outGrid0.begin(), outGrid0.end(), useForNull);
		std::fill(outGrid1.begin(), outGrid1.end(), useForNull);
		std::fill(outGrid2.begin(), outGrid2.end(), useForNull);

		// output data ranges
		uint8_t const & minOut = xform.theOut.first;
		uint8_t const & maxOut = xform.theOut.second;

		// configure iterator to input and output structures
		dat::grid<float>::const_iterator itIn0(inGrid0.begin());
		dat::grid<float>::const_iterator itIn1(inGrid1.begin());
		dat::grid<float>::const_iterator itIn2(inGrid2.begin());
		dat::grid<uint8_t>::iterator itOut0(outGrid0.begin());
		dat::grid<uint8_t>::iterator itOut1(outGrid1.begin());
		dat::grid<uint8_t>::iterator itOut2(outGrid2.begin());

		// working space
		uint8_t oPix0(127u); // always overwritten
		uint8_t oPix1(127u); // always overwritten
		uint8_t oPix2(127u); // always overwritten

		// transform input values via linear
		while (inGrid0.end() != itIn0)
		{
			// get current input pixel value
			float const inPix0(*itIn0++);
			float const inPix1(*itIn1++);
			float const inPix2(*itIn2++);

			// catch extreme values

			// prefer saturated values vs under
			if (xform.over(inPix0, inPix1, inPix2))
			{
				// input overflow
				oPix0 = maxOut;
				oPix1 = maxOut;
				oPix2 = maxOut;
			}
			else
			if (xform.under(inPix0, inPix1, inPix2))
			{
				// input underflow
				oPix0 = minOut;
				oPix1 = minOut;
				oPix2 = minOut;
			}
			else
			if ( img::isActive(inPix0)
			  && img::isActive(inPix1)
			  && img::isActive(inPix2)
			   )
			{
				// valid input
				oPix0 = (uint8_t)(std::floor(xform(inPix0)));
				oPix1 = (uint8_t)(std::floor(xform(inPix1)));
				oPix2 = (uint8_t)(std::floor(xform(inPix2)));
			}
			else
			{
				// use last pixel?
				oPix0 = useForNull;
				oPix1 = useForNull;
				oPix2 = useForNull;
			}

			// set output to pixel value
			*itOut0++ = oPix0;
			*itOut1++ = oPix1;
			*itOut2++ = oPix2;
		}
	}
	return outGrids;
}

//======================================================================
}
}

