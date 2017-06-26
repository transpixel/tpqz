
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
\brief  This file contains unit test for polar interpolation operations
*/


#include "libmath/CircleTab.h"
#include "libmath/CosTable.h"

#include "libio/stream.h"
#include "libga/ga.h"

#include <complex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
math_Polar_test0
	()
{
	std::ostringstream oss;
	/*
	math::Polar const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	*/
	return oss.str();
}


//! Check combination of polar and azimuth interpolation
std::string
math_Polar_test1
	()
{
	std::ostringstream oss;

	using ZALoc = std::pair<double, double>;
	using DataValue = std::complex<double>;
	using AVPair = std::pair<double, DataValue>;

	// generate unique test values
	std::function<DataValue(ZALoc const &)> const dataValueAt
		( []
			( ZALoc const & za )
			{ return DataValue{ std::cos(za.first), std::sin(za.second) }; }
		);

	// assign test values to azimuth
	std::function<AVPair(ZALoc const &)> const avAt
		( [& dataValueAt]
			( ZALoc const & za )
			{ return AVPair{ za.second, dataValueAt(za) }; }
		);

	// generate unique values for this zenith angle
	constexpr size_t numAzim{ 64u };
	std::function<std::vector<AVPair>(double const &)> const avPairsAt
		( [& avAt]
			( double const & zenith )
			{
				std::vector<AVPair> avPairs;
				for (size_t nn(0u) ; nn < numAzim ; ++nn)
				{
					double const azim(math::twoPi * double(nn)/double(numAzim));
					avPairs.push_back(avAt({ zenith, azim }));
				}
				return avPairs;
			}
		);

	// configure test zenith zones
	double const da(math::pi / 8.);
	std::vector<double> const zeniths{ 1.*da, 3.*da, 5.*da, 7.*da };

	// configure test azimuth values for each zenith
	math::CircleTab<DataValue> const nullAzims;
	math::CosTable<math::CircleTab<DataValue> > zatab
		(zeniths.size(), nullAzims);
	for (double const & zenith : zeniths)
	{
		using AVPair = std::pair<double, DataValue>;

		std::vector<AVPair> const avPairs(avPairsAt(zenith));
		size_t const numAzim(avPairs.size());
		math::CircleTab<DataValue> const azimTab(avPairs, numAzim);
		zatab.assignValueAt(azimTab, std::cos(zenith));
	}

	// find value associated with a direction
	for (double const & zenith : zeniths)
	{
		double const cz(std::cos(zenith));
		constexpr double dazim{ math::twoPi / double(numAzim) };
		for (double azim(-math::pi) ; azim <= math::pi ; azim += dazim)
		{
			size_t const gotNdx(zatab.indexForCosine(cz));
			math::CircleTab<DataValue> const & gotAzimTab = zatab[gotNdx];
			DataValue const expValue(dataValueAt({ zenith, azim }));
			DataValue const gotValue(gotAzimTab.nearestTableValue(azim));
			if (! dat::nearlyEquals(gotValue, expValue))
			{
				oss << "Failure of exact z/a lookup test" << std::endl;
				goto AbortTest;
			}
		}
	}
AbortTest:

	return oss.str();
}


}

//! Unit test for math::Polar
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << math_Polar_test0();
	oss << math_Polar_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
