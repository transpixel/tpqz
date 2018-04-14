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
\brief Inline definitions for img::dilate
*/


namespace img
{

namespace dilate
{

namespace priv
{
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
	template <typename ElemType>
	struct NeighborHoodsOf
	{
		dat::grid<ElemType> const * const thePtGrid;
		size_t const theDelta;
		std::shared_ptr<std::vector<RowColDist> > const thePtRCDs;

		// cache
		std::pair<size_t, size_t> const theRowRange;
		std::pair<size_t, size_t> const theColRange;

		//! Attach to grid
		explicit
		NeighborHoodsOf
			( dat::grid<ElemType> const * const & ptGrid
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
					ElemType const & tryElem = (*thePtGrid)(rcLook);
					if (dat::isValid(tryElem))
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
	template <typename ElemType>
	inline
	ElemType
	valueOfNearest
		( std::vector<RowColDist> const & rcds
		, dat::grid<ElemType> const & origGrid
		)
	{
		ElemType outElem{ dat::nullValue<ElemType>() };
		math::Extreme<std::vector<RowColDist>::const_iterator> const extreme
			(rcds.begin(), rcds.end(), isMinDist);
		if (extreme.isValid())
		{
			dat::RowCol const & rcNear = extreme.theExVal.theRowCol;
			outElem = origGrid(rcNear);
		}
		return outElem;
	}

	//! Fill each invalid pixel with the value of valid pixel nearest to it
	template <typename ElemType>
	inline
	dat::grid<ElemType>
	floodWithNearest
		( dat::grid<ElemType> const & origGrid
		, size_t const & rcDel
		, size_t * const & ptNumFilled = nullptr
		)
	{
		dat::grid<ElemType> fillGrid(origGrid);
		dat::Extents const gridSize(origGrid.hwSize());

		// create neighborhood producer
		NeighborHoodsOf<ElemType> const hoodsOf(&origGrid, rcDel);

		size_t fillCount{ 0u };
		for (dat::ExtentsIterator iter(gridSize) ; iter ; ++iter)
		{
			// grid location to be updated
			dat::RowCol const & currRowCol = *iter;

			// compute output pixel given neighborhood of current pixel
			ElemType const & srcElem = origGrid(currRowCol);
			ElemType outElem{ srcElem };
			if (! dat::isValid(srcElem))
			{
				// track number of fill operations
				++fillCount;

				// grab valid neighbors
				std::vector<RowColDist> const & rcds = hoodsOf(currRowCol);

				// if any valid neighbors, find the one closest
				if (! rcds.empty())
				{
					outElem = valueOfNearest(rcds, origGrid);
				}
			}

			// assign output value
			fillGrid(currRowCol) = outElem;
		}

		// report number of elements filled
		if (ptNumFilled)
		{
			*ptNumFilled = fillCount;
		}

		return fillGrid;
	}
}

template <typename ElemType>
inline
dat::grid<ElemType>
floodFilled
	( dat::grid<ElemType> const & srcGrid
	)
{
	dat::grid<ElemType> fillGrid(srcGrid);

	// seems faster to do less filling each iteration and use more iterations
	// therefore, keep per iteration fill windows size smaller
	constexpr size_t halfStep{ 2u }; // maybe forces cache misses if too big?
	size_t fillCount{ fillGrid.size() };
	for (size_t nn{0u} ; (0u < fillCount) ; ++nn)
	{
		size_t currFillCount{};
		using priv::floodWithNearest;
		fillGrid = floodWithNearest(fillGrid, halfStep, &currFillCount);
		assert(! (fillCount < currFillCount));
		if (fillCount == currFillCount)
		{
			break;
		}
		fillCount = currFillCount;
	}
	return fillGrid;
}

} // dilate

} // img

