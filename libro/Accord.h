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


#include "libro/QuintSoln.h"
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
	//! Solution to evaluate
	Solution const theSoln;

	//! Pointer to implicit external collection of UV direction pairs
	std::vector<PairUV> const * const thePtPairUVs;

public: // static methods

	//! (Pseudo)Probability of quintSoln considering only NOT-fit UVs
	static
	double
	probFor
		( QuintSoln const & quintSoln
		, std::vector<PairUV> const & uvPairs
		, double const & gapSigma
		);

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

	//! Sum of squared gaps for all corresponding pairs
	double
	sumSqGapAll
		() const;

	//! Expected gap value (root-mean-square)
	double
	rmsGapAll
		() const;

	//! Sum-square of gap values computed with *other than* omitNdx UVs
	double
	sumSqGapExcluding
		( ro::FiveOf<size_t> const & omitNdxs
		) const;

	//! Sum-square of gap values computed with *other than* omitNdx UVs
	double
	rmsGapExcluding
		( ro::FiveOf<size_t> const & omitNdxs
		) const;

	//! (Pseudo)Probability of solution considering *other than* omitNdxs
	double
	probExcluding
		( ro::FiveOf<size_t> const & omitNdxs
		, double const & gapSigma
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

private:

	//! Gap value for ndx - w/o any validity checking
	inline
	double
	gapNoCheck
		( size_t const & ndx
		) const
	{
		ro::PairUV const & uvPair = (*thePtPairUVs)[ndx];
		return theSoln.theRoPair->tripleProductGap(uvPair);
	}


}; // Accord

} // ro

// Inline definitions
// #include "libro/Accord.inl"

#endif // ro_Accord_INCL_

