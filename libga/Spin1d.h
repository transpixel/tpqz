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

#ifndef ga_Spin1d_INCL_
#define ga_Spin1d_INCL_

/*! \file
\brief Declarations for ga::Spin1d
*/


#include "libga/ga.h"
#include "libdat/validity.h"

#include <string>
#include <utility>


namespace ga
{
/*! \brief Functor representing 1D family of solution spinors.

\par Example
\dontinclude testga/uSpin1d.cpp
\skip ExampleStart
\until ExampleEnd
*/

struct Spin1d
{
	ga::Vector const sVec{};
	ga::Vector const dVec{};

	// cached values
	std::pair<double, ga::Vector> const sMagDir{ dat::nullValue<double>(), {} };
	std::pair<double, ga::Vector> const dMagDir{ dat::nullValue<double>(), {} };
	ga::BiVector const dirD{};
	ga::BiVector const dirA0{};

	//! Default ctor
	Spin1d
		() = default;

	//! Spinor-valued function rotating x into y via (y = R * x * R.rev)
	explicit
	Spin1d
		( ga::Vector const & xx
		, ga::Vector const & yy
		);

	//! Spinor-valued function rotating X into Y via (y = R * x * R.rev)
	explicit
	Spin1d
		( ga::BiVector const & bivX
		, ga::BiVector const & bivY
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Spinor angle (NOTE! not physical angle)
	ga::BiVector
	argFor
		( double const & ww
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

private: // NOTE: there are special cases to check before calling these

	//! Spinor angle magnitude (NOTE! not phyical angle)
	double
	argMagFor
		( double const & ww
		) const;

	//! Spinor angle direction (NOTE! not phyical angle)
	ga::BiVector
	argDirFor
		( double const & ww
		) const;

}; // Spin1d

} // ga

// Inline definitions
// #include "libga/Spin1d.inl"

#endif // ga_Spin1d_INCL_

