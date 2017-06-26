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

#ifndef cam_ViewPyramid_INCL_
#define cam_ViewPyramid_INCL_

/*! \file
\brief Declarations for cam::ViewPyramid
*/


#include "libcam/Camera.h"
#include "libdat/validity.h"
#include "libga/Rigid.h"
#include "libgeo/Triangle.h"

#include <string>
#include <vector>


namespace cam
{

/*! \brief Representation of a classic rectangular field of view.

\par Example
\dontinclude testcam/uViewPyramid.cpp
\skip ExampleStart
\until ExampleEnd
*/

struct ViewPyramid
{

	Camera const theCamera{};
	double const theEdgeMag{ dat::nullValue<double>() };

public: // methods

	//! A null instance
	ViewPyramid
		() = default;

	//! Value ctor
	explicit
	ViewPyramid
		( Camera const & camera
		, double const & edgeMag
		);

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! FOV pyramid expressed in (this') camera frame
	std::vector<geo::Triangle>
	trianglesInCam
		() const;

	//! FOV pyramid expressed in arbitrary reference frame
	std::vector<geo::Triangle>
	trianglesInRef
		( ga::Rigid const & xRefWrtCam
		) const;


	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

};

}

// Inline definitions
// #include "libcam/ViewPyramid.inl"

#endif // cam_ViewPyramid_INCL_

