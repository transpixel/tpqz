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
\brief Definitions for blk::RelOriPool
*/


#include "libblk/RelOriPool.h"

#include <sstream>


namespace blk
{

// static
EdgeOri
RelOriPool :: nullEdgeOri
	()
{
	constexpr size_t badNdx{ dat::nullValue<size_t>() };
	ga::Rigid const badOri{};
	return EdgeOri{ EdgeKey{ badNdx, badNdx}, badOri };
}

// static
RelOriPool
RelOriPool :: from
	( std::vector<blk::OriPair> const & rops
	)
{
	RelOriPool pool;
	for (blk::OriPair const & rop : rops)
	{
		blk::OriPair const ijRop{ rop.principalPair() };
		EdgeKey const ijPair{ ijRop.ndxI(), ijRop.ndxJ() };
		pool.theRelOriMap[ijPair] = ijRop.theOriJwI;
	}
	return pool;
}

// explicit
RelOriPool :: RelOriPool
	( std::map<EdgeKey, ga::Rigid> const & relOris
	)
	: theRelOriMap{ relOris }
{
}

EdgeOri
RelOriPool :: edgeOriFor
	( size_t const & ndx1
	, size_t const & ndx2
	) const
{
	EdgeOri lhOri{ nullEdgeOri() };

	EdgeKey findEdge{ ndx1, ndx2 };
	ga::Rigid ori2w1;

	if (ndx1 == ndx2) // ori of node w.r.t. self
	{
		ori2w1 = ga::Rigid::identity();
	}
	else
	{
		// theRelOriMap is required to have ordered index pair as key
		bool isForward{ true };
		if (ndx2 < ndx1)
		{
			isForward = false;
			findEdge = { ndx2, ndx1 };
		}

		// search for (ordered) node pair key within RelOri collection
		std::map<EdgeKey, ga::Rigid>::const_iterator const itFind
			{ theRelOriMap.find(findEdge) };
		if (theRelOriMap.end() != itFind)
		{
			ga::Rigid const & oriFound = itFind->second;
			if (isForward)
			{
				ori2w1 = oriFound;
			}
			else // requested keys in reversed order
			{
				ori2w1 = oriFound.inverse();
			}
		}
	}

	// if orientation is good, set/validate return structure
	if (dat::isValid(ori2w1))
	{
		lhOri.first = findEdge;
		lhOri.second = ori2w1;
	}

	return lhOri;
}

std::string
RelOriPool :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	if (! theRelOriMap.empty())
	{
	}
	else
	{
		oss << " <empty>";
	}
	return oss.str();
}

} // blk

