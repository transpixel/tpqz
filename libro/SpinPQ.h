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

#ifndef ro_SpinPQ_INCL_
#define ro_SpinPQ_INCL_

/*! \file
\brief Declarations for ro::SpinPQ
*/


#include "libga/ga.h"
#include "libga/spin.h"
#include "libro/PairRel.h"
#include "libro/ro.h"

#include <array>
#include <string>
#include <sstream>


namespace ro
{

/*! \brief Spinor pair representing RO formulation from Horn.

\par Example
\dontinclude testro/uSpinPQ.cpp
\skip ExampleStart
\until ExampleEnd
*/

class SpinPQ
{
	std::pair<ga::Spinor, ga::Spinor> thePQ;

private: // static methods

	//! Create in accordance with explicit defintion
	static
	std::pair<ga::Spinor, ga::Spinor>
	pairPQ
		( ga::BiVector const & phi
		, ga::BiVector const & theta
		, ga::BiVector const & basePlane
		);

	//! Forward and optionally negated values
	inline
	static
	std::pair<ga::Spinor, ga::Spinor>
	permForward
		( std::pair<ga::Spinor, ga::Spinor> const & aPQ
		, double const & multA = 1. //!< E.g. +1. or -1.
		, double const & multB = 1. //!< E.g. +1. or -1.
		);

	//! Reversed and optionally negated values
	inline
	static
	std::pair<ga::Spinor, ga::Spinor>
	permReverse
		( std::pair<ga::Spinor, ga::Spinor> const & aPQ
		, double const & multA = 1. //!< E.g. +1. or -1.
		, double const & multB = 1. //!< E.g. +1. or -1.
		);

public: // static methods

	//! Construct per definition
	static
	SpinPQ
	from
		( ga::BiVector const & phi
		, ga::BiVector const & theta
		, ga::BiVector const & basePlane = ga::E12
		);

	//! Construct from an aribtrary pair of orientations
	static
	SpinPQ
	from
		( std::pair<ga::Rigid, ga::Rigid> const & oriPair
		);

public: // methods

	//! default null constructor
	SpinPQ
		() = default;

	//! Value ctor
	explicit
	SpinPQ
		( std::pair<ga::Spinor, ga::Spinor> const & pairPQ
		);

	//! Construct from arbitrary relative orientation
	explicit
	SpinPQ
		( ro::Pair const & anyRO
		);

	//! Evaluate triple product
	double
	tripleProductGap
		( std::pair<ga::Vector, ga::Vector> const & uvPair
		) const;

	//! True if instance is valid
	bool
	isValid
		() const;

	//! Number of unqiue permutations
	static constexpr size_t theNumPerms{ 4u };

	//! Solutions in combination (combinations of +/- and reversals)
	SpinPQ const &
	operator[]
		( size_t const & ndx
		) const
	{
		static std::array<SpinPQ, theNumPerms> const perms
			{{ SpinPQ(permForward(thePQ,  1.,  1.))  // 0 *
			 , SpinPQ(permForward(thePQ,  1., -1.))  // 1 *
		//	 , SpinPQ(permForward(thePQ, -1.,  1.))  // == 1
		//	 , SpinPQ(permForward(thePQ, -1., -1.))  // == 0
			 , SpinPQ(permReverse(thePQ,  1.,  1.))  // 4 *
			 , SpinPQ(permReverse(thePQ,  1., -1.))  // 5 *
		//	 , SpinPQ(permReverse(thePQ, -1.,  1.))  // == 5
		//	 , SpinPQ(permReverse(thePQ, -1., -1.))  // == 4
			}};
		assert(ndx < perms.size());
		return perms[ndx];
	}

	//! Expression of current values as dependent relative orientation.
	OriPair
	pair
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		, std::string const & fmt = std::string("%9.6f")
		) const;


}; // SpinPQ

} // ro

// Inline definitions
#include "libro/SpinPQ.inl"

#endif // ro_SpinPQ_INCL_

