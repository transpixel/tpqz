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
\brief Definitions for prob::Frac9
*/


#include "libprob/Frac9.h"

#include "libdat/dat.h"
#include "libio/sprintf.h"
#include "libprob/prob.h"

#include <sstream>


namespace
{
	//! Array of invalid values
	inline
	std::array<double, 9u>
	badArray
		()
	{
		std::array<double, 9u> values;
		std::fill(values.begin(), values.end(), dat::nullValue<double>());
		return values;
	}

	//! Populate array with fractile values
	std::array<double, 9u>
	fracValuesFor
		( prob::CdfInverse const & cdfInv
		)
	{
		std::array<double, 9u> values(badArray());

		// use inverse interpolator to find frac values
		if (cdfInv.isValid())
		{
			std::array<double, 9u> const fracs(prob::Frac9::fractiles());
			for (size_t ndx(0u) ; ndx < fracs.size() ; ++ndx)
			{
				values[ndx] = cdfInv(fracs[ndx]);
			}
		}

		return values;
	}

	//! Populate array with fractile values
	std::array<double, 9u>
	fracValuesFor
		( prob::CdfForward const & cdfFwd
		)
	{
		std::array<double, 9u> values(badArray());

		if (cdfFwd.isValid())
		{
			constexpr size_t lutSize{ 4u * 1024u };
			prob::CdfInverse const cdfInv
				(prob::CdfInverse::fromCdfForward(cdfFwd, lutSize));

			// use inverse interpolator to find frac values
			values = fracValuesFor(cdfInv);
		}

		return values;
	}

}

namespace prob
{
//======================================================================


Frac9 :: Frac9
	()
	: theValues(badArray())
{
}

// explicit
Frac9 :: Frac9
	( CdfForward const & cdfFwd
	)
	: theValues(fracValuesFor(cdfFwd))
{
}

// explicit
Frac9 :: Frac9
	( CdfInverse const & cdfInv
	)
	: theValues(fracValuesFor(cdfInv))
{
}

// explicit
Frac9 :: Frac9
	( std::array<double, 9u> const & values
	)
	: theValues(values)
{
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

std::string
Frac9 :: infoString
	( std::string const & title
	, std::string const & fmt
	, std::string const & sep
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << '\n';
	}

	if (isValid())
	{
		oss 
			<< std::fixed
			<< "  " << io::sprintf(fmt, theValues[0])
			<< "  " << io::sprintf(fmt, theValues[1])
			<< "  " << io::sprintf(fmt, theValues[2])
			<< "  " << io::sprintf(fmt, theValues[3])
			<< sep
			<< "  " << io::sprintf(fmt, theValues[4])
			<< sep
			<< "  " << io::sprintf(fmt, theValues[5])
			<< "  " << io::sprintf(fmt, theValues[6])
			<< "  " << io::sprintf(fmt, theValues[7])
			<< "  " << io::sprintf(fmt, theValues[8])
			;
	}
	else
	{
		oss << " <null>";
	}

	return oss.str();
}

std::string
Frac9 :: infoStringOneLine
	( std::string const & title
	, std::string const & fmt
	) const
{
	return infoString(title, fmt, "  "); // add some space around 50%ile
}

//======================================================================
}
