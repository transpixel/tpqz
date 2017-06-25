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
\brief Inline definitions for math::smooth
*/


#include "libmath/interp.h"

#include <cassert>


namespace math
{
namespace smooth
{
//======================================================================

namespace priv
{
	//! Weight values for a triangle shaped window
	std::vector<double>
	triangleWeights
		( size_t const & halfSize
		)
	{
		std::vector<double> weights(1u + 2u*halfSize);

		// configure for interpolation
		size_t const & ndxMax(weights.size() - 1u);
		std::pair<double, double> const ndxRange(0., (double)(halfSize + 1u));
		static std::pair<double, double> const wRange(0., 1.);

		// fill weights with relative values
		weights[halfSize] = 1.;
		for (size_t kk(0u) ; kk < halfSize ; ++kk)
		{
			double const weight
				(interp::linear(ndxRange, (double)(kk+1u), wRange));
			weights[kk] = weight;
			weights[ndxMax-kk] = weight;
		}

		// normlize to unit area
		double const sumW
			(std::accumulate(weights.begin(), weights.end(), 0.));
		assert(0. < sumW); // should be at least 1 (even for 0==halfSize)
		double const scale(1./sumW);
		std::transform
			( weights.begin(), weights.end()
			, weights.begin()
			, [& scale] (double const & value) { return scale*value; }
			);

		return weights;
	}
}

template <typename Type>
inline
std::vector<Type>
triangleWrapped
	( std::vector<Type> const & data
	, size_t const & halfSize
	)
{
	std::vector<Type> result(data.size(), static_cast<Type>(0));

	size_t const dSize(data.size());
	std::vector<double> const weights(priv::triangleWeights(halfSize));
	size_t const wSize(weights.size());

	// convolve data with window
	size_t const outDelta(halfSize);
	for (size_t datNdx(0u) ; datNdx < dSize ; ++datNdx)
	{
		for (size_t winNdx(0u) ; winNdx < wSize ; ++winNdx)
		{
			size_t const srcNdx((datNdx + winNdx) % dSize);
			size_t const outNdx((datNdx + outDelta) % dSize);
			result[outNdx] += weights[winNdx] * data[srcNdx];
		}
	}

	return result;
}

//======================================================================
}
}

