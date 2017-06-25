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

#ifndef sig_io_INCL_
#define sig_io_INCL_

/*! \file
\brief Declarations for sig::io
*/


#include "libdat/Spot.h"
#include "libsig/FilterContext.h"
#include "libsig/MatchConfig.h"

#include <string>
#include <utility>
#include <vector>


namespace sig
{

//! \brief Functions for Input/Output of libsig entities
namespace io
{
	//! Save general matching configuration info
	void
	saveMatchConfig
		( sig::MatchConfig const & matchBwA
		  // hardcoded paths
		);

	//! Save filter configuration info
	void
	saveFilterContexts
		( std::vector<sig::FilterContext> const & fconSamps
		, sig::MatchConfig const & matchBwA
		  // hardcoded paths
		);

	//! Save image space parallax values to fpath
	bool
	saveParallax
		( std::vector<std::pair<dat::Spot, dat::Spot> > const & spotPairs
		, std::string const & fpath
		);
}

}

// Inline definitions
// #include "libsig/io.inl"

#endif // sig_io_INCL_

