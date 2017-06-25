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
\brief Inline definitions for dat::MinMax
*/


#include "libio/sprintf.h"

#include <iomanip>
#include <sstream>


namespace dat
{
//======================================================================

template <typename Type>
inline
MinMax<Type> :: MinMax
	()
	: theMinMax{nullValue<Type>(), nullValue<Type>()}
{
}

template <typename Type>
inline
// explicit
MinMax<Type> :: MinMax
	( Type const & min
	, Type const & max
	)
	: theMinMax{min, max}
{
	if (! (theMinMax.first <= theMinMax.second))
	{
		*this = MinMax();
	}
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

template <typename Type>
inline
bool
MinMax<Type> :: isValid
	() const
{
	return
		(  dat::isValid(theMinMax.first)
		&& dat::isValid(theMinMax.second)
		);
}

template <typename Type>
inline
Type
MinMax<Type> :: min
	() const
{
	return theMinMax.first;
}

template <typename Type>
inline
Type
MinMax<Type> :: max
	() const
{
	return theMinMax.second;
}

template <typename Type>
inline
std::pair<Type, Type>
MinMax<Type> :: pair
	() const
{
	return std::pair<Type, Type>(min(), max());
}

template <typename Type>
inline
bool
MinMax<Type> :: nearlyEquals
	( MinMax<Type> const & other
	, Type const & tol
	) const
{
	bool same(false);
	if (isValid() && other.isValid())
	{
		same =
			(  dat::nearlyEquals(theMinMax.first, other.theMinMax.first, tol)
			&& dat::nearlyEquals(theMinMax.second, other.theMinMax.second, tol)
			);
	}
	return same;
}

template <typename Type>
inline
bool
MinMax<Type> :: containsInclusive
	( Type const & value
	) const
{
	return
		(  (theMinMax.first <= value)
		&& (value <= theMinMax.second)
		);
}

template <typename Type>
inline
MinMax<Type>
MinMax<Type> :: expandedWith
	( Type const & value
	) const
{
	MinMax<Type> result;
	bool const okayValue{ dat::isValid(value) };
	if (isValid())
	{
		if (okayValue)
		{
			result = MinMax
				( std::min(theMinMax.first, value)
				, std::max(theMinMax.second, value)
				);
		}
		else
		{
			result = *this;
		}
	}
	else
	{
		if (okayValue)
		{
			result = MinMax<Type>(value, value);
		}
		// else // return null
	}
	return result;
}

template <typename Type>
inline
MinMax<Type>
MinMax<Type> :: expandedWith
	( MinMax<Type> const & other
	) const
{
	MinMax<Type> const xLo(expandedWith(other.theMinMax.first));
	MinMax<Type> const xHiLo(xLo.expandedWith(other.theMinMax.second));
	return xHiLo;
}

template <typename Type>
inline
std::string
MinMax<Type> :: infoString
	( std::string const & title
	, std::string const & fmt
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << std::setw(15u) << title << "  ";
	}
	if (isValid())
	{
		oss
			<< ::io::sprintf(fmt, min())
			<< " "
			<< ::io::sprintf(fmt, max())
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

//======================================================================
}

