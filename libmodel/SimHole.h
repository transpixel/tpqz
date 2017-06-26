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

#ifndef model_SimHole_INCL_
#define model_SimHole_INCL_

/*! \file
\brief Declarations for model::SimHole
*/


#include "libdat/Range.h"
#include "libga/ga.h"
#include "libgeo/Ray.h"
#include "libmodel/Part.h"

#include <string>
#include <utility>


namespace model
{

namespace sim
{
	// hole dimensions (in meters)
	constexpr double sHoleTopZ{ -1.000 }; // z-value at bottom of hole
	constexpr double sHoleBotZ{ -3.000 }; // z-value at top of hole
	constexpr double sHoleRadius{ .500 }; // hole radius
	constexpr double sHoleRadDelta{ sHoleRadius/4. }; // undulation of surface
}

/*! \brief Simulated manhole surface.

\par Example
\dontinclude testmodel/uSimHole.cpp
\skip ExampleStart
\until ExampleEnd
*/
struct SimHole
{

private:

	dat::Range<double> const theRangeZ{ sim::sHoleBotZ, sim::sHoleTopZ };
	double const theRadius{ sim::sHoleRadius };
	static constexpr double theDeltaRad{ sim::sHoleRadDelta };

public: // methods expected to be present (for generic sensing libs)

	//! Geometric intersection of ray with model (e.g. surface/ray meet)
	ga::Vector
	locationFor
		( geo::Ray const & rayInModel
		) const;

	//! Component of model at this location
	Part
	partFor
		( ga::Vector const & locInModel
		) const;

public: // methods expected to be present (for demonstration programs)

	//! Location on centerline half-way between top and bottom
	ga::Vector
	centroid
		() const;

	//! Range of "z" coordinate values
	dat::Range<double>
	zRange
		() const;

	//! Characteristic dimension: all parts within this distance of centroid
	double
	maxRadius
		() const;

public: // query methods

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Range of vertical values within this model
	double
	depthSpan
		() const;

	//! Approximate horizontal perimeter of the "hole" model
	double
	perimeterSpan
		() const;

	//! Nominal radius of hole
	double
	nominalRadius
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

private: // simulation-intrinsic methods

	//! Horizontal radius from center(0,0) along direction
	inline
	double
	hRadiusToward
		( std::pair<double, double> const & azimCosSin
		) const;

	//! Horizontal radius values that bracket intersection with ray
	std::pair<double, double>
	hRangeBrackets
		( geo::Ray const & hRay
		) const;

	//! Merit function for hRange root-finding operations
	std::pair<double, double>
	hMeritNegPos
		( ga::Vector const & hPntMin
		, ga::Vector const & hPntMax
		) const;

	//! Geometric intersection of horizontal ray with model surface
	ga::Vector
	horizontalXY
		( geo::Ray const & rayInModel
		) const;

	//! Geometric intersection of horizontal ray with model surface
	ga::Vector
	horizontalXYZ
		( geo::Ray const & rayInModel
		) const;

};

}

// Inline definitions
// #include "libmodel/SimHole.inl"

#endif // model_SimHole_INCL_

