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

#ifndef geo_Coordinates_INCL_
#define geo_Coordinates_INCL_

/*! \file
\brief Declarations for geo::Coordinates
*/


#include "libga/ga.h"

#include <array>
#include <string>
#include <vector>


namespace geo
{

/*! \brief Operations on independent coordinate (components).

\par Example
\dontinclude testgeo/uCoordinates.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Coordinates
{
	std::array<std::vector<double>, 3u> theComps;

public: // methods

	//! default null constructor
	Coordinates
		() = default;

	//! Construct with reserved size
	explicit
	Coordinates
		( size_t const & estSize
		);

	//! Incorporate coordinates from pnt
	void
	addPoint
		( ga::Vector const & pnt
		);

	//! Point comprised of individual coordinate medians
	ga::Vector
	componentMedianPoint
		() const;

}; // Coordinates

} // geo

// Inline definitions
// #include "libgeo/Coordinates.inl"

#endif // geo_Coordinates_INCL_

