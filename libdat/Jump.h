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

#ifndef dat_Jump_INCL_
#define dat_Jump_INCL_

/*! \file
\brief Declarations for dat::Jump
*/


#include "libdat/compare.h"
#include "libdat/limits.h"
#include "libdat/validity.h"

#include <string>
#include <vector>


namespace dat
{

/*! \brief Representation of a discontinuity in a collection.

\par Example
\dontinclude testdat/uJump.cpp
\skip ExampleStart
\until ExampleEnd
*/

struct Jump
{
	//! Offset into collection at which jump occurs
	size_t theNdx{ nullValue<size_t>() };

	//! Min value at the discontinuity
	double theLo{ nullValue<double>() };

	//! Max value at the discontinuity
	double theHi{ nullValue<double>() };

public: // static methods

	//! Extract jumps from data stream
	template <typename DataType, typename FwdIter>
	inline
	static
	std::vector<Jump>
	fromData
		( FwdIter const & dataBeg
		, FwdIter const & dataEnd
		, DataType const & tol = smallValue<DataType>()
		);

public: // methods

	//! default null constructor
	Jump
		() = default;

	//! Value constructor
	explicit
	Jump
		( size_t const & ndx
		, double const & lo
		, double const & hi
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Value same fraction between lo/hi as theNdx/maxNdx
	inline
	double
	mean
		() const;

	//! true if same as other within tol
	bool
	nearlyEquals
		( Jump const & other
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

};

}

// Inline definitions
#include "libdat/Jump.inl"

#endif // dat_Jump_INCL_

