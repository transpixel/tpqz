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
\brief Inline definitions for prob::Histogram
*/



namespace prob
{
//======================================================================

template <typename FwdIter>
inline
// explicit
Histogram :: Histogram
	( math::Partition const & part
	, FwdIter const & sampBeg
	, FwdIter const & sampEnd
	)
	: Histogram(part)
{
	for (FwdIter iter(sampBeg) ; sampEnd != iter ; ++iter)
	{
		double const samp(static_cast<double>(*iter));
		incorporateSample(samp);
	}
}

inline
void
Histogram :: addSample
	( double const & sample
	)
{
	incorporateSample(sample);
}

template <typename FwdIter>
inline
void
Histogram :: addSamples
	( FwdIter const & sampBeg
	, FwdIter const & sampEnd
	)
{
	for (FwdIter iter(sampBeg) ; sampEnd != iter ; ++iter)
	{
		double const samp(static_cast<double>(*iter));
		incorporateSample(samp);
	}
}

inline
void
Histogram :: incorporateSample
	( double const & sample
	)
{
	if (sample < thePart.min())
	{
		++theCountUnder;
	}
	else
	if (! (sample < thePart.max()))
	{
		++theCountOver;
	}
	else
	{
		size_t const binNdx(thePart.binIndexFor(sample));
		assert(binNdx < theCounts.size());
		++( theCounts[binNdx] );
	}
}

//======================================================================
}

