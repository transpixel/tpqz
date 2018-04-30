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

#ifndef prob_SampleGen_INCL_
#define prob_SampleGen_INCL_

/*! \file
\brief Declarations for prob::SampleGen
*/


#include "libprob/CdfInverse.h"

#include <random>


namespace prob
{

//! Random sample generation functor.
class SampleGen
{
	prob::CdfInverse theCdfInv;

public: // static methods

	//! Pseudo random sample from system library funcs
	inline
	static
	double
	sampleFromUniformProb
		();

public: // methods

	//! default null constructor
	SampleGen
		() = default;

	//! Construct to generate samples with the CDF
	explicit
	SampleGen
		( CdfInverse const & cdfInv
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Pseudo-random sample in dataRange (from ctor cdfInv)
	inline
	double
	operator()
		() const;

	//! Collection of pseudo random samples
	std::vector<double>
	samples
		( size_t const & numSamp
		) const;
};

}

// Inline definitions
#include "libprob/SampleGen.inl"

#endif // prob_SampleGen_INCL_

