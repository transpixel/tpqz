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

#ifndef tri_Domain_INCL_
#define tri_Domain_INCL_

/*! \file
\brief Declarations for tri::Domain
*/


#include "libdat/Area.h"

#include <string>


namespace tri
{

/*! \brief Representation of region over which tritille nodes should be valid.

\par Example
\dontinclude testtri/uDomain.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Domain
{

protected: // data

	dat::Area<double> theAreaBounds{};

public: // methods

	//! default null constructor
	Domain
		() = default;

	//! Defined via rectangular area
	explicit
	Domain
		( dat::Area<double> const & area
		);

	//! Noop dtor
	virtual
	~Domain
		() = default;

	//! Bounding area of valid domain region
	dat::Area<double>
	areaBounds
		() const;

public: // virtual methods

	//! True if instance is valid
	virtual
	bool
	isValid
		() const;

	//! True if xyLoc is within valid region of domain
	virtual
	bool
	contains
		( dat::Spot const & xyLoc
		) const;

	//! Descriptive information about this instance.
	virtual
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // Domain

} // tri

// Inline definitions
// #include "libtri/Domain.inl"

#endif // tri_Domain_INCL_

