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

#ifndef ro_PairAbs_INCL_
#define ro_PairAbs_INCL_

/*! \file
\brief Declarations for ro::PairAbs
*/


#include "libro/Pair.h"


namespace ro
{

/*! \brief Implicit relative orientation - via pair of absolute orientations.

\par Example
\dontinclude testro/uPair.cpp
\skip ExampleStart
\until ExampleEnd
*/

class PairAbs : public ro::Pair
{
	ga::Rigid theOri1wRef;
	ga::Rigid theOri2wRef;

public: // methods

	//! default null constructor
	PairAbs
		() = default;

	//! Construct from pair of orientations
	explicit
	PairAbs
		( ga::Rigid const & ori1wRef
		, ga::Rigid const & ori2wRef
		)
		: ro::Pair("PairAbs")
		, theOri1wRef{ ori1wRef }
		, theOri2wRef{ ori2wRef }
	{ }

	//! Check if instance is valid
	virtual
	bool
	isValid
		() const
	{
		return
			(  theOri1wRef.isValid()
			&& theOri2wRef.isValid()
			);
	}

	//! Orientation of primary member
	virtual
	ga::Rigid
	rigid1w0
		() const
	{
		return theOri1wRef;
	}

	//! Orientation of secondary member
	virtual
	ga::Rigid
	rigid2w0
		() const
	{
		return theOri2wRef;
	}

	//! Orientation of model frame w.r.t. reference
	inline
	virtual
	ga::Rigid
	rigid0wRef
		() const
	{
		// orientations are w.r.t. frame of ctor inputs
		return ga::Rigid::identity();
	}

	//! Descriptive information about this instance.
	// virtual


}; // PairAbs

} // ro

// Inline definitions
// #include "libro/PairAbs.inl"

#endif // ro_PairAbs_INCL_

