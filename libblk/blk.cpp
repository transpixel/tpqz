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

#include <algorithm>
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
		ga::Rigid const oriXw1{ ori1wX.inverse() };
		return { ori2wX * oriXw1 };
	}

	//! Functor for conversion of Oris w.r.t. frame 1 into oris w.r.t. frame 2
	struct ToTgtFrame
	{
		// orientation to apply
		ga::Rigid theSrcWrtTgt;

		explicit
		ToTgtFrame
			( ga::Rigid const & oriAnyWrtTgt
			, ga::Rigid const & oriAnyWrtSrc
			)
			: theSrcWrtTgt{ oriAnyWrtSrc.inverse() * oriAnyWrtTgt }
		{ }

		//! Returns oriTgtWrtRef
		ga::Rigid
		operator()
			( ga::Rigid const & oriWrtSrc
			) const
		{
			return { oriWrtSrc * theSrcWrtTgt };
		}

		//! Returns oriTgtWrtRef
		std::pair<NodeKey, ga::Rigid>
		operator()
			( std::pair<NodeKey, ga::Rigid> const & pairWrtSrc
			) const
		{
			return { pairWrtSrc.first, (*this)(pairWrtSrc.second) };
		}

		//! Descriptive information about this instance
		std::string
		infoString
			( std::string const & title = std::string{}
			) const
		{
			std::ostringstream oss;
			if (! title.empty())
			{
				oss << title << std::endl;
			}
			oss << theSrcWrtTgt.infoStringShort("theSrcWrtTgt");
			oss << std::endl;
			/*
			ga::Rigid const oriTgtWrtSrc{ theSrcWrtTgt.inverse() };
			oss << oriTgtWrtSrc.infoStringShort("oriTgtWrtSrc");
			*/

			return oss.str();
		}
	};

	template <typename ContainerOut, typename ContainerIn>
	void
	transformed
		( ContainerOut * const & outCon // expressed in Tgt frame
		, ga::Rigid const & oriAnyWrtTgt
		, ContainerIn const & inCon // expressed in Src frame
		, ga::Rigid const & oriAnyWrtSrc
		)
	{
		// compute relationship of target frame w.r.t. source frame
		ToTgtFrame const xTgtFromSrc(oriAnyWrtTgt, oriAnyWrtSrc);

		// transform all orientations from expression w.r.t. source to target
		std::transform
			( inCon.begin(), inCon.end()
			, std::inserter(*outCon, outCon->end())
			, xTgtFromSrc
			);
	}

}

std::vector<ga::Rigid>
transformed
	( std::vector<ga::Rigid> const & oriNodeWrtSrcs
	, ga::Rigid const & oriAnyWrtTgt
	, ga::Rigid const & oriAnyWrtSrc
	)
{
	std::vector<ga::Rigid> oriNodeWrtTgts;
	oriNodeWrtTgts.reserve(oriNodeWrtSrcs.size());

	// transform all orientations from expression w.r.t. source to target
	transformed
		(&oriNodeWrtTgts, oriAnyWrtTgt, oriNodeWrtSrcs, oriAnyWrtSrc);

	return oriNodeWrtTgts;
}

std::map<NodeKey, ga::Rigid>
transformed
	( std::map<NodeKey, ga::Rigid> const & pairNodeWrtSrcs
	, ga::Rigid const & oriAnyWrtTgt
	, ga::Rigid const & oriAnyWrtSrc
	)
{
	std::map<NodeKey, ga::Rigid> pairNodeWrtTgts;

	// transform all orientations from expression w.r.t. source to target
	transformed
		(&pairNodeWrtTgts, oriAnyWrtTgt, pairNodeWrtSrcs, oriAnyWrtSrc);

	return pairNodeWrtTgts;
}

} // blk

