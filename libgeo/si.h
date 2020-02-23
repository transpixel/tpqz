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

#ifndef geo_si_INCL_
#define geo_si_INCL_

/*! \file
\brief Declarations for geo::si
*/


#include "libgeo/Plane.h"
#include "libgeo/Ray.h"

#include <vector>


namespace geo
{

/*! \brief Space intersection operations

\par Example
\dontinclude testgeo/usi.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace si
{
	using ObsWeight = double;
	using WRay = std::pair<ObsWeight, geo::Ray>;
	using WPlane = std::pair<ObsWeight, geo::Plane>;

	//! Uncertain observation data
	template <typename GeoType>
	struct Obs
	{
		GeoType theGeoItem;
		double theDirSigma;
		double theStaSigma;

		//! Weight for this ray based on nominal range
		inline
		double
		obsWeight
			( double const & nomRange
			) const;

		//! Descriptive information about this instance
		inline
		std::string
		infoString
			( std::string const & title = {}
			) const;

	}; // ObsRay

	//! Normal system singular geometry (e.g. inverse ellipsoid axes)
	struct SemiAxis
	{
		double theMag;
		ga::Vector theDir;

		//! Descriptive information about this instance
		std::string
		infoString
			( std::string const & title = {}
			) const;

		//! Full vectos (product of Mag and Dir)
		ga::Vector
		asVector
			() const;

	};  // Singular Axis

	//! Data container for a 3D point and uncertainty
	struct PointSoln
	{
		ga::Vector const theLoc;
		std::array<SemiAxis, 3u> const theSemiAxes;

		//! Descriptive information about this instance
		std::string
		infoString
			( std::string const & title = {}
			) const;

		//! Vector semi-axis (from theLoc, to ndx-th tip on stdEllipsoid)
		SemiAxis
		kthLargestSemiAxis
			( size_t const & ndx  //!< 0,1,2: smallest to largest
			) const;

		//! Coordinates for endpoints of semiaxes (6 pnts)
		std::vector<ga::Vector>
		ellipsoidTips
			() const;

	}; // PointSoln

	using Dyadic = std::array<double, 9u>;
	using RhsVec = std::array<double, 3u>;

	//! Tensor dyadic product formation - Container must have op[]() [0,1,2]
	template <typename Container>
	inline
	Dyadic
	planeDyadicFor
		( Container const & dir
		);

	//! Tensor dyadic product formation - Container must have op[]() [0,1,2]
	template <typename Container>
	inline
	Dyadic
	rayDyadicFor
		( Container const & dir
		);

	//! Normal equation system
	class PointSystem
	{
		Dyadic theNormCo{};
		RhsVec theNormRhs{};
		size_t theNumRays{ 0u };
		size_t theNumPlanes{ 0u };

	public: // methods

		//! Construct with fully zeroed accumulation buffers
		PointSystem
			();

		//! Incorporate weighted dyadic observation into normal coeff matrix
		inline
		void
		addWeightedDyadic
			( double const & weightSq
			, Dyadic const & bigQ
			);

		//! Incorporate weighted dyadic observation into RHS of normal system
		inline
		void
		addWeightedRhs
			( double const & weightSq
			, Dyadic const & bigQ
			, ga::Vector const & vec
			);

		//! Incorporate a collection of weighted ray observations
		void
		addWeightedRays
			( std::vector<WRay> const & wrays
			);

		//! Incorporate a collection of weighted plane observations
		void
		addWeightedPlanes
			( std::vector<WPlane> const & wplanes
			);

		//! Least squares solution (or as close as it gets)
		PointSoln
		pointSolution
			() const;

		//! Descriptive information about this instance
		std::string
		infoString
			( std::string const & title = {}
			) const;

	}; // PointSystem

} // si

} // geo

// Inline definitions
#include "libgeo/si.inl"

#endif // geo_si_INCL_

