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

#ifndef model_PointCloud_INCL_
#define model_PointCloud_INCL_

/*! \file
\brief Declarations for model::PointCloud
*/


#include "libdat/validity.h"
#include "libgeo/Ray.h"
#include "libmodel/atom.h"
#include "libmodel/Part.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace model
{

/*! \brief Simple collection of 3D points.

\par Example
\dontinclude testmodel/uPointCloud.cpp
\skip ExampleStart
\until ExampleEnd
*/

class PointCloud
{
	//! Tolerance to consider a point proximal
	double theProxTol{ dat::nullValue<double>() };

	std::vector<atom::Point> thePoints;

private: // disable -- can be arbitrarily large

	//! Disable implicit copy and assignment
	PointCloud(PointCloud const &) = delete;
	PointCloud & operator=(PointCloud const &) = delete;

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

	//! Centroid coordinates expressed in domain of definition
	ga::Vector
	centroid
		() const;

	//! Range of elevation values present in the data
	dat::Range<double>
	zRange
		() const;

	//! Characteristic dimension: all parts within this distance of centroid
	double
	maxRadius
		() const;

public: // methods

	//! default null constructor
	PointCloud
		() = default;

	//! Value ctor
	explicit
	PointCloud
		( double const & proximityTolerance
		, size_t const & expectedSize = dat::nullValue<size_t>()
			//!< if !=0, reserve this much total space
		);

	//! Add contents from ascii CSV formatted input <x,y,z[arbitrary]>
	bool
	addAsciiCSV
		( std::istream & istrm
		, size_t const & expectedSize = dat::nullValue<size_t>()
			//!< if !=0, expect this much data from istream
		);

	//! Check if instance is valid (NOTE: may still be empty)
	bool
	isValid
		() const;

	//! Number of points in this model
	size_t
	size
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

};

}

// Inline definitions
// #include "libmodel/PointCloud.inl"

#endif // model_PointCloud_INCL_

