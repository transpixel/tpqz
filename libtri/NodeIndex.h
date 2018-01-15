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

#ifndef tri_NodeIndex_INCL_
#define tri_NodeIndex_INCL_

/*! \file
\brief Declarations for tri::NodeIndex
*/


#include "libdat/Offset2D.h"
#include "libdat/grid.h" // NOTE: wastes ~2x index storage, but fast
#include "libtri/IsoTille.h"

#include <cassert>
#include <string>


namespace tri
{

/*! \brief Remap tritille (I,J) NodeKeys to scalar index

\par Example
\dontinclude testtri/uNodeIndex.cpp
\skip ExampleStart
\until ExampleEnd
*/

class NodeIndex
{

public: // data types

	using index_type = size_t;

private:

	dat::Offset2D<size_t, long> theRowColMap{};
	dat::grid<index_type> theNdxGrid;
	size_t theSize{ dat::nullValue<size_t>() };

public: // methods

	//! default null constructor
	NodeIndex
		() = default;

	//! Allocate enough space for all notes in active domain.
	explicit
	NodeIndex
		( tri::IsoTille const & trinet
		);

	//! True if instance is valid
	bool
	isValid
		() const;

	//! Size for collection to be addressed by indexForNodeKey() values
	size_t
	size
		() const;

	//! Index(offset) into an assumed external container
	inline
	index_type
	indexForNodeKey
		( NodeKey const & keyIJ
		) const
	{
		index_type ndx{ dat::nullValue<index_type>() };
		assert(isValid());
		dat::RowCol const rcNdxGrid(theRowColMap(keyIJ));
		if (dat::isValid(rcNdxGrid))
		{
			if (theNdxGrid.hwSize().includes(rcNdxGrid))
			{
				ndx = theNdxGrid(rcNdxGrid);
			}
		}
		return ndx;
	}

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // NodeIndex

} // tri

// Inline definitions
// #include "libtri/NodeIndex.inl"

#endif // tri_NodeIndex_INCL_

