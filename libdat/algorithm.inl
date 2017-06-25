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
\brief Declarations for dat::algorithm
*/


namespace dat
{
//======================================================================

template <typename FwdIter>
std::pair<size_t, size_t>
indexBounds
	( double const & findVal
	, FwdIter const & beg
	, FwdIter const & end
	)
{
	static size_t const badNdx(dat::nullValue<size_t>());
	std::pair<size_t, size_t> ndxLoHi(badNdx, badNdx);
	size_t & ndxLo = ndxLoHi.first;
	size_t & ndxHi = ndxLoHi.second;

	// check for empty case
	if (end != beg)
	{
		// find first element for which value is too big
		FwdIter itHi(beg);
		FwdIter itLo(beg);
		while ((end != itHi) && (*itHi <= findVal))
		{
			itLo = itHi; // track itHi with one element lag
			++itHi;
		}

		// determine index values from iterators
		ndxLo = std::distance(beg, itLo);
		if (end == itHi)
		{
			ndxHi = badNdx;
		}
		else
		{
			ndxHi = std::distance(beg, itHi);
			if (ndxLo == ndxHi)
			{
				if (findVal < (*itLo))
				{
					ndxLo = badNdx;
				}
				else
				if (findVal == (*itLo))
				{
					ndxHi = ndxLo + 1u;
				}
			}
		}
	}

	return ndxLoHi;
}

template <typename Type>
inline
Type
clamped
	( Type const & candidate
	, std::pair<Type, Type> const & range
	)
{
	Type value(nullValue<Type>());
	if (isValid(candidate))
	{
		Type const & minValue = range.first;
		Type const & maxValue = range.second;
		if (minValue <= maxValue)
		{
			value = std::min(std::max(minValue, candidate), maxValue);
		}
	}
	return value;
}

//======================================================================
}

