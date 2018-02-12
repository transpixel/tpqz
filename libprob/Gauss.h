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

#ifndef prob_Gauss_INCL_
#define prob_Gauss_INCL_

/*! \file
\brief Declarations for prob::Gauss
*/


#include "libdat/validity.h"

#include <string>


namespace prob
{

/*! \brief Classic normal (Gauss-Laplace) distribution

\par Example
\dontinclude testprob/uGauss.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Gauss
{
	double theSigma{ dat::nullValue<double>() };
	double theMean{ dat::nullValue<double>() };
	double theArgCo{ dat::nullValue<double>() };
	double theNormCo{ dat::nullValue<double>() };

public: // methods

	//! default null constructor
	Gauss
		() = default;

	//! Construct with deviation
	explicit
	Gauss
		( double const & sigma
		, double const & mean = { 0. }
		);

	//! True if instance is valid
	bool
	isValid
		() const;

	//! Probability of observing value
	double
	operator()
		( double const value
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // Gauss

} // prob

// Inline definitions
// #include "libprob/Gauss.inl"

#endif // prob_Gauss_INCL_

