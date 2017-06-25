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
\brief Inline definitions for geo::stats
*/


namespace geo
{
namespace stats
{

template <typename FwdIter>
inline
ga::Vector
centroidOf
	( FwdIter const & beg
	, FwdIter const & end
	)
{
	ga::Vector ave{};

	// accumulate point samples across all acquisitions
	ga::Vector pntSum(ga::vZero);
	double count{ 0. };
	for (FwdIter iter{beg} ; end != iter ; ++iter)
	{
		ga::Vector const & pnt = *iter;
		if (dat::isValid(pnt))
		{
			pntSum = pntSum + pnt;
			count += 1.;
		}
	}

	// compute average point location
	if (0. < count)
	{
		ga::Vector const avePnt((1./count) * pntSum);
		ave = avePnt;
	}

	return ave;
}

} // stats
} // geo

