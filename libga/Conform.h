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

#ifndef ga_Conform_INCL_
#define ga_Conform_INCL_

/*! \file
\brief Declarations for ga::Conform
*/


#include "libga/Rigid.h"

#include <string>


namespace ga
{

/*! \brief Conformal transformation in 3D space

\par Example
\dontinclude testga/uConform.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Conform
{
	double theScaleFwd{ dat::nullValue<double>() };
	Rigid theRigidFwd{};

public: // methods

	//! default null constructor
	Conform
		() = default;

	//! Value construction
	inline
	explicit
	Conform
		( double const & scale
		, Rigid const & rigid // = { Rigid::identity() }
		);

	//! Indirect value construction
	inline
	explicit
	Conform
		( double const & scale
		, ga::Vector const & location
		, ga::Pose const & pose
		);

	//! Indirect value construction
	inline
	explicit
	Conform
		( double const & scale
		, ga::Vector const & location
		, ga::BiVector const & physAngle
		);

	//! True if instance is valid
	inline
	bool
	isValid
		() const;

	//! Transform vector from domain into range
	inline
	Vector
	operator()
		( Vector const & vec
		) const;

	//! Inverse of this instance
	inline
	Conform
	inverse
		() const;

	//! Net transformation this after rhsFirst
	inline
	Conform
	operator*
		( Conform const & rhsFirst
		) const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;

}; // Conform

} // ga

// Inline definitions
#include "libga/Conform.inl"

#endif // ga_Conform_INCL_

