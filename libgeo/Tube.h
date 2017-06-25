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

#ifndef geo_Tube_INCL_
#define geo_Tube_INCL_

/*! \file
\brief Declarations for geo::Tube
*/


#include "libgeo/Ray.h"
#include "libdat/info.h"
#include "libdat/validity.h"

#include <string>
#include <sstream>


namespace geo
{

/*! \brief Infinite circular tube (e.g. right-cylinder withOUT endcaps)

\par Example
\dontinclude testgeo/uTube.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Tube
{

public: // data

	geo::Ray theAxis{};
	double theRadius{ dat::nullValue<double>() };

public: // methods

	//! default null constructor
	Tube
		() = default;

	//! Value ctor
	explicit
	Tube
		( geo::Ray const & axis
		, double const & radius
		)
		: theAxis{ axis }
		, theRadius{ radius }
	{
	}

	//! Check if instance is valid
	bool
	isValid
		() const
	{
		return
			(  dat::isValid(theAxis)
			&& dat::isValid(theRadius)
			);
	}

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const
	{
		std::ostringstream oss;
		if (! title.empty())
		{
			oss << title << std::endl;
		}
		if (isValid())
		{
			oss << dat::infoString(theAxis, "theAxis");
			oss << std::endl;
			oss << dat::infoString(theRadius, "theRadius");
		}
		else
		{
			oss << " <null>";
		}
		return oss.str();
	}

}; // Tube

} // geo

// Inline definitions
// #include "libgeo/Tube.inl"

#endif // geo_Tube_INCL_

