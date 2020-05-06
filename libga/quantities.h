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

#ifndef ga_quantities_INCL_
#define ga_quantities_INCL_

/*! \file
\brief Declarations for ga::quantities
*/


#include "libdat/array.h"
#include "libdat/dat.h"
#include "libdat/validity.h"
#include "libmath/math.h"

#include <array>
#include <string>


namespace ga
{
// Support for basic geometric quantities


	//
	// Unique elements
	//

//! Additive identity element
struct Zero
{
};

//! Multiplicative identity element
struct One
{
};

	//
	// Blades
	//

//! Standard scalar type.
struct Scalar
{
	double theValue{ dat::badDub };

	//! An instance which is not valid
	static
	Scalar
	null
		()
	{
		return Scalar{};
	}

	Scalar () = default;

	//! Zero element compatible with vector operations
	inline
	explicit
	Scalar
		( Zero const & zero
		);

	//! Identity element
	inline
	explicit
	Scalar
		( One const & one
		);

	//! Value ctor.
	inline
	explicit
	Scalar
		( double const & value
		);

	//! True if this instance is not valid
	inline
	bool
	isValid
		() const;

	//! Descriptive information about this instance
	std::string
	infoString
		( std::string const & title = std::string()
		, bool const & showGrade = false // if true, display grade info
		) const;

	//! Magnitude of this instance.
	inline
	double
	magnitude
		() const;

	//! Reverse of this instance.
	inline
	Scalar
	reverse
		() const;

	//! True if this and other are 'same' within tolerance
	inline
	bool
	nearlyEquals
		( Scalar const & other
		, double const & tol = math::eps
		) const;

};

//! Vector in 3D space
struct Vector
{
	std::array<double, 3u> theValues
		{{ dat::badDub, dat::badDub, dat::badDub }};

	//! An instance which is not valid
	static
	Vector
	null
		()
	{
		return Vector{};
	}

	Vector () = default;

	//! Zero element compatible with vector operations
	inline
	explicit
	Vector
		( Zero const & zero
		);

	//! Value ctor.
	inline
	explicit
	Vector
		( double const & rx
		, double const & ry
		, double const & rz
		);

	//! Value ctor.
	inline
	explicit
	Vector
		( std::array<double, 3u> const & values
		);

	//! True if this instance is not valid
	inline
	bool
	isValid
		() const;

	//! Descriptive information about this instance
	std::string
	infoString
		( std::string const & title = std::string()
		, bool const & showGrade = false // if true, display grade info
		, bool const & showMag = false // if true, display magnitude
		) const;

	//! Descriptive information about this instance
	std::string
	infoStringFmt
		( std::string const & title = std::string()
		, std::string const & fmt = "%9.6f"
		, bool const & showGrade = false // if true, display grade info
		, bool const & showMag = false // if true, display magnitude
		) const;

	//! Magnitude of this instance.
	inline
	double
	magnitude
		() const;

	//! Reverse of this instance.
	inline
	Vector
	reverse
		() const;

	//! Shortcut to theValues[ndx]
	inline
	double const &
	operator[]
		( size_t const & validNdx //!< Assumed to be 0,1,2 !!
		) const;

	//! Shortcut to theValues[ndx]
	inline
	double &
	operator[]
		( size_t const & validNdx //!< Assumed to be 0,1,2 !!
		);

	//! True if this and other are 'same' within tolerance
	inline
	bool
	nearlyEquals
		( Vector const & other
		, double const & tol = math::eps
		) const;
};

//! Bi-vector in 3D space
struct BiVector
{
	std::array<double, 3u> theValues
		{{ dat::badDub, dat::badDub, dat::badDub }};

	//! An instance which is not valid
	static
	BiVector
	null
		()
	{
		return BiVector{};
	}

	BiVector () = default;

	//! Zero element compatible with vector operations
	inline
	explicit
	BiVector
		( Zero const & zero
		);

	//! Value ctor
	inline
	explicit
	BiVector
		( double const & ix
		, double const & iy
		, double const & iz
		);

	//! Value ctor.
	inline
	explicit
	BiVector
		( std::array<double, 3u> const & values
		);

	//! True if this instance is not valid
	inline
	bool
	isValid
		() const;

	//! Descriptive information about this instance
	std::string
	infoString
		( std::string const & title = std::string()
		, bool const & showGrade = false // if true, display grade info
		, bool const & showMag = false // if true, display magnitude
		) const;

	//! Descriptive information about this instance
	std::string
	infoStringFmt
		( std::string const & title = std::string()
		, std::string const & fmt = "%9.6f"
		, bool const & showGrade = false // if true, display grade info
		, bool const & showMag = false // if true, display magnitude
		) const;

	//! Magnitude of this instance.
	inline
	double
	magnitude
		() const;

	//! Reverse of this instance.
	inline
	BiVector
	reverse
		() const;

	//! Shortcut to theValues[ndx]
	inline
	double const &
	operator[]
		( size_t const & validNdx //!< Assumed to be 0,1,2 !!
		) const;

	//! Shortcut to theValues[ndx]
	inline
	double &
	operator[]
		( size_t const & validNdx //!< Assumed to be 0,1,2 !!
		);

	//! True if this and other are 'same' within tolerance
	inline
	bool
	nearlyEquals
		( BiVector const & other
		, double const & tol = math::eps
		) const;
};

//! Tri-vector in 3D space
struct TriVector
{
	double theValue{ dat::badDub };

	//! An instance which is not valid
	static
	TriVector
	null
		()
	{
		return TriVector{};
	}

	TriVector () = default;

	//! Zero element compatible with vector operations
	inline
	explicit
	TriVector
		( Zero const & zero
		);

	//! Value ctor
	inline
	explicit
	TriVector
		( double const & value
		);

	//! True if this instance is not valid
	inline
	bool
	isValid
		() const;

	//! Descriptive information about this instance
	std::string
	infoString
		( std::string const & title = std::string()
		, bool const & showGrade = false // if true, display grade info
		) const;

	//! Magnitude of this instance.
	inline
	double
	magnitude
		() const;

	//! Reverse of this instance.
	inline
	TriVector
	reverse
		() const;

	//! True if this and other are 'same' within tolerance
	inline
	bool
	nearlyEquals
		( TriVector const & other
		, double const & tol = math::eps
		) const;
};

	//
	// Composites
	//

//! Element of even sub-algrebra
struct Spinor
{
	Scalar theS{};
	BiVector theB{};

	//! An instance which is not valid
	static
	Spinor
	null
		()
	{
		return Spinor{};
	}

	Spinor () = default;

	//! Identity element of G2 sub-algebra
	inline
	explicit
	Spinor
		( One const & one
		);

	//! Value ctor
	inline
	explicit
	Spinor
		( Scalar const & ss
		, BiVector const & bv
		);

	//! Value ctor
	inline
	explicit
	Spinor
		( double const & ss
		, BiVector const & bv
		);

	//! True if this instance is not valid
	inline
	bool
	isValid
		() const;

	//! Descriptive information about this instance
	std::string
	infoString
		( std::string const & title = std::string()
		, bool const & showGrade = false // if true, display grade info
		, bool const & showMag = false // if true, display magnitude
		) const;

	//! Descriptive information about this instance
	std::string
	infoStringFmt
		( std::string const & title = std::string()
		, std::string const & fmt = "%9.6f"
		, bool const & showGrade = false // if true, display grade info
		, bool const & showMag = false // if true, display magnitude
		) const;

	//! Magnitude of this instance.
	inline
	double
	magnitude
		() const;

	//! Reverse of this instance.
	inline
	Spinor
	reverse
		() const;

	//! True if this and other are 'same' within tolerance
	inline
	bool
	nearlyEquals
		( Spinor const & other
		, double const & tol = math::eps
		) const;
};

//! An 'imaginary' spinor - dual to Spinor
struct ImSpinor
{
	Vector theV{};
	TriVector theT{};

	//! An instance which is not valid
	static
	ImSpinor
	null
		()
	{
		return ImSpinor{};
	}

	ImSpinor () = default;

	//! Value ctor
	inline
	explicit
	ImSpinor
		( Vector const & vec
		, TriVector const & tv
		);

	//! True if this instance is not valid
	inline
	bool
	isValid
		() const;

	//! Descriptive information about this instance
	std::string
	infoString
		( std::string const & title = std::string()
		, bool const & showGrade = false // if true, display grade info
		, bool const & showMag = false // if true, display magnitude
		) const;

	//! Magnitude of this instance.
	inline
	double
	magnitude
		() const;

	//! Reverse of this instance.
	inline
	ImSpinor
	reverse
		() const;

	//! True if this and other are 'same' within tolerance
	inline
	bool
	nearlyEquals
		( ImSpinor const & other
		, double const & tol = math::eps
		) const;
};

using G2 = Spinor;
using IG2 = ImSpinor;

//
// Basis blades
//

extern Zero const zero;
extern One const one;

extern Scalar const e0;
extern Vector const e1;
extern Vector const e2;
extern Vector const e3;
extern BiVector const E23;
extern BiVector const E31;
extern BiVector const E12;
extern TriVector const E123;
extern std::array<Vector, 3u> const ei;
extern std::array<BiVector, 3u> const Eij;

extern Scalar const sZero;
extern Vector const vZero;
extern BiVector const bZero;
extern TriVector const tZero;


}

// Inline definitions
#include "libga/quantities.inl"

#endif // ga_quantities_INCL_

