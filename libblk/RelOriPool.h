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

#ifndef blk_RelOriPool_INCL_
#define blk_RelOriPool_INCL_

/*! \file
\brief Declarations for blk::RelOriPool
*/


#include "libblk/OriPair.h"
#include "libblk/blk.h"
#include "libga/Rigid.h"

#include <map>
#include <string>

// TODO
#include <vector>

namespace blk
{

/*! \brief Collection of RelOri measurements accessable block Node index pair.

\par Example
\dontinclude testblk/uRelOriPool.cpp
\skip ExampleStart
\until ExampleEnd
*/

class RelOriPool
{

public: // data

	//! RelOri measurement data
	std::map<EdgeKey, ga::Rigid> theRelOriMap;

public: // static methods

	//! Invalid RelOri edge
	static
	EdgeOri
	nullEdgeOri
		();

	//! Construct instance from collection of OriPair
	static
	RelOriPool
	from
		( std::vector<blk::OriPair> const & rops
		);

public: // methods

	//! Constructs an empty collection
	RelOriPool
		() = default;

	//! Value constructor
	explicit
	RelOriPool
		( std::map<EdgeKey, ga::Rigid> const & relOris
		);

	//! Directed orientation for node2 w.r.t. node1
	EdgeOri
	edgeOriFor
		( size_t const & ndx1
		, size_t const & ndx2
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // RelOriPool

} // blk

// Inline definitions
// #include "libblk/RelOriPool.inl"

#endif // blk_RelOriPool_INCL_

