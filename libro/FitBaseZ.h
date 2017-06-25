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

#ifndef ro_FitBaseZ_INCL_
#define ro_FitBaseZ_INCL_

/*! \file
\brief Declarations for ro::FitBaseZ
*/


#include "libro/PairBaseZ.h"
#include "libga/ga.h"

#include <array>
#include <string>
#include <utility>


namespace ro
{

/*! \brief Wrapper around iterative improvement of a PairBaseZ model.

\par Example
\dontinclude testro/uFitBaseZ.cpp
\skip ExampleStart
\until ExampleEnd
*/

class FitBaseZ
{

public: // types

	using PairUV = std::pair<ga::Vector, ga::Vector>;
	using PtrPairUV = PairUV const * const;

public: // data

	std::array<PtrPairUV, 5u> const theUVPtrs
		{{ nullptr, nullptr, nullptr, nullptr, nullptr }};

public: // methods

	//! default null constructor
	FitBaseZ
		() = default;

	//! Construct to fit a collection of measurements
	explicit
	FitBaseZ
		( std::array<PtrPairUV, 5u> const & uvPtrs
		);

	//! True if instance is valid
	bool
	isValid
		() const;

	//! Condition number associated with this ro
	double
	condNumFor
		( ro::PairBaseZ const & roAt
		) const;

	//! Pair with parameters improved by a single linearized iteration.
	ro::PairBaseZ
	improvedNear
		( ro::PairBaseZ const & roNom
		, double const & maxCondNum = { 1.e6 }
		, double * const & ptCondNum = nullptr
		) const;

	//! Solution via linearized iteration starting from roNom
	ro::PairBaseZ
	solutionNear
		( ro::PairBaseZ const & roNom
		, double const & tol = math::eps
		, size_t const & itMax = 25u //!< should only need a handful
		, double const & maxCondNum = { 1.e6 }
		, double * const & ptCondNum = nullptr
		) const;

	//! Root-sum-squared tripleProductGap() for measurements
	double
	rmsGapFor
		( ro::Pair const & roNom
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // FitBaseZ

} // ro

// Inline definitions
// #include "libro/FitBaseZ.inl"

#endif // ro_FitBaseZ_INCL_

