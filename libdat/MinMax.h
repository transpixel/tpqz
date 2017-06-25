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

#ifndef dat_MinMax_INCL_
#define dat_MinMax_INCL_

/*! \file
\brief Declarations for dat::MinMax
*/


#include "libdat/dat.h"

#include <string>
#include <utility>


namespace dat
{

/*! \brief Simple *floating-point* container for Min/Max values.

\par Example
\dontinclude testdat/uMinMax.cpp
\skip ExampleStart
\until ExampleEnd
*/

template <typename Type>
class MinMax
{
	std::pair<Type, Type> theMinMax;

public: // methods

	//! default null constructor
	inline
	MinMax
		();

	//! Value ctor
	inline
	explicit
	MinMax
		( Type const & min
		, Type const & max
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Minimum value
	inline
	Type
	min
		() const;

	//! Maximum value
	inline
	Type
	max
		() const;

	//! Range of entire partition
	inline
	std::pair<Type, Type>
	pair
		() const;

	//! True if this is same as other within tol
	inline
	bool
	nearlyEquals
		( MinMax<Type> const & other
		, Type const & tol = smallValue<Type>()
		) const;

	//! True if value is [min() <= value <= max())
	inline
	bool
	containsInclusive
		( Type const & value
		) const;

	//! An instance expanded to contain value as well as this
	inline
	MinMax<Type>
	expandedWith
		( Type const & value
		) const;

	//! An instance expanded to contain value as well as this
	inline
	MinMax<Type>
	expandedWith
		( MinMax<Type> const & other
		) const;

	//! Descriptive information about this instance.
	inline
	std::string
	infoString
		( std::string const & title = std::string()
		, std::string const & fmt = std::string("%12.6f")
		) const;
};

}

// Inline definitions
#include "libdat/MinMax.inl"

#endif // dat_MinMax_INCL_

