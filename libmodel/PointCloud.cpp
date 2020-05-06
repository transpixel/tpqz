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


/*! \file
\brief Definitions for model::PointCloud
*/


#include "libmodel/PointCloud.h"

#include "libgeo/io.h"
#include "libimg/color.h"
#include "libmath/interp.h"
#include "libmodel/traits.h"



namespace model
{

	//! Functor for rejection of point from ray
	struct Proximity
	{
		geo::Ray const theRay;
		double theTolRejSq;

		//! Shortest distance from absPnt to theRay
		inline
		double
		operator()
			( atom::Point const & absPnt
			) const
		{
			double magRej{ dat::nullValue<double>() };
			ga::Vector const & orig = theRay.theStart;
			ga::Vector const & dir = theRay.theDir;
			ga::Vector const relPnt(absPnt - orig);
			double const magPrj{ ga::dot(relPnt, dir).theValue };
			if (0. < magPrj) // point in front of ray
			{
				// magSq == sq(projection) + sq(rejection)
				double const magVecSq
					{ math::sq(relPnt[0])
					+ math::sq(relPnt[1])
					+ math::sq(relPnt[2])
					};
				double const magPrjSq{ math::sq(magPrj) };
				double const magRejSq{ magVecSq - magPrjSq };
				if (magRejSq < theTolRejSq)
				{
					magRej = std::sqrt(magRejSq);
				}
			}

			return magRej;
		}
	};


namespace
{
	//! True if both rnA and rnB are valid and rnA is closer than rnB
	bool
	isCloser
		( std::pair<double, size_t> const & rnA
		, std::pair<double, size_t> const & rnB
		)
	{
		bool isLess{ false };
		double const & rejA = rnA.first;
		double const & rejB = rnB.first;
		// only compare valid entities
		if (dat::isValid(rejA) && dat::isValid(rejB))
		{
			isLess = (rejA < rejB);
		}
		return isLess;
	}
}


ga::Vector
PointCloud :: locationFor
	( geo::Ray const & rayInModel
	) const
{
	ga::Vector loc;

	if (! thePoints.empty())
	{
		// find closest point
		Proximity const prox{ rayInModel, math::sq(theProxTol) };

		// TODO replace with spatial index

		// TODO can be a single pass algorithm
		std::vector<std::pair<double, size_t> > proxNdxPairs;
		proxNdxPairs.reserve(thePoints.size());

		// compute proximity of each point
		for (size_t nn{0u} ; nn < thePoints.size() ; ++nn)
		{
			atom::Point const & point = thePoints[nn];
			double const rej{ prox(point) };
			if (dat::isValid(rej))
			{
				proxNdxPairs.emplace_back
					(std::pair<double, size_t>{ rej, nn });
			}
		}

		// find minimum valid proximity
		if (! proxNdxPairs.empty())
		{
			//! find nearest valid instance
			std::vector<std::pair<double, size_t> >::const_iterator const itMin
				( std::min_element
					(proxNdxPairs.begin(), proxNdxPairs.end(), isCloser)
				);
			double const & rejMin = itMin->first;
			double const & ndxMin = itMin->second;
			if (dat::isValid(rejMin))
			{
				if (rejMin < theProxTol)
				{
					loc = thePoints[ndxMin];
				}
			}
		}
	}

	return loc;
}

namespace
{
	//! Assign (fake) color to model locations
	inline
	atom::Color
	colorForLoc
		( ga::Vector const & locInModel
		)
	{
		atom::Color color(atom::nullColor);
		double const hRad{ math::hypot(locInModel[0], locInModel[1]) };
		if (0. < hRad)
		{
			constexpr std::pair<float, float> xyRange{ -1.0f, 1.0f };
			constexpr std::pair<float, float> outRangea{ -1.5f, 2.0f };
			constexpr std::pair<float, float> outRangeb{ -1.5f, 1.9f };
			// img::FPix const lab( 1.f, 0.f, 0.f );
			img::FPix const lab
				( .5f // (float)(locInModel[2])
				, math::interp::linear(xyRange, locInModel[0], outRangea)
				, math::interp::linear(xyRange, locInModel[1], outRangeb)
				);
			img::UPix8 const rgb(img::color::toLRGB8fromLab(lab, nullptr));
			color =
				{{ static_cast<float>(rgb[0])
				 , static_cast<float>(rgb[1])
				 , static_cast<float>(rgb[2])
				}};
		}
		return color;
	}
}

Part
PointCloud :: partFor
	( ga::Vector const & locInModel
	) const
{
	atom::Color const color(colorForLoc(locInModel));
	model::traits::Radiometry const radTraits{ color };
	static model::traits::Geometry const geoTraits{};
	return Part{ locInModel, radTraits, geoTraits };
}

ga::Vector
PointCloud :: centroid
	() const
{
	ga::Vector mean;
	if (! thePoints.empty())
	{
		ga::Vector sum{ ga::vZero };
		for (atom::Point const & point : thePoints)
		{
			sum = sum + point;
		}
		mean = (1./double(thePoints.size())) * sum;
	}
	return mean;
}

dat::Range<double>
PointCloud :: zRange
	() const
{
	return dat::Range<double>::spanning
		( thePoints.begin()
		, thePoints.end()
		, [] (atom::Point const & point) { return point[2]; }
		);
}

double
PointCloud :: maxRadius
	() const
{
	double maxRad{ dat::nullValue<double>() };
	ga::Vector const center(centroid());
	if (dat::isValid(center))
	{
		assert(! thePoints.empty()); // else centroid should be invalid
		// initialize radius to first point
		maxRad = ga::magnitude(thePoints[0] - center);
		for (atom::Point const & point : thePoints)
		{
			// initialize radius to first point
			double const currRad{ ga::magnitude(point - center) };
			maxRad = std::max(maxRad, currRad);
		}
	}
	return maxRad;
}

// explicit
PointCloud :: PointCloud
	( double const & proximityTolerance
	, size_t const & expectedSize
	)
	: theProxTol{ proximityTolerance }
	, thePoints{}
{
	if (dat::isValid(expectedSize))
	{
		thePoints.reserve(expectedSize);
	}
}

bool
PointCloud :: addAsciiCSV
	( std::istream & istrm
	, size_t const & expectedSize
	)
{
	// TODO - could be optimized further
	//        e.g. to emplace decoded ascii directly into thePoints
	std::vector<ga::Vector> const pnts
		(geo::io::loadFromCSV(istrm, expectedSize));
	thePoints.insert(thePoints.end(), pnts.begin(), pnts.end());
	return (! istrm.fail());
}

bool
PointCloud :: isValid
	() const
{
	return (dat::isValid(theProxTol));
}

size_t
PointCloud :: size
	() const
{
	return thePoints.size();
}

std::string
PointCloud :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	oss << dat::infoString(thePoints.size(), "numPoints");
	return oss.str();
}

}

