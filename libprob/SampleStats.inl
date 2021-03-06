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
\brief Inline definitions for prob::SampleStats
*/


#include "libprob/prob.h"

#include <iterator>


namespace prob
{
//======================================================================


template <typename FwdIter>
inline
// explicit
SampleStats :: SampleStats
	( FwdIter const & sampBeg
	, FwdIter const & sampEnd
	, math::Partition const & dataPart
	)
	: theDataPart(dataPart)
	, theCount(std::distance(sampBeg, sampEnd))
	, theHist(prob::histo::countsFromSamps(sampBeg, sampEnd, theDataPart))
	, thePdf(prob::pdf::normalizedToUnitSum(theHist.begin(), theHist.end()))
	, theEntropy(theCount, pdf::entropy(thePdf))
	, theCdfFwd
		( prob::CdfForward::fromFreqs
			(thePdf.begin(), thePdf.end(), theDataPart.range())
		)
	, theFrac9(theCdfFwd)
{
}

//======================================================================
}

