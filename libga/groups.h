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

#ifndef ga_groups_INCL_
#define ga_groups_INCL_

/*! \file
\brief Declarations for ga::groups
*/


#include "libga/quantities.h"

#include <vector>


namespace ga
{

/*! \brief Useful groups of entities.

\par Example
\dontinclude testga/ugroups.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace groups
{
	//! Spinors for (2/2)*pi (half-turn) rotations (Cube symmetry).
	std::vector<ga::Spinor>
	cubeSpinors
		();

	//! Spinors for (2/3)*pi (60-deg) rotations (Tetrahedron symmetry).
	std::vector<ga::Spinor>
	tetraSpinors
		();

	//! Spinors for (2/2 & 2/4)*pi (45-deg) rotations (Octahedron symmetry).
	std::vector<ga::Spinor>
	octaSpinors
		();

	/*! Spinor values associated with vertices of platonic solids.
	 *
	 * Ref. "Relative Orientation" Horn, B. K.P.
	 */
	std::vector<ga::Spinor>
	spreadOfSpinors
		();

	//! Physical angles associated with spinors
	std::vector<ga::BiVector>
	physicalAnglesFor
		( std::vector<ga::Spinor> const & spins
		);

} // groups

} // ga

// Inline definitions
// #include "libga/groups.inl"

#endif // ga_groups_INCL_

