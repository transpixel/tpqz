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

#include <iterator>
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

namespace
{
	//! Into w.r.t From orientationon from independent reference orientations
	inline
	ga::Rigid
	rigid2w1
		( ga::Rigid const & ori2wX
		, ga::Rigid const & ori1wX
		)
	{
		ga::Rigid const oriAnyWrtFrom{ ori1wX.inverse() };
		return { ori2wX * oriAnyWrtFrom };
	}

	//! Functor for conversion of Oris w.r.t. frame 1 into oris w.r.t. frame 2
	struct Ori2w1
	{
		ga::Rigid theOri2w1;

		explicit
		Ori2w1
			( ga::Rigid const & ori2wX
			, ga::Rigid const & ori1wX
			)
			: theOri2w1{ rigid2w1(ori2wX, ori1wX) }
		{ }

		//! Returns ori2wX
		ga::Rigid
		operator()
			( ga::Rigid const & ori1wX
			) const
		{
			return { theOri2w1 * ori1wX };
		}

		//! Returns ori2wX
		std::pair<NodeKey, ga::Rigid>
		operator()
			( std::pair<NodeKey, ga::Rigid> const & edge1wX
			) const
		{
			return { edge1wX.first, (*this)(edge1wX.second) };
		}
	};

}

std::vector<ga::Rigid>
fitOnto
	( std::vector<ga::Rigid> const & oriFromWrtRefs
	, ga::Rigid const & oriIntoWrtAny
	, ga::Rigid const & oriFromWrtAny
	)
{
	std::vector<ga::Rigid> oriIntoWrtRefs;
	oriIntoWrtRefs.reserve(oriFromWrtRefs.size());

	// compute relationship of target frame w.r.t. source frame
	Ori2w1 const xIntoWrtFrom(oriIntoWrtAny, oriFromWrtAny);

	// transform all orientations from expression w.r.t. source to target
	for (ga::Rigid const & oriFromWrtRef : oriFromWrtRefs)
	{
		oriIntoWrtRefs.emplace_back(xIntoWrtFrom(oriFromWrtRef));
	}

	return oriIntoWrtRefs;
}

std::map<NodeKey, ga::Rigid>
fitOnto
	( std::map<NodeKey, ga::Rigid> const & oriFromWrtRefs
	, ga::Rigid const & oriIntoWrtAny
	, ga::Rigid const & oriFromWrtAny
	)
{
	std::map<NodeKey, ga::Rigid> oriIntoWrtRefs;

	// compute relationship of target frame w.r.t. source frame
	Ori2w1 const xIntoWrtFrom(oriIntoWrtAny, oriFromWrtAny);

	// transform all orientations from expression w.r.t. source to target
	for (std::pair<NodeKey, ga::Rigid> const & oriFromWrtRef : oriFromWrtRefs)
	{
		oriIntoWrtRefs.insert
			(oriIntoWrtRefs.end(), xIntoWrtFrom(oriFromWrtRef));
	}

	return oriIntoWrtRefs;
}

} // blk

