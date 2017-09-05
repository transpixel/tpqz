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

#ifndef prob_Histogram_INCL_
#define prob_Histogram_INCL_

/*! \file
\brief Declarations for prob::Histogram
*/


#include "libmath/Partition.h"

#include <iostream>
#include <string>
#include <vector>


namespace prob
{

/*! \brief Basic histogram concept.

\par Example
\dontinclude testprob/uHistogram.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Histogram
{

public: // data

	math::Partition thePart{};
	std::vector<size_t> theCounts{};
	size_t theCountUnder{ 0u };
	size_t theCountOver{ 0u };

public: // methods

	//! default null constructor
	Histogram
		();

	//! Construct empty histogram with basic properties
	explicit
	Histogram
		( math::Partition const & part
		);

	//! Construct and populate with samples
	template <typename FwdIter>
	inline
	explicit
	Histogram
		( math::Partition const & part
		, FwdIter const & sampBeg
		, FwdIter const & sampEnd
		);

	//! Incorporate sample into this instance
	void
	inline
	addSample
		( double const & sample
		);

	//! Incorporate many samples into this instance
	template <typename FwdIter>
	inline
	void
	addSamples
		( FwdIter const & sampBeg
		, FwdIter const & sampEnd
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Number of bins
	size_t
	size
		() const;

	//! Total number of counts (sum of bins)
	size_t
	total
		() const;

	//! Probability density/mass function (normalized counts)
	std::vector<double>
	pdf
		() const;

	//! sample value associated with center of bins
	std::vector<double>
	binCenters
		() const;

	//! Range with which constructed.
	dat::Range<double>
	range
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

	//! Descriptive information including contents
	std::string
	infoStringContents
		( std::string const & title = std::string()
		) const;

	//! Put (dataValue, probability) pairs to stream
	bool
	writeDataProbs
		( std::ostream & oss
		, std::string const & title
		) const;

private:

	//! Put sample into collection -- assumes everything is valid
	inline
	void
	incorporateSample
		( double const & sample
		);

};

}

// Inline definitions
#include "libprob/Histogram.inl"

#endif // prob_Histogram_INCL_

