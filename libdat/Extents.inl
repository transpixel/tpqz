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
\brief Inline definitions for dat::Extents
*/



namespace dat
{
//======================================================================

inline
Extents :: Extents
	()
	: theArea
		{{ nullValue<size_t>()
		 , nullValue<size_t>()
		}}
{
}

constexpr
inline
// explicit
Extents :: Extents
	( size_t const & high
	, size_t const & wide
	)
	: theArea{{ high, wide }}
{
}

inline
bool
Extents :: empty
	() const
{
	return ((0u == theArea[0]) || (0u == theArea[1]));
}

inline
bool
Extents :: isValid
	() const
{
	return
		(  dat::isValid(theArea[0])
		&& dat::isValid(theArea[1])
		);
}

inline
size_t const &
Extents :: high
	() const
{
	return theArea[0];
}

inline
size_t const &
Extents :: wide
	() const
{
	return theArea[1];
}

inline
size_t
Extents :: size
	() const
{
	return (high() * wide());
}

inline
size_t const &
Extents :: operator[]
	( size_t const & ndx
	) const
{
	assert(ndx < theArea.size());
	return theArea[ndx];
}

inline
bool
Extents :: operator==
	( Extents const & other
	) const
{
	return
		(  (theArea[0] == other.theArea[0])
		&& (theArea[1] == other.theArea[1])
		);
}

inline
bool
Extents :: includes
	( std::array<size_t, 2u> const & rowcol
	) const
{
	return
		(  (rowcol[0] < theArea[0]) // NOTE!! strict inequality
		&& (rowcol[1] < theArea[1]) // for half open interval!
		);
}

inline
dat::Extents
Extents :: halfSizeMod2
	() const
{
	dat::RowCol const half(centerMod2());
	return dat::Extents(half[0], half[1]);
}

inline
dat::RowCol
Extents :: centerMod2
	() const
{
	return dat::RowCol{{ (high()/2u) , (wide()/2u) }};
}


//======================================================================
}

