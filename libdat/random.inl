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
\brief Inline definitions for dat::random
*/

namespace dat
{

namespace random
{

namespace
{
	//! Update sorted array and usecount with new values
	template <size_t Dim>
	void
	insertIntoSorted // TODO could cleanup and make (more)public
		( std::array<size_t, Dim> & values
		, size_t & inUseSize
		, size_t const & addValue
		)
	{
		// Could use STL, but here combine find and move operations
		size_t const prevUseSize{ inUseSize };
		if (0u < prevUseSize) // check if bubbling needed
		{
			size_t lastNdx{ prevUseSize - 1 };

			// run backward through existing values
			while (0u <= lastNdx)
			{
				// start current value end of output
				size_t const currValue = values[lastNdx];
				if (currValue < addValue) // add at tail
				{
					// add current value after previous values
					values[lastNdx + 1u] = addValue;
					break;
				}
				else
				if (addValue < currValue)
				{
					// bump previous values toward end of output
					values[lastNdx + 1u] = values[lastNdx];
					values[lastNdx] = addValue;
				}
				else
				{
					break;
				}

				// consider next input value
				if (0u < lastNdx)
				{
					--lastNdx; // consider next earliest element
				}
			}
		}
		else
		{
			// create initial output population
			values[0] = addValue;
		}
		++inUseSize;
	}

}

template <size_t Dim, typename RandGen>
void
index_sample
	( std::array<size_t, Dim> * const & ptSampNdxs
	, size_t const & sourceSizeN
	, RandGen & rgen
	)
{
	std::array<size_t, Dim> & sampNdxs = *ptSampNdxs;
	size_t sampUseCount{ 0u };
	constexpr size_t ndxMin{ 0u }; // assume standard offsets

	// for sane cases
	if (Dim < sourceSizeN)
	{
		// perform Dim random selections
		for (size_t nn{0u} ; nn < Dim ; ++nn)
		{
			// determine an offset into *UN*selected indices
			size_t const ndxMax{ sourceSizeN - 1u - nn};
			std::uniform_int_distribution<size_t> rdistro(ndxMin, ndxMax);
			size_t candNdx{ rdistro(rgen) };

			// check for initialization state
			if (0u == sampUseCount)
			{
				insertIntoSorted<Dim>(sampNdxs, sampUseCount, candNdx);
			}
			else
			{
				// start by assuming candidate index is useful
				for (size_t nn{0u} ; nn < sampUseCount ; ++nn)
				{
					// check for collision with existing index
					size_t const & hitNdx = sampNdxs[nn];
					if (candNdx < hitNdx) // candidate is free slot
					{
						// candidate is before next colision so use it
						break;
					}
					else // collision with existing index
					{
						// bump candidate to try next open slot
						++candNdx;
					}
				}
				// candidate is now valid, so record it
				assert(candNdx < sourceSizeN);
				insertIntoSorted<Dim>(sampNdxs, sampUseCount, candNdx);
			}
		}
	}
	else
	if (Dim == sourceSizeN)
	{
		std::iota(sampNdxs.begin(), sampNdxs.end(), ndxMin);
	}
	else
	{
		static size_t const badNdx(dat::nullValue<size_t>());
		std::fill(sampNdxs.begin(), sampNdxs.end(), badNdx);
	}
	return;
}

template <size_t Dim, typename RandGen>
std::array<size_t, Dim>
index_sample
	( size_t const & sourceSizeN
	, RandGen & rgen //!< Random generator e.g. std::19937_64
	)
{
	std::array<size_t, Dim> sampNdxs{ dat::nullValue<size_t>() };
	index_sample(& sampNdxs, sourceSizeN, rgen);
	return sampNdxs;
}

} // random

} // dat

