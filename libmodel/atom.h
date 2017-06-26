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

#ifndef model_atom_INCL_
#define model_atom_INCL_

/*! \file
\brief Declarations for model::atom
*/


#include "libga/quantities.h"

#include <array>


namespace model
{

/*! \brief Fundamental information element types.

\par Example
\dontinclude testmodel/uatom.cpp
\skip ExampleStart
\until ExampleEnd
*/
namespace atom
{

	//! Fundamental element for radiometric information
	using Color = std::array<float, 3u>;
	constexpr Color nullColor
		{{ dat::nullValue<float>()
		 , dat::nullValue<float>()
		 , dat::nullValue<float>()
		}};

	//! Fundamental element for range-distance information
	using Range = float;
	constexpr Range nullRange{ dat::nullValue<float>() };

	//! Fundamental element for point information
	using Point = ga::Vector;

}

}

// Inline definitions
// #include "libmodel/atom.inl"

#endif // model_atom_INCL_

