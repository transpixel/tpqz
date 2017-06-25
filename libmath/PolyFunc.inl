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
\brief Definitions for math::PolyFunc
*/


#include <vector>

namespace math
{
//======================================================================

inline
bool
// static
PolyFunc :: xIsLess
	( dat::Spot const & vA
	, dat::Spot const & vB
	)
{
	return (vA[0] < vB[0]);
}

template <typename FwdIter>
inline
// explicit
PolyFunc :: PolyFunc
	( FwdIter const & vertsBeg
	, FwdIter const & vertsEnd
	)
	: theRangeX()
	, theVerts()
{
	std::vector<dat::Spot> & verts = theVerts;
	verts.insert(verts.end(), vertsBeg, vertsEnd);
	if (! verts.empty())
	{
		sort(verts.begin(), verts.end(), xIsLess);

		// set ranges using first and last vertices (which may be same)
		theRangeX = dat::Range<double>(verts.front()[0], verts.back()[0]);
	}

	// fill cached values
	static std::function<double(dat::Spot const &)> const getX
		([] (dat::Spot const & vert) { return vert[0]; });
	static std::function<double(dat::Spot const &)> const getY
		([] (dat::Spot const & vert) { return vert[1]; });
	theValuesX.resize(theVerts.size());
	theValuesY.resize(theVerts.size());
	std::transform
		(theVerts.begin(), theVerts.end(), theValuesX.begin(), getX);
	std::transform
		(theVerts.begin(), theVerts.end(), theValuesY.begin(), getY);

}

//======================================================================
}

