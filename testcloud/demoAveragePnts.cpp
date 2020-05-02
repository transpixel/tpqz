//
//
// MIT License
//
// Copyright (c) 2020 Stellacore Corporation.
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
\brief  This file contains main application program demoAveragePnts
*/


#include "libapp/Usage.h"

#include "build/version.h"
#include "libio/stream.h"

#include "libga/ga.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>


namespace
{
	using Key = std::string;

	struct PntCnt
	{
		ga::Vector theSum{ ga::vZero };
		double theCount{ 0. };

		void
		addPoint
			( ga::Vector const & pnt
			)
		{
			theSum = theSum + pnt;
			theCount += 1.;
		}

		ga::Vector
		average
			() const
		{
			ga::Vector ave{};
			if (0. < theCount)
			{
				ave = (1. / theCount) * theSum;
			}
			return ave;
		}

	}; // PntCnt;

	struct Accumulator
	{
		std::map<Key, PntCnt> theKPCs{};

		void
		addPoint
			( Key const & key
			, ga::Vector const & pnt
			)
		{
			PntCnt & pntcount = theKPCs[key];
			pntcount.addPoint(pnt);
		}

		std::map<Key, ga::Vector>
		averagePoints
			() const
		{
			std::map<Key, ga::Vector> aves;
			for (std::map<Key, PntCnt>::value_type const & pair : theKPCs)
			{
				aves[pair.first] = pair.second.average();
			}
			return aves;
		}
	};

	std::map<Key, ga::Vector>
	mapFromAscii
		( std::istream & ifs
		)
	{
		Accumulator accum{};
		std::string line;
		Key key{};
		while (ifs && (! ifs.eof()))
		{
			std::getline(ifs, line);
			if (! line.empty())
			{
				std::istringstream iss(line);
				double xx{ dat::nullValue<double>()};
				double yy{ dat::nullValue<double>()};
				double zz{ dat::nullValue<double>()};
				iss >> key >> xx >> yy >> zz;
				bool const okay{ (! iss.fail()) };
				if ( okay
				  && dat::isValid(key)
				  && dat::isValid(xx)
				  && dat::isValid(yy)
				  && dat::isValid(zz)
					)
				{
					accum.addPoint(key, ga::Vector{ xx, yy, zz });
				}
			}
		}
		return accum.averagePoints();
	}
}

//! Average coordinates of points in a text file that have same ID
int
main
	( int const argc
	, char const * const * const argv
	)
{
	// Don't mix stdio & streams (for performance)
	//std::ios::sync_with_stdio(false);

	// check args
	app::Usage usage;
	usage.setSummary
		( "Read ascii point files and compute averages of points with same ID"
		  "\nOutput results to standard out stream"
		);
	usage.addArg("inPntFile", "Ascii points <id xx yy zz> format");
	// ...
	if (usage.argStatus(argc, argv) != app::Usage::Valid)
	{
		std::string const fname(argv[0]);
		io::err()
			<< std::endl << build::version::buildInfo(argv[0]) << std::endl
			<< usage.infoString(fname) << std::endl;
		return 1;
	}

	// parse input argument
	int argnum(0);
	std::string const inpath(argv[++argnum]);

	// Accumulate coordiantes for same keys while loading data
	std::ifstream ifs(inpath);
	std::map<Key, ga::Vector> const pnts{ mapFromAscii(ifs) };

	// report averages to stdout
	for (std::map<Key, ga::Vector>::value_type const & pair : pnts)
	{
		io::out() << dat::infoString(pair.second, pair.first) << std::endl;
	}


	return 0;
}
