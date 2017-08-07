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


/*! \file
\brief Definitions for blk::blk
*/


#include "libblk/blk.h"

#include <sstream>


/*! \brief Types and values useful for libblk module.
*/

namespace blk
{

EdgeOri
principalEdgeOri
	( EdgeOri const & anyEdgeOri
	)
{
	EdgeOri ijEdgeOri{ anyEdgeOri };
	EdgeKey const & key = anyEdgeOri.first;
	if (! (key.first < key.second))
	{
		EdgeKey const keyRev{ key.second, key.first };
		ga::Rigid const & ori = anyEdgeOri.second;
		ijEdgeOri = { keyRev, ori.inverse() };
	}
	return ijEdgeOri;
}

std::vector<ga::Rigid>
fitOnto
	( std::vector<ga::Rigid> const & oriHaveWrtRefs
	, ga::Rigid const & oriWantWrtAny
	, ga::Rigid const & oriHaveWrtAny
	)
{
	std::vector<ga::Rigid> oriWantWrtRefs;
	oriWantWrtRefs.reserve(oriHaveWrtRefs.size());

	// compute relationship of target frame w.r.t. source frame
	ga::Rigid const oriAnyWrtHave{ oriHaveWrtAny.inverse() };
	ga::Rigid const oriWantWrtHave{ oriWantWrtAny * oriAnyWrtHave };

	// transform all orientations from expression w.r.t. source to target
	for (ga::Rigid const & oriInHave : oriHaveWrtRefs)
	{
		ga::Rigid const oriInWant{ oriWantWrtHave * oriInHave };
		oriWantWrtRefs.emplace_back(oriInWant);
	}

	return oriWantWrtRefs;
}

} // blk

