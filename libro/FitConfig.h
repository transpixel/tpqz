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

#ifndef ro_FitConfig_INCL_
#define ro_FitConfig_INCL_

/*! \file
\brief Declarations for ro::FitConfig
*/


#include "libmath/math.h"

#include <cstddef>
#include <string>


namespace ro
{

/*! \brief Configuration parameters for data fitting operations

\par Example
\dontinclude testro/uFitConfig.cpp
\skip ExampleStart
\until ExampleEnd
*/

struct FitConfig
{
	double const theMaxCondNum{ 1.e6 }; //!< discard solns less stable
	size_t const theMaxItCount{ 25u }; //!< should only need a handful
	double const theConvergeGap{ math::eps }; //!< convergence tolerance

public: // methods

	//! default null constructor
	FitConfig
		() = default;

	//! Override default values
	explicit
	FitConfig
		( double const & maxCondNum
		);

	//! Override default values
	explicit
	FitConfig
		( double const & maxCondNum
		, size_t const & maxItCount
		);

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // FitConfig

} // ro

// Inline definitions
// #include "libro/FitConfig.inl"

#endif // ro_FitConfig_INCL_

