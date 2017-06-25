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


#include "libprob/pdf.h"

#include "libdat/algorithm.h"
#include "libdat/ops.h"

#include <algorithm>


namespace prob
{
namespace pdf
{
//======================================================================

std::array<double, 256u>
fromHist
	( std::array<size_t, 256u> const & hist
	)
{
	std::array<double, 256> pdf;

	// copy (while promoting) hist values into PDF storage
	std::copy(hist.begin(), hist.end(), pdf.begin());

	// normalize to unit "area"
	pdf = normalized(pdf);

	return pdf;
}

std::array<double, 256u>
normalized
	( std::array<double, 256u> const & anyProbs
	)
{
	std::array<double, 256> normPdf;
	normalizeToUnitSum
		( anyProbs.begin(), anyProbs.end()
		, normPdf.begin()
		);
	return normPdf;
}

std::array<double, 256u>
pdfClipped
	( std::array<double, 256u> const & inPdf
	, double * const & ptFractionKept
	, std::pair<size_t, size_t> const & keepIndexBegEnd
	)
{
	static constexpr size_t Dim(256u);
	std::array<double, Dim> outPdf;

	// leave unset values at zero prob
	std::fill(outPdf.begin(), outPdf.end(), 0.);

	double fracKept(0.);
	if (keepIndexBegEnd.first < keepIndexBegEnd.second)
	{
		// compute offsets to begin/end of "keepers"
		static constexpr std::pair<size_t, size_t> ndxBegEnd(0u, Dim);
		size_t const ndxBeg(dat::clamped(keepIndexBegEnd.first, ndxBegEnd));
		size_t const ndxEnd(dat::clamped(keepIndexBegEnd.second, ndxBegEnd));

		// discard values outside the range
		std::copy
			( inPdf.begin() + ndxBeg, inPdf.begin() + ndxEnd
			, outPdf.begin() + ndxBeg
			);

		// renormalize the output PDF to unit area
		double const keepSum
			(std::accumulate(outPdf.begin(), outPdf.end(), 0.));
		using dat::operator*;
		outPdf = (1./keepSum) * outPdf;

		// remember fraction of subset
		static constexpr double inSum(1.);
		fracKept = keepSum / inSum;
	}

	// set return scale factor when requested
	if (ptFractionKept)
	{
		*ptFractionKept = fracKept;
	}

	return outPdf;
}

//======================================================================
}
}

