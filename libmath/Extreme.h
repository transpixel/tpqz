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

#ifndef math_Extreme_INCL_
#define math_Extreme_INCL_

/*! \file
\brief Declarations for math::Extreme
*/


#include "libdat/validity.h"

#include <iterator>
#include <string>


namespace math
{

/*! \brief Functor for finding extreme values

\par Example
\dontinclude testmath/uExtreme.cpp
\skip ExampleStart
\until ExampleEnd
*/

template <typename FwdIter>
class Extreme
{

public: // data

	using ValueType = typename std::iterator_traits<FwdIter>::value_type;

	ValueType theExVal;
	FwdIter theExIter;

public: // methods

	//! default null constructor
	inline
	Extreme
		();

	//! Construct from collection
	template <typename CompFunc>
	inline
	explicit
	Extreme
		( FwdIter const & beg
		, FwdIter const & end
		, CompFunc const & comp
		);

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Descriptive information about this instance.
	inline
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

private:

	//! Update extreme tracking with element
	template <typename CompFunc>
	inline
	bool
	shouldUpdate
		( ValueType const & elem
		, CompFunc const & comp
		);

};

}

// Inline definitions
#include "libmath/Extreme.inl"

#endif // math_Extreme_INCL_

