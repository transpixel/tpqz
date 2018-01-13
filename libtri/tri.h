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

#ifndef tri_tri_INCL_
#define tri_tri_INCL_

/*! \file
\brief Declarations for namespace tri
*/


#include "libdat/Area.h"
#include "libdat/array.h"
#include "libdat/QuantumFrac.h"


/*! \brief Triangulation related types and values

\par Example
\dontinclude testtri/utri.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace tri
{
	using NodeNdxType = dat::QuantumFrac::index_type;
	static NodeNdxType const sNullNdx{ dat::nullValue<NodeNdxType>() };

	using NodeKey = std::pair<NodeNdxType, NodeNdxType>;
	static NodeKey const sNullNdxPair{ sNullNdx, sNullNdx};

	//! First member of key - index in Mu direction
	inline
	NodeNdxType const &
	ndxI
		( NodeKey const & keyIJ
		)
	{
		return keyIJ.first;
	}

	//! Second member of key - index in Nu direction
	inline
	NodeNdxType const &
	ndxJ
		( NodeKey const & keyIJ
		)
	{
		return keyIJ.second;
	}

} // tri

// Inline definitions
// #include "libtri/tri.inl"

#endif // tri_tri_INCL_

