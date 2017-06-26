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
\brief Inline definitions for img::convert
*/


namespace img
{
namespace convert
{
//======================================================================

template <typename OutChanType, typename InChanType>
dat::grid<std::array<OutChanType, 3u> >
downCast
	( dat::grid<std::array<InChanType, 3u> > const & inGrid
	)
{
	using InType = std::array<InChanType, 3u>;
	using OutType = std::array<OutChanType, 3u>;
	dat::grid<OutType> ugrid;

	if (inGrid.isValid())
	{
		// allocate output
		ugrid = dat::grid<OutType>(inGrid.hwSize());

		typename dat::grid<InType>::const_iterator itIn(inGrid.begin());
		typename dat::grid<OutType>::iterator itOut(ugrid.begin());
		while (inGrid.end() != itIn)
		{
			InType const & inPix = *itIn;
			OutType const outPix
				{{ static_cast<OutChanType>(inPix[0])
				 , static_cast<OutChanType>(inPix[1])
				 , static_cast<OutChanType>(inPix[2])
				}};
			*itOut++ = outPix;
			++itIn;
		}

	}
	return ugrid;
}

template <typename PixType>
inline
dat::grid<std::array<PixType, 3u> >
multiplexed
	( std::array<dat::grid<PixType>, 3u> const & chans
	)
{
	dat::grid<std::array<PixType, 3u> > pixels;

	typedef std::array<PixType, 3u> rgb_t;

	if (chans[0].isValid() && chans[1].isValid() && chans[2].isValid())
	{
		// check inputs
		dat::Extents const inSize(chans[0].hwSize());
		assert(inSize == chans[1].hwSize());
		assert(inSize == chans[2].hwSize());

		// allocate space
		size_t const high(inSize.high());
		size_t const wide(inSize.wide());
		pixels = dat::grid<rgb_t>(high, wide);

		// process each pixel in turn
		typename dat::grid<PixType>::const_iterator itIn_R(chans[0].begin());
		typename dat::grid<PixType>::const_iterator itIn_G(chans[1].begin());
		typename dat::grid<PixType>::const_iterator itIn_B(chans[2].begin());
		typename dat::grid<rgb_t>::iterator itOut
			(pixels.begin());
		while (chans[0].end() != itIn_R)
		{
			// multiplex this pixel
			rgb_t const pix{{*itIn_R++, *itIn_G++, *itIn_B++}};
			*itOut++ = pix;
		}
	}

	return pixels;
}

//======================================================================
}
}

