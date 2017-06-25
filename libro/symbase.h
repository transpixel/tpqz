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

#ifndef ro_symbase_INCL_
#define ro_symbase_INCL_

/*! \file
\brief Declarations for ro::symbase
*/


#include "libga/Rigid.h"
#include "libro/Tweak.h"


namespace ro
{

/*! \brief Functions for symmetric base-frame representation of a stereo-pair

\par Example
\dontinclude testro/uTweak.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace symbase
{

	//! Create a symmetric expression from general EO pair
	Tweak
	tweakFrom
		( ga::Rigid const & oriAwRef
		, ga::Rigid const & oriBwRef
		);

	//! EO pair expressed w.r.t. Ref for given tweak and model orientation
	std::pair<ga::Rigid, ga::Rigid>
	roPairFor
		( Tweak const & twk
		, ga::Rigid const & oriSymWrtRef
		);

} // symbase

} // ro

// Inline definitions
// #include "libro/symbase.inl"

#endif // ro_symbase_INCL_

