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

#ifndef ro_sampcon_INCL_
#define ro_sampcon_INCL_

/*! \file
\brief Declarations for ro::sampcon
*/


#include "libdat/validity.h"
#include "libprob/median.h"
#include "libro/FitConfig.h"
#include "libro/ro.h"
#include "libro/Solution.h"

#include <vector>


namespace ro
{

/*! \brief Robust solutions for relative orientation given many measurements.

\par Example
\dontinclude testro/uFitBaseZ.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace sampcon
{
	struct QuintSoln
	{
		FiveOf<size_t> theFitNdxs{ dat::nullValue<size_t, 5u>() };
		Solution theSoln{};

		QuintSoln
			() = default;

		//! Value ctor
		explicit
		QuintSoln
			( FiveOf<size_t> const & fitNdxs
			, Solution const & soln
			)
			: theFitNdxs{ fitNdxs }
			, theSoln{ soln }
		{ }
	};

	//! All solutions (N-choose-5 of them!!)
	std::vector<QuintSoln>
	allByCombo
		( std::vector<PairUV> const & uvPairs
		, OriPair const & roPairNom
		, FitConfig const & fitConfig = {}
		);

	//! All solutions from random sampling
	std::vector<QuintSoln>
	allBySample
		( std::vector<PairUV> const & uvPairs
		, OriPair const & roPairNom
		, size_t const & numDraws = { 640u }
		, FitConfig const & fitConfig = {}
		, size_t const & maxTrys = { 10u }
		);

	//! Select the "best" (TODO metric?) from quintSolns
	std::vector<QuintSoln>
	bestOf
		( std::vector<QuintSoln> const & quintSolns
		, std::vector<PairUV> const & uvPairs
		, size_t const & numBest = { 1u }
		, double const & dirSigma = { 1./1000. }
		);

	//! Best solution after exhaustive evaluation of all combinations
	QuintSoln
	byCombo
		( std::vector<PairUV> const & uvPairs
		, OriPair const & roPairNom
		, FitConfig const & fitConfig = {}
		, double const & dirSigma = { 1./1000. }
		);

	//! Best solution from random sampling
	QuintSoln
	bySample
		( std::vector<PairUV> const & uvPairs
		, OriPair const & roPairNom
		, size_t const & numDraws = { 640u }
		, FitConfig const & fitConfig = {}
		, double const & dirSigma = { 1./1000. }
		, size_t const & maxTrys = { 10u }
		);

} // sampcon

} // ro

// Inline definitions
// #include "libro/sampcon.inl"

#endif // ro_sampcon_INCL_

