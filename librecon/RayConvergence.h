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

#ifndef recon_RayConvergence_INCL_
#define recon_RayConvergence_INCL_

/*! \file
\brief Declarations for recon::RayConvergence
*/


#include "libgeo/Ray.h"
#include "libga/ga.h"

#include <limits>
#include <string>
#include <vector>


namespace recon
{

/*! \brief Collection of rays assumed to intersect at a common point

\par Example
\dontinclude testrecon/uRayConvergence.cpp
\skip ExampleStart
\until ExampleEnd
*/

class RayConvergence
{

	std::vector<geo::Ray> theRays{};

public: // methods

	//! default null constructor
	RayConvergence
		() = default;

	//! Value ctor
	explicit
	RayConvergence
		( std::vector<geo::Ray> const & rays
		);

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Robustly estimated intersection point
	ga::Vector
	robustPoint
		( double const minAngle = 1./8.
		, std::vector<double> * const & ptGapMags = nullptr
		) const;

	//! Intersection of rays that approach trialPoint within tolerance
	ga::Vector
	meanNearTo
		( ga::Vector const & evalPoint = {} //!< if null, use robustPoint()
		, double const & maxRejTol = std::numeric_limits<double>::max()
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // RayConvergence

} // recon

// Inline definitions
// #include "librecon/RayConvergence.inl"

#endif // recon_RayConvergence_INCL_

