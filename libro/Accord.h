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

#ifndef ro_Accord_INCL_
#define ro_Accord_INCL_

/*! \file
\brief Declarations for ro::Accord
*/


#include "libro/ro.h"
#include "libro/Solution.h"

#include <string>
#include <vector>


namespace ro
{

/*! \brief Data wrapper to assess solution's goodness of fit to data.

\par Example
\dontinclude testro/uAccord.cpp
\skip ExampleStart
\until ExampleEnd
*/

struct Accord
{
	Solution const theSoln;
	std::vector<PairUV> const * const thePtPairUVs;
//	std::array<size_t, 5u> theNdxQuint{ dat::nullValue<size_t, 5u>() };
//	std::vector<double> theGaps{};

public: // methods

	//! True if instance is valid
	bool
	isValid
		() const;

	//! Number of measurements in total
	size_t
	numTotalUVs
		() const;

	//! Number of measurements not consumed by (5-dof) RO fit
	size_t
	numFreeUVs
		() const;

	//! Gap associated with uvPair[ndx]
	double
	gapForNdx
		( size_t const & ndx
		) const;

	//! Gap values for measurements *other than* omitNdxs
	std::vector<double>
	gapsExcluding
		( ro::FiveOf<size_t> const & omitNdxs
		) const;

	//! Expected gap value (root-mean-square)
	double
	rmsGap
		() const;

	//! Root-sum-square of gap values computed with *other than* omitNdx UVs
	double
	rssGapExcluding
		( ro::FiveOf<size_t> const & omitNdxs
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // Accord

} // ro

// Inline definitions
// #include "libro/Accord.inl"

#endif // ro_Accord_INCL_

