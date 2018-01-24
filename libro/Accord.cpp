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
\brief Definitions for ro::Accord
*/


#include "libro/Accord.h"

#include <sstream>


namespace ro
{

bool
Accord :: isValid
	() const
{
	return
		(  thePtPairUVs
		&& theSoln.isValid()
		);
}

size_t
Accord :: numTotalUVs
	() const
{
	size_t num{ 0u };
	if (thePtPairUVs)
	{
		num = thePtPairUVs->size();
	}
	return num;
}

size_t
Accord :: numFreeUVs
	() const
{
	size_t num{ 0u };
	if (4u < numTotalUVs())
	{
		num = numTotalUVs() - 5u;
	}
	return num;
}

double
Accord :: gapForNdx
	( size_t const & ndx
	) const
{
	double gap{ dat::nullValue<double>() };
	if (isValid())
	{
		if (ndx < thePtPairUVs->size())
		{
			ro::PairUV const & uvPair = (*thePtPairUVs)[ndx];
			gap = theSoln.theRoPair->tripleProductGap(uvPair);
		}
	}
	return gap;
}

double
Accord :: rmsGap
	() const
{
	double rms{ dat::nullValue<double>() };
	if (isValid())
	{
		double const dof{ (double)numFreeUVs() };
		if (0. < dof)
		{
			std::vector<double> gaps(numTotalUVs());
			for (size_t nn{0u} ; nn < numTotalUVs() ; ++nn)
			{
				double const gap{ gapForNdx(nn) };
				if (dat::isValid(gap))
				{
					gaps[nn] = gap;
				}
			}

			double const sumSq
				{ math::sumSqs<double>(gaps.begin(), gaps.end()) };
			rms = std::sqrt(sumSq / dof);
		}
	}
	return rms;
}

std::string
Accord :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (isValid())
	{
		oss << dat::infoString(theSoln, title);
		oss << std::endl;
		oss
			<< " " << dat::infoString(numTotalUVs(), "numTotalUVs")
			<< " " << dat::infoString(numFreeUVs(), "numFreeUVs")
			<< " " << dat::infoString(rmsGap(), "rmsGap")
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

} // ro

