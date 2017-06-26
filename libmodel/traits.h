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

#ifndef model_traits_INCL_
#define model_traits_INCL_

/*! \file
\brief Declarations for model::traits
*/


#include "libdat/validity.h"
#include "libga/ga.h"
#include "libga/Rigid.h"
#include "libmodel/atom.h"

#include <string>


namespace model
{

/*! \brief Specification for general model properties.

\par Example
\dontinclude testmodel/utraits.cpp
\skip ExampleStart
\until ExampleEnd
*/
namespace traits
{

//! "Visual" attributes of interest
struct Radiometry
{
	atom::Color const theColor;

	//! Value constructor
	explicit
	Radiometry
		( atom::Color const & color = atom::nullColor
		);

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

//! Geometric attributes of interest
struct Geometry
{
	ga::Vector const theNorm;

	//! Value constructor
	explicit
	Geometry
		( ga::Vector const & norm = ga::Vector::null()
		);

	//! Check if instance is valid
	inline
	bool
	isValid
		() const;

	//! Geometry expressed in "Out" frame
	inline
	Geometry
	transformedBy
		( ga::Rigid const & xOutWrtThis
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};


}
}

// Inline definitions
#include "libmodel/traits.inl"

#endif // model_traits_INCL_

