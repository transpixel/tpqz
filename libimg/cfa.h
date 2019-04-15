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

#ifndef img_cfa_INCL_
#define img_cfa_INCL_

/*! \file
\brief Declarations for img::cfa
*/


#include "libdat/Extents.h"
#include "libdat/grid.h"
#include "libimg/img.h"

#include <array>


namespace img
{
//! Functions for handling image detector color filter arrays.
namespace cfa
{
	//! An individual CFA cell (2x2)
	template <typename ElemType>
	struct Cell
	{
		using value_type = ElemType;

		using RowPix = std::array<ElemType, 2u>;
		std::array<RowPix, 2u> theElems;
	};

	//! Size in elements associated with cell
	inline
	dat::Extents
	elemSizeFor
		( dat::Extents const & cellSize
		);

	//! Expanded values after multiplexing cell data into CFA layout
	template <typename CellType>
	inline
	dat::grid<typename CellType::value_type>
	elemGridFor
		( dat::grid<CellType> const & cellGrid
		);

	//! Grid after applying cellGain factors to rawGrid
	template <typename ElemType>
	inline
	dat::grid<ElemType>
	grayGridFor
		( dat::grid<ElemType> const & rawGrid
		, Cell<ElemType> const & cellGain
		);

	//! Extracts nearest samples on 2x2 grid
	std::array<dat::grid<fpix_t>, 3u>
	rgbFastFromRGGB
		( dat::grid<fpix_t> const & rawGrid
		, std::array<fpix_t, 3u> const & rgbGainFactors
		, bool const & setEdgeToNull = true
		);

	//! Returns (weighted) average of 2x2 CFA cells
	dat::grid<fpix_t>
	grayFastFrom2x2
		( dat::grid<fpix_t> const & rawGrid
		, std::array<fpix_t, 3u> const & rgbGainFactors
		, bool const & setEdgeToNull
		);

	//! Extracts nearest samples on 2x2 grid
	template <typename Type>
	inline
	std::array<dat::grid<Type>, 4u>
	channelsFromRGGB
		( dat::grid<Type> const & rawGrid
		);
}

}

// Inline definitions
#include "libimg/cfa.inl"

#endif // img_cfa_INCL_

