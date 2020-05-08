//
//
// MIT License
//
// Copyright (c) 2020 Stellacore Corporation.
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

#ifndef cam_InnerVertex_INCL_
#define cam_InnerVertex_INCL_

/*! \file
\brief Declarations for cam::InnerVertex
*/

#include "libdat/Extents.h"
#include "libdat/Spot.h"
#include "libga/ga.h"
#include "libgeo/VertGangle.h"
#include "libgeo/Wedge.h"

#include <utility>


namespace cam
{

/*! \brief Interior camera vertex geometry for particular detector size.

A kind of general function object for computing angle properties associated
with measurements on detector and scalar principal distance values.

\par Example
\dontinclude testcam/uInnerVertex.cpp
\skip ExampleStart
\until ExampleEnd
*/

struct InnerVertex
{
	dat::Extents const theDetSize{};
	dat::Spot const theCenterRC{};
	ga::Vector const theCenterVec{};

public: // methods

	//! default null constructor
	InnerVertex
		() = default;

	// Attach angle computations to specific detector geometry
	explicit
	InnerVertex
		( dat::Extents const & detSize
		);

	//! True if instance is valid
	bool
	isValid
		() const;

	//! Wedge associated with measurements subtended from exit pupil
	geo::Wedge
	wedgeFor
		( double const & pd
		, std::pair<dat::Spot, dat::Spot> const & meaPair
		) const;

	//! Full interior generalized vertex angle
	inline
	geo::VertGangle
	gangleFor
		( double const & pd
		, std::pair<dat::Spot, dat::Spot> const & meaPair
		) const;

	//! Scalar magnitude of interior vertex angle
	inline
	double
	angleMag
		( double const & pd
		, std::pair<dat::Spot, dat::Spot> const & meaPair
		) const;

}; // InnerVertex

} // cam

// Inline definitions
#include "libcam/InnerVertex.inl"

#endif // cam_InnerVertex_INCL_

