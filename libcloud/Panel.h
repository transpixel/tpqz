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

#ifndef cloud_Panel_INCL_
#define cloud_Panel_INCL_

/*! \file
\brief Declarations for cloud::Panel
*/


#include "libdat/Area.h"
#include "libdat/Extents.h"
#include "libga/Rigid.h"

#include <string>
#include <vector>


namespace cloud
{

/*! \brief A "panel" of regularly distributed points (e.g. grid of points)

\par Example
\dontinclude testcloud/uPanel.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Panel
{

public: // data

	std::vector<ga::Vector> thePoints{};

public: // methods

	//! default null constructor
	Panel
		() = default;

	//! Construct with specified characteristics
	explicit
	Panel
		( dat::Area<double> const & area
		, dat::Extents const & hwSize = dat::Extents(2u, 2u)
		, ga::Rigid const & oriPanWrtRef = ga::Rigid::identity()
			//!< Orientation of panel(XY) plane w.r.t. Reference frame
		);

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // Panel

} // cloud

// Inline definitions
// #include "libcloud/Panel.inl"

#endif // cloud_Panel_INCL_

