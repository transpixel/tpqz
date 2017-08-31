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
Triangle
IsoTille :: triangleFor
	( dat::Spot const & xrel
	, IsoGeo const & tileGeo
	)
{
	Triangle tri;

	dat::QuantumFrac const muNdxFrac{ tileGeo.muNdxFrac(xrel) };
	dat::QuantumFrac const nuNdxFrac{ tileGeo.nuNdxFrac(xrel) };

	size_t const & ndxI = muNdxFrac.floor();
	size_t const & ndxJ = nuNdxFrac.floor();
	double const & muFrac = muNdxFrac.fraction();
	double const & nuFrac = nuNdxFrac.fraction();

	if (muFrac < nuFrac)
	{
		// use triangle Tv
		tri.theVerts[0].theI = ndxI;
		tri.theVerts[0].theJ = ndxJ;
		tri.theVerts[0].theW = 1. - nuFrac;

		tri.theVerts[1].theI = ndxI + 1u;
		tri.theVerts[1].theJ = ndxJ + 1u;
		tri.theVerts[1].theW = muFrac;

		tri.theVerts[2].theI = ndxI;
		tri.theVerts[2].theJ = ndxJ + 1u;
		tri.theVerts[2].theW = nuFrac - muFrac;
	}
	else
	{
		// use triangle Tu
		tri.theVerts[0].theI = ndxI;
		tri.theVerts[0].theJ = ndxJ;
		tri.theVerts[0].theW = 1. - muFrac;

		tri.theVerts[1].theI = ndxI + 1u;
		tri.theVerts[1].theJ = ndxJ;
		tri.theVerts[1].theW = muFrac - nuFrac;

		tri.theVerts[2].theI = ndxI + 1u;
		tri.theVerts[2].theJ = ndxJ + 1u;
		tri.theVerts[2].theW = nuFrac;
	}

	return tri;
}

template <typename SampFunc>
inline
typename SampFunc::value_type
IsoTille :: operator()
	( Vec2D const & xrel //!< location relative to tile origin
	, SampFunc const & propSampFunc
	) const
{
	// get triangle covering this area...
	Triangle const tri{ triangleFor(xrel, theTileGeo) };
	// ... and return interpolated value
	return tri.valueFrom<SampFunc>(propSampFunc);
}

} // tri

