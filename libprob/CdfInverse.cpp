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
\brief Definitions for prob::CdfInverse
*/

#include "libprob/CdfInverse.h"

#include "libdat/algorithm.h"
#include "libmath/interp.h"
#include "libmath/Partition.h"

#include <cassert>
#include <cmath>
#include <sstream>


namespace
{
	//! Fill table of probabilities
	std::vector<double>
	probabilitiesFor
		( std::function<double(double const & frac)> const & probFunc
		, size_t const & lutSize
		)
	{
		std::vector<double> probs;
		probs.resize(lutSize);
		double const fracPerNdx(1. / (double)lutSize);
		for (size_t nn(0u) ; nn < lutSize ; ++nn)
		{
			double const frac(fracPerNdx * (double)nn);
			double const prob = probFunc(frac);
			probs[nn] = prob;
		}
		return probs;
	}
}

namespace prob
{
//======================================================================

// static
CdfInverse
CdfInverse :: fromCdfForward
	( CdfForward const & cdfFwd
	, size_t const & lutSize
	)
{
	return CdfInverse
		( cdfFwd.theFracValues.begin()
		, cdfFwd.theFracValues.end()
		, cdfFwd.theDataPart.range()
		, lutSize
		);
}

// static
CdfInverse
CdfInverse :: generateFor
	( std::function<double(double const & frac)> const & probFunc
	, dat::Range<double> const & dataRange
	, size_t const & lutSize
	)
{
	CdfInverse cdfInv;

	// evaluate probFunc at each range value
	size_t const mult(8u); // sample more densely to support inversion
	size_t const pdfSize(mult * lutSize);
	std::vector<double> const probs(probabilitiesFor(probFunc, pdfSize));

	// create forward CDF
	CdfForward const cdfFwd
		(CdfForward::fromFreqs(probs.begin(), probs.end(), dataRange));

	// return inverse CDF
	cdfInv = CdfInverse::fromCdfForward(cdfFwd, lutSize);
	return cdfInv;
}

// static
CdfInverse
CdfInverse :: uniform
	( dat::Range<double> const dataRange
	)
{
	static std::vector<double> const cdfData{ 0., 1. };
	return CdfInverse(cdfData.begin(), cdfData.end(), dataRange, 2u);
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

bool
CdfInverse :: isValid
	() const
{
	return
		(  (! theDataIndices.empty())
		&& theFracPart.isValid()
		&& theDataPart.isValid()
		);
}

std::vector<std::pair<double, double> >
CdfInverse :: probDataPairs
	( double const & probDelta
	) const
{
	std::vector<std::pair<double, double> > fracValDataValPairs;
	assert(0. < probDelta);
	assert(probDelta <= 1.);

	// adjust to equal increments - and compute sizes
	size_t const count((size_t)std::ceil(1. / probDelta));
	size_t const nBetween(count - 1u);
	fracValDataValPairs.reserve(nBetween + 2u);

	// be sure start pair is included
	fracValDataValPairs.push_back(std::make_pair(0., theDataPart.min()));

	// fill in-between pairs
	if (0u < nBetween)
	{
		// fill table
		double const useDelta(1. / (double)nBetween);
		for (size_t nn(1u) ; nn < nBetween ; ++nn)
		{
			double const fracVal((double)nn * useDelta);
			double const dataVal(operator()(fracVal));
			fracValDataValPairs.push_back(std::make_pair(fracVal, dataVal));
		}
	}

	// be sure end pair is included
	fracValDataValPairs.push_back(std::make_pair(1., theDataPart.max()));

	return fracValDataValPairs;
}

std::string
CdfInverse :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}

	if (isValid())
	{
		oss << "theFracPart: " << dat::infoString(theFracPart);

		oss << std::endl;
		oss << "theDataPart: " << dat::infoString(theDataPart);

		oss << std::endl;
		oss << "theTableSize: " << dat::infoString(theDataIndices.size());
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

std::string
CdfInverse :: infoStringContents
	( std::string const & title
	) const
{
	std::ostringstream oss;
	oss << infoString(title);
	if (isValid())
	{
		oss << std::endl;
		oss << dat::infoString
			(theDataIndices.begin(), theDataIndices.end(), "theDataIndices");
	}
	return oss.str();
}

bool
CdfInverse :: writeProbIndices
	( std::ostream & oss
	, std::string const & title
	, double const & probDelta
	) const
{
	if (! title.empty())
	{
		oss << "# " << title << '\n';
	}

	std::vector<std::pair<double, double> > const fracValDataValPairs
		(probDataPairs(probDelta));
	for (std::pair<double, double>
		const & fracValDataValPair : fracValDataValPairs)
	{
		double const & fracVal = fracValDataValPair.first;
		double const & dataVal = fracValDataValPair.second;
		oss
			<< " " << dat::infoString(fracVal, "", 14u, 6u)
			<< " " << dat::infoString(dataVal, "",  9u, 6u)
			<< '\n';
	}

	return (! oss.fail());
}

//======================================================================
}

