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

#include <string>


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
	Vec2D theDirA{{ dat::nullValue<double>(), dat::nullValue<double>() }};
	Vec2D theDirU{{ dat::nullValue<double>(), dat::nullValue<double>() }};
	Vec2D theDirV{{ dat::nullValue<double>(), dat::nullValue<double>() }};
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
		);

	//! True if instance is valid
	bool
	isValid
		() const;

	//! Tessellation lattice spacing (same in both direction for iso-tritille)
	double
	delta
		() const;

private:

	//! Projection of xyLoc onto udir axis
	inline
	double
	muForXY
		( dat::Spot const & xyLoc //!< location relative to tile origin
		) const;

	//! Projection of xyLoc onto vdir axis
	inline
	double
	nuForXY
		( dat::Spot const & xyLoc //!< location relative to tile origin
		) const;

public:

	//! Projection of xyLoc onto (udir,vdir) axis - relative to origin
	inline
	dat::Spot
	mnLocForXY
		( dat::Spot const & xyLoc //!< location relative to tile origin
		) const;

	//! Limits (half open) on mu and nu values given domain area limits
	dat::Area<double>
	mnAreaForXY
		( Domain const & xyDomain
		) const;

	//! Node index and fraction into cell along ("u","v") directions
	inline
	std::pair<dat::QuantumFrac, dat::QuantumFrac>
	cellPairForXY
		( dat::Spot const & xyLoc
		) const;

	//! Node index associated with mu value
	inline
	long
	indexForMu
		( double const & mu
		) const;

	//! Node index associated with nu value
	inline
	long
	indexForNu
		( double const & nu
		) const;

	//! Value of mu from node index in "u" direction
	inline
	double
	muFromIndex
		( long const & muNdx
		) const;

	//! Value of nu from node index in "v" direction
	inline
	double
	nuFromIndex
		( long const & nuNdx
		) const;

	//! Range value of (mu,nu) for node with indices
	inline
	dat::Spot
	mnLocForNode
		( std::pair<long, long> const & ndxPairMuNu
		) const;

	//! Node index and fraction into cell along ("u","v") directions
	inline
	std::pair<dat::QuantumFrac, dat::QuantumFrac>
	cellPairForMuNu
		( dat::Spot const & mnLoc
		) const;

	//! Reconstruction of (orthogonal) xyLoc from (non-ortho) tile coordinates
	inline
	dat::Spot
	xyLocForMuNu
		( dat::Spot const & mnRel
		) const;

	//! Domain value of (x,y) for node with indices
	inline
	dat::Spot
	xyLocForNode
		( std::pair<long, long> const & ndxPairMuNu
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

