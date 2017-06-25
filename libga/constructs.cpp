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


/*! \file
\brief Definitions for ga::constructs
*/


#include "libga/constructs.h"

#include "libga/functions.h"
#include "libga/operators.h"

#include <algorithm>
#include <cmath>


namespace ga
{
	namespace
	{
		//! index of smallest magnitude 
		size_t
		ndxSmallestMag
			( std::array<double, 3u> const & dat
			)
		{
			size_t ndx(0u);

			// quick and easy (for coding) two-pass algorithm
			std::array<double, 3u> const mags
				{{ std::abs(dat[0])
				 , std::abs(dat[1])
				 , std::abs(dat[2])
				}};

			// find minimum mag (combine with compute for/if large collection)
			std::array<double, 3u>::const_iterator const itMin
				(std::min_element(mags.begin(), mags.end()));

			// compute offset
			ndx = (size_t)(itMin - mags.begin());

			return ndx;
		}
	}

//======================================================================

Vector
anyPerpendicularDir
	( Vector const & vec
	)
{
	Vector perp;

	if (vec.isValid())
	{
		// evaluate rejection candidates
		static std::array<Vector, 3u> const dirs{{ e1, e2, e3 }};
		std::array<double, 3u> const dirdots
			{{ dot(vec, dirs[0]).theValue
			 , dot(vec, dirs[1]).theValue
			 , dot(vec, dirs[2]).theValue
			 }};

		// compute prependicular from most stable candidate
		size_t const ndx(ndxSmallestMag(dirdots));
		BiVector const plane((vec * dirs[ndx]).theB); // non-unitary
		Vector const tmpPerp((vec * plane).theV);

		// transform perpendicular to unit direction
		perp = unit(tmpPerp);
	}

	return perp;
}


//======================================================================
}

