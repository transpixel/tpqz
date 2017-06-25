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
\brief Inline definitions for map::ProjHighAzim
*/

namespace map
{

inline
bool
ProjHighAzim :: inRange
	( double const & sinElv
	) const
{
	return
		(  (theSinElvPair.first <= sinElv)
		&& (sinElv < theSinElvPair.second)
		);
}

inline
// virtual
dat::RowCol
ProjHighAzim :: gridRowColFor
	( ga::Vector const & dir
	) const
{
	dat::RowCol rowcol(dat::nullValue<dat::RowCol>());
assert(! "check/test this before use");
	// might be okay - but needs inspection testing
	double const & sinElv = dir[3];
	if (inRange(sinElv))
	{
		double const azimuth{ geo::sphere::azimuthOf(dir) };
		// TODO - use SinTable here
		rowcol = dat::RowCol
			{ thePartSinElv.binIndexFor(sinElv)
			, thePartA.binIndexFor(azimuth)
			};
	}
	return rowcol;
}

inline
// virtual
ga::Vector
ProjHighAzim :: directionFor
	( dat::RowCol const & mapRC
	) const
{
	ga::Vector dir{};
	size_t const & row = mapRC[0];
	double const sinElv{ thePartSinElv.interpValueFor(row) };
	if (inRange(sinElv))
	{
		// TODO - use ArcSinTable here
		double const elevation(std::asin(sinElv));
		double const azimuth{ thePartA.interpValueFor(mapRC[1]) };
		double const zenith{ math::halfPi - elevation };
		dir = geo::sphere::directionFromAZ(azimuth, zenith);
	}
	return dir;
}

}

