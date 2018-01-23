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

#ifndef ro_Solution_INCL_
#define ro_Solution_INCL_

/*! \file
\brief Declarations for ro::Solution
*/

#include "libdat/array.h"
#include "libdat/validity.h"
#include "libmath/math.h"
#include "libro/ro.h"

#include <string>
#include <vector>

namespace ro
{

/*! \brief Container for results associated with a RO solution

\par Example
\dontinclude testro/uSolution.cpp
\skip ExampleStart
\until ExampleEnd
*/

class Solution
{

public: // data

	ro::OriPair theOriPair{};
	std::array<size_t, 5u> theNdxQuint{ dat::nullValue<size_t, 5u>() };
	std::vector<double> theGaps{};
	size_t theItCount{ dat::nullValue<size_t>() };
	size_t theCondNum{ dat::nullValue<size_t>() };

public: // methods

	//! default null constructor
	Solution
		() = default;

	//! Value ctor
	explicit
	Solution
		( ro::OriPair const & oriPair
		, std::array<size_t, 5u> const & ndxQuint
		, std::vector<double> const & gaps
		, size_t const & itCount
		, size_t const & condNum
		)
		: theOriPair{ oriPair }
		, theNdxQuint{ ndxQuint }
		, theGaps{ gaps }
		, theItCount{ itCount }
		, theCondNum{ condNum }
	{ }

	//! True if instance is valid
	bool
	isValid
		() const
	{
		// TODO
		return false;
	}

	//! Gap associated with uvPair[ndx]
	double
	gapForNdx
		( size_t const & ndx
		) const
	{
		double gap{ dat::nullValue<double>() };
		if (ndx < theGaps.size())
		{
			gap = theGaps[ndx];
		}
		return gap;
	}

	//! Expected gap value (root-mean-square)
	double
	rmsGap
		() const
	{
		double rms{ dat::nullValue<double>() };
		double const dof{ double(theGaps.size()) - 1. };
		if (0. < dof)
		{
			double const sumSq
				{ math::sumSqs<double>(theGaps.begin(), theGaps.end()) };
			rms = std::sqrt(sumSq / dof);
		}
		return rms;
	}

	//! Descriptive information about this instance.
	std::string
	infoString
		( std::string const & title = std::string()
//		, std::string const & fmt = std::string("%12.6f")
		) const
	{
		std::ostringstream oss;
		if (! title.empty())
		{
			oss << title << std::endl;
		}
		if (isValid())
		{
		}
		else
		{
			oss << " <null>";
		}
		return oss.str();
	}

}; // Solution

} // ro

// Inline definitions
// #include "libro/Solution.inl"

#endif // ro_Solution_INCL_

