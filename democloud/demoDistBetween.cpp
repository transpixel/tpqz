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
\brief  This file contains main application program demoDistBetween
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
	using KeyPair = std::pair<Key, Key>;

	std::map<Key, ga::Vector>
	keyedPoints
		( std::ifstream & ifs
		)
	{
		std::map<Key, ga::Vector> kpnts;
		std::string line;
		while (ifs && (! ifs.eof()))
		{
			std::getline(ifs, line);
			if (! line.empty())
			{
				std::istringstream iss(line);
				Key key{};
				double xx{ dat::nullValue<double>() };
				double yy{ dat::nullValue<double>() };
				double zz{ dat::nullValue<double>() };
				iss >> key >> xx >> yy >> zz;
				bool okay { (! iss.fail()) };
				if ( okay
				  && dat::isValid(key)
				  && dat::isValid(xx)
				  && dat::isValid(yy)
				  && dat::isValid(zz)
				   )
				{
					ga::Vector const pnt{ xx, yy, zz };
					kpnts.emplace_hint
						( std::end(kpnts)
						, std::make_pair(key, pnt)
						);
				}
			}
		}
		return kpnts;
	}

	std::map<KeyPair, double>
	keyedDistPairs
		( std::ifstream & ifs
		)
	{
		std::map<KeyPair, double> kdists;
		std::map<Key, ga::Vector> const pnts{ keyedPoints(ifs) };
		using Iter = typename std::map<Key, ga::Vector>::const_iterator;
		for (Iter it1{pnts.cbegin()} ; pnts.cend() != it1 ; ++it1)
		{
			Key const & key1 = it1->first;
			ga::Vector const & pnt1 = it1->second;
			Iter it2{ it1 };
			++it2;
			for ( ; pnts.cend() != it2 ; ++it2)
			{
				Key const & key2 = it2->first;
				ga::Vector const & pnt2 = it2->second;
				double const dist{ ga::magnitude(pnt2 - pnt1) };
				kdists.emplace_hint
					( std::end(kdists)
					, std::make_pair(std::make_pair(key1, key2), dist)
					);
			}
		}
		return kdists;
	}
}

//! Compute combinations of distances between 3D points <ID xx yy zz>
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
		( "Compute pairwise combinations of distances between points in list"
		  "\nOutput results to standard out stream"
		);
	usage.addArg("inPntFile", "Ascii points <id xx yy zz> format");
//	usage.addArg("outDistCombos", "Ascii output <id1 id2 dist12>");
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
//	std::string const outpath(argv[++argnum]);

	std::ifstream ifs(inpath);
	std::map<KeyPair, double> const dists{ keyedDistPairs(ifs) };

	for (std::map<KeyPair, double>::value_type const & pair : dists)
	{
		io::out()
			<< " " << dat::infoString(pair.first.first)
			<< " " << dat::infoString(pair.first.second)
			<< " " << dat::infoString(pair.second)
			<< std::endl;
	}

	return 0;
}
