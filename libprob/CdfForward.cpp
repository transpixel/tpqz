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
\brief Definitions for prob::CdfForward
*/


#include "libprob/CdfForward.h"

#include "libdat/compare.h"
#include "libdat/info.h"

#include <sstream>


namespace prob
{
//======================================================================

// static
CdfForward
CdfForward :: uniform
	( dat::Range<double> const & dataRange
	)
{
	std::vector<double> const linearCdf{ 0., 1. };
	return CdfForward(linearCdf.begin(), linearCdf.end(), dataRange);
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

bool
CdfForward :: isValid
	() const
{
	return
		(  (! theFracValues.empty())
		&& theDataPart.isValid()
		);
}

std::vector<std::pair<double, double> >
CdfForward :: dataProbs
	() const
{
	std::vector<std::pair<double, double> > dps;
	size_t const numElem(theFracValues.size());
	dps.reserve(numElem);
	for (size_t nn(0u) ; nn < numElem ; ++nn)
	{
		double const dataValue
			{ theDataPart.interpValueFor(static_cast<double>(nn)) };
		double const cdfValue(theFracValues[nn]);
		dps.push_back(std::make_pair(dataValue, cdfValue));
	}
	return dps;
}

bool
CdfForward :: nearlyEquals
	( CdfForward const & other
	, double const & tol
	) const
{
	bool okay(isValid() && other.isValid());
	if (okay)
	{
		okay = dat::nearlyEquals<double>
			( theFracValues.begin(), theFracValues.end()
			, other.theFracValues.begin(), other.theFracValues.end()
			, tol
			);
	}
	return okay;
}

std::string
CdfForward :: infoString
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
		oss << dat::infoString(theDataPart, "theDataPart:");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

std::string
CdfForward :: infoStringContents
	( std::string const & title
	) const
{
	std::ostringstream oss;
	oss << infoString(title);
	if (isValid())
	{
		oss << std::endl;
		oss << dat::infoString
			(theFracValues.begin(), theFracValues.end(), "theFracValues");
	}
	return oss.str();
}

bool
CdfForward :: writeDataProbs
	( std::ostream & oss
	, std::string const & title
	) const
{
	if (! title.empty())
	{
		oss << "# " << title << '\n';
	}

	std::vector<std::pair<double, double> > const dps(dataProbs());
	for (std::pair<double, double> const & dp : dps)
	{
		double const & dataValue = dp.first;
		double const & cdfValue = dp.second;
		oss
			<< " " << dat::infoString(dataValue, "", 14u, 6u)
			<< " " << dat::infoString(cdfValue, "", 9u, 6u)
			<< '\n';
	}

	return (! oss.fail());
}

//======================================================================
}

