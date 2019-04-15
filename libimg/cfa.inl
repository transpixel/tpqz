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
\brief Inline definitions for img::cfa
*/


namespace img
{
namespace cfa
{

inline
dat::Extents
elemSizeFor
	( dat::Extents const & cellSize
	)
{
	return dat::Extents(2u*cellSize.high(), 2u*cellSize.wide());
}

inline
dat::Extents
cellSizeFor
	( dat::Extents const & elemSize
	)
{
	dat::Extents cellSize{};
	if (elemSize.isValid())
	{
		if ( (0u == (elemSize.high() % 2u))
		  && (0u == (elemSize.wide() % 2u))
		   )
		{
			size_t const halfHigh{ elemSize.high() / 2u };
			size_t const halfWide{ elemSize.wide() / 2u };
			cellSize = dat::Extents(halfHigh, halfWide);
		}
	}
	return cellSize;
}

template <typename CellType>
inline
dat::grid<typename CellType::value_type>
elemGridFor
	( dat::grid<CellType> const & cellGrid
	)
{
	using ElemType = typename CellType::value_type;
	dat::grid<ElemType> elemGrid;
	if (cellGrid.isValid())
	{
		// allocate space
		elemGrid = dat::grid<ElemType>(elemSizeFor(cellGrid.hwSize()));
		// multiplex data while copying
		using CellIter = typename dat::grid<CellType>::const_iterator;
		CellIter itCell{ cellGrid.begin() };
		using ElemIter = typename dat::grid<ElemType>::iterator;
		ElemIter itBeg0{ elemGrid.begin() };
		ElemIter itBeg1{ itBeg0 + elemGrid.wide() };
		for (size_t row{ 0u } ; row < cellGrid.high() ; ++row)
		{
			for (size_t col{0u} ; col < cellGrid.wide() ; ++col)
			{
				CellType const & cell = cellGrid(row, col);
				*itBeg0++ = cell.theElems[0][0];
				*itBeg0++ = cell.theElems[0][1];
				*itBeg1++ = cell.theElems[1][0];
				*itBeg1++ = cell.theElems[1][1];
				++itCell;
			}
			std::advance(itBeg0, elemGrid.wide());
			std::advance(itBeg1, elemGrid.wide());
		}
	}
	return elemGrid;
}

template <typename CellType>
inline
dat::grid<CellType>
cellGridFor
	( dat::grid<typename CellType::value_type> const & elemGrid
	)
{
	dat::grid<CellType> cellGrid;
	if (elemGrid.isValid())
	{
		dat::Extents const cellSize{ cellSizeFor(elemGrid.hwSize()) };
		cellGrid = dat::grid<CellType>(cellSize);

		using ElemIter
			= typename dat::grid<typename CellType::value_type>::const_iterator;
		ElemIter itElem0{ elemGrid.begin() };
		ElemIter itElem1{ elemGrid.begin() + elemGrid.wide() };
		for (size_t cellRow{0u} ; cellRow < cellGrid.high() ; ++cellRow)
		{
			for (size_t cellCol{0u} ; cellCol < cellGrid.wide() ; ++cellCol)
			{
				CellType & cell = cellGrid(cellRow, cellCol);
				// fill current cell with appropriate elements
				cell.theElems[0][0] = *itElem0++;
				cell.theElems[0][1] = *itElem0++;
				cell.theElems[1][0] = *itElem1++;
				cell.theElems[1][1] = *itElem1++;
			}
			// skip next row for both inputs
			std::advance(itElem0, elemGrid.wide());
			std::advance(itElem1, elemGrid.wide());
		}

	}
	return cellGrid;
}

template <typename ElemType>
inline
dat::grid<ElemType>
grayGridFor
	( dat::grid<ElemType> const & rawGrid
	, img::cfa::Cell<ElemType> const & cellGain
	)
{
	dat::grid<ElemType> outGrid;
	if (rawGrid.isValid())
	{
		assert(0u == (rawGrid.high() % cellGain.theElems.size()));
		assert(0u == (rawGrid.wide() % cellGain.theElems[0].size()));
		size_t const wide{ rawGrid.wide() };

		// expand gain values to cover two rows
		assert(2u == cellGain.theElems.size());
		assert(2u == cellGain.theElems[0].size());
		std::vector<ElemType> gainEvens;
		std::vector<ElemType> gainOdds;
		gainEvens.reserve(wide);
		gainOdds.reserve(wide);
		for (size_t nn{0u} ; nn < wide ; nn += 2u)
		{
			gainEvens[nn+0] = cellGain.theElems[0][0];
			gainEvens[nn+1] = cellGain.theElems[0][1];
			gainOdds[nn+0] = cellGain.theElems[1][0];
			gainOdds[nn+1] = cellGain.theElems[1][1];
		}

		// allocate space for output
		outGrid = dat::grid<ElemType>(rawGrid.hwSize());

		// scale data while copying
		size_t const high{ outGrid.high() };
		size_t row{0u};
		while (row < high)
		{
			std::transform
				( rawGrid.beginRow(row), rawGrid.endRow(row)
				, gainEvens.begin()
				, outGrid.beginRow(row)
				, []
					(ElemType const & raw, ElemType const & gain)
					{ return (raw * gain); }
				);
			++row;
			std::transform
				( rawGrid.beginRow(row), rawGrid.endRow(row)
				, gainOdds.begin()
				, outGrid.beginRow(row)
				, []
					(ElemType const & raw, ElemType const & gain)
					{ return (raw * gain); }
				);
			++row;
		}
	}
	return outGrid;
}

template <typename Type>
inline
std::array<dat::grid<Type>, 4u>
channelsFromRGGB
	( dat::grid<Type> const & rawGrid
	)
{
	std::array<dat::grid<Type>, 4u> chans;

	// check pre-conditions
	assert(0u == (rawGrid.high() % 2u));
	assert(0u == (rawGrid.wide() % 2u));
	size_t const outHigh(rawGrid.high() / 2u);
	size_t const outWide(rawGrid.wide() / 2u);
	size_t const inWide(rawGrid.wide());

	// allocate space
	chans[0] = dat::grid<Type>(outHigh, outWide);
	chans[1] = dat::grid<Type>(outHigh, outWide);
	chans[2] = dat::grid<Type>(outHigh, outWide);
	chans[3] = dat::grid<Type>(outHigh, outWide);
	typename dat::grid<Type>::iterator itOut00(chans[0].begin());
	typename dat::grid<Type>::iterator itOut01(chans[1].begin());
	typename dat::grid<Type>::iterator itOut10(chans[2].begin());
	typename dat::grid<Type>::iterator itOut11(chans[3].begin());

	// demultiplex channels
	typename dat::grid<Type>::const_iterator itIn00(rawGrid.begin());
	typename dat::grid<Type>::const_iterator itIn01(itIn00 + 1u);
	typename dat::grid<Type>::const_iterator itIn10(itIn00 + inWide);
	typename dat::grid<Type>::const_iterator itIn11(itIn10 + 1u);

	for (size_t outRow(0u) ; outRow < outHigh ; ++outRow)
	{
		for (size_t outCol(0u) ; outCol < outWide ; ++outCol)
		{
			// grab channel data
			*itOut00++ = *itIn00++;
			*itOut01++ = *itIn01++;
			*itOut10++ = *itIn10++;
			*itOut11++ = *itIn11++;
			// advance to next CFA cell
			++itIn00;
			++itIn01;
			++itIn10;
			++itIn11;
		}
		// advance to next row of cells
		itIn00 += inWide;
		itIn01 += inWide;
		itIn10 += inWide;
		itIn11 += inWide;
	}

	return chans;
}

}
}

