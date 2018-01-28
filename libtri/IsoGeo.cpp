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
	double const cosUV{ dat::dot(udir, vdir) };
	double const sinSqUV{ 1. - math::sq(cosUV) };
	if (std::numeric_limits<double>::min() < sinSqUV)
	{
		// compute cache values
		double const alpha{ 1. / sinSqUV };
		double const beta{ -cosUV / sinSqUV };
		Vec2D const ubar(alpha*udir +  beta*vdir);
		Vec2D const vbar( beta*udir + alpha*vdir);

		// spacing along tessellation directions
		double const dmu{ .5 * std::hypot(da, db) };
		double const & dnu = dmu; // the "iso" part of iso-tritille

		// set members
		theSplitterMu = dat::quantum::Splitter<NodeNdxType, double>(dmu);
		theSplitterNu = dat::quantum::Splitter<NodeNdxType, double>(dnu);
		theDirA = adir;
		theDirU = udir;
		theDirV = vdir;
		theBarU = ubar;
		theBarV = vbar;

		/*
		double const & gamma = cosUV;
		io::out() << std::endl;
		io::out() << dat::infoString(theDirA, "theDirA") << std::endl;
		io::out() << dat::infoString(theDirU, "theDirU") << std::endl;
		io::out() << dat::infoString(theDirV, "theDirV") << std::endl;
		io::out() << std::endl;
		io::out() << dat::infoString(gamma, "gamma") << std::endl;
		io::out() << dat::infoString(alpha, "alpha") << std::endl;
		io::out() << dat::infoString(beta, "beta") << std::endl;
		io::out() << std::endl;
		io::out() << dat::infoString(theBarU, "theBarU") << std::endl;
		io::out() << dat::infoString(theBarV, "theBarV") << std::endl;
		io::out() << std::endl;
		*/
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

dat::Area<double>
IsoGeo :: tileAreaForRefArea
	( dat::Area<double> const & refArea
	) const
{
	dat::Area<double> tileArea;
	if (refArea.isValid())
	{
		dat::MinMax<double> muMinMax;
		dat::MinMax<double> nuMinMax;
		std::array<dat::Spot, 4u> const xyCorners
			(refArea.extrema<dat::Spot>());
		for (dat::Spot const & xyCorner : xyCorners)
		{
			dat::Spot const mnSpot(tileSpotForRefSpot(xyCorner));
			// expand the mu,nu dimensions (independently)
			muMinMax = muMinMax.expandedWith(mnSpot[0]);
			nuMinMax = nuMinMax.expandedWith(mnSpot[1]);
		}
		tileArea = dat::Area<double>{ muMinMax.pair(), nuMinMax.pair() };
	}
	return tileArea;
}

dat::Area<NodeNdxType>
IsoGeo :: ijAreaForTileArea
	( dat::Area<double> const & tileArea
	) const
{
	dat::Area<NodeNdxType> ijArea;
	if (tileArea.isValid())
	{
		dat::MinMax<NodeNdxType> iMinMax;
		dat::MinMax<NodeNdxType> jMinMax;
		std::array<dat::Spot, 4u> const mnCorners
			(tileArea.extrema<dat::Spot>());
		for (dat::Spot const & mnCorner : mnCorners)
		{
			QuantPair const qpair{ fracPairForTileSpot(mnCorner) };
			NodeNdxType const & ndxI = qpair.first.theFloor;
			NodeNdxType const & ndxJ = qpair.second.theFloor;

			// expand the mu,nu dimensions (independently)
			iMinMax = iMinMax.expandedWith(ndxI);
			jMinMax = jMinMax.expandedWith(ndxJ);
		}
		dat::Range<NodeNdxType> const iRange(iMinMax.min(), iMinMax.max());
		dat::Range<NodeNdxType> const jRange(jMinMax.min(), jMinMax.max());
		ijArea = dat::Area<NodeNdxType>{ iRange, jRange };
	}
	return ijArea;
}

dat::Area<double>
IsoGeo :: tileAreaForDomain
	( Domain const & xyDomain
	) const
{
	return tileAreaForRefArea(xyDomain.areaBounds());
}

double
IsoGeo :: delta
	() const
{
	return std::hypot(theSplitterMu.theDelta, theSplitterNu.theDelta);
}

IsoGeo::Vec2D
IsoGeo :: dirA
	() const
{
	return theDirA;
}

IsoGeo::Vec2D
IsoGeo :: dirU
	() const
{
	return theDirU;
}

IsoGeo::Vec2D
IsoGeo :: dirV
	() const
{
	return theDirV;
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

