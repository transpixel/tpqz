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

#ifndef ro_ro_INCL_
#define ro_ro_INCL_

/*! \file
\brief Declarations for namespace ro
*/


#include "libga/Rigid.h"

#include <array>
#include <utility>


/*! \brief Declarations and Definitions for libro.

\par General Concept:

Photogrammetric relative orientation (aka structure form (paired) motion)

\par Special Notes:

+ emphasizes physical interpration of RO geometry (e.g. angles and offsets)
+ provides robust solutions to RO determination

\par Example
\dontinclude testro/uro.cpp
\skip ExampleStart
\until ExampleEnd

*/
namespace ro
{

	// measured direction types
	using PairUV = std::pair<ga::Vector, ga::Vector>;
	using PtrPairUV = PairUV const * const;

	// station orientation types
	using LocPair = std::pair<ga::Vector, ga::Vector>;
	using AttPair = std::pair<ga::Pose, ga::Pose>;
	using OriPair = std::pair<ga::Rigid, ga::Rigid>;

	// model point types
	using PntPair = std::pair<ga::Vector, ga::Vector>;

	// useful for all things 5
	template <typename Type>
	using FiveOf = typename std::array<Type, 5u>;

} // ro

// Inline definitions
// #include "libro/ro.inl"

#endif // ro_ro_INCL_

