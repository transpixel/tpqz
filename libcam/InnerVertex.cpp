//
//
// MIT License
//
// Copyright (c) 2020 Stellacore Corporation.
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
\brief Definitions for cam::InnerVertex
*/


#include "libcam/InnerVertex.h"

#include "libdat/ops.h"


namespace cam
{

// explicit
InnerVertex :: InnerVertex
	( dat::Extents const & detSize
	)
	: theDetSize{ detSize }
	, theCenterRC{ dat::centerOf(theDetSize) }
	, theCenterVec{ theCenterRC[0], theCenterRC[1], 0. }
{ }

geo::Wedge
InnerVertex :: wedgeFor
	( double const & pd
	, std::pair<dat::Spot, dat::Spot> const & meaPair
	) const
{
	dat::Spot const & meaRowCol1 = meaPair.first;
	dat::Spot const & meaRowCol2 = meaPair.second;
	ga::Vector const vecMea1{ meaRowCol1[0], meaRowCol1[1], 0. };
	ga::Vector const vecMea2{ meaRowCol2[0], meaRowCol2[1], 0. };
	ga::Vector const vecPD{ theCenterVec + pd * ga::e3 };
	return geo::Wedge(vecPD, std::make_pair(vecMea1, vecMea2));
}

} // cam

