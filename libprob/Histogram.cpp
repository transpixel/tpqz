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
\brief Definitions for prob::Histogram
*/


#include "libprob/Histogram.h"

#include "libdat/info.h"

#include <algorithm>
#include <cassert>
#include <sstream>


namespace prob
{
//======================================================================

Histogram :: Histogram
	()
	: thePart()
	, theCounts()
{
}

// explicit
Histogram :: Histogram
	( math::Partition const & part
	)
	: thePart(part)
	, theCounts(thePart.size())
{
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

bool
Histogram :: isValid
	() const
{
	return
		(  thePart.isValid()
		&& (! theCounts.empty())
		);
}

size_t
Histogram :: size
	() const
{
	return theCounts.size();
}

size_t
Histogram :: total
	() const
{
	return std::accumulate(theCounts.begin(), theCounts.end(), 0u);
}

std::vector<double>
Histogram :: pdf
	() const
{
	std::vector<double> probs;
	if (isValid())
	{
		probs.resize(size());
		double const sum(static_cast<double>(total()));
		if (0. < sum)
		{
			std::transform
				( theCounts.begin(), theCounts.end()
				, probs.begin()
				, [&sum]
					( size_t const & count )
					{ return ((double)count / sum); }
				);
		}
	}
	return probs;
}

std::vector<double>
Histogram :: binCenters
	() const
{
	std::vector<double> centers;
	if (isValid())
	{
		size_t const numBins(size());
		centers.resize(numBins);
		for (size_t ndx(0u) ; ndx < numBins ; ++ndx)
		{
			std::pair<double, double> const binRange(thePart.rangeForBin(ndx));
			double const center(.5 * (binRange.first + binRange.second));
			centers[ndx] = center;
		}
	}
	return centers;
}

dat::Range<double>
Histogram :: range
	() const
{
	return thePart.range();
}

std::string
Histogram :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << " ";
	}

	if (isValid())
	{
		oss
			<< "thePart: " << thePart.infoString()
			<< " " << "total: " << total()
			;
	}
	else
	{
		oss << " <null>";
	}

	return oss.str();
}

std::string
Histogram :: infoStringContents
	( std::string const & title
	) const
{
	std::ostringstream oss;
	oss << infoString(title);
	if (isValid())
	{
		std::vector<double> const locs(binCenters());
		std::vector<double> const probs(pdf());
		size_t const numLocs(locs.size());
		assert(numLocs == probs.size());

		double sum(0.);
		for (size_t nn(0u) ; nn < numLocs ; ++nn)
		{
			double const & loc = locs[nn];
			double const & prob = probs[nn];
			sum += prob;
			oss << '\n'
				<< " " << dat::infoString(loc)
				<< " " << dat::infoString(prob, "", 9u, 6u)
				<< " " << dat::infoString(sum, "", 9u, 6u)
				;
		}
	}
	return oss.str();
}

bool
Histogram :: writeDataProbs
	( std::ostream & oss
	, std::string const & title
	) const
{
	bool okay(false);
	oss << "# " << title << " ";
	if (isValid())
	{
		double const sumCounts(total());
		if (std::numeric_limits<double>::min() < sumCounts)
		{
			size_t const numBins(theCounts.size());
			for (size_t ndx(0u) ; ndx < numBins ; ++ndx)
			{
				std::pair<double, double> const binRange
					(thePart.rangeForBin(ndx));
				double const & dataValue = binRange.first;
				double const prob(double(theCounts[ndx]) / sumCounts);

				oss
					<< '\n'
					<< dat::infoString(dataValue, "", 14u, 6u)
					<< " " << dat::infoString(prob, "", 14u, 9u)
					;
			}
			okay = (! oss.fail());
		}
		oss << std::endl;
	}
	else
	{
		oss << " <null>";
	}

	return okay;
}

//======================================================================
}

