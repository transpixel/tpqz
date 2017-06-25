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

#ifndef math_MapSizeArea_INCL_
#define math_MapSizeArea_INCL_

/*! \file
\brief Declarations for math::MapSizeArea
*/


#include "libdat/Area.h"
#include "libdat/Extents.h"
#include "libdat/Spot.h"

#include <string>


namespace math
{

/*! \brief Isomorphic mapping between Extents and Area coordinate frames

\par Example
\dontinclude testmath/uMapSizeArea.cpp
\skip ExampleStart
\until ExampleEnd
*/

class MapSizeArea
{

	dat::Area<double> theAreaHW{};
	dat::Area<double> theAreaXY{};

public: // methods

	//! default null constructor
	MapSizeArea
		() = default;

	//! Value ctor
	explicit
	MapSizeArea
		( dat::Extents const & hwSize
		, dat::Area<double> const & xyArea
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Spot within hwSize corresponding to area location
	dat::Spot
	hwSizeSpotForNoCheck
		( dat::Spot const & xyAreaSpot
		) const;

	//! Spot within area corresponding to hwSize location
	dat::Spot
	xyAreaSpotForNoCheck
		( dat::Spot const & hwSizeSpot
		) const;

	//! Spot within hwSize corresponding to area location
	dat::Spot
	hwSizeSpotFor
		( dat::Spot const & xyAreaSpot
		) const;

	//! Spot within area corresponding to hwSize location
	dat::Spot
	xyAreaSpotFor
		( dat::Spot const & hwSizeSpot
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // MapSizeArea

} // math

// Inline definitions
// #include "libmath/MapSizeArea.inl"

#endif // math_MapSizeArea_INCL_

