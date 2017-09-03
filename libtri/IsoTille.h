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

	//! Perform interpolation at refSpot
	template <typename SampFunc>
	inline
	typename SampFunc::value_type
	linearInterpWithCheck
		( Vec2D const & refSpot //!< location relative to tile origin
		, SampFunc const & propSampFunc
		) const;

	//! Perform linear interpolation at refSpot *assumed to be IN domain*.
	template <typename SampFunc>
	inline
	typename SampFunc::value_type
	linearInterpForValid
		( Vec2D const & refSpot //!< location relative to tile origin
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
