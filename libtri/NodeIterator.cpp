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
NodeIterator :: NodeIterator
	( IsoGeo const & trigeo
	, Domain const & xyDomain
	)
	: NodeIterator()
{
	dat::Area<double> const mnRange{ trigeo.tileAreaForRefArea(xyDomain) };
	if (mnRange.isValid())
	{
		// get bounding (mu,nu) limits of tile space
		dat::Spot const tileSpotBeg{{ mnRange[0].min(), mnRange[1].min() }};
		dat::Spot const tileSpotEnd{{ mnRange[0].max(), mnRange[1].max() }};

		// transform to index+resid form
		IsoGeo::QuantPair const fracPairBeg
			(trigeo.fracPairForTileSpot(tileSpotBeg));
		IsoGeo::QuantPair const fracPairEnd
			(trigeo.fracPairForTileSpot(tileSpotEnd));

		// identify node indices with STL style iterators
		long const & iBeg = fracPairBeg.first.theFloor;
		long const iEnd{ fracPairEnd.first.theFloor + 1 };
		long const & jBeg = fracPairBeg.second.theFloor;
		long const jEnd{ fracPairEnd.second.theFloor + 1 };
		assert (iBeg < iEnd);
		assert (jBeg < jEnd);

		// set member vars
		theGeo = trigeo;
		theDomain = xyDomain;
		theBegEndI = { iBeg, iEnd };
		theBegEndJ = { jBeg, jEnd };
		theAtIJ = { theBegEndI.first, theBegEndJ.first };

		// ensure first node is over valid domain location (or end)
		theIsActive = true; // assume unless contradicted
		if (! atNodeIsValid())
		{
			advanceToNextValid();
		}
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

	// oss << theGeo.infoString("theGeo");
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

