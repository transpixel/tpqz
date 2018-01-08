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
\brief Definitions for sig::Peak
*/


#include "libsig/Peak.h"

#include "libdat/Spot.h"
#include "libmath/math.h"

#include <algorithm>
#include <utility>


namespace sig
{

namespace
{
	//! Cast of 'floor' value for positive number
	inline
	size_t
	ndxFor
		( double const & dval
		)
	{
		assert(! (dval < 0.));
		return static_cast<size_t>(std::floor(dval));
	}

	//! Collection of row/col values within minRadius of centerSpot
	std::vector<dat::RowCol>
	nearbyRowCols
		( dat::Spot const & centerSpot
		, dat::Extents const & hwDomain
		, double const & minRadius
		)
	{
		std::vector<dat::RowCol> rowcols;

		// compute "large enough" region limits
		using dat::operator-;
		using dat::operator+;
		dat::Spot const delta{{ minRadius, minRadius }};
		constexpr dat::Spot const oneElem{{ 1., 1. }};
		dat::Spot const spotUL(centerSpot - delta);
		dat::Spot const spotBR(centerSpot + delta + oneElem);

		size_t const rowBeg{ ndxFor(std::max(spotUL[0], 0.)) };
		size_t const colBeg{ ndxFor(std::max(spotUL[1], 0.)) };
		size_t const rowEnd{ std::min(ndxFor(spotBR[0]), hwDomain.high()) };
		size_t const colEnd{ std::min(ndxFor(spotBR[1]), hwDomain.wide()) };

		/*
		dat::RowCol const rcBeg{{ rowBeg, colBeg }};
		dat::RowCol const rcEnd{{ rowEnd, colEnd }};
		io::out() << "nearbyRowCols" << std::endl;
		io::out() << dat::infoString(hwDomain, "hwDomain") << std::endl;
		io::out() << dat::infoString(centerSpot, "centerSpot") << std::endl;
		io::out() << dat::infoString(rcBeg, "rcBeg") << std::endl;
		io::out() << dat::infoString(rcEnd, "rcEnd") << std::endl;
		*/

		rowcols.reserve((rowEnd-rowBeg) * (colEnd-colBeg));

		// qualify individual row/col values by proximity
		double const minRadSq{ math::sq(minRadius) };
		for (size_t row(rowBeg) ; row < rowEnd ; ++row)
		{
			for (size_t col(colBeg) ; col < colEnd ; ++col)
			{
				dat::Spot const atSpot{{ double(row), double(col) }};
				dat::Spot const delta(atSpot - centerSpot);
				double const radSq{ math::sq(delta[0]) + math::sq(delta[1]) };
				if (radSq <= minRadSq)
				{
					rowcols.emplace_back(dat::RowCol{{ row, col }});
				}
			}
		}
		return rowcols;
	}

	//! Largest value of the 8 direct neighbors
	std::vector<double>
	rankedNeighbors
		( dat::grid<double> const & grid
		, std::vector<dat::RowCol> const  rowcols
		)
	{
		std::vector<double> nearValues;

		// record (valid) scores for nearby locations
		nearValues.reserve(rowcols.size());
		for (dat::RowCol const & rowcol : rowcols)
		{
			double const & value = grid(rowcol);
			if (dat::isValid(value))
			{
				nearValues.emplace_back(value);
			}
		}

		// sort such that peak is first element, then decreasing
		std::sort(nearValues.rbegin(), nearValues.rend());

		return nearValues;
	}

	//! TTue if currValue should replace prevLimit
	template <typename Type, typename QualFunc>
	inline
	bool
	isNewLimit
		( Type const & currValue
		, Type const & prevLimit
		, QualFunc const & qualFunc
		)
	{
		bool update{ false };
		bool const isFirst{ (! dat::isValid(prevLimit)) };
		if (isFirst)
		{
			update = true;
		}
		else
		{
			bool const isQualified(qualFunc(currValue, prevLimit));
			update = isQualified;
		}
		return update;
	}

	//! True if rcA is within (<=) minSep (taxicab) distance from rcB
	inline
	bool
	areNeighbors
		( dat::RowCol const rcA
		, dat::RowCol const rcB
		, size_t const & minSep
		)
	{
		size_t const sepRow{ math::separation(rcA[0], rcB[0]) };
		size_t const sepCol{ math::separation(rcA[1], rcB[1]) };
		bool const isOutRow{ minSep < sepRow };
		bool const isOutCol{ minSep < sepCol };
		return (! (isOutRow || isOutCol));
	}

	//! Like max_element but only for valid elements
	dat::grid<double>::const_iterator
	iteratorForMaxValid
		( dat::grid<double> const & grid
		)
	{
		using GridIter = dat::grid<double>::const_iterator;
		GridIter itMax(grid.end());

		// compare condition
		using larger = std::greater<double>;

		// find largest valid item
		double maxValue{ dat::nullValue<double>() };
		for (GridIter iter(grid.begin()) ; grid.end() != iter ; ++iter)
		{
			double const & value = *iter;
			if (dat::isValid(value))
			{
				if (isNewLimit(value, maxValue, larger()))
				{
					maxValue = value;
					itMax = iter;
				}
			}
		}

		return itMax;
	}

	//! like iteratorForMaxValid() but don't consider values near rcSkip
	dat::RowCol
	secondBestRowCol
		( dat::grid<double> const & grid
		, dat::RowCol const & rcSkip
		, double const & minPeakRadius
		)
	{
		dat::RowCol rcElse(dat::nullValue<dat::RowCol>());;

		// compare condition
		using larger = std::greater<double>;

		double maxValue{ dat::nullValue<double>() };
		for (size_t row(0u) ; row < grid.high() ; ++row)
		{
			for (size_t col(0u) ; col < grid.wide() ; ++col)
			{
				dat::RowCol const currRC{{ row, col }};
				if (! areNeighbors(currRC, rcSkip, minPeakRadius))
				{
					double const & value = grid(row, col);
					if (dat::isValid(value))
					{
						if (isNewLimit(value, maxValue, larger()))
						{
							maxValue = value;
							rcElse = currRC;
						}
					}
				}
			}
		}

		return rcElse;
	}

	//! Location of maximum value
	std::vector<dat::RowCol>
	topRowCols
		( dat::grid<double> const & grid
		, double const & minPeakRadius
		)
	{
		std::vector<dat::RowCol> topRCs;
		if (grid.isValid())
		{
			using GridIter = dat::grid<double>::const_iterator;

			// find best value
			GridIter const itBest(iteratorForMaxValid(grid));
			if (grid.end() != itBest)
			{
				// record primary peak location
				topRCs.emplace_back(grid.rowColFor(itBest));
				dat::RowCol const & rcBest = topRCs.back();

				// find second best value that is not "near" the best value
				dat::RowCol const rcElse
					(secondBestRowCol(grid, rcBest, minPeakRadius));
				if (dat::isValid(rcElse))
				{
					// record secondary peak location
					topRCs.emplace_back(rcElse);
				}
			}
		}

		return topRCs;
	}
}

// static
Peak
Peak :: fromGrid
	( dat::grid<double> const & scoreGrid
	, double const & usePeakRadius
	)
{
	Peak peak;

	// Find best score and issolated secondary best
	std::vector<dat::RowCol> const topRCs
		(topRowCols(scoreGrid, usePeakRadius));

	if (! topRCs.empty())
	{
		Peak tmp;

		// extract best peak value
		dat::RowCol const & rcBest = topRCs[0];
		double const bestScore{ scoreGrid(rcBest) };
		tmp.theBestRowCol = rcBest;
		tmp.theBestScore = bestScore;

		// get locations near to center
		dat::Spot const bestSpot(dat::cast::Spot(rcBest));
		std::vector<dat::RowCol> const hoodRCs
			(nearbyRowCols(bestSpot, scoreGrid.hwSize(), usePeakRadius));

		// extract neighbors ordered by rank 
		tmp.theHoodRadius = usePeakRadius;
		tmp.theHoodScores = rankedNeighbors(scoreGrid, hoodRCs);

		// extract second best
		if (1u < topRCs.size())
		{
			dat::RowCol const & rcElse = topRCs[1];
			double const nextScore{ scoreGrid(rcElse) };
			tmp.theElseRowCol = rcElse;
			tmp.theElseScore = nextScore;
		}

		// if peak contains valid data, return it
		if (tmp.isValid())
		{
			peak = tmp;
		}
	}

	return peak;
}

namespace
{
	//! Weighting function to use for peak values
	inline
	double
	weightFor
		( double const & score
		, double const & refScore
		)
	{
		double const deltaSq{ math::sq(refScore - score) };
		return std::exp(-deltaSq);
	}

	//! Weighted centroid using values within minRadius of centerRC
	dat::Spot
	centroidFrom
		( dat::grid<double> const & scoreGrid
		, dat::Spot const & centerSpot
		, double const & usePeakRadius
		, double const & refScore
		)
	{
		dat::Spot ave(dat::nullValue<dat::Spot>());

		// get locations near to center
		std::vector<dat::RowCol> const rowcols
			(nearbyRowCols(centerSpot, scoreGrid.hwSize(), usePeakRadius));

		// accumulate score-weighted sums of nearby locations
		double sumRowW{ 0. };
		double sumColW{ 0. };
		double sumW{ 0. };
		for (dat::RowCol const & rowcol : rowcols)
		{
			double const & score = scoreGrid(rowcol);
			if (dat::isValid(score))
			{
				dat::Spot const atSpot(dat::cast::Spot(rowcol));
				double const weight{ weightFor(score, refScore) };
				sumRowW += weight * atSpot[0];
				sumColW += weight * atSpot[1];
				sumW += weight;
			}
		}

		// evaluate centroid
		if (dat::smallValue<double>() < sumW)
		{
			ave = dat::Spot{{ sumRowW/sumW, sumColW/sumW }};
		}

		return ave;
	}
}

// static
dat::Spot
Peak :: fitSpotFor
	( dat::grid<double> const & scoreGrid
	, Peak const & initPeak
	, double const & peakRadius
	)
{
	dat::Spot tipSpot(dat::nullValue<dat::Spot>());

	// if peak not provided, then search for one
	Peak qualPeak(initPeak);
	if (! qualPeak.isValid())
	{
		// find best score offset
		qualPeak = Peak::fromGrid(scoreGrid, peakRadius);
	}

	// perform peak fit
	if (qualPeak.isValid())
	{
		// use this as reference value for weight computation
		double const & refScore = initPeak.theBestScore;

		// estimate peak with values nearest max
		dat::Spot const nomPeak(dat::cast::Spot(qualPeak.theBestRowCol));
		tipSpot = centroidFrom(scoreGrid, nomPeak, peakRadius, refScore);

		// refine estimate with prior fit
		// (weighting function changes based on peak location)
		// (also may change neighborhood and hence samples used to compute fit)
		constexpr size_t maxN{ 4u }; // ad hoc value
		for (size_t nn(0u) ; (dat::isValid(tipSpot) && (nn < maxN)) ; ++nn)
		{
			tipSpot = centroidFrom(scoreGrid, tipSpot, peakRadius, refScore);
		}
	}

	return tipSpot;
}


Peak :: Peak
	()
	: theBestRowCol(dat::nullValue<dat::RowCol>())
	, theBestScore{ dat::nullValue<double>() }
	, theElseRowCol(dat::nullValue<dat::RowCol>())
	, theElseScore{ dat::nullValue<double>() }
	, theHoodRadius{ dat::nullValue<double>() }
	, theHoodScores{}
{
}

bool
Peak :: isValid
	() const
{
	return
		(  dat::isValid(theBestRowCol)
		&& dat::isValid(theBestScore)
		);
}

double
Peak :: prominenceRank
	() const
{
	double rank{ dat::nullValue<double>() };
	if (isValid() && (! theHoodScores.empty()))
	{
		// determine where secondary peak ranks relative to primary hood
		size_t count{ 0u };
		for (double const & hoodScore : theHoodScores)
		{
			if (theElseScore < hoodScore)
			{
				++count;
			}
		}
		double const peakArea{ double(count) };
		double const maxArea{ double(hoodArea()) };
		rank = 0.; // e.g. if peak radius includes only center pixel
		if (1. < maxArea)
		{
			rank = peakArea / (maxArea - 1.);
		}
	}

	return rank;
}

double
Peak :: ambiguityRatio
	() const
{
	double ratio{ dat::nullValue<double>() };
	if (isValid())
	{
		ratio = 1.; // totally ambiguous
		if (math::eps < std::abs(theElseScore))
		{
			ratio = (theBestScore / theElseScore);
		}
	}
	return ratio;
}

std::string
Peak :: infoString
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
			<< "rcsBest:"
			<< " " << dat::infoString(theBestRowCol[0], "", 4u)
			<< " " << dat::infoString(theBestRowCol[1], "", 4u)
			<< " " << dat::infoString(theBestScore, "", 7u)
			<< " "
			<< " rcsElse:"
			<< " " << dat::infoString(theElseRowCol[0], "", 4u)
			<< " " << dat::infoString(theElseRowCol[1], "", 4u)
			<< " " << dat::infoString(theElseScore, "", 7u)
			<< " "
			<< " prom:"
			<< " " << dat::infoString(prominenceRank(), "", 7u)
			<< " amib:"
			<< " " << dat::infoString(ambiguityRatio(), "", 7u)
			<< " hood:"
			<< " " << theHoodScores.size() << " of " << hoodArea()
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

std::string
Peak :: infoStringDetail
	( std::string const & title
	) const
{
	std::ostringstream oss;
	oss << infoString(title);
	if (isValid())
	{
		for (double const & hoodScore : theHoodScores)
		{
			oss << std::endl;
			oss << dat::infoString(hoodScore);
			if (theElseScore < hoodScore)
			{
				oss << "  *";
			}
			
		}
	}
	return oss.str();
}

size_t
Peak :: hoodArea
	() const
{
	double const area{ std::ceil(math::pi * math::sq(theHoodRadius)) };
	return static_cast<size_t>(area);
}

}

