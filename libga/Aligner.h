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

#ifndef ga_Aligner_INCL_
#define ga_Aligner_INCL_

/*! \file
\brief Declarations for ga::Aligner
*/


#include "libga/ga.h"
#include "libga/Pose.h"

#include <string>


namespace ga
{

/*! \brief Manipulate alignment values.

\par Example
\dontinclude testga/uAligner.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Aligner
{
	BiVector thePhysAngle;

public: // data

	//! Pair of entities to align exactly
	template <typename Type>
	struct ExactFit
	{
		std::pair<Type, Type> theFromInto;

		//! Pair of entities to be aligned exactly
		inline
		explicit
		ExactFit
			( Type const & from
			, Type const & into
			);
	};

	//! Pair of entities to be aligned as best as possible
	template <typename Type>
	struct BestTry
	{
		std::pair<Type, Type> theFromInto;

		//! Pair of entities to be aligned exactly
		inline
		explicit
		BestTry
			( Type const & from
			, Type const & into
			);
	};

public: // static methods

	//! Spinor such that: v'=spin*v*spin.reverse() via shortest rotation.
	static
	Spinor
	passiveSpinorFor
		( Vector const & from
		, Vector const & into
		);

	//! Spinor such that: B'=spin*B*spin.reverse() via shortest rotation.
	static
	Spinor
	passiveSpinorFor
		( BiVector const & from
		, BiVector const & into
		);

public: // methods

	//! default null constructor
	Aligner
		();

	//! Construct by aligning pairs of vectors
	explicit
	Aligner
		( ExactFit<Vector> const & exactFromInto
		, BestTry<Vector> const & bestFromInto
		);

	// copy constructor -- compiler provided
	// assignment operator -- compiler provided
	// destructor -- compiler provided

	//! Check if instance is valid
	bool
	isValid
		() const;

	//! Attitude of 'Into' frame w.r.t. 'From' frame.
	Pose
	pose
		() const;

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
		) const;
};

}

// Inline definitions
#include "libga/Aligner.inl"

#endif // ga_Aligner_INCL_

