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
#include <map>
#include <string>
#include <utility>
#include <vector>


/*! \brief Types and values useful for libblk module.
*/

namespace blk
{

	//! A member of block (associated with its own coordinate frame)
	using NodeKey = size_t;
	constexpr NodeKey NullKey{ dat::nullValue<NodeKey>() };

	// -- works okay, but no need to slow with string compares
	//    if need this, then probably consider general templatizing
	// using NodeKey = std::string;
	// NodeKey const NullKey{};

	//! Terminology associated with graph operations
	using EdgeKey = std::pair<NodeKey, NodeKey>;

	//! Type of a RelOri observation
	using EdgeOri = std::pair<EdgeKey, ga::Rigid>;


	//! Same relationship but with indices satisfying order I<J
	EdgeOri
	principalEdgeOri
		( EdgeOri const & eOri
		);

	//! Transform collection of orientations consistent with two orientations
	std::vector<ga::Rigid>
	transformed
		( std::vector<ga::Rigid> const & oriNodeWrtSrcs
		, ga::Rigid const & oriTgtWrtAny
		, ga::Rigid const & oriSrcWrtAny
		);

	//! Transform collection of orientations consistent with two orientations
	std::map<NodeKey, ga::Rigid>
	transformed
		( std::map<NodeKey, ga::Rigid> const & oriNodeWrtSrcs
		, ga::Rigid const & oriTgtWrtAny
		, ga::Rigid const & oriSrcWrtAny
		);

} // blk

// Inline definitions
// #include "libblk/blk.inl"

#endif // blk_blk_INCL_

