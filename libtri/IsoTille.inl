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

	NodeNdxType const & ndxI = muNodeFrac.theFloor;
	NodeNdxType const & ndxJ = nuNodeFrac.theFloor;
	double const & muFrac = muNodeFrac.theResid;
	double const & nuFrac = nuNodeFrac.theResid;

	if (muFrac < nuFrac)
	{
		// use triangle Tv
		triangle.theVerts[0].theNdxIJ.first = ndxI;
		triangle.theVerts[0].theNdxIJ.second = ndxJ;
		triangle.theVerts[0].theW = 1. - nuFrac;

		triangle.theVerts[1].theNdxIJ.first = ndxI + 1u;
		triangle.theVerts[1].theNdxIJ.second = ndxJ + 1u;
		triangle.theVerts[1].theW = muFrac;

		triangle.theVerts[2].theNdxIJ.first = ndxI;
		triangle.theVerts[2].theNdxIJ.second = ndxJ + 1u;
		triangle.theVerts[2].theW = nuFrac - muFrac;
	}
	else
	{
		// use triangle Tu
		triangle.theVerts[0].theNdxIJ.first = ndxI;
		triangle.theVerts[0].theNdxIJ.second = ndxJ;
		triangle.theVerts[0].theW = 1. - muFrac;

		triangle.theVerts[1].theNdxIJ.first = ndxI + 1u;
		triangle.theVerts[1].theNdxIJ.second = ndxJ;
		triangle.theVerts[1].theW = muFrac - nuFrac;

		triangle.theVerts[2].theNdxIJ.first = ndxI + 1u;
		triangle.theVerts[2].theNdxIJ.second = ndxJ + 1u;
		triangle.theVerts[2].theW = nuFrac;
	}

	return triangle;
}

inline
bool
IsoTille :: contains
	( NodeKey const & keyIJ
	) const
{
	dat::Spot const xyLoc(theTileGeo.refSpotForNodeKey(keyIJ));
	return theDomain.contains(xyLoc);
}

template <typename SampFunc>
inline
typename SampFunc::value_type
IsoTille :: linearInterpWithCheck
	( dat::Spot const & refSpot
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
	( dat::Spot const & refSpot
	, SampFunc const & propSampFunc
	) const
{
	// get triangle covering this area...
	FaceVerts const triangle{ triangleFor(refSpot, theTileGeo) };

	// ... and return interpolated value
	return triangle.valueFrom<SampFunc>(propSampFunc);
}

template <typename SampFunc>
inline
typename SampFunc::value_type
IsoTille :: nodeValueViaInvDist
	( NodeKey const & ndxGone
	, double const maxRefDist
	, SampFunc const & propSampFunc
	) const
{
	using DataType = typename SampFunc::value_type;
	DataType samp{};

	std::vector<DistKeyPair> const nearDistKeyPairs
		{ distKeysNearTo(ndxGone, maxRefDist) };

	// computed (inverse distance) weighted mean
	DataType sumWV{};
	double sumW{ 0. };
	for (tri::IsoTille::DistKeyPair const & nearDistKeyPair : nearDistKeyPairs)
	{
		// get info for a neighbor
		double const & dist = nearDistKeyPair.first;
		tri::NodeKey const & ndxIJ = nearDistKeyPair.second;

		// retrieve property at neighbor
		DataType const & value{ propSampFunc(ndxIJ.first, ndxIJ.second) };
		if (dat::isValid(value))
		{
			// use inverse (Ref)distance as weight
			double const weight{ 1. / dist};
			if (0. == sumW)
			{
				sumWV = weight * value;
				sumW = weight;
			}
			else
			{
				sumWV = sumWV + weight * value;
				sumW = sumW + weight;
			}
		}
	}
	if (std::numeric_limits<double>::min() < sumW)
	{
		samp = (1./sumW) * sumWV;
	}

	return samp;
}

} // tri

