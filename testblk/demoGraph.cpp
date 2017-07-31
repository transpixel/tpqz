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
\brief  This file contains main application program demoGraph
*/


#include "libapp/Usage.h"

#include "build/version.h"
#include "libio/stream.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/connected_components.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>


namespace
{
}

//! Demonstrate simple operations with boost graph library (BGL)
int
main
	( int const // argc
	, char const * const * const // argv
	)
{
	/*
	// property for each vertex
	struct NodeProp
	{
		size_t theComp;
		std::string theName{ "nullNode" };
	};

	// property for each edge
	struct EdgeProp
	{
		std::string theName{ "nullEdge" };
	};
	*/

	// type of graph structure to use
	using GraphType = boost::adjacency_list
		< boost::setS // EdgeStore
		, boost::setS // VertexStore
		, boost::undirectedS
		, boost::no_property // NodeProp
		, boost::property<boost::edge_weight_t, double> // EdgeProp
		>;

	// construct graph with specified number of vertices
	constexpr size_t numVerts{ 5u };
	GraphType graph(numVerts);

	// display initial value vertices
	for (GraphType::vertex_iterator
		itV{vertices(graph).first} ; vertices(graph).second != itV ; ++itV)
	{
		GraphType::vertex_descriptor const & subV = *itV;
		io::out() << subV << std::endl;
	}

	// create a vNdx[vDesc] map - for reverse lookup
	using VDescNdxMap = std::map
		< boost::graph_traits<GraphType>::vertex_descriptor
		, boost::graph_traits<GraphType>::vertices_size_type
		>;
	VDescNdxMap compIds;
	boost::associative_property_map<VDescNdxMap> compMap(compIds);
	for (size_t vNdx{0u} ; vNdx < numVerts ; ++vNdx)
	{
		compMap[boost::vertex(vNdx, graph)] = vNdx + 100;
	}

	// get vertex descriptors vDesc[vNdx] ...
	std::vector<GraphType::vertex_descriptor> vSubs(numVerts);
	std::copy
		( vertices(graph).first, vertices(graph).second
		, std::inserter(vSubs, vSubs.begin())
		);
	// ... and use them to add a few edges
	boost::add_edge(vSubs[0], vSubs[1], 1.6, graph);
	boost::add_edge(vSubs[0], vSubs[2], 3.2, graph);
	boost::add_edge(vSubs[0], vSubs[3], 1.4, graph);
	boost::add_edge(vSubs[1], vSubs[2], 2.7, graph);
	boost::add_edge(vSubs[1], vSubs[3], 1.9, graph);
	boost::add_edge(vSubs[2], vSubs[3], 7.1, graph);

	// determine connected components
	boost::connected_components
		(graph, compMap, boost::vertex_index_map(compMap));

	// report connected components
	std::for_each
		( compIds.begin(), compIds.end()
		, []
			(VDescNdxMap::value_type const & compId)
			{ io::out() << "..." << compId.second << std::endl; }
		);



	return 0;
}


