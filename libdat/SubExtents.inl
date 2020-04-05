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
\brief Inline definitions for dat::SubExtents
*/


namespace dat
{

inline
size_t const &
SubExtents :: high
	() const
{
	return theSize.high();
}

inline
size_t const &
SubExtents :: wide
	() const
{
	return theSize.wide();
}

inline
dat::RowCol
SubExtents :: subRowColFor
	( dat::RowCol const & fullRowCol
	) const
{
	using dat::operator-;
	return (fullRowCol - theUL);
}

inline
dat::RowCol
SubExtents :: fullRowColFor
	( dat::RowCol const & subRowCol
	) const
{
	using dat::operator+;
	return (theUL + subRowCol);
}

inline
bool
SubExtents :: includes
	( dat::RowCol const & fullRowCol
	) const
{
	return theSize.includes(subRowColFor(fullRowCol));
}

inline
dat::Spot
SubExtents :: fullSpotFor
	( dat::Spot const & relSpot
	) const
{
	dat::Spot const spotUL{{ double(theUL[0]), double(theUL[1]) }};
	return (spotUL + relSpot);
}

template <typename PixType>
inline
std::pair
	< typename dat::grid<PixType>::const_iterator
	, typename dat::grid<PixType>::const_iterator
	>
SubExtents :: iterFullBegEnd
	( dat::grid<PixType> const & fullGrid
	, size_t const & subRow
	) const
{
	using InIter = typename dat::grid<PixType>::const_iterator;
	size_t const & inRow = theUL[0] + subRow;
	size_t const & inCol = theUL[1];
	size_t const & numCols = theSize.wide();
	InIter const itInBeg(fullGrid.beginRow(inRow) + inCol);
	InIter const itInEnd(itInBeg + numCols);
	return { itInBeg, itInEnd };
}

}

