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

#ifndef ga_spin_INCL_
#define ga_spin_INCL_

/*! \file
\brief Declarations for ga::spin
*/


#include "libga/quantities.h"


namespace ga
{

//! spinor utilities
namespace spin
{

	//! Determine spinor: into = spin * from * spin.rev
	Spinor
	between
		( Vector const & dirFrom //!< Unit vector
		, Vector const & dirInto //!< Unit vector
		, BiVector const & planeForPi = {} //!< Unit plane
		);

	//! Spinor between(): for *PROJECTIONS* of vectors onto the plane
	Spinor
	betweenProjectionsOnto
		( Vector const & vecFrom
		, Vector const & vecInto
		, BiVector const & projPlane
		);

	//! Specialization of 'Log' which ignores magnitude of spin
	BiVector
	argAngleFrom
		( Spinor const & spin
		, BiVector const & planeForPi = {}
			//!< Use this plane to resolve half-turn rotations
		);

	//! 2*logAngleFrom
	BiVector
	physicalAngleFrom
		( Spinor const & spin
		, BiVector const & planeForPi = {}
			//!< Use this plane to resolve half-turn rotations
		);

	//! Numeric components of associated spinor
	std::array<double, 4u>
	components
		( Spinor const & spin
		);

}
}

// Inline definitions
// #include "libga/spin.inl"

#endif // ga_spin_INCL_

