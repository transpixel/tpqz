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
\brief Inline definitions for dat::quantum
*/


#include "libio/stream.h"
#include "libdat/info.h"

#include <cassert>


namespace dat
{

namespace quantum
{

template
	< typename BaseType, typename FracType
	>
inline
std::pair<BaseType, FracType>
Splitter<BaseType, FracType> :: operator()
	( FracType const & someValue
	) const
{
	std::pair<BaseType, FracType> bfResult;

	constexpr FracType zero{ 0 };
	if (! (someValue < zero))
	{
		//
		// handle positive input value
		//

		BaseType const numDeltas
			{ static_cast<BaseType>(std::floor(someValue / theDelta)) };

		FracType const baseValue{ static_cast<FracType>(numDeltas) * theDelta };
		FracType const magResid{ someValue - baseValue };
		FracType fracResid{ magResid / theDelta };

		bfResult = { numDeltas, fracResid };
	}
	else
	{
		//
		// handle negative input value
		//

		FracType const posValue{ std::abs(someValue) };
		BaseType const posBase
			{ static_cast<BaseType>(std::floor(posValue / theDelta)) };

		constexpr BaseType bOne{ 1 };
		BaseType numDeltas{ -posBase - bOne};

		// compute value at floor
		FracType const baseValue{ static_cast<FracType>(numDeltas) * theDelta };
		FracType const magResid{ someValue - baseValue };
		FracType fracResid{ magResid / theDelta };

		constexpr FracType fOne{ 1 };
		if (fOne == fracResid)
		{
			++numDeltas;
			fracResid = fracResid - fOne;
		}

		bfResult = { numDeltas, fracResid };
	}

	assert((bfResult.first*theDelta) <= someValue);
	assert(FracType{0} <= bfResult.second);
	assert(bfResult.second < FracType{1});

	return bfResult;
}

} // quantum

} // dat

