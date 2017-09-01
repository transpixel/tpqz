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
\brief Definitions for tri::IsoGeo
*/


#include "libtri/IsoGeo.h"

#include "libdat/MinMax.h"
#include "libga/ga.h"
#include "libmath/math.h"

#include <sstream>


namespace tri
{

// explicit
IsoGeo :: IsoGeo
	( double const & da //!< primary spacing (rhombus height)
	, double const & db //!< secondary spacing (rhombus width)
	, Vec2D const & avec //!< non-zero vector for primary alignment
	)
	: IsoGeo()
{
	using dat::operator*;
	using dat::operator-;
	using dat::operator+;

	// definte orthogonal tiling axes
	Vec2D const adir(dat::unit(avec));
	Vec2D const bdir{{ -adir[1], adir[0] }};

	// define (non-orthogonal) tessellation directions
	Vec2D const udir(dat::unit(da * adir - db * bdir));
	Vec2D const vdir(dat::unit(da * adir + db * bdir));

	// check for singularity
	double const gamma{ dat::dot(udir, vdir) };
	double const tmp{ 1. - math::sq(gamma) };
	if (std::numeric_limits<double>::min() < tmp)
	{
		// compute cache values
		double const inv{ 1./tmp };
		double const & beta = inv;
		double const alpha{ -inv * gamma };
		Vec2D const ubar(alpha*udir +  beta*vdir);
		Vec2D const vbar( beta*udir + alpha*vdir);

		// set members
		theSplitterMu = { da };
		theSplitterNu = { db };
		theDirA = adir;
		theDirU = udir;
		theDirV = vdir;
		theBarU = ubar;
		theBarV = vbar;
	}
}

bool
IsoGeo :: isValid
	() const
{
	return
		{  dat::isValid(theBarU)
		&& dat::isValid(theBarV)
		};
}

double
IsoGeo :: delta
	() const
{
	return std::hypot(theSplitterMu.theDelta, theSplitterNu.theDelta);
}

dat::Area<double>
IsoGeo :: mnArea
	( Domain const & xyDomain
	) const
{
	dat::Area<double> mnArea;
	dat::Area<double> const areaXY{ xyDomain.areaBounds() };
	if (areaXY.isValid())
	{
		dat::MinMax<double> muMinMax;
		dat::MinMax<double> nuMinMax;
		std::array<dat::Spot, 4u> const xyCorners
			(areaXY.extrema<dat::Spot>());
		for (dat::Spot const & xyCorner : xyCorners)
		{
			// expand the mu,nu dimensions (independently)
			muMinMax = muMinMax.expandedWith(mu(xyCorner));
			nuMinMax = nuMinMax.expandedWith(nu(xyCorner));
		}
		mnArea = dat::Area<double>{ muMinMax.pair(), nuMinMax.pair() };
	}
	return mnArea;
}

std::string
IsoGeo :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	if (isValid())
	{
		ga::Vector const aVec(theDirA[0], theDirA[1], 0.);
		ga::Vector const uVec(theDirU[0], theDirU[1], 0.);
		ga::Vector const vVec(theDirV[0], theDirV[1], 0.);
		ga::BiVector const angUwA
			{ ga::spin::physicalAngleFrom(ga::spin::between(aVec, uVec)) };
		ga::BiVector const angVwA
			{ ga::spin::physicalAngleFrom(ga::spin::between(aVec, vVec)) };

		double const uRad{ angUwA[2] };
		double const vRad{ angVwA[2] };
		double const uDeg{ uRad * 180. / math::pi };
		double const vDeg{ vRad * 180. / math::pi };

		oss << std::endl;
		oss << dat::infoString(theSplitterMu.theDelta, "muDelta");
		oss << std::endl;
		oss << dat::infoString(theSplitterNu.theDelta, "nuDelta");

		oss << std::endl;
		oss
			<< dat::infoString(theDirA, "theDirA")
			;
		oss << std::endl;
		oss
			<< dat::infoString(theDirU, "theDirU")
			<< "  angleFromA:"
			<< " " << dat::infoString(uRad) << " [rad]"
			<< " " << dat::infoString(uDeg) << " [deg]"
			;
		oss << std::endl;
		oss
			<< dat::infoString(theDirV, "theDirV")
			<< "  angleFromA:" 
			<< " " << dat::infoString(vRad) << " [rad]"
			<< " " << dat::infoString(vDeg) << " [deg]"
			;

		oss << std::endl;
		oss << dat::infoString(theBarU, "theBarU");
		oss << std::endl;
		oss << dat::infoString(theBarV, "theBarV");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

} // tri

