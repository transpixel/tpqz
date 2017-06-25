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

#ifndef trans_DataMap_INCL_
#define trans_DataMap_INCL_

/*! \file
\brief Declarations for trans::DataMap
*/


#include "libdat/Range.h"
#include "libmath/interp.h"

#include <string>
#include <vector>


namespace trans
{

/*! \brief A unit-range to unit-range mapping function via a data collection.

The input values are assumed to be in unit range [0., 1.)

The input value is interpreted as a "fraction of the way into" the data
collection.

The (interpolated) data value is then rescaled such that valid values
are also in the unit range [0., 1.)

\par Example
\dontinclude testtrans/uDataMap.cpp
\skip ExampleStart
\until ExampleEnd
*/

class DataMap
{
	std::vector<double> theDataValues;
	dat::Range<double> theIndexRange;
	dat::Range<double> theDataRange;

public: // methods

	//! default null constructor
	DataMap
		();

	//! Value ctor.
	template <typename FwdIter>
	inline
	explicit
	DataMap
		( FwdIter const & dataBeg
		, FwdIter const & dataEnd
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Value associated with absicissa in the unit range [0., 1.)
	inline
	double
	operator()
		( double const & xUnit
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
#include "libtrans/DataMap.inl"

#endif // trans_DataMap_INCL_

