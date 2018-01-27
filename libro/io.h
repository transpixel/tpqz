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

#ifndef ro_io_INCL_
#define ro_io_INCL_

/*! \file
\brief Declarations for ro::io
*/


#include "libro/ro.h"

#include <iostream>
#include <vector>


namespace ro
{

/*! \brief Functions for object I/O

\par Example
\dontinclude testro/uio.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace io
{

namespace gnuplot
{
	enum WhichOne
	{
		  First
		, Second
	};

	//! Save directions as 'gnuplot vector' ascii data
	bool
	saveImageRays
		( std::ostream & ostrm
		, WhichOne const & which
		, OriPair const & oriPair
		, std::vector<PairUV> const & uvPairs
		);

	//! Draw an asymmetric shape to represent axis triad
	bool
	drawOXYZ
		( std::ostream & ostrm
		, ga::Rigid const & oriStaWrtRef
		, double const & axisMag = { 1. }
		);

	//! Perform space intersections and save as 'gnuplot vector' ascii data
	bool
	saveModelRays
		( std::ostream & ostrm
		, OriPair const & oriPair
		, std::vector<PairUV> const & uvPairs
		);
}


} // io

} // ro

// Inline definitions
// #include "libro/io.inl"

#endif // ro_io_INCL_

