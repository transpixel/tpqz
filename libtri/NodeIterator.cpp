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
\brief Definitions for tri::NodeIterator
*/


#include "libtri/NodeIterator.h"

#include <sstream>


namespace tri
{

// explicit
NodeIterator::IndexLimits :: IndexLimits
	( tri::IsoGeo const & trigeo
	, dat::Area<double> const & mnArea
	)
{
	// get bounding (mu,nu) limits of tile space
	dat::Spot const tileSpotBeg{{ mnArea[0].min(), mnArea[1].min() }};
	dat::Spot const tileSpotEnd{{ mnArea[0].max(), mnArea[1].max() }};

	// transform to index+resid form
	theFracPairBeg = trigeo.fracPairForTileSpot(tileSpotBeg);
	theFracPairEnd = trigeo.fracPairForTileSpot(tileSpotEnd);
}

std::pair<long, long>
NodeIterator::IndexLimits :: ndxBegEndI
	() const
{
	std::pair<long, long> pairBegEndI
		{ dat::nullValue<long>(), dat::nullValue<long>() };

	long const & iBeg = theFracPairBeg.first.theFloor;
	long const iEnd{ theFracPairEnd.first.theFloor + 1 };
	assert (iBeg < iEnd);
	pairBegEndI = { iBeg, iEnd };

	return pairBegEndI;
}

std::pair<long, long>
NodeIterator::IndexLimits :: ndxBegEndJ
	() const
{
	std::pair<long, long> pairBegEndJ
		{ dat::nullValue<long>(), dat::nullValue<long>() };

	long const & jBeg = theFracPairBeg.second.theFloor;
	long const jEnd{ theFracPairEnd.second.theFloor + 1 };
	assert (jBeg < jEnd);
	pairBegEndJ = { jBeg, jEnd };

	return pairBegEndJ;
}


//======================================================================
//======================================================================
//======================================================================



// explicit
NodeIterator :: NodeIterator
	( IsoGeo const & trigeo
	, Domain const & xyDomain
	)
	: theTileGeo{ trigeo }
	, theDomain{ xyDomain }
	, theNdxLimits
		{ IndexLimits(trigeo, theTileGeo.tileAreaForRefArea(xyDomain)) }
	, theBegEndI{ theNdxLimits.ndxBegEndI() }
	, theBegEndJ{ theNdxLimits.ndxBegEndJ() }
	, theAtIJ{ theBegEndI.first, theBegEndJ.first }
	, theIsActive{ true }
{
	if (! atNodeIsValid())
	{
		advanceToNextValid();
	}
}

std::string
NodeIterator :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}

	// oss << theTileGeo.infoString("theTileGeo");
	// oss << std::endl;

	oss << "theBegEndI: " << dat::infoString(theBegEndI);

	oss << std::endl;
	oss << "theBegEndJ: " << dat::infoString(theBegEndJ);

	oss << std::endl;
	if (dat::isValid(theAtIJ))
	{
		oss << "theAtIJ: " << dat::infoString(theAtIJ);
	}
	else
	{
		oss << "theAtIJ: " << "<null>";
	}

	oss << std::endl;
	oss << "active: " << dat::infoString(theIsActive);

	return oss.str();
}

} // tri

