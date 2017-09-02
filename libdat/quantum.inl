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
	( FracType const & valueInReal
	) const
{
	std::pair<BaseType, FracType> qfValue;
	BaseType & numQuanta = qfValue.first;
	FracType & residInQuanta = qfValue.second;

	// normalize input w.r.t. quanta size
	constexpr FracType fOne{ 1 };
	FracType const invDelta{ (fOne/theDelta) };
	FracType const valueInQuanta{ invDelta * valueInReal };

	// determine floor
	FracType const floorInQuanta{ std::floor(valueInQuanta) };
	numQuanta = static_cast<BaseType>(floorInQuanta);

	// set residual to remainder above the floor
	residInQuanta = valueInQuanta - floorInQuanta;

	return qfValue;
}

} // quantum

} // dat

