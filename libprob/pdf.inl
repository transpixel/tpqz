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
\brief Definitions for prob::pdf
*/


#include "libdat/dat.h"

#include <algorithm>
#include <cmath>
#include <iterator>


namespace prob
{
namespace pdf
{
//======================================================================

template <typename FwdIter>
inline
std::vector<double>
normalizedToUnitSum
	( FwdIter const & freqBeg
	, FwdIter const & freqEnd
	)
{
	std::vector<double> normVals;
	size_t const numData(std::distance(freqBeg, freqEnd));
	normVals.resize(numData);
	normalizeToUnitSum(freqBeg, freqEnd, normVals.begin());
	return normVals;
}

template <typename FwdIter, typename OutIter>
inline
void
normalizeToUnitSum
	( FwdIter const & freqBeg
	, FwdIter const & freqEnd
	, OutIter const & normBeg
	)
{
	if (freqEnd != freqBeg)
	{
		// accumulate frequency values - using high precision
		double total(0.);
		for (FwdIter iter(freqBeg) ; freqEnd != iter ; ++iter)
		{
			total += static_cast<double>(*iter);
		}

		// determine scale (if possible)
		double scale(dat::nullValue<double>());
		if (std::numeric_limits<double>::min() < total)
		{
			// compute scale
			scale = (1. / total);
		}

		// normalize data while copy to output
		typedef typename std::iterator_traits<FwdIter>::value_type FreqType; 
		typedef typename std::iterator_traits<OutIter>::value_type NormType; 
		std::transform
			( freqBeg, freqEnd
			, normBeg
			, [& scale]
				( FreqType const & freq )
				{ return static_cast<NormType>(scale*freq); }
			);
	}
}

template <typename Collection>
inline
double
entropy
	( Collection const & pdf
	)
{
	double bigH(0.);
	typedef typename Collection::const_iterator FwdIter;
	for (FwdIter iter(pdf.begin()) ; pdf.end() != iter ; ++iter)
	{
		double const & prob = *iter;
		if (0. < prob) // for 0, limit of P*ln(P) is zero - skip any negs
		{
			bigH -= prob * std::log(prob);
		}
	}
	return bigH;
}

//======================================================================
}
}

