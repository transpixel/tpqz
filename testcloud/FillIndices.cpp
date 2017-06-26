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
\brief  This file contains main application program FillIndices
*/


#include "libapp/Usage.h"

#include "build/version.h"
#include "libdat/info.h"
#include "libio/stream.h"

#include "libapp/Timer.h"
#include "libcloud/cloud.h"
#include "libdat/algorithm.h"
#include "libdat/cast.h"
#include "libdat/compare.h"
#include "libdat/Extents.h"
#include "libdat/ExtentsIterator.h"
#include "libdat/grid.h"
#include "libdat/IndexIterator.h"
#include "libdat/io.h"
#include "libdat/SubExtents.h"
#include "libimg/io.h"
#include "libio/sprintf.h"
#include "libio/string.h"
#include "libmath/Extreme.h"
#include "libmath/math.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>


namespace
{
	using PixVal = cloud::NdxType;
	constexpr bool sShowProgress{ true };
	constexpr bool sShowTiming{ true };
	constexpr bool sSaveTmpGrids{ true };

	//! Ancillary structure for proximity assessment
	struct RowColDist
	{
		dat::RowCol theRowCol
			{{ dat::nullValue<size_t>(), dat::nullValue<size_t>() }};
		double theDist{ dat::nullValue<double>() };

		RowColDist
			() = default;

		RowColDist
			( dat::RowCol const & rowcol
			, double const & dist
			)
			: theRowCol(rowcol)
			, theDist{ dist }
		{ }

		//! Invalid instance
		inline
		static
		RowColDist
		null
			()
		{
			static RowColDist const aNull;
			return aNull;
		}

		//! True if this instance is valid
		inline
		bool
		isValid
			() const
		{
			return dat::isValid(theDist);
		}
	};

	//! True if dist member of A is less than that of B
	inline
	bool
	isMinDist
		( RowColDist const & rcdA
		, RowColDist const & rcdB
		)
	{
		return (rcdA.theDist < rcdB.theDist);
	}

	//! Return limits associated with value
	dat::IndexIterator
	indexIteratorFor
		( size_t const & value
		, size_t const & delta
		, std::pair<size_t, size_t> const & range
		)
	{
		size_t beg{ 0u };
		if (dat::canSubtract(value, delta))
		{
			beg = value - delta;
		}
		size_t const end{ dat::clamped((value + delta), range) };
		return dat::IndexIterator({ beg, end });
	}

	//! Functor providing distance to reference point
	struct Distanator
	{
		dat::Spot const theRefSpot;

		//! Construct with rcRef as one endpoint of distances
		explicit
		Distanator
			( dat::RowCol const & refRowCol
			)
			: theRefSpot(dat::cast::Spot(refRowCol))
		{ }

		//! Distance to this rowcol
		inline
		double
		operator()
			( dat::RowCol const & anyRowCol
			) const
		{
			dat::Spot const currSpot(dat::cast::Spot(anyRowCol));
			using dat::operator-;
			return dat::magnitude(currSpot - theRefSpot);
		}
	};

	//! Neighborhood functor - extracts RowColDist instances from grid
	struct NeighborHoodsOf
	{
		dat::grid<PixVal> const * const thePtGrid;
		size_t const theDelta;
		std::shared_ptr<std::vector<RowColDist> > const thePtRCDs;

		// cache
		std::pair<size_t, size_t> const theRowRange;
		std::pair<size_t, size_t> const theColRange;

		//! Attach to grid
		explicit
		NeighborHoodsOf
			( dat::grid<PixVal> const * const & ptGrid
			, size_t const & delta
			)
			: thePtGrid{ ptGrid }
			, theDelta{ delta }
			, thePtRCDs
				{ std::make_shared<std::vector<RowColDist> >
					(math::sq(2u*theDelta + 1u))
				}
			, theRowRange{0u, thePtGrid->high()}
			, theColRange{0u, thePtGrid->wide()}
		{
			assert(thePtGrid); // 
			assert(0u < theDelta);
		}

		//! (Only) valid RowColDist instances from neighborhood of atRowCol
		std::vector<RowColDist> const &
		operator()
			( dat::RowCol const & atRowCol
			) const
		{
			thePtRCDs->clear();

			// compute distances relative to input rowcol
			Distanator const distanceTo(atRowCol);

			dat::IndexIterator itRow
				(indexIteratorFor(atRowCol[0], theDelta, theRowRange));
			for ( ; itRow ; ++itRow)
			{
				dat::IndexIterator itCol
					(indexIteratorFor(atRowCol[1], theDelta, theColRange));
				for ( ; itCol ; ++itCol)
				{
					dat::RowCol const rcLook{{ *itRow, *itCol }};
					PixVal const & tryPix = (*thePtGrid)(rcLook);
					if (dat::isValid(tryPix))
					{
						double const dist(distanceTo(rcLook));
						RowColDist const rcd{ rcLook, dist };
						thePtRCDs->emplace_back(rcd);
					}
				}
			}
			return *thePtRCDs;
		}
	};

	//! Value of nearest valid pixel
	PixVal
	valueOfNearest
		( std::vector<RowColDist> const & rcds
		, dat::grid<PixVal> const & origGrid
		)
	{
		PixVal outPix{ dat::nullValue<PixVal>() };
		math::Extreme<std::vector<RowColDist>::const_iterator> const extreme
			(rcds.begin(), rcds.end(), isMinDist);
		if (extreme.isValid())
		{
			dat::RowCol const & rcNear = extreme.theExVal.theRowCol;
			outPix = origGrid(rcNear);
		}
		return outPix;
	}

	//! Fill each invalid pixel with the value of valid pixel nearest to it
	dat::grid<PixVal>
	fillWithNearest
		( dat::grid<PixVal> const & origGrid
		, size_t const & rcDel
		, size_t * const & ptNumFilled = nullptr
		)
	{
		dat::grid<PixVal> fullGrid(origGrid);
		dat::Extents const gridSize(origGrid.hwSize());

		// create neighborhood producer
		NeighborHoodsOf const hoodsOf(&origGrid, rcDel);

		size_t fillCount{ 0u };
		for (dat::ExtentsIterator iter(gridSize) ; iter ; ++iter)
		{
			// grid location to be updated
			dat::RowCol const & currRowCol = *iter;

			// compute output pixel given neighborhood of current pixel
			PixVal const & srcPix = origGrid(currRowCol);
			PixVal outPix{ srcPix };
			if (! dat::isValid(srcPix))
			{
				// track number of fill operations
				++fillCount;

				// grab valid neighbors
				std::vector<RowColDist> const & rcds = hoodsOf(currRowCol);

				// if any valid neighbors, find the one closest
				if (! rcds.empty())
				{
					outPix = valueOfNearest(rcds, origGrid);
				}
			}

			// assign output value
			fullGrid(currRowCol) = outPix;
		}

		// report number of elements filled
		if (ptNumFilled)
		{
			*ptNumFilled = fillCount;
		}

		return fullGrid;
	}

	//! An "on" value for pixels that are valid - else 0
	inline
	uint8_t
	outValue
		( PixVal const & pix
		)
	{
		uint8_t outVal{ 100u };
		if (dat::isValid(pix))
		{
			outVal = 200u;
		}
		return outVal;
	}

	//! Save visualization grid
	void
	saveGridViz
		( dat::grid<PixVal> const & ndxGrid
		, std::string const & fpath
		)
	{
		dat::grid<uint8_t> ugrid(ndxGrid.hwSize());
		std::fill(ugrid.begin(), ugrid.end(), 0u);
		std::transform
			( ndxGrid.begin(), ndxGrid.end()
			, ugrid.begin()
			, [] (PixVal const & pix) { return outValue(pix); }
			);
		(void)img::io::saveViaOpenCV(ugrid, fpath);
	}
}

//! Fill sparsely populated index map
int
main
	( int const argc
	, char const * const * const argv
	)
{
	// Don't mix stdio & streams (for performance)
	//std::ios::sync_with_stdio(false);

	// check args
	app::Usage usage;
	usage.setSummary
		( "Fill missing index values with nearest index"
		);
	usage.addArg("OutPath", "path to output index [.u32 format]");
	usage.addArg("InPath", "path to input index [.u32 format]");
	usage.addArg("InHigh", "height of input .u32 grid");
	usage.addArg("InWide", "width of input .u32 grid");
	// ...
	if (usage.argStatus(argc, argv) != app::Usage::Valid)
	{
		std::string const fname(argv[0]);
		io::err()
			<< std::endl << build::version::buildInfo(argv[0]) << std::endl
			<< usage.infoString(fname) << std::endl;
		return 1;
	}

	// parse input argument
	int argnum(0);
	std::string const outpath(argv[++argnum]);
	std::string const inpath(argv[++argnum]);
	std::string const strhigh(argv[++argnum]);
	std::string const strwide(argv[++argnum]);

	constexpr size_t nullSize{ dat::nullValue<size_t>() };
	size_t const inHigh{ io::string::from(strhigh, nullSize) };
	size_t const inWide{ io::string::from(strwide, nullSize) };
	dat::Extents const hwSize(inHigh, inWide);
	if (! dat::isValid(hwSize))
	{
		io::err() << "ERROR: Bad input size specification."
			<< " " << dat::infoString(hwSize, "hwSize")
			<< std::endl;
		return 1;
	}

	// load input cloud
	dat::grid<PixVal> origGrid(hwSize);
	std::ifstream ifs(inpath);
	std::streamsize gotFileSize{};
	bool const okayLoad
		( dat::io::loadBinary
			(ifs, origGrid.begin(), origGrid.end(), &gotFileSize)
		);
	if (okayLoad)
	{
		// fill indices
		app::Timer timer;
		constexpr size_t halfSize{ 2u }; // seems faster to iterate
		constexpr size_t maxIter{ 32u }; // than to use a larger window
		dat::grid<PixVal> fullGrid(origGrid);
		size_t fillCount{ fullGrid.size() };
		io::out() << "iter,fillCount:" << std::endl;
		for (size_t nn{0u} ; (0u < fillCount) && (nn < maxIter) ; ++nn)
		{
			std::string const nstr{ io::sprintf("%02d", nn) };
			if (sShowProgress)
			{
				io::out()
					<< " " << dat::infoString(nn)
					<< " " << dat::infoString(fillCount)
					<< std::endl;
			}
			if (sSaveTmpGrids)
			{
				std::string const tmpname{ "tmpGrid" + nstr + ".png" };
				saveGridViz(fullGrid, tmpname);
			}

			timer.start(nstr);
			fullGrid = fillWithNearest(fullGrid, halfSize, &fillCount);
			timer.stop();
		}

		if (sShowTiming)
		{
			io::out() << dat::infoString(timer) << std::endl;
		}

		// save visualization
		if (sSaveTmpGrids)
		{
			saveGridViz(fullGrid, "fullGrid.png");
		}

		std::ofstream ofs(outpath);
		bool const okaySave
			(dat::io::saveBinary(ofs, fullGrid.begin(), fullGrid.end()));
		if (okaySave)
		{
			io::out() << "Success" << std::endl;
		}
		else
		{
			io::err() << "FAILURE: Can't load save to file:"
				<< " " << outpath << std::endl;
		}
	}
	else
	{
		size_t const expFileSize(hwSize.size() * sizeof(uint32_t));
		io::err() << "FAILURE: Can't load index map from:"
			<< " '" << inpath << "'\n"
			<< " Specified size: " << dat::infoString(expFileSize) << '\n'
			<< "Found byte size: " << dat::infoString(gotFileSize) << '\n'
			<< std::endl;
	}

	return 0;
}
