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


/*! \file
\brief Definitions for tri::NodeIndex
*/


#include "libtri/NodeIndex.h"

#include <sstream>

#include "libio/stream.h" // TODO


namespace
{
	// Area covered by tritille (I,J) index values
	dat::Area<tri::NodeNdxType>
	ijAreaFor
		( tri::IsoTille const & trinet
		)
	{
		tri::IsoGeo const & trigeo = trinet.theTileGeo;
		return trigeo.ijAreaForTileArea
			( trigeo.tileAreaForRefArea(trinet.theDomain.areaBounds())
			);
	}

	//! Required offset to access (I,J) minimums with positive indices
	dat::Offset2D<size_t, long>
	offsetFor
		( tri::IsoTille const & trinet
		)
	{
		// determine extents of trinet node keys
		return dat::Offset2D<size_t, long>(ijAreaFor(trinet).minimums());
	}

	//! High/Wide size extents which can hold *already offset* (I,J) indices
	dat::Extents
	hwSizeActive
		( tri::IsoTille const & trinet
		)
	{
		// determine extents of trinet node keys
		dat::Area<tri::NodeNdxType> const ijArea{ ijAreaFor(trinet) };
		return dat::Extents
			( ijArea[0].magnitude() + 1u // pad to include end of interval
			, ijArea[1].magnitude() + 1u
			);
	}

	//! Number of valid elements in trinet (slow! requires checking each)
	size_t
	sizeFor
		( tri::IsoTille const & trinet
		, dat::grid<tri::NodeIndex::index_type> * const & ptNdxGrid
		, dat::Offset2D<size_t, long> const & offMap
		)
	{
		assert(ptNdxGrid);
		assert(ptNdxGrid->isValid());
		std::fill
			( ptNdxGrid->begin(), ptNdxGrid->end()
			, dat::nullValue<tri::NodeIndex::index_type>()
			);
		size_t count{ 0u };
		for (tri::NodeIterator iter(trinet.begin()) ; iter ; ++iter)
		{
			dat::RowCol const rowcol(offMap(iter.nodeKey()));
			(*ptNdxGrid)(rowcol) = count;
			++count;
		}
		return count;
	}

}

namespace tri
{

// explicit
NodeIndex :: NodeIndex
	( tri::IsoTille const & trinet
	)
	: theRowColMap{ offsetFor(trinet) }
	, theNdxGrid{ hwSizeActive(trinet) }
	, theSize{ sizeFor(trinet, &theNdxGrid, theRowColMap) }
{
	/*
	io::out() << dat::infoString(theRowColMap, "theRowColMap") << std::endl;
	io::out() << dat::infoString(theSize, "theSize") << std::endl;
	io::out() << dat::infoString(theNdxGrid, "theNdxGrid") << std::endl;
	size_t const byteSize{ theNdxGrid.byteSize() };
	io::out() << dat::infoString(byteSize, "byteSize") << std::endl;
	for (size_t row{0u} ; row < theNdxGrid.high() ; ++row)
	{
		for (size_t col{0u} ; col < theNdxGrid.wide() ; ++col)
		{
			io::out() << " " << dat::infoString(theNdxGrid(row, col));
		}
		io::out() << std::endl;
	}
	*/
}

bool
NodeIndex :: isValid
	() const
{
	return
		(  dat::isValid(theRowColMap)
		&& dat::isValid(theNdxGrid)
		&& (0u < theSize)
		);
}

size_t
NodeIndex :: size
	() const
{
	return theSize;
}

std::string
NodeIndex :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	if (isValid())
	{
		oss << dat::infoString(theNdxGrid, "theNdxGrid");

		oss << std::endl;
		oss << dat::infoString(theRowColMap, "theRowColMap");

		oss << std::endl;
		oss << dat::infoString(theSize, "theSize");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

} // tri

