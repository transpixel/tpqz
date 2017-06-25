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

#ifndef trans_RangeMap_INCL_
#define trans_RangeMap_INCL_

/*! \file
\brief Declarations for trans::RangeMap
*/


#include "libdat/Range.h"

#include <functional>
#include <string>
#include <utility>


namespace trans
{

/*! \brief Transformation function mapping one range onto another.

NOTE: the mapping function is assumed to map the unit input range
[0,1) onto the unit output range [0,1).

The input range values are first transformed into a fraction. The unit
map is applied to that fraction. The results are transformed to fill the
output range.

\par Example
\dontinclude testtrans/uRangeMap.cpp
\skip ExampleStart
\until ExampleEnd
*/

class RangeMap
{

public: // data

	dat::Range<double> theDomain;
	std::function<double(double const &)> theUnitMap;
	dat::Range<double> theRange;

public: // methods

	//! default null constructor
	inline
	RangeMap
		();

	//! Value constructor
	inline
	explicit
	RangeMap
		( dat::Range<double> const & domain
		, std::function<double(double const &)> const & unitMapping
		, dat::Range<double> const & range = dat::Range<double>()
			//!< If null, set to same as domain
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! RangeMap value associated with arg (or null if arg out of range)
	inline
	double
	operator()
		( double const & arg
		) const;

	//! Descriptive information about this instance.
	inline
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
#include "libtrans/RangeMap.inl"

#endif // trans_RangeMap_INCL_

