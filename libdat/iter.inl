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
\brief Inline definitions for dat::iter
*/


#include "libio/stream.h"
#include "libdat/info.h"


namespace dat
{
namespace iter
{

	namespace
	{
		//! Sizes for maximally uniform partition of numTotal
		std::vector<size_t>
		groupSizes
			( size_t const numTotal
			, size_t const numGroups
			)
		{
			std::vector<size_t> groupSizes(numGroups);

			assert(0u < numTotal);
			assert(0u < numGroups);

			// fill to min size
			size_t sumOfGroups{ 0u };
			if (numTotal < numGroups)
			{
				// will be some empty groups - fill residuals below
				std::fill(groupSizes.begin(), groupSizes.end(), 0u);
			}
			else // if (numGroups <= numTotal)
			{
				// fill each group to same size - add residuals below
				size_t const minPerGroup{ numTotal / numGroups };
				std::fill(groupSizes.begin(), groupSizes.end(), minPerGroup);
				sumOfGroups += minPerGroup * numGroups;
			}

			// distribute residual amount over groups from first to last
			for (size_t nn{0u} ; (sumOfGroups < numTotal); ++nn)
			{
				++(groupSizes[nn]);
				++sumOfGroups;
			}

			return groupSizes;
		}
	}

template <typename Iter>
std::vector<std::pair<Iter, Iter> >
groups
	( Iter const & beg
	, Iter const & end
	, size_t const & numGroups
	)
{
	std::vector<std::pair<Iter, Iter> > pairs;

	size_t const totalSize{ (size_t)(std::distance(beg, end)) };
	if (0u < totalSize)
	{
		if (0u < numGroups)
		{
			std::vector<size_t> const deltas(groupSizes(totalSize, numGroups));

			Iter it0{ beg };
			for (size_t nn{0u} ; nn < deltas.size() ; ++nn)
			{
				size_t const delta = deltas[nn];
				if (0u < delta) // skip empty groups (e.g. for overages)
				{
					Iter it1{ it0 };
					std::advance(it1, delta);
					pairs.emplace_back(std::pair<Iter, Iter>{ it0, it1 });
					it0 = it1;
				}
			}

			/*
			for (std::pair<Iter, Iter> const & pair : pairs)
			{
				ptrdiff_t const d1{ std::distance(beg, pair.first) };
				ptrdiff_t const d2{ std::distance(beg, pair.second) };
				ptrdiff_t const groupSize{ d2 - d1 };
				io::out()
					<< "d1,d2:"
					<< " " << dat::infoString(d1)
					<< " " << dat::infoString(d2)
					<< " " << dat::infoString(groupSize, "groupSize")
					<< std::endl;
			}
			*/

		}
	}

	return pairs;
}

}
}

