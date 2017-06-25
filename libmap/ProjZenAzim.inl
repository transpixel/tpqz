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
\brief Inline definitions for map::ProjZenAzim
*/



namespace map
{

inline
// virtual
dat::RowCol
ProjZenAzim :: gridRowColFor
	( ga::Vector const & dir
	) const
{
	double const azimuth{ geo::sphere::azimuthOf(dir) };
	double const zenith{ geo::sphere::zenithOf(dir) };
	return dat::RowCol
		{ thePartZ.binIndexFor(zenith)
		, thePartA.binIndexFor(azimuth)
		};
}

inline
// virtual
ga::Vector
ProjZenAzim :: directionFor
	( dat::RowCol const & mapRC
	) const
{
	double const zenith{ thePartZ.interpValueFor(mapRC[0]) };
	double const azimuth{ thePartA.interpValueFor(mapRC[1]) };
	return geo::sphere::directionFromAZ(azimuth, zenith);
}

}

