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


namespace tri
{

// explicit
NodeIndex :: NodeIndex
	( tri::IsoTille const & trinet
	)
{
	// determine extents of trinet node keys
	tri::IsoGeo const & trigeo = trinet.theTileGeo;
	dat::Area<NodeNdxType> const ijArea
		{ trigeo.ijAreaForTileArea
			( trigeo.tileAreaForRefArea(trinet.theDomain.areaBounds())
			)
		};
	io::out() << dat::infoString(ijArea, "ijArea") << std::endl;
}

bool
NodeIndex :: isValid
	() const
{
	return false;
}

size_t
NodeIndex :: size
	() const
{
	return {}; // TODO
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
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

} // tri

