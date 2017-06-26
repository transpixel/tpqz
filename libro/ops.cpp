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
\brief Definition for ro::ops
*/


#include "libro/ops.h"


namespace ro
{

std::pair<ga::Rigid, ga::Rigid>
sqrtOriPairFor
	( std::pair<ga::Rigid, ga::Rigid> const & roPairInRef
	, ga::Rigid * const & ptOriFitWrtRef
	)
{
	// default to null values
	std::pair<ga::Rigid, ga::Rigid> roPairInMean{};
	if (ptOriFitWrtRef)
	{
		*ptOriFitWrtRef = ga::Rigid{};
	}

	// access sort data
	ga::Rigid const & ori1wRef = roPairInRef.first;
	ga::Rigid const & ori2wRef = roPairInRef.second;
	if (dat::isValid(roPairInRef))
	{
		// shorthand access
		ga::Spinor const & epI = ori1wRef.pose().theSpin;
		ga::Spinor const & epJ = ori2wRef.pose().theSpin;
		ga::Vector const & uu = ori1wRef.location();
		ga::Vector const & vv = ori2wRef.location();
		assert(0. <= epI.theS.theValue);
		assert(0. <= epJ.theS.theValue);

		// compute sqrt attitude
		ga::Spinor const enI{ epI.reverse() };
		ga::Spinor const e2K{ epJ * enI };
		ga::Spinor const epK{ ga::sqrt(e2K) };

		// compute sqrt offset
		double const & alpha = epK.theS.theValue; // cos|A|
		ga::Vector const bb{ (epI * (vv - uu) * enI).theV };
		ga::Vector mm{};
		if (alpha < math::eps)
		{
			// offset for special case for half-turn physical relative rotation
			mm = .5 * bb;
		}
		else
		{
			// offset for general relative rotation case
			ga::BiVector const & AA = epK.theB; // sin|A|
			ga::Vector const A_b{ .5 * (AA*bb - bb*AA).theV };
			mm = .5 * (bb + (1./alpha)*A_b);
		}

		// assemble results into rigid orientation form
		ga::Rigid const oriFw1(mm, ga::Pose(epK));
		ga::Rigid const ori2wF(mm, ga::Pose(epK));
		ga::Rigid const ori1wF{ oriFw1.inverse() };
		roPairInMean = { ori1wF, ori2wF };

		// compute intermediate frame
		if (ptOriFitWrtRef)
		{
			ga::Rigid const oriFwRef{ rigidModelWrtRef(ori1wF, ori1wRef) };
			constexpr bool verify{ true }; // e.g. for dev work
			if (verify)
			{
				ga::Rigid const chkFwRef{ rigidModelWrtRef(ori2wF, ori2wRef) };
				assert(chkFwRef.nearlyEquals(oriFwRef));
			}
			*ptOriFitWrtRef = oriFwRef;
		}
	}

	return roPairInMean;
}


} // ro

