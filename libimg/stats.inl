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
\brief Inline definitions for img::stats
*/


#include "libdat/info.h" // for debug
#include "libimg/sample.h"
#include "libio/stream.h"
#include "libprob/Stats.h"

#include <iterator>


namespace img
{
namespace stats
{
//======================================================================

template <typename PixType, typename FwdIter>
inline
dat::MinMax<PixType>
activeMinMax
	( FwdIter const & begin
	, FwdIter const & end
	)
{
	dat::MinMax<PixType> mmpair;

	// inspect each element
	FwdIter iter(begin);

	// find first active pixel
	while (end != iter)
	{
		PixType const pix(*iter++);
		if (isActive(pix))
		{
			mmpair = dat::MinMax<PixType>(pix, pix);
			break;
		}
	}

	// track min/max
	while (end != iter)
	{
		PixType const pix(*iter++);
		if (isActive(pix))
		{
			mmpair = mmpair.expandedWith(pix);
		}
	}

	return mmpair;
}

template <typename PixType>
inline
dat::MinMax<PixType>
pixelMinMax
	( std::array<dat::grid<PixType>, 3u> const & grids
	)
{
	dat::MinMax<PixType> mmpair;
	if (grids[0].isValid() && grids[1].isValid() && grids[2].isValid())
	{
		dat::MinMax<PixType> tmp;
		for (dat::grid<PixType> const & grid : grids)
		{
			dat::MinMax<PixType> const mm
				(activeMinMax<PixType>(grid.begin(), grid.end()));
			if (img::isActive(mm.min())) // if min is active, so is max
			{
				tmp = tmp.expandedWith(mm);
			}
		}
		mmpair = tmp;
	}
	return mmpair;
}

template <typename PixType>
inline
dat::MinMax<PixType>
pixelMinMax
	( dat::grid<std::array<PixType, 3u> > const & grid
	, std::vector<dat::Area<size_t> > const & inSampAreas
	)
{
	dat::MinMax<PixType> mmpair;
	if (grid.isValid())
	{
		// find min/max across all grids
		dat::MinMax<PixType> tmp;

		// if sample areas is default value, then sample full grid
		std::vector<dat::Area<size_t> > sampAreas;
		if (! inSampAreas.empty())
		{
			sampAreas = inSampAreas;
		}
		else
		{
			dat::Range<size_t> const fullHigh{ 0u, grid.high() };
			dat::Range<size_t> const fullWide{ 0u, grid.wide() };
			dat::Area<size_t> const fullArea{ fullHigh, fullWide };
			sampAreas = std::vector<dat::Area<size_t> >{ fullArea };
		}

		// propagate min/max through all areas of current grid
		for (dat::Area<size_t> const & sampArea : sampAreas)
		{
			using FullPixel = std::array<PixType, 3u>;
			// copy all the samples (2x) - wastefull but easy
			std::vector<FullPixel> const samps
				(sample::fromImageArea<FullPixel>(grid, sampArea));
			std::vector<FullPixel> const active
				( sample::activeValuesFrom<FullPixel>
					(samps.begin(), samps.end())
				);
			for (FullPixel const & fullpix : active)
			{
				tmp = tmp.expandedWith(fullpix[0]);
				tmp = tmp.expandedWith(fullpix[1]);
				tmp = tmp.expandedWith(fullpix[2]);
			}
		}

		mmpair = tmp;
	}
	return mmpair;
}

template <typename PixType>
inline
prob::Histogram
histogramFor
	( dat::grid<PixType> const & deepGrid
	, std::vector<dat::Area<size_t> > const & sampAreas
	, dat::MinMax<typename PixType::value_type> const & inMinMax
	)
{
	prob::Histogram hist;

	if (deepGrid.isValid() && (! sampAreas.empty()))
	{
		// get pixel samples
		// rather wasteful of memory but easy
		std::vector<PixType> const gridSamps
			(sample::fromImageAreas<PixType>(deepGrid, sampAreas));

		// select active pixels (again wasteful of memory but easy)
		std::vector<PixType> const activePix
			( sample::activeValuesFrom<PixType>
				(gridSamps.begin(), gridSamps.end())
			);

		// access the active elements at channel level
		using ChanType = typename PixType::value_type;
		using ChanIter = ChanType const * const;
		size_t const numChan{ sizeof(PixType) / sizeof(ChanType) };
		size_t const numElem{ activePix.size() * numChan };
		ChanIter chanBeg(reinterpret_cast<ChanIter>(activePix.data()));
		ChanIter chanEnd(reinterpret_cast<ChanIter>(chanBeg + numElem));

		// get range from input if available, else compute it
		dat::Range<double> histRange(inMinMax.pair());
		if (! inMinMax.isValid())
		{
			// get min/max pixel channel-component values
			dat::MinMax<ChanType> const chanMinMax
				(channelMinMax<PixType>(activePix.begin(), activePix.end()));
			histRange = dat::Range<double>{ chanMinMax.pair() };
		}

		// create histogram using only active-pixel samples
		math::Partition const part(histRange,  256u);
		hist = prob::Histogram(part, chanBeg, chanEnd);
	}

	return hist;
}

template <typename PixType>
inline
prob::CdfInverse
cdfInverseFor
	( dat::grid<PixType> const & deepGrid
	, std::vector<dat::Area<size_t> > const & sampAreas
	)
{
	prob::CdfInverse cdfInv;
	if (deepGrid.isValid() && (! sampAreas.empty()))
	{
		// get histrogram
		prob::Histogram const hist
			(img::stats::histogramFor<PixType>(deepGrid, sampAreas));

		// construct inverse-cdf
		constexpr size_t lutSize{ 1024u };
		cdfInv = prob::CdfInverse::fromFrequencies
			( hist.theCounts.begin(), hist.theCounts.end()
			, hist.range()
			, lutSize
			);
	}
	return cdfInv;
}

template
	< typename PixType
	, typename FwdIter
	, dat::EnableIf< std::is_same<PixType, std::array<float, 3u> > >...
	>
inline
dat::MinMax<typename PixType::value_type>
channelMinMax
	( FwdIter const & beg
	, FwdIter const & end
	)
{
	dat::MinMax<typename PixType::value_type> minmax;
	for (FwdIter iter(beg) ; end != iter ; ++iter)
	{
		PixType const & pix = *iter;
		if (dat::isValid(pix))
		{
			minmax = minmax.expandedWith(pix[0]);
			minmax = minmax.expandedWith(pix[1]);
			minmax = minmax.expandedWith(pix[2]);
		}
	}
	return minmax;
}

template
	< typename PixType
	, typename FwdIter
	, dat::EnableIf< std::is_arithmetic<PixType> >...
	>
inline
PixType
meanOf
	( FwdIter const & beg
	, FwdIter const & end
	)
{
	PixType ave{ dat::nullValue<PixType>() };
	double sum{ 0. };
	double count{ 0. };
	for (FwdIter iter(beg) ; end != iter ; ++iter)
	{
		PixType const & pix = *iter;
		if (isActive(pix))
		{
			sum = sum + static_cast<double>(pix);
			count += 1.;
		}
	}
	if (0. < count)
	{
		ave = sum / count;
	}
	return ave;
}

template
	< typename PixType
	, typename FwdIter
	, dat::EnableIf< std::is_same<PixType, std::array<float, 3u> > >...
	>
inline
PixType
meanOf
	( FwdIter const & beg
	, FwdIter const & end
	)
{
	PixType ave{ dat::nullValue<float>() };
	std::array<double, 3u> sum{ 0. };
	double count{ 0. };
	for (FwdIter iter(beg) ; end != iter ; ++iter)
	{
		PixType const & pix = *iter;
		if (isActive(pix))
		{
			sum[0] += pix[0];
			sum[1] += pix[1];
			sum[2] += pix[2];
			count += 1.;
		}
	}
	if (0. < count)
	{
		ave[0] = static_cast<float>(sum[0] / count);
		ave[1] = static_cast<float>(sum[1] / count);
		ave[2] = static_cast<float>(sum[2] / count);
	}
	return ave;
}

template <typename FwdIter>
inline
std::array<float, 3u>
medianOf
	( FwdIter const & beg
	, FwdIter const & end
	)
{
	std::array<float, 3u> median
		{{ dat::nullValue<float>()
		 , dat::nullValue<float>()
		 , dat::nullValue<float>()
		}};
	size_t const numPix(static_cast<size_t>(std::distance(beg, end)));

	if (0u < numPix)
	{
		// temp data structure for sort operations
		std::vector<double> chan0;
		std::vector<double> chan1;
		std::vector<double> chan2;
		chan0.reserve(numPix);
		chan1.reserve(numPix);
		chan2.reserve(numPix);
		size_t numActive{ 0u };
		for (FwdIter iter(beg) ; end != iter ; ++iter)
		{
			// save active channel data to sortable temporary
			std::array<float, 3u> const & pix = *iter;
			if (isActive(pix))
			{
				// demultiplex data into individual channels
				chan0.emplace_back(double(pix[0]));
				chan1.emplace_back(double(pix[1]));
				chan2.emplace_back(double(pix[2]));
				++numActive;
			}
		}

		// if any data are present
		if (0u < numActive)
		{
			// compute the per channel median values
			median = std::array<float, 3u>
				{{ float(prob::Stats::nonConstMedianValue(& chan0))
				 , float(prob::Stats::nonConstMedianValue(& chan1))
				 , float(prob::Stats::nonConstMedianValue(& chan2))
				}};
		}
	}
	return median;
}

//======================================================================
}
}

