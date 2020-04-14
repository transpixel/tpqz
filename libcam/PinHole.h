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

#ifndef cam_PinHole_INCL_
#define cam_PinHole_INCL_

/*! \file
\brief Declarations for cam::PinHole
*/


#include "libdat/dat.h"
#include "libga/ga.h"
#include "libmath/math.h"

#include <string>


namespace cam
{

/*! \brief Ideal "pin-hole" perspective camera model

\par Example
\dontinclude testcam/uPinHole.cpp
\skip ExampleStart
\until ExampleEnd
*/

class PinHole
{

public: // data

	double thePD;

public: // methods

	//! default null constructor
	PinHole
		();

	//! Construct with specific principal distance
	explicit
	PinHole
		( double const & pd
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! (Unit) sensor direction associated with object point
	inline
	ga::Vector
	directionTo
		( ga::Vector const & objpnt
		) const;

	//! Detector location assocated with object point (3rd component == PD)
	inline
	dat::Spot
	imageSpotFor
		( ga::Vector const & objpnt
		) const;

	//! Direction associated with image location
	inline
	ga::Vector
	directionOf
		( dat::Spot const & imgspot
		) const;

	//! True if this instance and other are same within tolerance
	bool
	nearlyEquals
		( PinHole const & other
		, double const & tol = { math::eps }
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		, std::string const & fmt = std::string("%12.6f")
		) const;
};

}

// Inline definitions
#include "libcam/PinHole.inl"

#endif // cam_PinHole_INCL_

