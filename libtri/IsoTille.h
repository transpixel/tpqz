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

#ifndef tri_IsoTille_INCL_
#define tri_IsoTille_INCL_

/*! \file
\brief Declarations for tri::IsoTille
*/


#include "libdat/Area.h"
#include "libdat/array.h"
#include "libdat/MinMax.h"
#include "libdat/validity.h"
#include "libga/ga.h"
#include "libmath/angle.h"

#include <array>
#include <string>
#include <sstream>


// TODO - factor into .inl,.cpp files

namespace tri
{
	using Vec2D = std::array<double, 2u>;

//! Individual tiles within tritille tessellation
namespace tile
{

	//! Representation of tritille geometry;
	struct TileGeo
	{
		// e.g. rows of transition matrix
		Vec2D theBarU{{ dat::nullValue<double>(), dat::nullValue<double>() }};
		Vec2D theBarV{{ dat::nullValue<double>(), dat::nullValue<double>() }};

		//! Construct with null values
		TileGeo
			() = default;

		//! Construct tessellation via defining parameters
		explicit
		TileGeo
			( double const & da
			, double const & db
			, Vec2D const & avec //!< non-zero vector
		//	, Vec2D uv0 // -- herein assumed to be {0.,0.}
			)
			: TileGeo()
		{
			using dat::operator*;
			using dat::operator-;
			using dat::operator+;

			Vec2D const adir(dat::unit(avec));

			Vec2D const bdir{{ -adir[1], adir[0] }};
			Vec2D const uvec(da * adir - db * bdir);
			Vec2D const udir(dat::unit(uvec));

			Vec2D const vvec(da * adir + db * bdir);
			Vec2D const vdir(dat::unit(vvec));

			double const gamma{ dat::dot(udir, vdir) };

			double const tmp{ 1. - math::sq(gamma) };
			if (std::numeric_limits<double>::min() < tmp)
			{
				double const inv{ 1./tmp };
				double const & alpha = inv;
				double const beta{ -inv * gamma };
				Vec2D const ubar(alpha*vdir + beta*udir);
				Vec2D const vbar(alpha*udir + beta*vdir);

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
			( Vec2D const & xrel //!< location relative to tile origin
			) const
		{
			return dat::dot(theBarU, xrel);
		}

		//! Projection of xrel onto vdir axis
		inline
		double
		nu
			( Vec2D const & xrel //!< location relative to tile origin
			) const
		{
			return dat::dot(theBarV, xrel);
		}

		//! The mu() and nu() values combined in a pair
		inline
		std::pair<double, double>
		pairMuNu
			( Vec2D const & xrel
			) const
		{
			return { mu(xrel), nu(xrel) };
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
	};

	//! A triple of (weighted) nodes within the tessellation
	struct Triangle
	{
		//! Tessellation indices and weight associated with the (i,j)-th node
		struct Vertex
		{
			size_t theI{ dat::nullValue<size_t>() };
			size_t theJ{ dat::nullValue<size_t>() };
			double theW{ dat::nullValue<double>() };
		};

		std::array<Vertex, 3u> theVerts{{}};

		/*! Interpolate value from a collection of properties
		 *
		 * PropType: Must support
		 *   op: double * PropType
		 *   op: PropType + PropType
		 *
		 * PropSampFunc: Property sampling function. Must support
		 *   PropType = PropSampFunc(size_t, size_t)
		*/
		template <typename PropSampFunc>
		inline
		typename PropSampFunc::value_type
		valueFrom
			( PropSampFunc const & propGrid
			) const
		{
			size_t const & i1 = theVerts[0].theI;
			size_t const & i2 = theVerts[1].theI;
			size_t const & i3 = theVerts[2].theI;

			size_t const & j1 = theVerts[0].theJ;
			size_t const & j2 = theVerts[1].theJ;
			size_t const & j3 = theVerts[2].theJ;

			double const & w1 = theVerts[0].theW;
			double const & w2 = theVerts[1].theW;
			double const & w3 = theVerts[2].theW;

			return
				{ w1 * propGrid(i1, j1)
				+ w2 * propGrid(i2, j2)
				+ w3 * propGrid(i3, j3)
				};
		}
	};

	//! Fractional index
	struct FracNdx
	{
		long theInt{ dat::nullValue<long>() };
		double theFrac{ dat::nullValue<double>() };

		//! Construct a null instance
		FracNdx
			() = default;

		//! Construct by splitting value into integer and remainders
		explicit
		FracNdx
			( double const & value
			)
			// force rounding downward (e.g. unlike std::div)
			: theInt{ static_cast<long>(std::floor(value)) }
			, theFrac{ value - (double)theInt }
		{ }

		//! True if this instance is not null
		bool
		isValid
			() const
		{
			return dat::isValid(theFrac);
		}
	};

	//! Transformation into tile coordinates
	struct TileFinder
	{
		double theDelta;

		//! Integer and fractional remainder associated with (value/delta)
		static
		inline
		FracNdx
		ndxFrac
			( double const & value
			, double const & delta
			)
		{
			assert(std::numeric_limits<double>::min() < delta);
			return FracNdx(value / delta);
		}

		//! Construct for tesselation with iso-triangle edge size delta
		explicit
		TileFinder
			( double const & delta
			)
			: theDelta{ delta }
		{ 
		}

		//! True if instance is valid
		bool
		isValid
			() const
		{
			return dat::isValid(theDelta);
		}

		//! Return triangle tile based on tessellation coordinates
		tile::Triangle
		triangleFor
			( double const & mu
			, double const & nu
			) const
		{
			tile::Triangle tri;

			FracNdx const muNdxFrac{ ndxFrac(mu, theDelta) };
			FracNdx const nuNdxFrac{ ndxFrac(nu, theDelta) };

			size_t const & ndxI = muNdxFrac.theInt;
			size_t const & ndxJ = nuNdxFrac.theInt;
			double const & muFrac = muNdxFrac.theFrac;
			double const & nuFrac = nuNdxFrac.theFrac;

			if (muFrac < nuFrac)
			{
				// use triangle Tv
				tri.theVerts[0].theI = ndxI;
				tri.theVerts[0].theJ = ndxJ;
				tri.theVerts[0].theW = 1. - nuFrac;

				tri.theVerts[1].theI = ndxI + 1u;
				tri.theVerts[1].theJ = ndxJ + 1u;
				tri.theVerts[1].theW = muFrac;

				tri.theVerts[2].theI = ndxI;
				tri.theVerts[2].theJ = ndxJ + 1u;
				tri.theVerts[2].theW = nuFrac - muFrac;
			}
			else
			{
				// use triangle Tu
				tri.theVerts[0].theI = ndxI;
				tri.theVerts[0].theJ = ndxJ;
				tri.theVerts[0].theW = 1. - muFrac;

				tri.theVerts[1].theI = ndxI + 1u;
				tri.theVerts[1].theJ = ndxJ;
				tri.theVerts[1].theW = muFrac - nuFrac;

				tri.theVerts[2].theI = ndxI + 1u;
				tri.theVerts[2].theJ = ndxJ + 1u;
				tri.theVerts[2].theW = nuFrac;
			}

			return tri;
		}

	};

} // tile


/*! \brief Iso-tritille interpolation entity.

\par Example
\dontinclude testtri/uIsoTille.cpp
\skip ExampleStart
\until ExampleEnd
*/
class IsoTille
{
	tile::TileGeo theTileGeo;
	tile::TileFinder theTileFinder;

public: // methods

	//! default null constructor
	IsoTille
		() = default;

	//! Construct tessellation of specified geometry
	explicit
	IsoTille
		( double const & da
		, double const & db
		, Vec2D const & adir
		)
		: theTileGeo(da, db, adir)
		, theTileFinder(std::hypot(da, db))
	{
	}

	//! True if instance is valid
	bool
	isValid
		() const
	{
		return
			{  dat::isValid(theTileGeo)
			&& dat::isValid(theTileFinder)
			};
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
			std::array<double, 2u> const mins(areaXY.minimums());
			std::array<double, 2u> const maxs(areaXY.maximums());
			std::array<Vec2D, 4u> const xyCorners
				{ Vec2D{{ mins[0], mins[1] }}
				, Vec2D{{ mins[0], maxs[1] }}
				, Vec2D{{ maxs[0], mins[1] }}
				, Vec2D{{ maxs[0], maxs[1] }}
				};
io::out() << dat::infoString(xyCorners[0], "xyCorners[0]") << std::endl;
io::out() << dat::infoString(xyCorners[1], "xyCorners[1]") << std::endl;
io::out() << dat::infoString(xyCorners[2], "xyCorners[2]") << std::endl;
io::out() << dat::infoString(xyCorners[3], "xyCorners[3]") << std::endl;
			for (Vec2D const & xyCorner : xyCorners)
			{
				// expand the mu,nu dimensions (independently)
				double const mu{ theTileGeo.mu(xyCorner) };
				double const nu{ theTileGeo.nu(xyCorner) };
				muMinMax = muMinMax.expandedWith(mu);
				nuMinMax = nuMinMax.expandedWith(nu);
io::out()
	<< "xy:"
	<< " " << dat::infoString(xyCorner[0])
	<< " " << dat::infoString(xyCorner[1])
	<< "   "
	<< "mn:"
	<< " " << dat::infoString(mu)
	<< " " << dat::infoString(nu)
	<< std::endl;
			}
			mnArea = dat::Area<double>{ muMinMax.pair(), nuMinMax.pair() };
		}
		return mnArea;
	}


	//! Perform interpolation at xrel
	template <typename SampFunc>
	typename SampFunc::value_type
	operator()
		( Vec2D const & xrel //!< location relative to tile origin
		, SampFunc const & propSampFunc
		) const
	{
		double const mu{ theTileGeo.mu(xrel) };
		double const nu{ theTileGeo.nu(xrel) };

		tile::Triangle const tri{ theTileFinder.triangleFor(mu, nu) };

io::out()
	<< "..."
	<< " " << "xrel"
	<< " " << dat::infoString(xrel)
	<< " " << "mu,nu"
	<< " " << dat::infoString(mu)
	<< " " << dat::infoString(nu)
	<< std::endl;

		return tri.valueFrom<SampFunc>(propSampFunc);
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
			oss << dat::infoString(theTileGeo, "theTileGeo");
			oss << std::endl;
			oss << dat::infoString(theTileFinder.theDelta, "delta");
		}
		else
		{
			oss << " <null>";
		}
		return oss.str();
	}

}; // IsoTille

} // tri

// Inline definitions
// #include "libtri/IsoTille.inl"

#endif // tri_IsoTille_INCL_

