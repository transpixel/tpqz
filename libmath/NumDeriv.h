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

#ifndef math_NumDeriv_INCL_
#define math_NumDeriv_INCL_

/*! \file
\brief Declarations for math::NumDeriv
*/


#include "libdat/validity.h"

#include <cmath>
#include <limits>
#include <sstream>
#include <string>
#include <vector>


namespace math
{

/*! \brief Construct to facilitate *CRUDE* numeric differentation

\par Example
\dontinclude testmath/uNumDeriv.cpp
\skip ExampleStart
\until ExampleEnd
*/

template <typename OutType>
class NumDeriv
{

private:

	double const theDelta{ dat::nullValue<double>() };
	double const theInvRun{ dat::nullValue<double>() };

public: // methods

	//! default null constructor
	inline
	NumDeriv // <OutType>
		( double const & eps = std::numeric_limits<double>::epsilon()
		);

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Estimate first derivative
	template <typename ArgType, typename Func>
	inline
	OutType
	derivative
		( ArgType const & val
		, ArgType const & del
		, Func const & func
		) const;

	//! A useful tolerance for comparison tests
	inline
	double
	tolerance
		() const;

	//! Perturbation in direction
	template <typename ArgType>
	inline
	ArgType
	deltaInDirOf
		( ArgType const & dir //!< (Generally) this should be unit magnitude
		) const;

	//! Collection of perturbations along directions
	template <typename ArgType>
	inline
	std::vector<ArgType>
	deltasFor
		( std::vector<ArgType> const & dirs //!< (Generally) unitary values
		) const;

	//! Descriptive information about this instance.
	inline
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // NumDeriv

} // math

// Inline definitions
#include "libmath/NumDeriv.inl"

#endif // math_NumDeriv_INCL_

