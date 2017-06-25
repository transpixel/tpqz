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

#ifndef map_Proj_INCL_
#define map_Proj_INCL_

/*! \file
\brief Declarations for map::Proj
*/


#include "libdat/Extents.h"
#include "libdat/types.h"
#include "libga/ga.h"

#include <string>


namespace map
{

/*! \brief Standard interface for (raster) map projection operations.

\par Example
\dontinclude testmap/uProj.cpp
\skip ExampleStart
\until ExampleEnd
*/
class Proj
{

	dat::Extents theGridSize{};

public: // methods

	//! Construct an invalid instance
	Proj
		() = default;

	//! Create an instance associated with a specific size
	explicit
	Proj
		( dat::Extents const & hwSize
		);

	//! Size of this map
	dat::Extents const &
	hwSize
		() const;

	//
	// Virtual
	//

	//! Noop dtor
	virtual
	~Proj
		() = default;

	//! Check if instance is valid
	virtual
	bool
	isValid
		() const;

	//! [Virtual] Raster location for a direction
	virtual
	dat::RowCol
	gridRowColFor
		( ga::Vector const & dir
		) const;

	//! [Virtual] Direction recovered from raster location
	virtual
	ga::Vector
	directionFor
		( dat::RowCol const & mapRC
		) const;

	//! Descriptive information about this instance.
	virtual
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
// #include "libmap/Proj.inl"

#endif // map_Proj_INCL_

