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

#ifndef sen_funcSenseOp_INCL_
#define sen_funcSenseOp_INCL_

/*! \file
\brief Declarations for sen::funcSenseOp
*/


#include "libmodel/atom.h"
#include "libmodel/Part.h"


namespace sen
{

namespace color
{
	//! Functor for color sensing
	struct SenseOp
	{
		inline
		model::atom::Color
		operator()
			( model::Part const & partInSen
			, geo::Ray const & rayInSen
			) const;
	};
}

namespace range
{
	//! Functor for range sensing
	struct SenseOp
	{
		inline
		model::atom::Range
		operator()
			( model::Part const & partInSen
			, geo::Ray const & rayInSen
			) const;
	};
}

namespace point
{
	//! Functor for range sensing
	struct SenseOp
	{
		inline
		model::atom::Point
		operator()
			( model::Part const & partInSen
			, geo::Ray const & rayInSen
			) const;
	};
}


}

// Inline definitions
#include "libsen/funcSenseOp.inl"

#endif // sen_funcSenseOp_INCL_

