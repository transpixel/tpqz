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
\brief Definitions for dat::KeyedValues
*/


#include "libdat/KeyedValues.h"

#include "libdat/dat.h"
#include "libio/string.h"

#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>


namespace dat
{
//======================================================================

// static
KeyedValues
KeyedValues :: fromFile
	( std::string const & fpath
	)
{
	KeyedValues kvs;

	// load file
	std::ifstream ifs(fpath);
	if (ifs)
	{
		// process each record in turn
		std::string line;
		std::string data;
		while (ifs.good() && (! ifs.eof()))
		{
			// grab line and remove comments
			std::getline(ifs, line);
			data = io::string::before(line, "#");
			// need at least 3 elements for valid key/delim/value
			if (2u < data.size())
			{
				// split into Key/Value
				std::istringstream iss(data);

				// get key from before the delimiter
				static char const delim(' ');
				size_t const ndx1(data.find(delim));
				assert(ndx1 != std::string::npos);
				assert(ndx1 < data.size());
				Key const key(data.substr(0, ndx1));

				// skip the space delimiter
				size_t const ndx2(ndx1 + 1u);

				// get value from after the delimiter
				size_t const endcount(data.size() - ndx2);
				Value const value(data.substr(ndx2, endcount));

				// check that both key and value are non-trivial
				if ((! key.empty()) && (! value.empty()))
				{
					// insert key value pair: if multiple keys, last one wins
					kvs.theKVs[key] = value;
				}
			}
		}
	}

	return kvs;
}

KeyedValues :: KeyedValues
	()
	: theKVs()
{
}

// copy constructor -- compiler provided
// assignment operator -- compiler provided
// destructor -- compiler provided

bool
KeyedValues :: isValid
	() const
{
	return (! theKVs.empty());
}

std::string
KeyedValues :: value
	( Key const & key
	) const
{
	std::string sval;
	std::map<Key, Value>::const_iterator const itFind(theKVs.find(key));
	if (theKVs.end() != itFind)
	{
		sval = itFind->second;
	}
	return sval;
}

double
KeyedValues :: dValue
	( Key const & key
	) const
{
	double dval(badDub);
	std::string const sval(value(key));
	if (dat::isValid(sval))
	{
		dval = io::string::from(sval, badDub);
	}
	return dval;
}

std::string
KeyedValues :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << " ";
	}
	oss << "theKVs.size: " << theKVs.size();
	return oss.str();
}

std::string
KeyedValues :: infoStringContents
	( std::string const & title
	) const
{
	std::ostringstream oss;
	oss << infoString(title);
	for (std::map<Key, Value>::value_type const & kv : theKVs)
	{
		oss << std::endl;
		oss
			<< "  " << std::setw(20) << kv.first
			<< " " << kv.second
			;
	}
	return oss.str();
}

//======================================================================
}

