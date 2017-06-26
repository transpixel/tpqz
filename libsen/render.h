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

#ifndef sen_render_INCL_
#define sen_render_INCL_

/*! \file
\brief Declarations for sen::render
*/


#include "libga/Rigid.h"


namespace sen
{

/*! \brief Functions for rendering sensor imprints.

\par Example
\dontinclude testsen/urender.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace render
{
	//! Create a sensor imprint (e.g. display, or simulated sensor record)
	template 
		< typename ImprintType
		, typename SenseOpType
		, typename ProjOpType
		, typename ModelType
		>
	inline
	ImprintType // currently coded to support grids
	imprintFrom
		( ModelType const & modelInRef
		, ga::Rigid const & xSenWrtRef
		, ProjOpType const & projOp
		, dat::Extents const & hwSize
		);

}

}

// Inline definitions
#include "libsen/render.inl"

#endif // sen_render_INCL_

