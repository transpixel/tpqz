//
//
// MIT License
//
// Copyright (c) 2020 Stellacore Corporation.
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

#ifndef geo_VertGangle_INCL_
#define geo_VertGangle_INCL_

/*! \file
\brief Declarations for geo::VertGangle
*/


#include "libdat/validity.h"
#include "libga/ga.h"
#include "libgeo/Wedge.h"

#include <string>
#include <utility>


namespace geo
{

/*! \brief Generalization concept of angle as G2-element of geometric algebra.

\par Example
\dontinclude testgeo/uVertGangle.cpp
\skip ExampleStart
\until ExampleEnd
*/

struct VertGangle
{
	ga::Spinor theGangle{};

	/*! Generalized (G2-subspace) angle second location w.r.t. first.
	 *
	 * Expresses angle relating two edges of Wedge, via:
 	 * \arg wedge.edge2() = (*this) * wedge.edge1()
	 */
	static
	ga::Spinor
	vertGangleFor
		( Wedge const & wedge
		);

	//! default null constructor
	VertGangle
		() = default;

	//! Generalized angle at vertex from first toward second
	explicit
	VertGangle
		( ga::Vector const & vert
		, std::pair<ga::Vector, ga::Vector> const & locPair
		);

	//! True if this instance is valid
	inline
	bool
	isValid
		() const;

	//! Scalar magnitude of scalar part of vertGangle()
	inline
	double
	ratioMag
		() const;

	//! Scalar magnitude of bivector part of vertGangle()
	inline
	double
	angleMag
		() const;

	//! Descriptive information about this instance
	std::string
	infoString
		( std::string const & title = {}
		) const;

}; // VertGangle

} // geo

// Inline definitions
#include "libgeo/VertGangle.inl"

#endif // geo_VertGangle_INCL_

