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

#ifndef dat_Range_INCL_
#define dat_Range_INCL_

/*! \file
\brief Declarations for dat::Range
*/


#include "libdat/limits.h"
#include "libdat/template.h"
#include "libdat/validity.h"

#include <algorithm>
#include <string>
#include <utility>


namespace dat
{


//! Wrapper around std::pair that provides type safety and various operations
template <typename Type>
class Range
{

	std::pair<Type, Type> theMinMax
		{ dat::nullValue<Type>(), dat::nullValue<Type>() };

public: // data

	//! Range (half open) suitable for uint8_t sample data
	static Range<Type> const uint8;

//======================================================================

public: // static methods

	//! Range spanning a collection of values
	template <typename FwdIter>
	inline
	static
	Range<Type>
	spanning
		( FwdIter const & itBeg
		, FwdIter const & itEnd
		, Type const & endEpsilon = dat::smallValue<Type>()
			//! Move far end this much beyond maxium value in the collection
		);

	//! Range spanning a collection of values
	template <typename FwdIter, typename ValueFunc>
	inline
	static
	Range<Type>
	spanning
		( FwdIter const & itBeg
		, FwdIter const & itEnd
		, ValueFunc const & valueGetter //!< pull value from collection
		, Type const & endEpsilon = dat::smallValue<Type>()
			//! Move far end this much beyond maxium value in the collection
		);

public: // methods

	//! default null constructor
	inline
	Range
		() = default;

	//! Value construction
	inline
	explicit
	Range
		( Type const & minValue
		, Type const & maxValue
		);

	//! Value construction
	inline
	explicit
	Range
		( std::pair<Type, Type> const & range
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Standard pair representation
	inline
	operator std::pair<Type, Type>
		() const;

	//! Range end points as a pair
	inline
	std::pair<Type, Type> const &
	pair
		() const;

	//! Access to endpoints ([0]==min(), [1]==max())
	inline
	Type
	operator[]
		( size_t const & ndx
		) const;

	//! Minimum end point (same as .first) treated as Included
	inline
	Type
	min
		() const;

	//! Maximum end point (same as .second) treated as Excluded
	inline
	Type
	max
		() const;

	//! Magnitude of range (max() - min())
	inline
	Type
	magnitude
		() const;

	//! Value half way between max() and min() for real data types
	inline
	Type
	middle
		() const;

	//! True if value is within range (min() <= value < max())
	inline
	bool
	contains
		( Type const & value
		) const;

	//! Range in common between this and other
	inline
	Range<Type>
	intersectWith
		( Range<Type> const & other
		) const;

	//! Range with max() expanded by deltaMax
	inline
	Range<Type>
	endExpanded
		( Type const & endDelta = dat::smallValue<Type>()
		) const;

	//! True if this is same as other within tolerance
	inline
	bool
	nearlyEquals
		( Range<Type> const & other
		, Type const & tol = dat::smallValue<Type>()
		) const;

	//! Descriptive information about this instance.
	inline
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

template <typename Type>
Range<Type> const Range<Type>::uint8(0., 256.);

}

// Inline definitions
#include "libdat/Range.inl"

#endif // dat_Range_INCL_


