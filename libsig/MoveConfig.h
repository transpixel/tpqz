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

#ifndef sig_MoveConfig_INCL_
#define sig_MoveConfig_INCL_

/*! \file
\brief Declarations for sig::MoveConfig
*/


#include "libdat/Extents.h"
#include "libdat/Spot.h"

#include <string>


namespace sig
{

/*! \brief Configuration specs for a moving window filter

\par Example
\dontinclude testsig/uMoveConfig.cpp
\skip ExampleStart
\until ExampleEnd
*/
class MoveConfig
{

public: // data

	//! Size of area input to matching assessment
	dat::Extents theHunkSize{};

	//! Bounding uncertainty in nominal location
	dat::Extents theMoveSize{};

public: // static methods

	//! Configuration which allows hunkSize to move over all of fullSize
	static
	MoveConfig
	fullConfig
		( dat::Extents const & fullSize
		, dat::Extents const & hunkSize
		);

public: // methods

	//! default null constructor
	MoveConfig
		() = default;

	//! Value construction
	explicit
	MoveConfig
		( dat::Extents const & hunkSize
		, dat::Extents const & moveSize
		);

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Maximum size involved with hunk and its motion
	dat::Extents
	fullExtents
		() const;

	//! Location for exact center of theHunkSize
	dat::Spot
	hunkCenterSpot
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
// #include "libsig/MoveConfig.inl"

#endif // sig_MoveConfig_INCL_

