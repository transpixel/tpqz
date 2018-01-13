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


#include "libdat/Spot.h"
#include "libdat/validity.h"
#include "libtri/Domain.h"
#include "libtri/FaceVerts.h"
#include "libtri/IsoGeo.h"
#include "libtri/NodeIterator.h"
#include "libtri/tri.h"

#include <string>


namespace tri
{

/*! \brief Iso-tritille interpolation entity.

\par Example
\dontinclude testtri/uIsoTille.cpp
\skip ExampleStart
\until ExampleEnd
*/
class IsoTille
{

public: // data

	tri::IsoGeo theTileGeo;
	tri::Domain theDomain;

public: // static methods

	//! Return triangle tile based on tessellation coordinates
	static
	inline
	FaceVerts
	triangleFor
		( dat::Spot const & refSpot
		, IsoGeo const & tileGeo
		);

	//! Construct a generic tilling
	static
	IsoTille
	genericTille
		( dat::Range<double> const & xRange
		, dat::Range<double> const & yRange
		, double const & xDelta
		, double const & yDelta
		);

public: // methods

	//! default null constructor
	IsoTille
		() = default;

	//! Construct tessellation of specified geometry
	explicit
	IsoTille
		( tri::IsoGeo const & geometry
		, tri::Domain const & domain
		);

	//! True if instance is valid
	bool
	isValid
		() const;

	//! Beginning iterator over nodes associated with valid domain locations.
	NodeIterator
	begin
		() const;

	//! Number nodes associated with valid domain location (NOTE! expensive)!
	size_t
	sizeValidNodes
		() const;

	//! True if keyIJ node has location within active domain
	inline
	bool
	contains
		( NodeKey const & keyIJ
		) const;

	//! Indices into tritille near to keyIJ - IRRESPECTIVE if in domain or not
	std::vector<NodeKey>
	nodeKeysNearTo
		( NodeKey const & keyIJ
		, double const & maxRefDist
		) const;

	//! Utility type
	using DistKeyPair = std::pair<double, NodeKey>;

	/*! Indices for nodes with maxRefDist of keyIJ in reference frame
	 *
	 * Returned nodes are:
	 * \arg associated to valid domain location
	 * \arg returned in order of proximity of ref location to ref loc of keyIJ
	 */
	std::vector<DistKeyPair>
	distKeysNearTo
		( NodeKey const & keyIJ
		, double const & maxRefDist
		) const;

	/*! Perform interpolation at refSpot
	 *
	 * /arg SampFunc is a functor that must be compatible with
	 * the "PropSampFunc" argument to tri::FaceVerts::valueFrom() method.
	 *
	 */
	template <typename SampFunc>
	inline
	typename SampFunc::value_type
	linearInterpWithCheck
		( dat::Spot const & refSpot //!< location relative to tile origin
		, SampFunc const & propSampFunc
		) const;

	//! Perform linear interpolation at refSpot *assumed to be IN domain*.
	template <typename SampFunc>
	inline
	typename SampFunc::value_type
	linearInterpForValid
		( dat::Spot const & refSpot //!< location relative to tile origin
		, SampFunc const & propSampFunc
		) const;

	//! Interpolated node property via inverse-distance weighted neighbors
	template <typename SampFunc>
	inline
	typename SampFunc::value_type
	nodeValueViaInvDist
		( NodeKey const & ndxGone //!< interpolate at this node
		, double const maxRefDist //!< using (valid) neighbors up to this far
		, SampFunc const & propSampFunc //!< ref. tri::FaceVerts::valueFrom()
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

