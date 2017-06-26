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

#ifndef model_Part_INCL_
#define model_Part_INCL_

/*! \file
\brief Declarations for model::Part
*/


#include "libga/Rigid.h"
#include "libmodel/traits.h"

#include <string>


namespace model
{

/*! \brief Fundamental component of an overall model.

\par Example
\dontinclude testmodel/uPart.cpp
\skip ExampleStart
\until ExampleEnd
*/

struct Part
{
	//! Location in model
	ga::Vector theLoc; // primary representative location

	//! Radiometric aspects
	traits::Radiometry theRadTraits;

	//! Geometric aspects
	traits::Geometry theGeoTraits;


	//! Check if instance is valid
	bool
	isValid
		() const;

	//! ModelPart transformed into 'out' frame
	Part
	transformedBy
		( ga::Rigid const & xOutWrtThis
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = {}
		) const;
};

}

// Inline definitions
// #include "libmodel/Part.inl"

#endif // model_Part_INCL_

