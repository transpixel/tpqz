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


#include "libtri/tri.h"

#include "libdat/Area.h"
#include "libdat/array.h"
#include "libdat/MinMax.h"
#include "libdat/validity.h"
#include "libga/ga.h"
#include "libmath/angle.h"
#include "libtri/IsoGeo.h"

#include <array>
#include <string>
#include <sstream>


// TODO - factor into .inl,.cpp files

namespace tri
{

//! Individual tiles within tritille tessellation
namespace tile
{

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
		 * PropSampFunc: Property sampling function. Must support
		 *   PropType = PropSampFunc(size_t, size_t)
		 *
		 * PropType: Must support
		 *   op: double * PropType
		 *   op: PropType + PropType
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

} // tile


/*! \brief Iso-tritille interpolation entity.

\par Example
\dontinclude testtri/uIsoTille.cpp
\skip ExampleStart
\until ExampleEnd
*/
class IsoTille
{
	tri::IsoGeo theTileGeo;

public: // static methods

	//! Return triangle tile based on tessellation coordinates
	static
	tile::Triangle
	triangleFor
		( dat::Spot const & xrel
		, IsoGeo const & tileGeo
		)
	{
		tile::Triangle tri;

		dat::QuantumFrac const muNdxFrac{ tileGeo.muNdxFrac(xrel) };
		dat::QuantumFrac const nuNdxFrac{ tileGeo.nuNdxFrac(xrel) };

		size_t const & ndxI = muNdxFrac.floor();
		size_t const & ndxJ = nuNdxFrac.floor();
		double const & muFrac = muNdxFrac.fraction();
		double const & nuFrac = nuNdxFrac.fraction();

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

public: // methods

	//! default null constructor
	IsoTille
		() = default;

	//! Construct tessellation of specified geometry
	explicit
	IsoTille
		( tri::IsoGeo const & geometry
		)
		: theTileGeo{ geometry }
	{
	}

	//! True if instance is valid
	bool
	isValid
		() const
	{
		return
			{  dat::isValid(theTileGeo)
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
			std::array<Vec2D, 4u> const xyCorners(areaXY.extrema<Vec2D>());
			for (Vec2D const & xyCorner : xyCorners)
			{
				// expand the mu,nu dimensions (independently)
				double const mu{ theTileGeo.mu(xyCorner) };
				double const nu{ theTileGeo.nu(xyCorner) };
				muMinMax = muMinMax.expandedWith(mu);
				nuMinMax = nuMinMax.expandedWith(nu);
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
		// get triangle covering this area...
		tile::Triangle const tri{ triangleFor(xrel, theTileGeo) };
		// ... and return interpolated value
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

