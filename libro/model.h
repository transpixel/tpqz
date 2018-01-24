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

#ifndef ro_model_INCL_
#define ro_model_INCL_

/*! \file
\brief Declarations for ro::model
*/


#include "libro/ro.h"

#include <vector>


namespace ro
{

/*! \brief Functions related to 3D (stereo) model concepts

\par Example
\dontinclude testro/umodel.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace model
{
	//! Pair of unit directions to point
	PairUV
	uvPair
		( ga::Vector const & pntIn0
		, OriPair const & oriPairIn0
		);

	//! Unit direction pairs to all points
	std::vector<PairUV>
	uvPairs
		( std::vector<ga::Vector> const & pntsIn0
		, OriPair const & oriPairIn0
		);

	//! 3D points associate with a ray pair
	PntPair
	pointPair
		( PairUV const & uvPair
		, OriPair const & oriPairIn0
		);

	//! Pairs of 3D points - one along each of (u,v) rays
	std::vector<PntPair>
	pointPairs
		( std::vector<PairUV> const & uvPairs
		, OriPair const & oriPairIn0
		);

	//! Number of directions from ptrQuintUV that intersect 'in-front-of'
	size_t
	numForward
		( OriPair const & oriPair
		, FiveOf<PairUV const * const> const & ptrQuintUV
		);

	//! True if all directions from quint are forward
	inline
	bool
	isForward
		( OriPair const & oriPair
		, FiveOf<PairUV const * const> const & quintPtrUVs
		)
	{
		return (quintPtrUVs.size() == numForward(oriPair, quintPtrUVs));
	}

} // model

} // ro

// Inline definitions
// #include "libro/model.inl"

#endif // ro_model_INCL_

