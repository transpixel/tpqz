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


namespace tri
{


// static
inline
FaceVerts
IsoTille :: triangleFor
	( dat::Spot const & xyLoc
	, IsoGeo const & tileGeo
	)
{
	FaceVerts triangle;

	std::pair<dat::QuantumFrac, dat::QuantumFrac>
		const mnNodeFracPair{ tileGeo.fracPairForRefSpot(xyLoc) };
	dat::QuantumFrac const & muNodeFrac = mnNodeFracPair.first;
	dat::QuantumFrac const & nuNodeFrac = mnNodeFracPair.second;

	long const & ndxI = muNodeFrac.floor();
	long const & ndxJ = nuNodeFrac.floor();
	double const & muFrac = muNodeFrac.fraction();
	double const & nuFrac = nuNodeFrac.fraction();

	if (muFrac < nuFrac)
	{
		// use triangle Tv
		triangle.theVerts[0].theI = ndxI;
		triangle.theVerts[0].theJ = ndxJ;
		triangle.theVerts[0].theW = 1. - nuFrac;

		triangle.theVerts[1].theI = ndxI + 1u;
		triangle.theVerts[1].theJ = ndxJ + 1u;
		triangle.theVerts[1].theW = muFrac;

		triangle.theVerts[2].theI = ndxI;
		triangle.theVerts[2].theJ = ndxJ + 1u;
		triangle.theVerts[2].theW = nuFrac - muFrac;
	}
	else
	{
		// use triangle Tu
		triangle.theVerts[0].theI = ndxI;
		triangle.theVerts[0].theJ = ndxJ;
		triangle.theVerts[0].theW = 1. - muFrac;

		triangle.theVerts[1].theI = ndxI + 1u;
		triangle.theVerts[1].theJ = ndxJ;
		triangle.theVerts[1].theW = muFrac - nuFrac;

		triangle.theVerts[2].theI = ndxI + 1u;
		triangle.theVerts[2].theJ = ndxJ + 1u;
		triangle.theVerts[2].theW = nuFrac;
	}

	return triangle;
}

template <typename SampFunc>
inline
typename SampFunc::value_type
IsoTille :: linearInterpWithCheck
	( Vec2D const & refSpot
	, SampFunc const & propSampFunc
	) const
{
	typename SampFunc::value_type samp{};
	if (theDomain.contains(refSpot))
	{
		samp = linearInterpForValid(refSpot, propSampFunc);
	}
	return samp;
}

template <typename SampFunc>
inline
typename SampFunc::value_type
IsoTille :: linearInterpForValid
	( Vec2D const & refSpot
	, SampFunc const & propSampFunc
	) const
{
	// get triangle covering this area...
	FaceVerts const triangle{ triangleFor(refSpot, theTileGeo) };

	// ... and return interpolated value
	return triangle.valueFrom<SampFunc>(propSampFunc);
}

} // tri

