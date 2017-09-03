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
\brief Inline definitions for tri::FaceVerts
*/


namespace tri
{

template <typename PropSampFunc>
inline
typename PropSampFunc::value_type
FaceVerts :: valueFrom
	( PropSampFunc const & propGrid
	) const
{
	long const & i1 = theVerts[0].theI;
	long const & i2 = theVerts[1].theI;
	long const & i3 = theVerts[2].theI;

	long const & j1 = theVerts[0].theJ;
	long const & j2 = theVerts[1].theJ;
	long const & j3 = theVerts[2].theJ;

	double const & w1 = theVerts[0].theW;
	double const & w2 = theVerts[1].theW;
	double const & w3 = theVerts[2].theW;

	return typename PropSampFunc::value_type
		( w1 * propGrid(i1, j1)
		+ w2 * propGrid(i2, j2)
		+ w3 * propGrid(i3, j3)
		);
}

} // tri
