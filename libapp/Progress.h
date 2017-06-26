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

#ifndef app_Progress_INCL_
#define app_Progress_INCL_

/*! \file
\brief Declarations for app::Progress
*/


#include "libdat/validity.h"

#include <string>


namespace app
{

/*! \brief Simple progress state class

\par Example
\dontinclude testapp/uProgress.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Progress
{

	size_t theMaxValue{ dat::nullValue<size_t>() };
	size_t theAtValue{ dat::nullValue<size_t>() };

public: // methods

	//! default null constructor
	Progress
		() = default;

	//! Initialization construction
	explicit
	Progress
		( size_t const & maxValue
		, size_t const & startValue = 0u
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! (Pre) Increment progress - and return modified self
	Progress const &
	operator++
		();

	//! Calls op++() numBump times - and return modified self
	Progress const &
	advance
		( size_t const & numBump
		);

	//! Amount done as fractional value [0.,1.]
	double
	fractionDone
		() const;

	//! Amount done as (nearest) percent value
	uint8_t
	percentDone
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
// #include "libapp/Progress.inl"

#endif // app_Progress_INCL_


