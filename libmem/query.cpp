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
\brief Definitions for mem::query
*/


#include "libmem/query.h"

#include <fstream>
#include <iomanip>
#include <map>


namespace mem
{
namespace query
{
//======================================================================


namespace
{
	//! Extract first string up to white space
	std::pair<std::string, std::string>
	kvFrom
		( std::string const & usekey
		, std::string const & line
		)
	{
		std::pair<std::string, std::string> kv;
		std::string & key = kv.first;
		std::string & value = kv.second;
		size_t const pos{ line.find(' ') };
		key = usekey;
		std::copy
			( line.begin()+pos, line.end()
			, std::inserter(value, value.begin())
			);
		return kv;
	}
}

std::string
infoString
	( std::string const & title
	, std::vector<std::string> const & findKeys
	, std::string const & sep
	)
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << std::setw(25u) << title << sep;
	}

	std::map<std::string, std::string> kvs;

	// gather information from system (linux only)
	std::ifstream ifs("/proc/self/status", std::ios::in);
	if (ifs)
	{
		std::string line;
		while (ifs && (! ifs.eof()))
		{
			std::getline(ifs, line);
			for (std::string const & findKey : findKeys)
			{
				if (line.find(findKey) < line.size())
				{
					kvs.insert(kvFrom(findKey, line));
				}
			}
		}
	}

	// report found info in order of key request
	for (size_t nn(0u) ; nn < findKeys.size() ; ++nn)
	{
		std::map<std::string, std::string>::const_iterator
			const itFind(kvs.find(findKeys[nn]));
		if (kvs.end() != itFind)
		{
			if (0u < nn)
			{
				oss << sep;
			}
			std::string const & key = itFind->first;
			std::string const & value = itFind->second;
			oss
				<< std::setw(10u) << key
				<< std::setw(15u) << value
				;
		}
	}

	return oss.str();
}

// explicit
Reporter :: Reporter
	( std::vector<std::string> const & findKeys
	, std::string const & sep
	)
	: theOss{}
	, theKeys(findKeys)
	, theSep(sep)
{
}

void
Reporter :: operator()
	( std::string const & title
	)
{
	theOss << mem::query::infoString(title, theKeys) << std::endl;
}

std::string
Reporter :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	oss << theOss.str();
	return oss.str();
}

//======================================================================
}
}


