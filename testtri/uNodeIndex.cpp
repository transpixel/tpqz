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
\brief  This file contains unit test for tri::NodeIndex
*/


#include "libtri/NodeIndex.h"

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
tri_NodeIndex_test0
	()
{
	std::ostringstream oss;
	tri::NodeIndex const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check simple cache
std::string
tri_NodeIndex_test1
	()
{
	std::ostringstream oss;

	// create a generic tritille
	dat::Range<double> const xRange{  -7.,  3. };
	dat::Range<double> const yRange{ -15., 37. };
	double const xDelta{ 1./17. };
	double const yDelta{ 1./19. };
	tri::IsoTille const trinet
		{ tri::IsoTille::genericTille(xRange, yRange, xDelta, yDelta) };

	// create cache
	tri::NodeIndex cache(trinet);
	size_t const numNodes{ cache.size() };
	if (! (0u < numNodes))
	{
		oss << "Failure of size test" << std::endl;
	}

	using NodeValue = float;

	// allocate data store
	std::vector<NodeValue> nodeItems(numNodes);
	NodeValue const nullItem{ dat::nullValue<NodeValue>() };
	std::fill(nodeItems.begin(), nodeItems.end(), nullItem);

	// fill storage using cache index lookup
	NodeValue value{ 0.f };
	for (tri::NodeIterator iter(trinet.begin()) ; iter ; ++iter)
	{
		tri::NodeKey const keyIJ{ iter.nodeKey() };
		tri::NodeIndex::index_type const ndx{ cache.indexForNodeKey(keyIJ) };
		if (! (ndx < numNodes))
		{
			oss << "Failure of node index size test" << std::endl;
			oss << dat::infoString(ndx, "ndx") << std::endl;
			oss << dat::infoString(numNodes, "numNodes") << std::endl;
			break;
		}
		nodeItems[ndx] = value;
		value += 1.f;
	}

	// check if all storage nodes have been set and are correct
	size_t errCount{ 0u };
	for (NodeValue const & nodeItem : nodeItems)
	{
		if (! dat::isValid(nodeItem))
		{
		//	oss << "Failure of nodeItem validity test" << std::endl;
		//	oss << dat::infoString(nodeItem, "nodeItem") << std::endl;
			++errCount;
		}
	}

	// check if any of the indices are invalid
	if (! (0u == errCount))
	{
		oss << "Failure of nodeItem validity test: errCount ="
			<< " " << errCount << std::endl;
	}

	tri::NodeKey const badKeyIJ{ 12345678L,  -654321L };
	tri::NodeIndex::index_type const badNdx{ cache.indexForNodeKey(badKeyIJ) };
	if (  dat::isValid(badNdx))
	{
		oss << "Failure of badKey test" << std::endl;
		oss << dat::infoString(badKeyIJ, "badKeyIJ") << std::endl;
		oss << dat::infoString(badNdx, "badNdx") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for tri::NodeIndex
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << tri_NodeIndex_test0();
	oss << tri_NodeIndex_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
