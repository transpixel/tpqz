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

#ifndef dat_Region_INCL_
#define dat_Region_INCL_

/*! \file
\brief Declarations for dat::Region
*/


#include "libdat/Range.h"

#include <array>
#include <string>


namespace dat
{

/*! \brief General N-Dimensional region comprising orthogonal Ranges

\par Example
\dontinclude testdat/uRegion.cpp
\skip ExampleStart
\until ExampleEnd
*/

template <size_t Dim, typename Type>
class Region
{
	std::vector<dat::Range<Type> > theRanges{};

public: // methods

	//! default null constructor
	Region () = default;

	//! Value constructor
	inline
	explicit
	Region
	  	( std::initializer_list<Range<Type> > const & ranges
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Access to the ndx-th range component
	inline
	Range<Type> const &
	operator[]
		( size_t const & ndx
		) const;

	//! Magnitude of this region (aka hypervolume)
	inline
	double
	magnitude
		() const;

	//! Minimum value of each component dimension
	inline
	std::array<Type, Dim>
	minimums
		() const;

	//! Maximum value of each component dimension
	inline
	std::array<Type, Dim>
	maximums
		() const;

	//! Center of each component dimension
	inline
	std::array<Type, Dim>
	center
		() const;

	//! True if each range component contains corresponding component of value
	inline
	bool
	contains
		( std::array<Type, Dim> const & value
		) const;

	//! Region in common between this and other
	inline
	Region<Dim, Type>
	intersectWith
		( Region<Dim, Type> const & other
		) const;

	//! True if this instance is the same as other within tolerance
	inline
	bool
	nearlyEquals
		( Region<Dim, Type> const & other
		, Type const & tol = dat::smallValue<Type>()
		) const;

	//! Descriptive information about this instance.
	inline
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

//! Classic 1D region
template <typename Type>
using Length = Region<1u, Type>;

//! Classic 2D region
template <typename Type>
using Area = Region<2u, Type>;

//! Classic 3D region
template <typename Type>
using Volume = Region<3u, Type>;

}


// Inline definitions
#include "libdat/Region.inl"

#endif // dat_Region_INCL_

