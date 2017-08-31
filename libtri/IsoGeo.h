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

#ifndef tri_IsoGeo_INCL_
#define tri_IsoGeo_INCL_

/*! \file
\brief Declarations for tri::IsoGeo
*/


#include "libdat/Area.h"
#include "libdat/MinMax.h"
#include "libdat/QuantumFrac.h"
#include "libdat/Spot.h"
#include "libmath/math.h"

#include <string>

#include <sstream>


namespace tri
{

/*! \brief Geometry associated with an iso-tritille.

\par Example
\dontinclude testtri/uIsoGeo.cpp
\skip ExampleStart
\until ExampleEnd
*/

class IsoGeo
{
	using Vec2D = std::array<double, 2u>;

	// e.g. rows of transition matrix
	dat::quantum::Splitter<long, double> theSplitterMu;
	dat::quantum::Splitter<long, double> theSplitterNu;
	Vec2D theBarU{{ dat::nullValue<double>(), dat::nullValue<double>() }};
	Vec2D theBarV{{ dat::nullValue<double>(), dat::nullValue<double>() }};

public: // methods

	//! Construct with null values
	IsoGeo
		() = default;

	//! Construct tessellation via defining parameters
	explicit
	IsoGeo
		( double const & da //!< primary spacing (rhombus height)
		, double const & db //!< secondary spacing (rhombus width)
		, Vec2D const & avec //!< non-zero vector for primary alignment
	//	, Vec2D uv0 // -- herein assumed to be {0.,0.}
		)
		: theSplitterMu{ da }
		, theSplitterNu{ db }
	{
		using dat::operator*;
		using dat::operator-;
		using dat::operator+;

		Vec2D const adir(dat::unit(avec));
		Vec2D const bdir{{ -adir[1], adir[0] }};

		Vec2D const udir(dat::unit(da * adir - db * bdir));
		Vec2D const vdir(dat::unit(da * adir + db * bdir));

		double const gamma{ dat::dot(udir, vdir) };
		double const tmp{ 1. - math::sq(gamma) };
		if (std::numeric_limits<double>::min() < tmp)
		{
			double const inv{ 1./tmp };
			double const & beta = inv;
			double const alpha{ -inv * gamma };
			Vec2D const ubar(alpha*udir +  beta*vdir);
			Vec2D const vbar( beta*udir + alpha*vdir);

			theBarU[0] = ubar[0];
			theBarU[1] = ubar[1];
			theBarV[0] = vbar[0];
			theBarV[1] = vbar[1];
		}
	}

	//! Projection of xrel onto udir axis
	inline
	double
	mu
		( dat::Spot const & xrel //!< location relative to tile origin
		) const
	{
		return dat::dot(theBarU, xrel);
	}

	//! Projection of xrel onto vdir axis
	inline
	double
	nu
		( dat::Spot const & xrel //!< location relative to tile origin
		) const
	{
		return dat::dot(theBarV, xrel);
	}

	//! Projection of xrel onto (udir,vdir) axis
	inline
	dat::Spot
	locMuNu
		( dat::Spot const & xrel //!< location relative to tile origin
		) const
	{
		return {{ mu(xrel), nu(xrel) }};
	}

	//! Index and residual fraction along "u" direction
	dat::QuantumFrac
	muNdxFrac
		( dat::Spot const & xrel //!< location relative to tile origin
		) const
	{
		return dat::QuantumFrac(mu(xrel), theSplitterMu);
	}

	//! Index and residual fraction along "v" direction
	dat::QuantumFrac
	nuNdxFrac
		( dat::Spot const & xrel //!< location relative to tile origin
		) const
	{
		return dat::QuantumFrac(nu(xrel), theSplitterNu);
	}

	//! Limits (half open) on mu and nu values given domain area limits
	dat::Area<double>
	areaMuNu
		( dat::Area<double> const & areaXY
		) const
	{
		dat::Area<double> mnArea;
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

	//! True if instance is valid
	bool
	isValid
		() const
	{
		return
			{  dat::isValid(theBarU)
			&& dat::isValid(theBarV)
			};
	}

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const
	{
		std::ostringstream oss;
		if (! title.empty())
		{
			oss << title << std::endl;
		}
		if (isValid())
		{
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

}; // IsoGeo

} // tri

// Inline definitions
// #include "libtri/IsoGeo.inl"

#endif // tri_IsoGeo_INCL_

