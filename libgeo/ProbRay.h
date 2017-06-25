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

#ifndef geo_ProbRay_INCL_
#define geo_ProbRay_INCL_

/*! \file
\brief Declarations for geo::ProbRay
*/


#include "libdat/validity.h"
#include "libgeo/Ray.h"
#include "libmath/math.h"
#include "libmath/Partition.h"

#include <string>
#include <vector>


namespace geo
{

/*! \brief Probabilistic representation of depth along a ray

\par Example
\dontinclude testgeo/uProbRay.cpp
\skip ExampleStart
\until ExampleEnd
*/

class ProbRay
{

public: // data

	geo::Ray theRay{};
	math::Partition thePart{};
	std::vector<double> theProbs{};
	double theDirSigma{ dat::nullValue<double>() };

public: // static methods

	//! Construction of ray within an environment of conical measurements
	template <typename FwdIter>
	static
	inline
	ProbRay
	fromCones
		( FwdIter const & beg //!< *iter = geo::Ray (define cone w.r.t. axis)
		, FwdIter const & end
		, FwdIter const & primary //!< between [beg,end): Probs for this ray
		, math::Partition const & probPart
		, ga::Vector const & coneAxis
		, double const & coneSigma
		);

	//! Construction of ray within an environment
	template <typename FwdIter>
	static
	inline
	ProbRay
	from
		( FwdIter const & beg //!< *iter = geo::Ray
		, FwdIter const & end
		, FwdIter const & primary //!< between [beg,end): Probs for this ray
		, math::Partition const & probPart
		, double const & rayDirSigma // = { (1./1024.) * math::pi }
		);

public: // methods

	//! default null constructor
	ProbRay
		() = default;

	//! Value construction
	explicit
	ProbRay
		( geo::Ray const & ray
		, math::Partition const & probPart
		, double const & rayDirSigma // = { (1./1024.) * math::pi }
		);

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Number of samples in probability distributions
	inline
	size_t
	numSamples
		() const;

	//! Incorporate point into probability
	void
	considerPoint
		( ga::Vector const & pnt
		, double const & pntSigma
		);

	//! Incorporate other ray into probability
	void
	considerRay
		( geo::Ray const & ray
		, double const & raySigma
		);

	//! Incorporate (only) the conical information of another ray
	void
	considerCone
		( geo::Ray const & rayOnCone
		, ga::Vector const & coneAxisDir
		, double const & apexSigma
		);

	//! Distance along ray, probability of that distance
	using DistProb = std::pair<double, double>;

	//! Sampling of (distance along ray, probability of this distance)
	std::vector<DistProb>
	distProbs
		() const;

	/*! Most probable (mode) depth along ray and associated (quasi) probability
	 *
	 * Provided probability has at least one peak value (probMin < probMax)
	 * return location of that peak (first one if multiple peaks).
	 *
	 */
	std::pair<double, double>
	likelyDistProb
		() const;

	//! Most probable (mode) depth along ray
	double
	likelyDistance
		() const;

	//! Point at likelyDistance along theRay
	ga::Vector
	likelyPoint
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

	//! Save Dist,Prob values to stream in ascii format
	void
	saveDistProbs
		( std::ostream & oss
		, std::string const & fmt = { "%9.6f" }
		) const;

private:

	//! Angle substended between raydir and pnt at ray start
	static
	double
	subtendedAngleFor
		( ga::Vector const & pnt
		, geo::Ray const & ray
		);

	//! Probability of deviation by angleMag from ray
	static
	inline
	double
	pseudoProbFor
		( double const & value
		, double const & sigma
		);


}; // ProbRay

} // geo

// Inline definitions
#include "libgeo/ProbRay.inl"

#endif // geo_ProbRay_INCL_

