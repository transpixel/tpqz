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

#ifndef ro_cast_INCL_
#define ro_cast_INCL_

/*! \file
\brief Declarations for ro::cast
*/


#include "libro/PairAbs.h"
#include "libro/PairRel.h"
#include "libro/PairBaseZ.h"


namespace ro
{

/*! \brief Functions for casting between libro objects.

\par Example
\dontinclude testro/uPair.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace cast
{
	//! General pair cast
	template <typename OutPair>
	inline
	OutPair
	intoPair
		( Pair const & anyPair
		)
	{
		return OutPair(anyPair.rigid1w0(), anyPair.rigid2w0());
	}

	//! Relative pair from any other
	inline
	PairRel
	intoPairRel
		( Pair const & anyPair
		)
	{
		return intoPair<PairRel>(anyPair);
	}

	//! Absolute pair from any other
	inline
	PairAbs
	intoPairAbs
		( Pair const & anyPair
		)
	{
		return intoPair<PairAbs>(anyPair);
	}

	//! ZBase pair from any other
	inline
	PairBaseZ
	intoPairBaseZ
		( Pair const & anyPair
		)
	{
		return intoPair<PairBaseZ>(anyPair);
	}

} // cast

} // ro

// Inline definitions
// #include "libro/cast.inl"

#endif // ro_cast_INCL_

