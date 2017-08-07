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

#ifndef blk_sim_INCL_
#define blk_sim_INCL_

/*! \file
\brief Declarations for blk::sim
*/


#include "libblk/blk.h"
#include "libga/Rigid.h"

#include <map>
#include <vector>
#include <string>


namespace blk
{

/*! \brief Functions that simulation block configurations

\par Example
\dontinclude testblk/uform.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace sim
{
	//! Convert index to key - distinct from index values for testing
	blk::NodeKey
	keyFromNdx
		( size_t const & ndx
		);

	//! Restore key from index
	size_t
	ndxFromKey
		( blk::NodeKey const & key
		);

	//! Orientations at the vertices of a cube
	std::map<NodeKey, ga::Rigid>
	cubeEOs
		( ga::Vector const & center = ga::vZero
		, double const & edgeSpan = 2.
		);

	//! Band-limited combinations of pairwise relative orientations
	std::vector<blk::EdgeOri>
	bandedROs
		( std::map<NodeKey, ga::Rigid> const & eos
		, size_t const & maxBandWidth = { 2u }
		);

	//! All combinations of pairwise relative orientations
	std::vector<blk::EdgeOri>
	allROs
		( std::map<NodeKey, ga::Rigid> const & eos
		);


} // sim

} // blk

// Inline definitions
// #include "libblk/sim.inl"

#endif // blk_sim_INCL_

