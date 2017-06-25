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

#ifndef prob_Remapper_INCL_
#define prob_Remapper_INCL_

/*! \file
\brief Declarations for prob::Remapper
*/


#include "libmath/Partition.h"
#include "libprob/CdfForward.h"
#include "libprob/CdfInverse.h"

#include <string>
#include <vector>


namespace prob
{

/*! \brief Factory for remapping samples to conform with a distribution.

The constructor computes a remapping function which gets applied to
the evaluation method, operator().  The remapping is implemented via a
look-up-table (LUT) so that the evaluation is relatively fast. However,
this of course means that the computed values are quantized to the
LUT resolution.

\par Example
\dontinclude testprob/uRemapper.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Remapper
{
	CdfForward theCdfFwd;
	CdfInverse theCdfInv;

public: // methods

	//! default null constructor
	Remapper
		() = default;

	//! Value ctor
	explicit
	Remapper
		( CdfForward const & cdfFwd
		, CdfInverse const & cdfInv
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Transform inValue via remapping function
	template <typename DataType>
	inline
	DataType
	operator()
		( DataType const & inValue
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
#include "libprob/Remapper.inl"

#endif // prob_Remapper_INCL_

