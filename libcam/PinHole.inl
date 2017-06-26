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
\brief Inline definitions for cam::PinHole
*/



namespace cam
{
//======================================================================

inline
bool
PinHole :: isValid
	() const
{
	return dat::isValid(thePD);
}

inline
ga::Vector
PinHole :: directionTo
	( ga::Vector const & objpnt
	) const
{
	// NOTE: pretty much just a wrapper to enforce consistent func name
	return ga::unit(objpnt);
}

inline
dat::Spot
PinHole :: imageSpotFor
	( ga::Vector const & objpnt
	) const
{
	dat::Spot imgloc(dat::nullValue<dat::Spot>());
	if (isValid())
	{
		// projection only points that are "below the z-plane"
		if (objpnt[2] < 0.)
		{
			double const & zz = objpnt[2];
			double const scale(-thePD / zz);
			imgloc =
				{{ scale*objpnt[0]
				 , scale*objpnt[1]
				}};
		}
	}
	return imgloc;
}

inline
ga::Vector
PinHole :: directionOf
	( dat::Spot const & imgspot
	) const
{
	return ga::unit(ga::Vector(imgspot[0], imgspot[1], -thePD));
}

//======================================================================
}

