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

#ifndef prob_SampleEntropy_INCL_
#define prob_SampleEntropy_INCL_

/*! \file
\brief Declarations for prob::SampleEntropy
*/


#include "libdat/validity.h"

#include <cstddef>
#include <string>


namespace prob
{

/*! \brief Basic entropy information

\par Example
\dontinclude testprob/uSampleEntropy.cpp
\skip ExampleStart
\until ExampleEnd
*/

class SampleEntropy
{
public:

	size_t theCount;
	double theEntropy;

	//! Construct a null instance
	explicit
	SampleEntropy
		();

	//! Value construction
	inline
	explicit
	SampleEntropy
		( size_t const & count
		, double const & entropy
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Entropy per element
	double
	perElement
		( double const & base = 2.
		) const;

	double
	inTotal
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
#include "libprob/SampleEntropy.inl"

#endif // prob_SampleEntropy_INCL_

