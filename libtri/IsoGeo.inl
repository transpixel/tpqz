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
\brief Inline definitions for tri::IsoGeo
*/


namespace tri
{

inline
double
IsoGeo :: mu
	( dat::Spot const & xrel //!< location relative to tile origin
	) const
{
	return dat::dot(theBarU, xrel);
}

inline
double
IsoGeo :: nu
	( dat::Spot const & xrel //!< location relative to tile origin
	) const
{
	return dat::dot(theBarV, xrel);
}

inline
dat::Spot
IsoGeo :: mnLoc
	( dat::Spot const & xrel //!< location relative to tile origin
	) const
{
	return {{ mu(xrel), nu(xrel) }};
}

inline
dat::Spot
IsoGeo :: xyRelative
	( dat::Spot const & mnRel
	) const
{
	using dat::operator*;
	using dat::operator+;
	return (mnRel[0]*theDirU + mnRel[1]*theDirV);
}

inline
dat::QuantumFrac
IsoGeo :: muNdxFrac
	( dat::Spot const & xrel //!< location relative to tile origin
	) const
{
	return dat::QuantumFrac(mu(xrel), theSplitterMu);
}

inline
dat::QuantumFrac
IsoGeo :: nuNdxFrac
	( dat::Spot const & xrel //!< location relative to tile origin
	) const
{
	return dat::QuantumFrac(nu(xrel), theSplitterNu);
}

} // tri

