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

#ifndef prob_Frac9_INCL_
#define prob_Frac9_INCL_

/*! \file
\brief Declarations for prob::Frac9
*/


#include "libprob/CdfForward.h"
#include "libprob/CdfInverse.h"

#include <array>
#include <string>


namespace prob
{

/*! \brief Fractile values at 9 key points

\par Example
\dontinclude testprob/uFrac9.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Frac9
{

public: // data

	std::array<double, 9u> theValues;

public: // static methods

	//! Fractile values associatd with statistics
	constexpr
	static
	std::array<double, 9u>
	fractiles
		()
	{
		return std::array<double, 9u>
			{{ .00, .01, .10, .25,  .50,  .75, .90, .99, 1.0 }};
	}

public: // methods

	//! default null constructor
	Frac9
		();

	//! Extract stats from forward cumulative distribution
	explicit
	Frac9
		( CdfForward const & cdfFwd
		);

	//! Extract stats from inverse cumulative distribution
	explicit
	Frac9
		( CdfInverse const & cdfInv
		);

	//! Value ctor
	explicit
	Frac9
		( std::array<double, 9u> const & values
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Direct access to theValues
	inline
	double const &
	operator[]
		( size_t const & ndx
		) const;

	//! Convenience
	inline
	double
	median
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		, std::string const & fmt = std::string("%7.3f")
		, std::string const & sep = std::string("\n")
		) const;

	//! Convenience call
	std::string
	infoStringOneLine
		( std::string const & title = std::string()
		, std::string const & fmt = std::string("%7.3f")
		) const;

};

}

// Inline definitions
#include "libprob/Frac9.inl"

#endif // prob_Frac9_INCL_

