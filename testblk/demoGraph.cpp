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
#include "libdat/info.h"
#include "libio/stream.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

#include <boost/graph/visitors.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

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
	// type of graph structure to use
	using TypeVNdx = boost::vertex_index_t;
	using TypeWgt = double;
	using TypeGraph = boost::adjacency_list
		< boost::setS // EdgeStore
		, boost::setS // VertexStore
		, boost::undirectedS
		, boost::no_property // NodeProp
		, boost::property<boost::edge_weight_t, TypeWgt> // EdgeProp
		>;
	using TypeEDes = TypeGraph::edge_descriptor;
	using TypeVDes = TypeGraph::vertex_descriptor;
	using TypeVSize = boost::graph_traits<TypeGraph>::vertices_size_type;

//	io::out() << "TypeVNdx: " << dat::nameOfType(TypeVNdx()) << std::endl;
//	io::out() << "TypeVSize: " << dat::nameOfType(TypeVSize()) << std::endl;

	// construct graph with specified number of vertices
	constexpr size_t numVerts{ 5u };
	TypeGraph graph(numVerts);

	// display initial value vertices
	for (TypeGraph::vertex_iterator
		itV{vertices(graph).first} ; vertices(graph).second != itV ; ++itV)
	{
		TypeGraph::vertex_descriptor const & subV = *itV;
		io::out() << "ctor vDesc: " << subV << std::endl;
	}

	// get vertex descriptors vDesc[vNdx] ...
	std::vector<TypeGraph::vertex_descriptor> vSubs(numVerts);
	std::copy
		( vertices(graph).first, vertices(graph).second
		, std::inserter(vSubs, vSubs.begin())
		);
	// ... and use them to add a few edges
	boost::add_edge(vSubs[0], vSubs[1], graph);
	boost::add_edge(vSubs[0], vSubs[2], graph);
	boost::add_edge(vSubs[0], vSubs[3], graph);
	boost::add_edge(vSubs[1], vSubs[2], graph);
	boost::add_edge(vSubs[1], vSubs[3], graph);
	boost::add_edge(vSubs[2], vSubs[3], graph);

	// create a vNdx[vDesc] map - for reverse lookup ...
	std::map<TypeVDes, TypeVSize> vSizeForDesData;
	boost::associative_property_map<std::map<TypeVDes, TypeVSize>>
		vSizeForDes(vSizeForDesData);
	// ... and fill it
	for (size_t vNdx{0u} ; vNdx < numVerts ; ++vNdx)
	{
		vSizeForDes[boost::vertex(vNdx, graph)] = vNdx;
	}

	// determine connected components
	boost::connected_components
		(graph, vSizeForDes, boost::vertex_index_map(vSizeForDes));

	// report connected components
	std::for_each
		( vSizeForDesData.begin(), vSizeForDesData.end()
		, []
			(std::pair<TypeVDes, TypeVSize> const & compId)
			{ io::out() << "..." << compId.second << std::endl; }
		);


	// Compute minimum spanning tree

	// - Input data stuctures
	std::map<TypeEDes, TypeWgt> wgtMapData;
	boost::associative_property_map<std::map<TypeEDes, TypeWgt> >
		wgtMap(wgtMapData);
	for (TypeGraph::edge_iterator itE{boost::edges(graph).first}
			; boost::edges(graph).second != itE ; ++itE)
	{
		wgtMapData[*itE] = 1.;
	}

	std::map<TypeVDes, TypeVNdx> vNdxForDesData;
	boost::associative_property_map<std::map<TypeVDes, TypeVNdx> >
		vNdxForDes(vNdxForDesData);

	// What is TypeVNdx struct to do with vertex sequences
	size_t ndx{ 0u };
	for (TypeGraph::vertex_iterator itV{boost::vertices(graph).first}
		; boost::vertices(graph).second != itV ; ++itV)
	{
		vNdxForDes[vSubs[ndx]] = TypeVNdx(ndx);
		++ndx;
	}

	// - Output data structures
	std::map<TypeVDes, TypeVDes> predMapData;
	boost::associative_property_map<std::map<TypeVDes, TypeVDes> >
		predMap(predMapData);

	std::map<TypeVDes, TypeWgt> distMapData;
	boost::associative_property_map<std::map<TypeVDes, TypeWgt> >
		distMap(distMapData);

	// compute MST
	prim_minimum_spanning_tree
		( graph
		// IN: vertex_descriptor - s
		, vSubs[0]
		// OUT: PredecessorMap - predecessor
		, predMap
		// OUT: DistanceMap - distance
		, distMap
		// IN: WeightMap - weight
		, wgtMap
		// IN: IndexMap - index_map
		, vNdxForDes
		// DijkstraVisitor - vis
		, boost::dijkstra_visitor<boost::null_visitor>()
		);

	for (auto const & predMapItem : predMapData)
	{
		io::out() << "predMapItem: " << predMapItem.second << std::endl;
	}
	for (auto const & distMapItem : distMapData)
	{
		io::out() << "distMapItem: " << distMapItem.second << std::endl;
	}

	return 0;
}


