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

#ifndef prob_SampleStats_INCL_
#define prob_SampleStats_INCL_

/*! \file
\brief Declarations for prob::SampleStats
*/


#include "libmath/Partition.h"
#include "libprob/CdfForward.h"
#include "libprob/Frac9.h"
#include "libprob/histo.h"
#include "libprob/pdf.h"
#include "libprob/SampleEntropy.h"

#include <string>
#include <vector>


namespace prob
{

/*! \brief Container for assorted image pixel statistics.
*/
/*
\par Example
\dontinclude testprob/uSampleStats.cpp
\skip ExampleStart
\until ExampleEnd
*/

class SampleStats
{

public: // data

	math::Partition theDataPart;
	size_t theCount{0u};
	std::vector<size_t> const theHist{};
	std::vector<double> const thePdf{};
	SampleEntropy theEntropy;
	prob::CdfForward theCdfFwd;
	prob::Frac9 theFrac9;

public: // methods

	//! Default null constructor
	SampleStats
		() = default;

	//! Extract statitics from pixel samples
	template <typename FwdIter>
	inline
	explicit
	SampleStats
		( FwdIter const & sampBeg
		, FwdIter const & sampEnd
		, math::Partition const & dataPart
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Convenient access to .50 fractile of the CDF
	double
	medianValue
		() const;

	//! Total information (per element entropy * number of elements)
	double
	totalInformation
		( double const & base = 2.
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
#include "libprob/SampleStats.inl"

#endif // prob_SampleStats_INCL_

