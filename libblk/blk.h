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

#ifndef blk_blk_INCL_
#define blk_blk_INCL_

/*! \file
\brief Declarations for blk::blk
*/


#include "libga/Rigid.h"

#include <cstddef>
#include <string>
#include <utility>
#include <vector>


/*! \brief Types and values useful for libblk module.
*/

namespace blk
{

	//! A member of block (associated with its own coordinate frame)
	using NodeNdx = size_t;

	//! Two Nodes (such as associated with a RelOri measurement)
	using NdxPair = std::pair<NodeNdx, NodeNdx>;
	//! Terminology associated with graph operations
	using EdgeKey = NdxPair;

	//! Type of a RelOri observation
	using EdgeOri = std::pair<EdgeKey, ga::Rigid>;


	//! Transform collection of orientations to agreed at ndxFit
	std::vector<ga::Rigid>
	fitOnto
		( std::vector<ga::Rigid> const & // oriSrcWrtRefs
		, size_t const & // keyNdxToFit
		, ga::Rigid const & // oriKeyWrtRef
		);

} // blk

// Inline definitions
// #include "libblk/blk.inl"

#endif // blk_blk_INCL_

