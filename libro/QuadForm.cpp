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
\brief Definitions for ro::QuadForm
*/


#include "libro/QuadForm.h"

#include "libdat/array.h"
#include "libro/PairRel.h"

#include <algorithm>
#include <sstream>


namespace ro
{

QuadForm :: QuadForm
	()
	: theMatF{{}}
{
	static double badVal{ dat::nullValue<double>() };
	std::fill(theMatF[0].begin(), theMatF[0].end(), badVal);
	std::fill(theMatF[1].begin(), theMatF[1].end(), badVal);
	std::fill(theMatF[2].begin(), theMatF[2].end(), badVal);
}

// explicit
QuadForm :: QuadForm
	( OriPair const & roPair
	)
	: theMatF{{}}
{
	static std::array<ga::Vector, 3u> const eDirs{{ ga::e1, ga::e2, ga::e3 }};
	PairRel const roRel(roPair);
	for (size_t ii{0u} ; ii < 3u ; ++ii)
	{
		for (size_t jj{0u} ; jj < 3u ; ++jj)
		{
			theMatF[ii][jj] = roRel.tripleProductGap(eDirs[ii], eDirs[jj]);
		}
	}
}

bool
QuadForm :: isValid
	() const
{
	return dat::isValid(theMatF[0][0]);
}

double
QuadForm :: tripleProductGap
	( ro::PairUV const & uvPair
	) const
{
	std::array<double, 3u> const & uu = uvPair.first.theValues;
	std::array<double, 3u> const & vv = uvPair.second.theValues;
	std::array<double, 3u> const fu
		{{ dat::dot(theMatF[0], vv)
		 , dat::dot(theMatF[1], vv)
		 , dat::dot(theMatF[2], vv)
		}};
	double const gap{ dat::dot(uu, fu) };
	return gap;
}

std::string
QuadForm :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	if (isValid())
	{
		oss << "theMatF:";
		oss << std::endl;
		oss << dat::infoString(theMatF[0]);
		oss << std::endl;
		oss << dat::infoString(theMatF[1]);
		oss << std::endl;
		oss << dat::infoString(theMatF[2]);
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

} // ro

