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

#ifndef ro_PairRel_INCL_
#define ro_PairRel_INCL_

/*! \file
\brief Declarations for ro::PairRel
*/


#include "libro/Pair.h"


namespace ro
{

/*! \brief Relative (dependent-style) orientation (primary is identity)

\par Example
\dontinclude testro/uPair.cpp
\skip ExampleStart
\until ExampleEnd
*/

class PairRel : public ro::Pair
{
	ga::Rigid the1wRef;
	ga::Rigid theOri2w1;

public: // methods

	//! default null constructor
	PairRel
		() = default;

	//! Construct from pair of orientations
	explicit
	PairRel
		( ga::Rigid const & ori1wRef
		, ga::Rigid const & ori2wRef
		)
		: ro::Pair("PairRel")
		, the1wRef(ori1wRef)
		, theOri2w1{ ori2wRef * the1wRef.inverse() }
	{ }

	//! Convenience version of above
	explicit
	PairRel
		( OriPair const & oriPair
		)
		: PairRel(oriPair.first, oriPair.second)
	{ }

	//! Check if instance is valid
	virtual
	bool
	isValid
		() const
	{
		return theOri2w1.isValid();
	}

	//! Orientation of primary member
	virtual
	ga::Rigid
	rigid1w0
		() const
	{
		return ga::Rigid::identity();
	}

	//! Orientation of secondary member
	virtual
	ga::Rigid
	rigid2w0
		() const
	{
		return theOri2w1;
	}

	//! Orientation of model frame w.r.t. reference
	inline
	virtual
	ga::Rigid
	rigid0wRef
		() const
	{
		// orientations are w.r.t. frame 1
		return the1wRef;
	}

	//! Descriptive information about this instance.
	// virtual


}; // PairRel

} // ro

// Inline definitions
// #include "libro/PairRel.inl"

#endif // ro_PairRel_INCL_

