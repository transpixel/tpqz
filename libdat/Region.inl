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
\brief Inline definitions for dat::Region
*/


#include "libdat/info.h"

#include <cassert>
#include <sstream>


namespace dat
{
//======================================================================

template <size_t Dim, typename Type>
inline
// explicit
Region<Dim, Type> :: Region
	( std::initializer_list<Range<Type> > const & ranges
	)
	: theRanges(ranges.begin(), ranges.end())
{
	assert(Dim == theRanges.size());
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

template <size_t Dim, typename Type>
inline
bool
Region<Dim, Type> :: isValid
	() const
{
	return (! theRanges.empty());
}

template <size_t Dim, typename Type>
inline
Range<Type> const &
Region<Dim, Type> :: operator[]
	( size_t const & ndx
	) const
{
	assert(ndx < theRanges.size());
	return theRanges[ndx];
}

template <size_t Dim, typename Type>
inline
double
Region<Dim, Type> :: magnitude
	() const
{
	double result(dat::nullValue<double>());
	if (isValid())
	{
		result = 1.;
		for (dat::Range<Type> const & range : theRanges)
		{
			result *= range.magnitude();
		}
	}
	return result;
}

template <size_t Dim, typename Type>
inline
std::array<Type, Dim>
Region<Dim, Type> :: minimums
	() const
{
	std::array<Type, Dim> result;
	for (size_t ndx(0u) ; ndx < Dim ; ++ndx)
	{
		result[ndx] = theRanges[ndx].min();
	}
	return result;
}

template <size_t Dim, typename Type>
inline
std::array<Type, Dim>
Region<Dim, Type> :: maximums
	() const
{
	std::array<Type, Dim> result;
	for (size_t ndx(0u) ; ndx < Dim ; ++ndx)
	{
		result[ndx] = theRanges[ndx].max();
	}
	return result;
}

template <size_t Dim, typename Type>
inline
std::array<Type, Dim>
Region<Dim, Type> :: center
	() const
{
	std::array<Type, Dim> result;
	for (size_t ndx(0u) ; ndx < Dim ; ++ndx)
	{
		result[ndx] = .5 * (theRanges[ndx].min() + theRanges[ndx].max());
	}
	return result;
}

template <size_t Dim, typename Type>
inline
bool
Region<Dim, Type> :: contains
	( std::array<Type, Dim> const & value
	) const
{
	bool isIn{ false };
	if (isValid())
	{
		isIn = true; // unless contradicted
		for (size_t nn{0u} ; nn < Dim ; ++nn)
		{
			bool const in{ theRanges[nn].contains(value[nn]) };
			if (! in)
			{
				isIn = false;
				break;
			}
		}
	}
	return isIn;
}

template <size_t Dim, typename Type>
inline
Region<Dim, Type>
Region<Dim, Type> :: intersectWith
	( Region<Dim, Type> const & other
	) const
{
	Region<Dim, Type> common;
	if (isValid() && other.isValid())
	{
		common.theRanges.reserve(Dim);
		for (size_t ndx(0u) ; ndx < Dim ; ++ndx)
		{
			Range<Type> const & rangeThis = theRanges[ndx];
			Range<Type> const & rangeThat = other.theRanges[ndx];
			common.theRanges.emplace_back(rangeThis.intersectWith(rangeThat));
		}
	}
	return common;
}

template <size_t Dim, typename Type>
inline
bool
Region<Dim, Type> :: nearlyEquals
	( Region<Dim, Type> const & other
	, Type const & tol
	) const
{
	bool same{ false };
	if (isValid() && other.isValid())
	{
		same = theRanges.size() == other.theRanges.size();
		size_t ndx(0u);
		while ((ndx < Dim) && same)
		{
			same = theRanges[ndx].nearlyEquals(other.theRanges[ndx], tol);
			++ndx;
		}
	}
	return same;
}

template <size_t Dim, typename Type>
inline
std::string
Region<Dim, Type> :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << " ";
	}

	oss << "Dimension: " << theRanges.size();

	if (isValid())
	{
		for (dat::Range<Type> const & range : theRanges)
		{
			oss << std::endl;
			oss << range.infoString();
		}

		oss << std::endl;
		oss << dat::infoString(magnitude(), "magnitude");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

//======================================================================
}

