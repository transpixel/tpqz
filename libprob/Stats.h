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

#ifndef prob_Stats_INCL_
#define prob_Stats_INCL_

/*! \file
\brief Declarations for prob::Stats
*/


#include "libdat/MinMax.h"

#include "libio/stream.h"

#include <deque>
#include <string>
#include <vector>


namespace prob
{

/*! \brief Support for basic numeric statistics.

\par Example
\dontinclude testprob/uStats.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Stats
{
	// simple implementation -- TODO add running stats if many samples
	std::deque<double> theSamples;

public: // data

	dat::MinMax<double> theMinMax;

public: // methods

	//! default null constructor
	Stats
		();

	//! Construct direct from collection of data
	template <typename FwdIter>
	inline
	Stats
		( FwdIter const & beg //!< (*it) must be convertable to double
		, FwdIter const & end
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Incorporate sample into statistics
	void
	add
		( double const & sample
		);

	//! Mean value of current sample content
	double
	mean
		() const;

	//! Value at middle of collection (expensive! - O(N*ln(N))
	double
	medianValue
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		, std::string const & fmt = "%12.6f"
		) const;
};

}

// Inline definitions
#include "libprob/Stats.inl"

#endif // prob_Stats_INCL_

