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

#ifndef prob_CdfForward_INCL_
#define prob_CdfForward_INCL_

/*! \file
\brief Declarations for prob::CdfForward
*/


#include "libdat/limits.h"
#include "libmath/Partition.h"

#include <string>


namespace prob
{

/*! \brief Cumulative probability (mass) distribution function.

\par Example
\dontinclude testprob/uCdfForward.cpp
\skip ExampleStart
\until ExampleEnd
*/

class CdfForward
{
public: // data

	std::vector<double> theFracValues;
	math::Partition theDataPart;

private: // static methods

	//! Fractions from relative frequency data extended by one element at 0
	template <typename FwdIter>
	inline
	static
	std::vector<double>
	preCdfValues
		( FwdIter const & freqBeg
		, FwdIter const & freqEnd
		);

	// (too confusing to make public (currently))
	//! Value construction
	template <typename FwdIter>
	inline
	explicit
	CdfForward
		( FwdIter const & cdfBeg
		, FwdIter const & cdfEnd
		, dat::Range<double> const & dataRange
		);

public: // static methods

	/*! Checks that cdf values satisfy expected criteria.
	 *
	 * Checks include:
	 *  - *(cdfBeg) == 0.
	 *  - *(cdfEnd) == 1.
	 *  - cdf[i] <= cdf[i+1]
	 *
	 */
	template <typename FwdIter>
	static
	bool
	isValidCdf
		( FwdIter const & cdfDataBeg
		, FwdIter const & cdfDataEnd
		);

	//! Construct from collection of relative frequency values (hist or pdf)
	template <typename FwdIter>
	inline
	static
	CdfForward
	fromSamps
		( FwdIter const & sampBeg
		, FwdIter const & sampEnd
		, math::Partition const & dataPart
		);

	//! Construct from collection of relative frequency values (hist or pdf)
	template <typename FwdIter>
	inline
	static
	CdfForward
	fromFreqs
		( FwdIter const & freqBeg
		, FwdIter const & freqEnd
		, dat::Range<double> const & dataRange
		);

	//! Cdf for uniform probability density (viz. a straight line)
	static
	CdfForward
	uniform
		( dat::Range<double> const & dataRange
		);

public: // methods

	//! default null constructor
	CdfForward () = default;

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

	//! Fractional values associated with domain
	inline
	std::vector<double> const &
	probabilities
		() const;

	//! CDF (interpolated) at data values
	inline
	double
	operator()
		( const double & dataValue
		) const;

	//! Representation as collection of (dataValue, cdf(dataValue)).
	std::vector<std::pair<double, double> >
	dataProbs
		() const;

	//! True if this instance is same as other within tolerance
	bool
	nearlyEquals
		( CdfForward const & other
		, double const & tol = dat::smallValue<double>()
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

	//! Save contents as (dataValue, cdf(dataValue)).
	bool
	writeDataProbs
		( std::ostream & oss
		, std::string const & title = std::string()
		) const;
};

}

// Inline definitions
#include "libprob/CdfForward.inl"

#endif // prob_CdfForward_INCL_

