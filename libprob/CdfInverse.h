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

#ifndef prob_CdfInverse_INCL_
#define prob_CdfInverse_INCL_

/*! \file
\brief Declarations for prob::CdfInverse
*/


#include "libdat/Range.h"
#include "libprob/CdfForward.h"
#include "libmath/Partition.h"

#include <functional>
#include <string>
#include <vector>


namespace prob
{

/*! \brief Inverse cumulative distribution function

\par Example
\dontinclude testprob/uCdfInverse.cpp
\skip ExampleStart
\until ExampleEnd
*/

class CdfInverse
{
public: // data

	std::vector<double> theDataIndices;
	math::Partition theFracPart;
	math::Partition theDataPart;

public: // static methods

	//! (Fractional) indices for uniform change in values
	template <typename FwdIter>
	static
	std::vector<double>
	ndxLocsForUniformDelta
		( FwdIter const & cdfFwdBeg
		, FwdIter const & cdfFwdEnd
		, size_t const & lutSize
		);

	//! Create via evaluation of probability function - 'stretch' to range
	static
	CdfInverse
	generateFor
		( std::function<double(double const & frac)> const & probFunc
		, dat::Range<double> const & dataRange
		, size_t const & lutSize
		);

	//! Construct by inversion of forward CDF
	static
	CdfInverse
	fromCdfForward
		( CdfForward const & cdfFwd
		, size_t const & lutSize
		);

	//! Construct from frequency data
	template <typename FwdIter>
	inline
	static
	CdfInverse
	fromFrequencies
		( FwdIter const & freqBeg //!< *iter used as double
		, FwdIter const & freqEnd
		, dat::Range<double> const & dataRange
		, size_t const & lutSize
		);

	//! Inverse CDF with linear mapping from probabilities to dataRange
	static
	CdfInverse
	uniform
		( dat::Range<double> const dataRange
		);

public: // methods

	//! default null constructor
	CdfInverse
		() = default;

	//! Construct by inversion of a standard CDF
	template <typename FwdIter>
	inline
	explicit
	CdfInverse
		( FwdIter const & cdfFwdBeg //!< *iter used as double
		, FwdIter const & cdfFwdEnd
		, dat::Range<double> const & dataRange
		, size_t const & lutSize
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Domain for this cdf
	inline
	dat::Range<double>
	dataRange
		() const;

	//! (Fractional) index associated with probability value
	inline
	double
	operator()
		( double const & prob
		) const;

	//! Collection of (probability, dataValue) pairs
	std::vector<std::pair<double, double> >
	probDataPairs
		( double const & probDelta = 1./100
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

	//! Descriptive information about this instance including internal values.
	std::string
	infoStringContents
		( std::string const & title = std::string()
		) const;

	//! Put (probability, index) pairs to stream
	bool
	writeProbIndices
		( std::ostream & oss
		, std::string const & title
		, double const & probDelta = 1./100
		) const;

};

}

// Inline definitions
#include "libprob/CdfInverse.inl"

#endif // prob_CdfInverse_INCL_

