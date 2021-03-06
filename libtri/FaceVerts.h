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

#ifndef tri_FaceVerts_INCL_
#define tri_FaceVerts_INCL_

/*! \file
\brief Declarations for tri::FaceVerts
*/


#include "libdat/validity.h"
#include "libtri/tri.h"

#include <algorithm>
#include <array>
#include <string>


namespace tri
{

/*! \brief Representation of a triangle face.

\par Example
\dontinclude testtri/uFaceVerts.cpp
\skip ExampleStart
\until ExampleEnd
*/

struct FaceVerts
{
	//! Tessellation indices and weight associated with the (i,j)-th node
	struct Vertex
	{
		NodeKey theNdxIJ{ sNullNdxPair };
		double theW{ dat::nullValue<double>() };

		//! Descriptive information about this instance.
		std::string
		infoString
			( std::string const & title = std::string()
			, std::string const & fmt = std::string("%12.6f")
			) const;

		//! True if this instance is valid
		inline
		bool
		isValid
			() const;
	};

	std::array<Vertex, 3u> theVerts{};

	//! True if all three vertices are valid
	inline
	bool
	isValid
		() const;

	/*! Interpolate value from a collection of properties
	 *
	 * PropSampFunc: Property sampling function. Must support
	 *   op(): PropType = PropSampFunc(NodeKey) const
	 *   have: using value_type = PropType
	 *
	 * Need operations for:
	 *   op+(PropType const &, PropType const &)
	 *   op*(double, PropType const &)
	*/
	template <typename PropSampFunc>
	inline
	typename PropSampFunc::value_type
	valueFrom
		( PropSampFunc const & propGrid
		) const;

	//! Node key of maximum weight value (std::max_element convention if dups)
	inline
	NodeKey
	nodeKeyMaxW
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		, std::string const & fmt = std::string("%12.6f")
		) const;

}; // FaceVerts

} // tri

// Inline definitions
#include "libtri/FaceVerts.inl"

#endif // tri_FaceVerts_INCL_

