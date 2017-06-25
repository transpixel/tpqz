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

#ifndef prob_distro_INCL_
#define prob_distro_INCL_

/*! \file
\brief Declarations for prob::distro
*/


#include "libdat/template.h"

#include <array>
#include <cstddef>
#include <vector>


namespace prob
{

/*! \brief Various probability distribution functions.

\par Example
\dontinclude testprob/uCLASSNAME.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace distro
{
	//! Triangle distribution on interval [0,1)
	inline
	double
	unitTriangle
		( double const & xFrac
		);

	//! Quadratic PDF spanning interval [0,1)
	inline
	double
	unitHumpProb
		( double const & xFrac
		);

}

}

// Inline definitions
#include "libprob/distro.inl"

#endif // prob_distro_INCL_

