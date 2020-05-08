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

/*! \file
\brief Inline Definitions for dat::grid
*/

#include "libio/sprintf.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iomanip>
#include <sstream>


template < typename Type >
inline
dat::grid<Type> :: grid
	( size_t const & high
	, size_t const & wide
	)
	: theHigh{ high }
	, theWide{ wide }
	, theData{ nullptr }
{
	if (0 < theHigh && 0 < theWide)
	{
		theData = new Type[theHigh * theWide];
	}
}

template < typename Type >
inline
// explicit
dat::grid<Type> :: grid
	( Extents const & hwSize
	)
	: grid(hwSize[0], hwSize[1])
{
}

template < typename Type >
inline
dat::grid<Type> :: grid
	( size_t const & high
	, size_t const & wide
	, Type const & fillValue
	)
	: grid(high, wide)
{
	size_t const size(theHigh * theWide);
	std::fill(theData, theData + size, fillValue);
}

template < typename Type >
inline
dat::grid<Type> :: grid
	( grid<Type> const & orig
	)
	: theHigh{ orig.theHigh }
	, theWide{ orig.theWide }
	, theData{ nullptr }
{
	if (0 < theHigh && 0 < theWide)
	{
		theData = new Type[theHigh * theWide];
		std::copy(orig.begin(), orig.end(), begin());
	}
}

template < typename Type >
inline
dat::grid<Type> &
dat::grid<Type> :: operator=
	( dat::grid<Type> const & rhs
	)
{
	if (&rhs != this)
	{
		resize(rhs.theHigh, rhs.theWide);
		std::copy(rhs.begin(), rhs.end(), begin());
	}

	return *this;
}

template < typename Type >
inline
dat::grid<Type> :: grid
	( grid<Type> && orig
	)
	: grid()
{
	std::swap(theHigh, orig.theHigh);
	std::swap(theWide, orig.theWide);
	std::swap(theData, orig.theData);
}

template < typename Type >
inline
dat::grid<Type> &
dat::grid<Type> :: operator=
	( dat::grid<Type> && rhs
	)
{
	if (&rhs != this)
	{
		std::swap(theHigh, rhs.theHigh);
		std::swap(theWide, rhs.theWide);
		std::swap(theData, rhs.theData);
	}

	return *this;
}

template < typename Type >
inline
dat::grid<Type> :: ~grid
	()
{
	if (theData)
	{
		delete [] theData;
	}
}

template < typename Type >
inline
bool
dat::grid<Type> :: isValid() const
{
	return theData != nullptr;
}

template < typename Type >
inline
dat::Extents
dat::grid<Type> :: hwSize
	() const
{
	return Extents(theHigh, theWide);
}

template < typename Type >
inline
size_t const &
dat::grid<Type> :: high
	() const
{
	return theHigh;
}

template < typename Type >
inline
size_t const &
dat::grid<Type> :: wide
	() const
{
	return theWide;
}

template < typename Type >
inline
size_t
dat::grid<Type> :: size
	() const
{
	return theHigh * theWide;
}

template < typename Type >
inline
size_t
dat::grid<Type> :: byteSize
	() const
{
	return size() * sizeof(Type);
}

//
// operator()
//
template < typename Type >
inline
Type const &
dat::grid<Type> :: operator()
	( size_t const & row
	, size_t const & col
	) const
{
	return *(theData + row * theWide + col);
}

//
// operator()
//
template < typename Type >
inline
Type &
dat::grid<Type> :: operator()
	( size_t const & row
	, size_t const & col
	)
{
	return *(theData + row * theWide + col);
}

template < typename Type >
inline
Type const &
dat::grid<Type> :: operator()
	( dat::RowCol const & rowcol
	) const
{
	return operator()(rowcol[0], rowcol[1]);
}

template < typename Type >
inline
Type &
dat::grid<Type> :: operator()
	( dat::RowCol const & rowcol
	)
{
	return operator()(rowcol[0], rowcol[1]);
}

//
// begin
//
template < typename Type >
inline
typename dat::grid<Type>::const_iterator
dat::grid<Type> :: begin() const
{
	return theData;
}

//
// end
//
template < typename Type >
inline
typename dat::grid<Type>::const_iterator
dat::grid<Type> :: end() const
{
	return theData + size();
}

//
// begin
//
template < typename Type >
inline
typename dat::grid<Type>::iterator
dat::grid<Type> :: begin()
{
	return theData;
}

//
// end
//
template < typename Type >
inline
typename dat::grid<Type>::iterator
dat::grid<Type> :: end()
{
	return theData + size();
}

//
// rbegin
//
template < typename Type >
inline
typename dat::grid<Type>::const_reverse_iterator
dat::grid<Type> :: rbegin() const
{
	return const_reverse_iterator(end());
}

//
// rend
//
template < typename Type >
inline
typename dat::grid<Type>::const_reverse_iterator
dat::grid<Type> :: rend() const
{
	return const_reverse_iterator(begin());
}

//
// rbegin
//
template < typename Type >
inline
typename dat::grid<Type>::reverse_iterator
dat::grid<Type> :: rbegin()
{
	return reverse_iterator(end());
}

//
// rend
//
template < typename Type >
inline
typename dat::grid<Type>::reverse_iterator
dat::grid<Type> :: rend()
{
	return reverse_iterator(begin());
}

//
// beginRow const
//
template < typename Type >
inline
typename dat::grid<Type>::const_iterator
dat::grid<Type> :: beginRow
	( size_t const & row
	) const
{
	return theData + (row * theWide);
}

//
// endRow const
//
template < typename Type >
inline
typename dat::grid<Type>::const_iterator
dat::grid<Type> :: endRow
	( size_t const & row
	) const
{
	return beginRow(row + 1);
}

//
// beginRow r/w
//
template < typename Type >
inline
typename dat::grid<Type>::iterator
dat::grid<Type> :: beginRow
	( size_t const & row
	)
{
	return theData + (row * theWide);
}

//
// endRow r/w
//
template < typename Type >
inline
typename dat::grid<Type>::iterator
dat::grid<Type> :: endRow
	( size_t const & row
	)
{
	return beginRow(row + 1);
}

//
// iterAt const
//
template < typename Type >
inline
typename dat::grid<Type>::const_iterator
dat::grid<Type> :: iterAt
	( size_t const & row
	, size_t const & col
	) const
{
	return theData + (row * theWide + col);
}

//
// iterAt r/w
//
template < typename Type >
inline
typename dat::grid<Type>::iterator
dat::grid<Type> :: iterAt
	( size_t const & row
	, size_t const & col
	)
{
	return theData + (row * theWide + col);
}

//
// rowColFor
//
template < typename Type >
inline
dat::RowCol
dat::grid<Type> :: rowColFor
	( typename dat::grid<Type>::const_iterator const & iter
	) const
{
	size_t const dist(std::distance(begin(), iter));
	return {{ dist / wide(), dist % wide() }};
}

//
// infoString
//
template < typename Type >
inline
std::string
dat::grid<Type> :: infoString
	( std::string const & title
	) const
{
	std::ostringstream os;

	if (!title.empty())
	{
		os << title << " ";
	}

	os << "High,Wide:"
		<< " " << std::setw(5) << theHigh
		<< " " << std::setw(5) << theWide
		;

	return os.str();
}

//
// infoStringContents
//
template < typename Type >
inline
std::string
dat::grid<Type> :: infoStringContents
	( std::string const & title
	, std::string const & fmt
	) const
{
	std::ostringstream oss;

	if (!title.empty())
	{
		oss << "# " << title << std::endl;
	}

	oss << "# HighWide: " << theHigh << ", " << theWide;

	if (isValid())
	{
		Type const * iter(begin());
		for (size_t row(0); row < theHigh; ++row)
		{
			oss << std::endl;
			for (size_t col(0); col < theWide; ++col)
			{
				oss << ' ' << ::io::sprintf(fmt, *iter++);
			}
		}
	}
	else
	{
		oss << std::endl;
		oss << " <null>";
	}

	return oss.str();
}

//
// operator==
//
template < typename Type >
inline
bool
dat::grid<Type> :: operator==
	( grid const & rhs
	) const
{
	return high() == rhs.high()
		&& wide() == rhs.wide()
		&& std::equal(begin(), end(), rhs.begin());
}

//
// castGrid
//
template <typename ElemType>
template <typename OutType>
inline
dat::grid<OutType>
dat::grid<ElemType> :: castGrid
	() const
{
	dat::grid<OutType> dgrid(high(), wide());
	typename dat::grid<OutType>::iterator itOut(dgrid.begin());
	for (typename dat::grid<ElemType>::const_iterator
		iter(begin()) ; iter != end() ; ++iter)
	{
		*itOut++ = static_cast<OutType>(*iter);
	}
	return dgrid;
}

template < typename Type >
inline
void
dat::grid<Type> :: resize
	( size_t const & high
	, size_t const & wide
	)
{
	size_t const sizeold(theHigh * theWide);
	size_t const sizenew(high * wide);
	if (sizeold != sizenew)
	{
		if (nullptr != theData) // deallocate old
		{
			delete [] theData;
			theData = nullptr;
		}

		if (0 < sizenew) // allocate new
		{
			theData = new Type[sizenew];
		}
	}

	// fix up high/wide
	if (0 < sizenew)
	{
		theHigh = high;
		theWide = wide;
	}
	else
	{
		theHigh = theWide = 0;
	}
}

template < typename Type >
inline
size_t
dat::grid<Type> :: validCountInRow
	( size_t const & rowNdx
	) const
{
	size_t num{ dat::nullValue<size_t>() };
	if (isValid())
	{
		/*
		dat::IsValid<Type> const validator{};
		num = (size_t)std::count_if
			(beginRow(rowNdx), endRow(rowNdx), validator);
		*/
		num = 0u;
		for (size_t col{0u} ; col < wide() ; ++col)
		{
			Type const & elem = (*this)(rowNdx, col);
			if (dat::isValid(elem))
			{
				++num;
			}
		}
	}
	return num;
}

template < typename Type >
inline
size_t
dat::grid<Type> :: validCountInCol
	( size_t const & colNdx
	) const
{
	size_t num{ dat::nullValue<size_t>() };
	if (isValid())
	{
		num = 0u;
		for (size_t row{0u} ; row < high() ; ++row)
		{
			Type const & elem = (*this)(row, colNdx);
			if (dat::isValid(elem))
			{
				++num;
			}
		}
	}
	return num;
}


