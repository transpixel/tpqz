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
\brief Inline definitions for tri::IsoTille
*/


#include "libtri/IsoTille.h"

#include "libdat/MinMax.h"

#include <algorithm>
#include <sstream>
#include <utility>


namespace tri
{

// explicit
IsoTille :: IsoTille
	( tri::IsoGeo const & geometry
	, tri::Domain const & domain
	)
	: theTileGeo{ geometry }
	, theDomain{ domain }
{
}

bool
IsoTille :: isValid
	() const
{
	return
		{  dat::isValid(theTileGeo)
		};
}

NodeIterator
IsoTille :: begin
	() const
{
	return NodeIterator(theTileGeo, theDomain);
}

size_t
IsoTille :: sizeValidNodes
	() const
{
	size_t numValid{ 0u };
	for (NodeIterator iter{begin()} ; iter ; ++iter)
	{
		++numValid;
	}
	return numValid;
}

std::vector<NodeNdxPair>
IsoTille :: ndxPairsNearTo
	( NodeNdxPair const & ndxAt
	, double const & maxRefDist
	) const
{
	std::vector<NodeNdxPair> candNodes;

	// location of anchor node
	dat::Spot const atSpot(theTileGeo.refSpotForIndices(ndxAt));

	// bounding box in (orthotonal) Ref frame
	double const & dr = maxRefDist;
	dat::Range<double> const xRange(atSpot[0] - dr, atSpot[0] + dr);
	dat::Range<double> const yRange(atSpot[1] - dr, atSpot[1] + dr);
	dat::Area<double> const refArea{ xRange, yRange };
	dat::Area<double> const candArea
		{ theDomain.areaBounds().intersectWith(refArea) };

	// corresponding box in (non-orthogonal) Tile frame
	dat::Area<double> const tileArea{ theTileGeo.tileAreaForRefArea(candArea) };

	// corresponding box in (non-orthogonal) tile Indices enumeration
	dat::Area<NodeNdxType> const ijArea
		{ theTileGeo.ijAreaForTileArea(tileArea) };

	// generate individual index pairings for index area
	NodeNdxType const iBeg{ ijArea[0][0] };
	NodeNdxType const iEnd{ ijArea[0][1] + 1L };
	NodeNdxType const jBeg{ ijArea[1][0] };
	NodeNdxType const jEnd{ ijArea[1][1] + 1L };

	for (NodeNdxType ndxI{iBeg} ; ndxI < iEnd ; ++ndxI)
	{
		for (NodeNdxType ndxJ{jBeg} ; ndxJ < jEnd ; ++ndxJ)
		{
			candNodes.emplace_back(NodeNdxPair{ ndxI, ndxJ });
		}
	}

	return candNodes;
}

std::vector<IsoTille::DistNode>
IsoTille :: nodesNearTo
	( NodeNdxPair const & ndxAt
	, double const & maxRefDist
	) const
{
	std::vector<DistNode> distNodes;

	// get all candidate indices
	std::vector<NodeNdxPair> const ndxIJs(ndxPairsNearTo(ndxAt, maxRefDist));

	// location of anchor node
	dat::Spot const atSpot(theTileGeo.refSpotForIndices(ndxAt));

	// find qualifying nodes and check distance
	for (NodeNdxPair const & ndxIJ : ndxIJs)
	{
		// don't include self
		if (ndxAt != ndxIJ)
		{
			// get location of neighbor ...
			dat::Spot const refSpotIJ(theTileGeo.refSpotForIndices(ndxIJ));

			// check if valid within domain
			if (theDomain.contains(refSpotIJ))
			{
				// ... and distance to it
				using dat::operator-;
				double const nodeDist{ dat::magnitude(refSpotIJ - atSpot) };

				// check if sufficiently "nearby"
				if (! (maxRefDist < nodeDist))
				{
					distNodes.emplace_back(DistNode{ nodeDist, ndxIJ });
				}
			}
		}
	}

	// sort by underyling domain (Euclidean) distances
	std::sort(distNodes.begin(), distNodes.end());

	return distNodes;
}

std::string
IsoTille :: infoString
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
		oss << dat::infoString(theTileGeo, "theTileGeo");

		oss << std::endl;
		oss << dat::infoString(theDomain, "theDomain");

		dat::Area<double> const mnArea
			{ theTileGeo.tileAreaForDomain(theDomain) };
		oss << std::endl;
		oss << dat::infoString(mnArea, "mnArea");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

} // tri

