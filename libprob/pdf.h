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

#ifndef prob_pdf_INCL_
#define prob_pdf_INCL_

/*! \file
\brief Declarations for prob::pdf
*/


#include <array>
#include <vector>


namespace prob
{

/*! \brief basic probability density/mass functions.

\par Example
\dontinclude testprob/updf.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace pdf
{
	//! Cumulative probabilites (values sum to 1.)
	std::array<double, 256u>
	fromHist
		( std::array<size_t, 256u> const & hist
		);

	//! Normalize PDF (to sum of values)
	std::array<double, 256u>
	normalized
		( std::array<double, 256u> const & anyProbs
		);

	//! Data normalized to have unit sum
	template <typename FwdIter>
	inline
	std::vector<double>
	normalizedToUnitSum
		( FwdIter const & freqBeg
		, FwdIter const & freqEnd
		);

	//! Transform input frequency values to output data with unit sum
	template <typename FwdIter, typename OutIter>
	inline
	void
	normalizeToUnitSum
		( FwdIter const & freqBeg
		, FwdIter const & freqEnd
		, OutIter const & normBeg
		);

	//! (Renormalized) subset of input PDF
	std::array<double, 256u>
	pdfClipped
		( std::array<double, 256u> const & inPdf
		, double * const & ptFractionKept = nullptr
			//!< set to area under subsection of PDF (before renormalizing)
		, std::pair<size_t, size_t> const & keepIndexBegEnd
			= std::pair<size_t, size_t>(0u, 256u)
			//!< iterator bounds [beg,end) defining subset to keep
		);

	//! Entropy of a PDF
	template <typename Collection>
	inline
	double
	entropy
		( Collection const & pdf //!< *iter is prob *VALUE* (not density)
		);

}

}

// Inline definitions
#include "libprob/pdf.inl"

#endif // prob_pdf_INCL_

