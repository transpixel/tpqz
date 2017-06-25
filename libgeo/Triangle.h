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

#ifndef geo_Triangle_INCL_
#define geo_Triangle_INCL_

/*! \file
\brief Declarations for geo::Triangle
*/


#include "libga/ga.h"

#include <array>
#include <string>


namespace geo
{

/*! \brief Triangle in 3D space

\par Example
\dontinclude testgeo/uTriangle.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Triangle
{
	std::array<ga::Vector, 3u> theVerts;

public: // methods

	//! default null constructor
	Triangle
		();

	//! value construction
	explicit
	Triangle
		( ga::Vector const & v1
		, ga::Vector const & v2
		, ga::Vector const & v3
		);

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Access to vertex by index
	ga::Vector const &
	operator[]
		( size_t const & ndx
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = {}
		) const;
};

}

// Inline definitions
// #include "libgeo/Triangle.inl"

#endif // geo_Triangle_INCL_

