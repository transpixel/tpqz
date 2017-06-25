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
\brief Inline definitions for dat::SpotX
*/


#include "libdat/SpotX.h"

#include "libdat/info.h"
#include "libdat/validity.h"


namespace dat
{

// static
SpotX
SpotX :: identity
	()
{
	static SpotX const ident(dat::Spot{{ 0., 0. }});
	return ident;
}

// explicit
SpotX :: SpotX
	( dat::Spot const & transSpot
	)
	: theTrans(transSpot)
{
}

// explicit
SpotX :: SpotX
	( dat::RowCol const & transRC
	)
	: theTrans(cast::Spot(transRC))
{
}

SpotX
SpotX :: compositeAfter
	( SpotX const & prior
	) const
{
	SpotX net;
	if (isValid() && prior.isValid())
	{
		using dat::operator+;
		Spot const netTrans(theTrans + prior.theTrans);
		net = SpotX{ netTrans };
	}
	return net;
}

SpotX
SpotX :: inverse
	() const
{
	SpotX net;
	if (isValid())
	{
		net = SpotX{ -theTrans };
	}
	return net;
}

bool
SpotX :: nearlyEquals
	( SpotX const & other
	, double const & tol
	) const
{
	// linear in translation
	return dat::nearlyEquals(theTrans, other.theTrans, tol);
}

std::string
SpotX :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << " ";
	}
	if (isValid())
	{
		oss << "theTrans:"
			<< " " << dat::infoString(theTrans)
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

}

