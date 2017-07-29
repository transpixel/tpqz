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

#ifndef blk_Block_INCL_
#define blk_Block_INCL_

/*! \file
\brief Declarations for blk::Block
*/


#include "libblk/OriPair.h"
#include "libga/Rigid.h"

#include <string>

#include <sstream>


namespace blk
{

/*! \brief A connected collection of node orientations.

\par Example
\dontinclude testblk/uBlock.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Block
{

public: // data

	/*
	struct Node
	{
		blk::NodeNdx const theNdx;//{ dat::nullValue<NodeNdx>() };
		ga::Rigid const theOriWrtRef;//{};
	};
	*/

public: // static methods

	/*
	//! Construct block from collection of connected RO pairs
	static
	Block
	from
		( FwdIter const & beg //!< *iter: ga::Rigid
		, FwdIter const & end
		);
	*/

public: // methods

	//! default null constructor
	Block
		() = default;

	/*
	//! XXX...
	explicit
	Block
		(
		);
	*/

	//! True if instance is valid
	bool
	isValid
		() const
	{
		return false;
	}

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
//		, std::string const & fmt = std::string("%12.6f")
		) const
	{
		std::ostringstream oss;
		if (! title.empty())
		{
			oss << title << std::endl;
		}
		if (isValid())
		{
		}
		else
		{
			oss << " <null>";
		}
		return oss.str();
	}

}; // Block

} // blk

// Inline definitions
// #include "libblk/Block.inl"

#endif // blk_Block_INCL_

