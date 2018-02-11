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
\brief Declarations for prob::Gauss
*/


#include "libprob/Gauss.h"

#include "libmath/math.h"

#include <limits>
#include <sstream>


namespace prob
{

// explicit
Gauss :: Gauss
	( double const & sigma
	, double const & mean
	)
	: theSigma{ sigma }
	, theMean{ mean }
	, theArgCo{ -1. / (2. * math::sq(theSigma)) }
	, theNormCo{ 1. / (std::sqrt(math::twoPi) * theSigma) }
{
}

bool
Gauss :: isValid
	() const
{
	return 
		(  dat::isValid(theSigma)
		&& (std::numeric_limits<double>::epsilon() < theSigma)
		&& dat::isValid(theArgCo)
		&& dat::isValid(theNormCo)
		);
}

double
Gauss :: operator()
	( double const value
	) const
{
	return theNormCo * std::exp(theArgCo * math::sq(value - theMean));
}

std::string
Gauss :: infoString
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
		oss
			<< "StdDev,Mean:"
			<< " " << dat::infoString(theSigma)
			<< " " << dat::infoString(theSigma)
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

} // prob

