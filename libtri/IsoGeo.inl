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
double
IsoGeo :: muForXY
	( dat::Spot const & xyLoc
	) const
{
	return dat::dot(theBarU, xyLoc);
}

inline
double
IsoGeo :: nuForXY
	( dat::Spot const & xyLoc
	) const
{
	return dat::dot(theBarV, xyLoc);
}

inline
dat::Spot
IsoGeo :: mnLocForXY
	( dat::Spot const & xyLoc
	) const
{
	return {{ muForXY(xyLoc), nuForXY(xyLoc) }};
}

inline
std::pair<dat::QuantumFrac, dat::QuantumFrac>
IsoGeo :: cellPairForXY
	( dat::Spot const & xyLoc
	) const
{
	dat::Spot const mnLoc(mnLocForXY(xyLoc));
	return cellPairForMuNu(mnLoc);
}

inline
long
IsoGeo :: indexForMu
	( double const & mu
	) const
{
	return dat::QuantumFrac(mu, theSplitterMu).floor();
}

inline
long
IsoGeo :: indexForNu
	( double const & nu
	) const
{
	return dat::QuantumFrac(nu, theSplitterNu).floor();
}

inline
double
IsoGeo :: muFromIndex
	( long const & muNdx
	) const
{
	return theSplitterMu.value({muNdx, 0.});
}

inline
double
IsoGeo :: nuFromIndex
	( long const & nuNdx
	) const
{
	return theSplitterNu.value({nuNdx, 0.});
}

inline
dat::Spot
IsoGeo :: mnLocForNode
	( std::pair<long, long> const & ndxPairMuNu
	) const
{
	long const & muNdx = ndxPairMuNu.first;
	long const & nuNdx = ndxPairMuNu.second;
	return dat::Spot{{ muFromIndex(muNdx), nuFromIndex(nuNdx) }};
}

inline
std::pair<dat::QuantumFrac, dat::QuantumFrac>
IsoGeo :: cellPairForMuNu
	( dat::Spot const & mnLoc
	) const
{
	return
		{ dat::QuantumFrac(mnLoc[0], theSplitterMu)
		, dat::QuantumFrac(mnLoc[1], theSplitterNu)
		};
}

inline
dat::Spot
IsoGeo :: xyLocForMuNu
	( dat::Spot const & mnRel
	) const
{
	using dat::operator*;
	using dat::operator+;
	return (mnRel[0]*theDirU + mnRel[1]*theDirV);
}

inline
dat::Spot
IsoGeo :: xyLocForNode
	( std::pair<long, long> const & ndxPairMuNu
	) const
{
	return xyLocForMuNu(mnLocForNode(ndxPairMuNu));
}

} // tri

