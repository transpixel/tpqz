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
\brief Inline definitions for tri::IsoGeo
*/


namespace tri
{

inline
dat::Spot
IsoGeo :: tileSpotForRefSpot
	( dat::Spot const & refSpot
	) const
{
	return
		{{ dat::dot(theBarU, refSpot)
		 , dat::dot(theBarV, refSpot)
		}};
}

inline
dat::Spot
IsoGeo :: tileSpotForFracPair
	( QuantPair const & fracPair
	) const
{
	long const & muNdx = fracPair.first.theFloor;
	long const & nuNdx = fracPair.second.theFloor;
	return dat::Spot
		{{ theSplitterMu.value({muNdx, 0.})
		 , theSplitterNu.value({nuNdx, 0.})
		}};
}

inline
IsoGeo::QuantPair
IsoGeo :: fracPairForRefSpot
	( dat::Spot const & refSpot
	) const
{
	dat::Spot const tileSpot(tileSpotForRefSpot(refSpot));
	return fracPairForTileSpot(tileSpot);
}

inline
IsoGeo::QuantPair
IsoGeo :: fracPairForTileSpot
	( dat::Spot const & tileSpot
	) const
{
	return
		{ dat::QuantumFrac(tileSpot[0], theSplitterMu)
		, dat::QuantumFrac(tileSpot[1], theSplitterNu)
		};
}

inline
IsoGeo::QuantPair
IsoGeo :: fracPairForIndices
	( long const & ndxI
	, long const & ndxJ
	) const
{
	return
		{ dat::QuantumFrac(double(ndxI), 0., theSplitterMu)
		, dat::QuantumFrac(double(ndxJ), 0., theSplitterNu)
		};
}

inline
dat::Spot
IsoGeo :: refSpotForTileSpot
	( dat::Spot const & tileSpot
	) const
{
	using dat::operator*;
	using dat::operator+;
	return (tileSpot[0]*theDirU + tileSpot[1]*theDirV);
}

inline
dat::Spot
IsoGeo :: refSpotForFracPair
	( QuantPair const & fracPair
	) const
{
	return refSpotForTileSpot(tileSpotForFracPair(fracPair));
}

} // tri

