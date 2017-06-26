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


/*! \file
\brief Definitions for model::Part
*/


#include "libmodel/Part.h"

#include <sstream>


namespace model
{

bool
Part :: isValid
	() const
{
	return theLoc.isValid();
}

Part
Part :: transformedBy
	( ga::Rigid const & xOutWrtThis
	) const
{
	// express part location in output frame
	ga::Vector const locInOut(xOutWrtThis(theLoc));

	// transform part geometry properties into out frame
	traits::Geometry const geoTraits
		(theGeoTraits.transformedBy(xOutWrtThis));

	// rad properties invariant to geo transform (well, assumed so here)
	traits::Radiometry const & radTraits = theRadTraits;

	// return model expressed in out frame
	return Part{ locInOut, radTraits, geoTraits };
}

std::string
Part :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	if (isValid())
	{
		oss << dat::infoString(theLoc, "theLoc");

		oss << std::endl;
		oss << dat::infoString(theRadTraits, "theRadTraits");

		oss << std::endl;
		oss << dat::infoString(theGeoTraits, "theGeoTraits");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

}

