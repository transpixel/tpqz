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

#ifndef cam_Camera_INCL_
#define cam_Camera_INCL_

/*! \file
\brief Declarations for cam::Camera
*/


#include "libcam/PinHole.h"
#include "libdat/algorithm.h"
#include "libdat/Area.h"
#include "libdat/Extents.h"
#include "libdat/Spot.h"
#include "libga/ga.h"


#include <array>
#include <string>


namespace cam
{

/*! \brief Camera system assembly (body, detector, optics)

\par Example
\dontinclude testcam/uCamera.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Camera
{
public: //data

	// TODO - if need copy, probably should use shared_ptr (make into baseclass)
	PinHole const theOptics{};
	dat::Extents const theDetSize{};
	dat::Area<double> const theAreaInImg{}; // symmetric about optical axis
	dat::Area<double> const theAreaInDet{}; // matches dat::Extents

public: // methods

	//! default null constructor
	Camera
		() = default;

	//! Camera with PC centered on detector
	explicit
	Camera
		( double const & pd
		, dat::Extents const & detSize
		);

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Directions (in camera frame) corresponding to detector corners
	std::array<ga::Vector, 4u>
	cornerDirections
		() const;

	//! Image location for detector location
	inline
	dat::Spot
	imageSpotFor
		( dat::Spot const & detspot
		) const;

	//! Detector location for image location
	inline
	dat::Spot
	detectorSpotFor
		( dat::Spot const & imgspot
		) const;

	//! Quantized detector location for image spot - or null if out of area
	inline
	dat::RowCol
	detectorRowColFor
		( dat::Spot const & imgspot
		) const;

	//! Projected location of objpnt (unbounded by detector)
	inline
	dat::Spot
	imageSpotFor
		( ga::Vector const & objpnt
		) const;

	//! Detector location for object point - or null if out of area
	inline
	dat::RowCol
	detectorRowColFor
		( ga::Vector const & objpnt
		) const;

	//! True if this rowcol is within detector
	inline
	bool
	isVisible
		( dat::Spot const & imgspot
		) const;

	//! Direction (w.r.t camera frame) associated with image spot
	inline
	ga::Vector
	directionOf
		( dat::Spot const & imgspot
		) const;

	/*! Primary viewing direction (i.e. along the optical axis away from cam).
	 * Convenience for: directionOf(dat::Spot{{0., 0.}});
	 */
	inline
	ga::Vector
	lookDir
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = {}
		) const;

};

}

// Inline definitions
#include "libcam/Camera.inl"

#endif // cam_Camera_INCL_

