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
\brief Inline definitions for math::CircleTab
*/


#include "libdat/dat.h"
#include "libio/sprintf.h"
#include "libmath/interp.h"
#include "libmath/smooth.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <iterator>
#include <limits>
#include <vector>

#include <iomanip> // debug
#include <iostream> // debug


namespace math
{
//======================================================================

template <typename Value>
inline
// static
CircleTab<Value>
CircleTab<Value> :: smoothed
	( CircleTab<Value> const & orig
	, double const & filterHalfAngle
	)
{
	CircleTab<Value> result;

	// else a bit non-sensical
	assert(0. <= filterHalfAngle);

	double const fdxHalf(orig.theIndexPerAngle * filterHalfAngle);
	size_t const ndxHalf((size_t)(std::floor(fdxHalf)));

	// set return values
	result.theTabValues
		= math::smooth::triangleWrapped(orig.theTabValues, ndxHalf);
	result.theIndexPerAngle = orig.theIndexPerAngle;

	return result;
}


template <typename Value>
inline
CircleTab<Value> :: CircleTab
	()
	: theTabValues()
	, theIndexPerAngle(dat::nullValue<double>())
{
}

namespace priv
{
	//! True if p1.first < p2.first
	template <typename Value>
	inline
	bool
	firstIsLess
		( std::pair<double, Value> const & p1
		, std::pair<double, Value> const & p2
		)
	{
		return (p1.first < p2.first);
	}

	//! True if dat::nearlyEquals(p1.first, p2.first)
	template <typename Value>
	inline
	bool
	firstIsSame
		( std::pair<double, Value> const & p1
		, std::pair<double, Value> const & p2
		)
	{
		return (dat::nearlyEquals(p1.first, p2.first));
	}

	// pairs with non-negative angle values in sorted order
	template <typename Value>
	inline
	std::deque<std::pair<double, Value> >
	uniqueSortedCyclicPairs
		( std::vector<std::pair<double, Value> > const & aavPairs
		)
	{
		std::deque<std::pair<double, Value> > pavPairs;

		// reduce input data to collection of principal values
		for (typename std::vector<std::pair<double, Value> >::const_iterator
			iter(aavPairs.begin()) ; aavPairs.end() != iter ; ++iter)
		{
			double const & anyA = iter->first;
			Value const & value = iter->second;

			// compute principal value and store it
			double const pvA(math::principalAngle(anyA));
			pavPairs.push_back(std::make_pair(pvA, value));
		}

		// ensure values are in order by angle
		std::sort(pavPairs.begin(), pavPairs.end(), priv::firstIsLess<Value>);

		// wrap min/max values
		std::pair<double, Value> const & min = pavPairs.front();
		std::pair<double, Value> const & max = pavPairs.back();
		using math::pi;
		std::pair<double, Value> const wrapmin(min.first + 2.*pi, min.second);
		std::pair<double, Value> const wrapmax(max.first - 2.*pi, max.second);
		pavPairs.push_back(wrapmin);
		pavPairs.push_front(wrapmax);

		// keep only unique angle values
		typename std::deque<std::pair<double, Value> >::iterator
			const itDuplicates
			( std::unique
				(pavPairs.begin(), pavPairs.end(), priv::firstIsSame<Value>)
			);
		pavPairs.erase(itDuplicates, pavPairs.end());

		return pavPairs;
	}

	//! Indices (n1,n2) for which (key[n1] <= key < key[n2])
	template <typename Value>
	std::pair<size_t, size_t>
	bracketingIndicesFor
		( std::deque<std::pair<double, Value> > const & keyVals
		, double const & keyFind
		, std::pair<size_t, size_t> const * const & ptPrevLoHi = nullptr
		)
	{
		std::pair<size_t, size_t> pairLoHi(0u, 0u);
		size_t ndxHi(1u);

		// TODO - replace with std::{lower,upper}_bound
		//        if find way to make it work on ciruclar data

		// NOTE: assume keyVals are unique and circular

		assert(1u < keyVals.size());
		size_t const ndxLast(keyVals.size() - 1u);
		if ( (keyFind < keyVals[0].first) // check for special cases
		  || (keyVals[ndxLast].first < keyFind)
		   )
		{
			// circular wrap
		//	ndxLo = ndxLast;
			ndxHi = 0u;
		}
		else // general case
		{
			// start at begining (e.g. if no previous starting values)
		//	ndxLo = 0u;
		//	ndxHi = ndxLo + 1u;
			ndxHi = 1u;
			if (ptPrevLoHi)
			{
				size_t const & tmpLo = ptPrevLoHi->first;
				size_t const & tmpHi = ptPrevLoHi->second;
				if (tmpLo != tmpHi)
				{
					// start at previous (valid location within queue)
				//	ndxLo = tmpLo;
					ndxHi = tmpHi;
				}
			}

			// search for bracket containing value
			while (ndxHi < keyVals.size())
			{
				double const & keyHi = keyVals[ndxHi].first;
				if (keyFind < keyHi)
				{
					break;
				}
				++ndxHi;
			//	++ndxLo;
			}
		}

		// set lower index
		size_t const ndxLo((ndxHi + (keyVals.size() - 1u)) % keyVals.size());
		pairLoHi = std::make_pair(ndxLo, ndxHi);

		return pairLoHi;
	}

	//! Angle data extracted from pavPairs *plus* first value appended to end
	template <typename Value>
	std::vector<double>
	circleWrapped
		( std::vector<std::pair<double, Value> > const & pavPairs
		)
	{
		std::vector<double> angles;
		if (! pavPairs.empty())
		{
			size_t const numPairs(pavPairs.size());
			angles = std::vector<double>(numPairs + 1u);
			std::transform
				( pavPairs.begin(), pavPairs.end()
				, angles.begin()
				, []
					(std::pair<double, Value> const aavPair)
					{ return aavPair.first; }
				);
			angles[numPairs] = pavPairs[0].first;
		}
		return angles;
	}

	//! Start/Finish angle pair associated with indices
	template <typename Value>
	std::pair<double, double>
	angleRangeFor
		( std::deque<std::pair<double, Value> > const & pavPairs
		, size_t const & ndxLo
		, size_t const & ndxHi
		)
	{
		std::pair<double, double> range;
		double & angleLo = range.first;
		double & angleHi = range.second;

		// check that indices are valid
		assert(ndxLo < pavPairs.size());
		assert(ndxHi < pavPairs.size());

		// extract angles from input values
		angleLo = pavPairs[ndxLo].first;
		angleHi = pavPairs[ndxHi].first;

		// deal with wrap discontinuity
		if (angleHi < angleLo)
		{
			angleHi += math::twoPi;
		}

		return range;
	}
}

template <typename Value>
inline
// explicit
CircleTab<Value> :: CircleTab
	( std::vector<std::pair<double, Value> > const & aavPairs
	, size_t const & numNodes
	)
	: theTabValues(numNodes)
	, theIndexPerAngle((double)theTabValues.size() / math::twoPi)
{
	if (! theTabValues.empty())
	{
		// preprocess input data
		// Associate to *non-negative* angle values in sorted order
		std::deque<std::pair<double, Value> > const pavPairs
			(priv::uniqueSortedCyclicPairs(aavPairs));

		// configure lookup table

		// fill table -- wastefull but easy to code // TODO optimize if needed

		// fill table in angle ingrements
		double const da(math::twoPi / (double)theTabValues.size());
		std::pair<size_t, size_t> ndxLoHi(0u, 0u);
		for (size_t ndx(0u) ; ndx < theTabValues.size() ; ++ndx)
		{
			// generate angle associated with lookup table
			double const angleKey((double)ndx * da - math::pi);

			// find bracketing indices
			ndxLoHi = priv::bracketingIndicesFor(pavPairs, angleKey, &ndxLoHi);
			size_t const & ndxLo = ndxLoHi.first;
			size_t const & ndxHi = ndxLoHi.second;

			// interpolate between values at this angle
			std::pair<double, double> const angleRange
				(priv::angleRangeFor(pavPairs, ndxLo, ndxHi));
			Value const & valueLo = pavPairs[ndxLo].second;
			Value const & valueHi = pavPairs[ndxHi].second;
			Value const tabValue
				(interp::linear
					( angleRange
					, angleKey
					, std::pair<Value, Value>(valueLo, valueHi)
					)
				);
			theTabValues[ndx] = tabValue;
		}
	}
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

template <typename Value>
inline
bool
CircleTab<Value> :: isValid
	() const
{
	return
		(  (! theTabValues.empty())
		&& dat::isValid(theIndexPerAngle)
		);
}

template <typename Value>
inline
Value
CircleTab<Value> :: nearestTableValue
	( double const & principalAngle
	) const
{
	// determine offset into table
	double const delta(principalAngle + math::pi);
	double const fdx(delta * theIndexPerAngle);

	// get index closest to fraction value
	size_t const ndx(std::floor(fdx + .5));

	// wrap index at far end
	size_t const ndxLo(ndx % theTabValues.size());

	// return nearest table value
	return theTabValues[ndxLo];
}

template <typename Value>
inline
std::string
CircleTab<Value> :: infoString
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
		oss << "numberNodes: " << theTabValues.size();
	}
	else
	{
		oss << " <empty>";
	}

	return oss.str();
}

template <typename Value>
inline
std::string
CircleTab<Value> :: infoStringContents
	( std::string const & title
	, std::string const & fmt
	) const
{
	std::ostringstream oss;

	// include basic information
	oss << infoString(title);

	// if valid, then include details
	if (isValid())
	{
		for (typename std::vector<Value>::const_iterator
			iter(theTabValues.begin()) ; theTabValues.end() != iter ; ++iter)
		{
			oss << '\n';
			oss << io::sprintf(fmt, *iter);
		}
	}

	return oss.str();
}


//======================================================================
}

