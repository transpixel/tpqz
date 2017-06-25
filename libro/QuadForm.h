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

#ifndef ro_QuadForm_INCL_
#define ro_QuadForm_INCL_

/*! \file
\brief Declarations for ro::QuadForm
*/


#include "libdat/validity.h"
#include "libro/ro.h"

#include <array>
#include <string>


namespace ro
{

/*! \brief Quadratic form representation of RO (e.g. Fundamental Matrix)

\par Example
\dontinclude testro/uQuadForm.cpp
\skip ExampleStart
\until ExampleEnd
*/

class QuadForm
{
	using Row = std::array<double, 3u>;
	using Matrix = std::array<Row, 3u>;

	Matrix theMatF;

public: // methods

	//! default null constructor
	QuadForm
		();

	//! Construct from generic RO (using tripleProductGap() operations)
	explicit
	QuadForm
		( OriPair const & roPair
		);

	//! True if instance is valid
	bool
	isValid
		() const;

	//! Matrix implementation (faster if cached) for tripleProductGap()
	double
	tripleProductGap
		( ro::PairUV const & uvPair
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // QuadForm

} // ro

// Inline definitions
// #include "libro/QuadForm.inl"

#endif // ro_QuadForm_INCL_

