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
\brief Inline definitions for geo::ProbRay
*/



namespace geo
{

template <typename FwdIter>
// static
inline
ProbRay
ProbRay :: fromCones
	( FwdIter const & beg
	, FwdIter const & end
	, FwdIter const & primary
	, math::Partition const & probPart
	, ga::Vector const & coneAxis
	, double const & coneSigma
	)
{
	ProbRay probRay;
	geo::Ray const & ray = *primary;
	if (ray.isValid() && probPart.isValid() && coneAxis.isValid())
	{
		// NOTE: coneSigma is unused for only considerCone() below
		probRay = ProbRay(ray, probPart, coneSigma);
		for (FwdIter iter{beg} ; end != iter ; ++iter)
		{
			if (primary != iter) // skip self
			{
				geo::Ray const & ray = *iter;
				if (ray.isValid())
				{
					// assume other rays have the same uncertainty
					probRay.considerCone(ray, coneAxis, coneSigma);
				}
			}
		}
	}
	return probRay;
}

template <typename FwdIter>
// static
inline
ProbRay
ProbRay :: from
	( FwdIter const & beg
	, FwdIter const & end
	, FwdIter const & primary
	, math::Partition const & probPart
	, double const & rayAngleSigma
	)
{
	ProbRay probRay;
	geo::Ray const & ray = *primary;
	if (ray.isValid() && probPart.isValid())
	{
		probRay = ProbRay(ray, probPart, rayAngleSigma);
		for (FwdIter iter{beg} ; end != iter ; ++iter)
		{
			if (primary != iter) // skip self
			{
				geo::Ray const & ray = *iter;
				if (ray.isValid())
				{
					// assume other rays have the same uncertainty
					probRay.considerRay(ray, rayAngleSigma);
				}
			}
		}
	}
	return probRay;
}

inline
size_t
ProbRay :: numSamples
	() const
{
	return thePart.size();
}

template <typename InitFunc>
void
ProbRay :: initAccumulator
	( InitFunc const & func
	)
{
	size_t const numSamp{ numSamples() };
	for (size_t ndx{0u} ; ndx < numSamp ; ++ndx)
	{
		double const mu{ thePart.interpValueFor(double(ndx)) };
		theAccums[ndx] = func(mu);
	}
}

/*
// static
inline
double
ProbRay :: probFor
	( double const & angleValue
	, double const & angleSigma
	)
{
	double const argSq{ -.5 * math::sq(angleValue / angleSigma) };
	double const den{ std::sqrt(math::twoPi) * angleSigma };
	double const normCo( 1. / den );
	return (normCo * std::exp(argSq));
}
*/

} // geo

