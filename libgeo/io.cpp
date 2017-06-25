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
\brief Definitions for geo::io
*/


#include "libgeo/io.h"

#include "libio/stream.h"

#include <cstdio>
#include <algorithm>


namespace geo
{
namespace io
{

std::vector<ga::Vector>
loadFromCSV
	( std::istream & istrm
	, size_t const & expectSize
	)
{
	std::vector<ga::Vector> pnts;
	if (dat::isValid(expectSize))
	{
		pnts.reserve(expectSize);
	}

	std::string line;
	ga::Vector pnt{};
	while (istrm.good() && (! istrm.eof()))
	{
		std::getline(istrm, line);
		if (! line.empty())
		{
			constexpr char fmt[] = { "%lf,%lf,%lf" };
			int const numGot
				{ sscanf(line.c_str(), fmt, &(pnt[0]), &(pnt[1]), &(pnt[2])) };
			if (3u == numGot)
			{
				pnts.emplace_back(pnt);
			}
		}
	}

	return pnts;
}

bool
saveToCSV
	( std::ostream & ostrm
	, std::vector<ga::Vector> const & pnts
	)
{
	bool okay{ false };
	constexpr char fmt[] = {"%.15e,%.15e,%.15e\n"}; // -7.123456789012345e-123
	constexpr size_t fieldSize{ 3u + 15u + 5u };
	constexpr size_t bufSize{ 3u*fieldSize + 1u + 1u};
	char buffer[bufSize];
	for (ga::Vector const & pnt : pnts)
	{
		snprintf(buffer, bufSize, fmt, pnt[0], pnt[1], pnt[2]);
		ostrm << buffer;
	}
	ostrm << std::flush;
	okay = ! ostrm.fail();
	return okay;
}

}
}

