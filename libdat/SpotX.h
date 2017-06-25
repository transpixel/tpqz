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

#ifndef dat_SpotX_INCL_
#define dat_SpotX_INCL_

/*! \file
\brief Declarations for dat::SpotX
*/


#include "libdat/Spot.h"

#include <string>


namespace dat
{

/*! \brief Coordinate transformation capabilities relative to dat::Spot.

Current implementation support coordinate system translation only.

\par Example
\dontinclude testdat/uSpotX.cpp
\skip ExampleStart
\until ExampleEnd
*/

class SpotX
{

	dat::Spot theTrans{{ dat::nullValue<double>(), dat::nullValue<double>() }};

public: // static methods

	//! Identity transform instance
	static
	SpotX
	identity
		();

public: // methods

	//! default null constructor
	SpotX
		() = default;

	//! Value constructor.
	explicit
	SpotX
		( dat::Spot const & transSpot
		);

	//! Value constructor.
	explicit
	SpotX
		( dat::RowCol const & transRC
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Transform domain value into range value
	inline
	dat::Spot
	operator()
		( dat::Spot const & xInDomain
		) const;

	//! Resultant composition of this transform applied after prior
	SpotX
	compositeAfter
		( SpotX const & prior
		) const;

	//! Composition that undoes this one
	SpotX
	inverse
		() const;

	//! True if this instance produces the same effect as other within tol
	bool
	nearlyEquals
		( SpotX const & other
		, double const & tol = smallValue<double>()
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
#include "libdat/SpotX.inl"

#endif // dat_SpotX_INCL_

