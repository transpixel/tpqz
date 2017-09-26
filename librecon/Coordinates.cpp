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
\brief Definitions for recon::Coordinates
*/


#include "librecon/Coordinates.h"

#include "libprob/median.h"

#include <sstream>


namespace
{
	inline
	double
	medianFor
		( std::vector<double> const & comps
		)
	{
		return prob::median::valueFromConst(comps.begin(), comps.end());
	}
}


namespace recon
{

// explicit
Coordinates :: Coordinates
	( size_t const & estSize
	)
	: theComps{{}}
{
	theComps[0].reserve(estSize);
	theComps[1].reserve(estSize);
	theComps[2].reserve(estSize);
}

void
Coordinates :: addPoint
	( ga::Vector const & pnt
	)
{
	if (pnt.isValid())
	{
		theComps[0].emplace_back(pnt[0]);
		theComps[1].emplace_back(pnt[1]);
		theComps[2].emplace_back(pnt[2]);
	}
}

ga::Vector
Coordinates :: pointAtMedians
	() const
{
	ga::Vector pnt{};
	double const xMed(medianFor(theComps[0]));
	double const yMed(medianFor(theComps[1]));
	double const zMed(medianFor(theComps[2]));
	if (dat::isValid(xMed) && dat::isValid(yMed) && dat::isValid(zMed))
	{
		pnt = ga::Vector(xMed, yMed, zMed);
	}
	return pnt;
}


} // recon

