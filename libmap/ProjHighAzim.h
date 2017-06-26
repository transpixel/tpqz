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

#ifndef map_ProjHighAzim_INCL_
#define map_ProjHighAzim_INCL_

/*! \file
\brief Declarations for map::ProjHighAzim
*/

#include "libmap/Proj.h"

#include "libmath/Partition.h"

#include "libgeo/sphere.h"


namespace map
{

/*! \brief Simple cylindrical projection.

\par Example
\dontinclude testmap/uProjHighAzim.cpp
\skip ExampleStart
\until ExampleEnd
*/

class ProjHighAzim : public Proj
{
	math::Partition const thePartSinElv{};
	math::Partition const thePartA{};
	std::pair<double, double> const theSinElvPair{};

private:

	//! True if sinElv is within active cylinder height
	inline
	bool
	inRange
		( double const & sinElv
		) const;

public: // methods

	//! default null constructor
	ProjHighAzim
		() = default;

	//! Map range of zenith angles onto cylinder height
	explicit
	ProjHighAzim
		( dat::Extents const & hwSize
		, double const & sinElv = std::sin(math::qtrPi)
		);

	//! Noop dtor
	~ProjHighAzim
		() = default;

	//! [Virtual] Raster location for a direction
	inline
	virtual
	dat::RowCol
	gridRowColFor
		( ga::Vector const & dir
		) const;

	//! [Virtual] Direction recovered from raster location
	inline
	virtual
	ga::Vector
	directionFor
		( dat::RowCol const & mapRC
		) const;

};

}

// Inline definitions
#include "libmap/ProjHighAzim.inl"

#endif // map_ProjHighAzim_INCL_

