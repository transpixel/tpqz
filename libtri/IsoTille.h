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
#include "libdat/Spot.h"
#include "libdat/validity.h"
#include "libtri/IsoGeo.h"

#include <array>
#include <string>


namespace tri
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
	inline
	Triangle
	triangleFor
		( dat::Spot const & xrel
		, IsoGeo const & tileGeo
		);

public: // methods

	//! default null constructor
	IsoTille
		() = default;

	//! Construct tessellation of specified geometry
	explicit
	IsoTille
		( tri::IsoGeo const & geometry
		);

	//! True if instance is valid
	bool
	isValid
		() const;

	//! Limits (half open) on mu and nu values given domain area limits
	dat::Area<double>
	areaMuNu
		( dat::Area<double> const & areaXY
		) const;

	//! Perform interpolation at xrel
	template <typename SampFunc>
	inline
	typename SampFunc::value_type
	operator()
		( Vec2D const & xrel //!< location relative to tile origin
		, SampFunc const & propSampFunc
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // IsoTille

} // tri

// Inline definitions
#include "libtri/IsoTille.inl"

#endif // tri_IsoTille_INCL_

