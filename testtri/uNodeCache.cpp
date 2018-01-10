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
\brief  This file contains unit test for tri::NodeCache
*/


#include "libtri/NodeCache.h"

#include "libtri/IsoTille.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <cassert>
#include <iostream>
#include <map>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
tri_NodeCache_test0
	()
{
	std::ostringstream oss;
	tri::NodeCache const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check simple cache
std::string
tri_NodeCache_test1
	()
{
	std::ostringstream oss;

	// create a generic tritille
	dat::Range<double> const xRange{  -7., -3. };
	dat::Range<double> const yRange{ -15., 37. };
	double const xDelta{ 2. };
	double const yDelta{ 7. };
	tri::IsoTille const trinet
		{ tri::IsoTille::genericTille(xRange, yRange, xDelta, yDelta) };

	// create cache
	tri::NodeCache cache(trinet);

	using NodeValue = float;
	std::map<tri::NodeKey, NodeValue> valueMap;

	// fill storage
	size_t const numNodes{ cache.size() };
	NodeValue value{ 0.f };
	for (size_t nn{0u} ; nn < numNodes ; ++nn)
	{
		for (tri::NodeIterator iter(trinet.begin()) ; iter ; ++iter)
		{
			tri::NodeKey const keyIJ{ iter.nodeKey() };
			cache.setValue(keyIJ, value);
			valueMap[keyIJ] = value;
			value += 1.f;
		}
	}

	// retrieve
	size_t errCount{ 0u };
	for (size_t nn{0u} ; nn < numNodes ; ++nn)
	{
		for (tri::NodeIterator iter(trinet.begin()) ; iter ; ++iter)
		{
			tri::NodeKey const keyIJ{ iter.nodeKey() };
			NodeValue const & gotValue = cache.valueAt(keyIJ);
			std::map<tri::NodeKey, NodeValue>::const_iterator
				const itFind{ valueMap.find(keyIJ) };
			assert(valueMap.end() != itFind);
			NodeValue const expValue{ itFind->second };
			if (! (gotValue == expValue))
			{
				++errCount;
			}
		}
	}

	io::out() << dat::infoString(trinet, "trinet") << std::endl;
	io::out() << dat::infoString(errCount, "errCount") << std::endl;

oss << "Failure: implement this test!" << std::endl;
	return oss.str();
}


}

//! Unit test for tri::NodeCache
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << tri_NodeCache_test0();
	oss << tri_NodeCache_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
