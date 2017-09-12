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
#include "libdat/QuantumFrac.h"
#include "libdat/Spot.h"
#include "libtri/Domain.h"
#include "libtri/tri.h"

#include <string>


namespace tri
{

/*! \brief Geometry associated with an iso-tritille.

Terminology is based on three reference frames:
\arg Ref: The original domain in which tritille is defined.
\arg Tile: The (skew) frame associated with "a", "u" and "v" edge directions.
\arg Node: The quantized frame for node indices and fractional locations.
\arg Index: The discrete (2) addressing of individual nodes

Note the reference frame is orthogonal, while the Tile and Node frames are
(in general) not. Locations in the Ref and Tile frames are associated with
2D floating point values via dat::Spot. Locations in the node frame are
specified with dat::Quantum values.

\par Example
\dontinclude testtri/uIsoGeo.cpp
\skip ExampleStart
\until ExampleEnd
*/

class IsoGeo
{

public:  // types

	using Vec2D = std::array<double, 2u>;

private: 

	// e.g. rows of transition matrix
	dat::quantum::Splitter<NodeNdxType, double> theSplitterMu;
	dat::quantum::Splitter<NodeNdxType, double> theSplitterNu;
	Vec2D theDirA{{ dat::nullValue<double>(), dat::nullValue<double>() }};
	Vec2D theDirU{{ dat::nullValue<double>(), dat::nullValue<double>() }};
	Vec2D theDirV{{ dat::nullValue<double>(), dat::nullValue<double>() }};
	Vec2D theBarU{{ dat::nullValue<double>(), dat::nullValue<double>() }};
	Vec2D theBarV{{ dat::nullValue<double>(), dat::nullValue<double>() }};

public: // methods

	using QuantPair = std::pair<dat::QuantumFrac, dat::QuantumFrac>;

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
		);

	//! True if instance is valid
	bool
	isValid
		() const;

	//! Tessellation lattice spacing (same in both direction for iso-tritille)
	double
	delta
		() const;

	//
	// Tile locations
	//

	//! Projection of refSpot onto (udir,vdir) axis - relative to origin
	inline
	dat::Spot
	tileSpotForRefSpot
		( dat::Spot const & refSpot //!< location relative to tile origin
		) const;

	//! Range value of (mu,nu) for node with indices
	inline
	dat::Spot
	tileSpotForFracPair
		( QuantPair const & fracPair
		) const;

	//
	// Node locations
	//

	//! Node index and fraction into cell along ("u","v") directions
	inline
	QuantPair
	fracPairForRefSpot
		( dat::Spot const & refSpot
		) const;

	//! Node index and fraction into cell along ("u","v") directions
	inline
	QuantPair
	fracPairForTileSpot
		( dat::Spot const & tileSpot
		) const;

	//! Cast node index pair to QuantPair with zero fraction
	inline
	QuantPair
	fracPairForIndices
		( NodeNdxPair const & nodeIJ
		) const;

	//! Integral part of fracPair (after discarding fractional part).
	inline
	NodeNdxPair
	indicesForFracPair
		( QuantPair const & fracPair
		) const;

	//
	// Ref locations
	//

	//! Reconstruction of (orthogonal) refSpot from (non-ortho) tile coords
	inline
	dat::Spot
	refSpotForTileSpot
		( dat::Spot const & tileSpot
		) const;

	//! Domain value of (x,y) for tile location
	inline
	dat::Spot
	refSpotForFracPair
		( QuantPair const & fracPair
		) const;

	//
	// Node indices
	//

	//! Integral part of fracPair (after discarding fractional part).
	inline
	NodeNdxPair
	indicesForRefSpot
		( dat::Spot const & refSpot
		) const;

	//! Domain value of (x,y) for node with indices
	inline
	dat::Spot
	refSpotForIndices
		( NodeNdxPair const & nodeIJ
		) const;

	//
	// General
	//

	//! (Non-orthogonal) Tile area associated with (orthogonal) Ref area
	dat::Area<double>
	tileAreaForRefArea
		( dat::Area<double> const & refArea
		) const;

	//! Limits (half open) on mu and nu values given domain area limits
	dat::Area<double>
	tileAreaForDomain
		( Domain const & xyDomain
		) const;

	//! "Area" of indices which are *INCLUDED* within tileArea
	dat::Area<NodeNdxType>
	ijAreaForTileArea
		( dat::Area<double> const & tileArea
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // IsoGeo

} // tri

// Inline definitions
#include "libtri/IsoGeo.inl"

#endif // tri_IsoGeo_INCL_

