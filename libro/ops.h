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

#ifndef ro_ops_INCL_
#define ro_ops_INCL_

/*! \file
\brief Declarations for ro::ops
*/


#include "libga/Rigid.h"
#include "libro/ro.h"

#include <iomanip>
#include <sstream>
#include <string>
#include <utility>


namespace ro
{

//! Operations on pairs of transforms

	//! Pose components of oriPair
	inline
	std::pair<ga::Pose, ga::Pose>
	attitudesOf
		( std::pair<ga::Rigid, ga::Rigid> const & oriPair
		);

	//! EO pair from attitude pair and separation vector
	inline
	std::pair<ga::Rigid, ga::Rigid>
	orientationsFrom
		( LocPair const & locPair
		, std::pair<ga::Pose, ga::Pose> const & attPair
		);

	//! Pair of inverses of original pair elements
	template <typename Type>
	inline
	std::pair<Type, Type>
	inverseOf
		( std::pair<Type, Type> const & pairBwA
		);

	//! Composition of attitude pair
	template <typename Type>
	inline
	std::pair<Type, Type>
	compositeOf
		( std::pair<Type, Type> const & pairCwB
		, std::pair<Type, Type> const & pairBwA
		);

	//! Composition of attitude pair
	template <typename Type>
	inline
	std::pair<Type, Type>
	compositeOf
		( Type const & attCwB
		, std::pair<Type, Type> const & pairBwA
		);

	//! Composition of attitude pair
	template <typename Type>
	inline
	std::pair<Type, Type>
	compositeOf
		( std::pair<Type, Type> const & pairCwB
		, Type const & attBwA
		);

	//! Relative difference
	template <typename Type>
	inline
	Type
	relative2w1
		( std::pair<Type, Type> const & pair
		);

	//! Vector from first to second
	inline
	ga::Vector
	vectorBetween
		( std::pair<ga::Rigid, ga::Rigid> const & roPair
		);

	//! Direction vector from first toward second
	inline
	ga::Vector
	directionBetween
		( std::pair<ga::Rigid, ga::Rigid> const & roPair
		);

	//! Separation between first and second
	inline
	double
	gapBetween
		( std::pair<ga::Rigid, ga::Rigid> const & roPair
		);

	//! Point midway between the two stations
	inline
	ga::Vector
	midPointOf
		( std::pair<ga::Rigid, ga::Rigid> const & roPair
		);

	//! Orientation of pair frame w.r.t. reference
	inline
	ga::Rigid
	rigidModelWrtRef
		( ga::Rigid const & oriAnyWrtMod
		, ga::Rigid const & oriAnyWrtRef
		);

	//! OriPair for which (2 w.r.t. Sqrt) == (Sqrt w.r.t. 1)
	std::pair<ga::Rigid, ga::Rigid>
	sqrtOriPairFor
		( std::pair<ga::Rigid, ga::Rigid> const & roPairInRef
		, ga::Rigid * const & ptOriFitWrtRef = nullptr
		);

	//! Descriptive information about pair
	template <typename Type>
	inline
	std::string
	infoString
		( std::pair<Type, Type> const & aPair
		, std::string const & title = {}
		);

	//! Descriptive information about pair
	template <typename Type>
	inline
	std::string
	infoStringRelative
		( std::pair<Type, Type> const & aPair
		, std::string const & title = {}
		);


} // ro

// Inline definitions
#include "libro/ops.inl"

#endif // ro_ops_INCL_

