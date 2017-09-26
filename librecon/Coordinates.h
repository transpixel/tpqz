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

#ifndef recon_Coordinates_INCL_
#define recon_Coordinates_INCL_

/*! \file
\brief Declarations for recon::Coordinates
*/


#include "libga/ga.h"

#include <array>
#include <string>
#include <vector>


namespace recon
{

/*! \brief Operations on independent coordinate (components).

\par Example
\dontinclude testrecon/uCoordinates.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Coordinates
{
	std::array<std::vector<double>, 3u> theComps;

public: // methods

	//! default null constructor
	Coordinates
		() = default;

	//! Construct with reserved size
	explicit
	Coordinates
		( size_t const & estSize
		);

	//! Incorporate coordinates from pnt
	void
	addPoint
		( ga::Vector const & pnt
		);

	//! Incorporate collection of points
	template <typename FwdIter>
	void
	addPoints
		( FwdIter const & beg
		, FwdIter const & end
		);

	//! Point comprised of individual coordinate medians
	ga::Vector
	pointAtMedians
		() const;

}; // Coordinates

} // recon

// Inline definitions
#include "librecon/Coordinates.inl"

#endif // recon_Coordinates_INCL_

