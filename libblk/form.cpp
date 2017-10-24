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
\brief Definitions for blk::form
*/


#include "libblk/form.h"
#include "libblk/info.h"

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <sstream>


namespace blk
{
namespace form
{

//! Private utilities for blk::form implementations
namespace priv
{
	struct Node
	{
		NodeKey theNodeKey{ NullKey };
	};

	// Define a simple graph structure for topographic connectivity
	using TopoWgt = double;
	using TopoGraph = boost::adjacency_list
		< boost::vecS // OutEdgeList
		, boost::vecS // VertexList
		, boost::undirectedS // Directed
		, Node // VertexProperties
		, boost::property<boost::edge_weight_t, TopoWgt> // EdgeProperties
		// GraphProperties
		// EdgeList
		>;
	using TopoEDes = TopoGraph::edge_descriptor;
	using TopoEIter = TopoGraph::edge_iterator;

	// display connectivity content of graph
	template <typename TypeGraph>
	std::string
	infoString
		( TypeGraph const & graph
		, std::string const & title = {}
		)
	{
		std::ostringstream oss;

		if (! title.empty())
		{
			oss << title << std::endl;
		}

		std::string const pad{ "  " };
		oss << pad << "numVerts: " << boost::num_vertices(graph) << std::endl;
		oss << pad << "numEdges: " << boost::num_edges(graph) << std::endl;

		using EIter = typename TypeGraph::edge_iterator;
		std::pair<EIter, EIter> const topoBE{ boost::edges(graph) };
		for (EIter itE{topoBE.first} ; topoBE.second != itE ; ++itE)
		{
			oss << pad << pad << "edge: " << *itE << std::endl;
		}

		return oss.str();
	}

	//! Graph with all edge connections
	TopoGraph
	fullTopoGraph
		( std::map<EdgeKey, ga::Rigid> const & relOriMap
		)
	{
		TopoGraph graph;

		// associated VDes with block NodeIds
		std::set<NodeKey> uniqNodeKeys;
		for (EdgeOri const & relOriItem : relOriMap)
		{
			EdgeKey const & eKey = relOriItem.first;
			uniqNodeKeys.insert(eKey.first);
			uniqNodeKeys.insert(eKey.second);
		}

		// add exactly as many vertices to graph as unique block nodes
		using VDes = boost::graph_traits<TopoGraph>::vertex_descriptor;
		std::map<blk::NodeKey, VDes> vDesForKey;
		for (NodeKey const & nodeKey : uniqNodeKeys)
		{
			VDes const vdes{ boost::add_vertex(graph) };
			graph[vdes].theNodeKey = nodeKey;
			vDesForKey[nodeKey] = vdes;
		}

		// add graph edges - one for each source relOri
		for (EdgeOri const & relOriItem : relOriMap)
		{
			EdgeKey const & eKey = relOriItem.first;
			NodeKey const & keyI = eKey.first;
			NodeKey const & keyJ = eKey.second;
			VDes const vdesI = vDesForKey[keyI];
			VDes const vdesJ = vDesForKey[keyJ];
			constexpr TopoWgt weight{ 1. };
			boost::add_edge(vdesI, vdesJ, weight, graph).first;
		}

		return graph;
	}

	//! Graph with MinSpanningTree within edgeOris
	TopoGraph
	minTopoGraph
		( TopoGraph const & fullGraph
		)
	{
		TopoGraph spanGraph;

		using VDes = boost::graph_traits<TopoGraph>::vertex_descriptor;
		using VIter = boost::graph_traits<TopoGraph>::vertex_iterator;

		// copy block node info into spanning graph
		std::pair<VIter, VIter> const itFVBE{ boost::vertices(fullGraph) };
		for (VIter itFV{itFVBE.first} ; itFVBE.second != itFV ; ++itFV)
		{
			VDes const fullVDes{ *itFV };
			VDes const spanVDes{ boost::add_vertex(spanGraph) };
			spanGraph[spanVDes] = fullGraph[fullVDes];
		}

		// compute minimum spanning tree (minimize number of edges involved)
		std::map<VDes, VDes> predMapData;
		boost::associative_property_map<std::map<VDes, VDes> >
			predMap(predMapData);
		boost::prim_minimum_spanning_tree(fullGraph, predMap);

		// populate new/return graph to represent MST
		for (auto const & predMapItem : predMapData)
		{
			VDes const & fromVDes = predMapItem.first;
			VDes const & intoVDes = predMapItem.second;
			add_edge(fromVDes, intoVDes, spanGraph);
		}

		return spanGraph;
	}

	//! Set *both* referenced EOs based on ori2w1
	void
	initBlockEOs
		( std::map<NodeKey, ga::Rigid> * const ptEOs
		, NodeKey const & key1
		, NodeKey const & key2
		, ga::Rigid const & ori2w1
		)
	{
		(*ptEOs)[key1] = ga::Rigid::identity();
		(*ptEOs)[key2] = ori2w1;
	}

	//! Set EO[2] by propagating ori2w1 starting from EO[1]
	void
	updateBlockEOs
		( std::map<NodeKey, ga::Rigid> * const ptEOs
		, NodeKey const & key1
		, NodeKey const & key2
		, ga::Rigid const & ori2w1
		)
	{
		ga::Rigid const & ori1 = (*ptEOs)[key1];
		ga::Rigid const ori2{ ori2w1 * ori1 };
		(*ptEOs)[key2] = ori2;
	}

	//! Propagate orentations as each spanning RO is descovered/traversed
	class TreeBuilder : public boost::default_bfs_visitor
	{
		std::shared_ptr<RelOriPool> const thePtPool;
		bool theIsInit;
		std::map<NodeKey, ga::Rigid> * const thePtEOs;

	public:

		//! Attach to relOriMap as source for tree build
		explicit
		TreeBuilder
			( std::shared_ptr<RelOriPool> const & ptRoPool
			, std::map<NodeKey, ga::Rigid> * const ptEOs
			)
			: boost::default_bfs_visitor{}
			, thePtPool{ ptRoPool }
			, theIsInit{ false }
			, thePtEOs{ ptEOs }
		{ }

		//! Update theEOs with orientation from input edge
		void
		tree_edge
			( TopoGraph::edge_descriptor const & edesc
			, TopoGraph const & graph
			)
		{
			// access endpoint vertices for this edge
			using VDes = boost::graph_traits<TopoGraph>::vertex_descriptor;
			VDes const fromVDes{ boost::source(edesc, graph) };
			VDes const intoVDes{ boost::target(edesc, graph) };

			// get block node info from vertices
			NodeKey const fromKey{ graph[fromVDes].theNodeKey };
			NodeKey const intoKey{ graph[intoVDes].theNodeKey };

			assert(thePtEOs);
			EdgeOri const eOri{ thePtPool->edgeOriFor(fromKey, intoKey) };
			ga::Rigid const & ori2w1 = eOri.second;
			assert(ori2w1.isValid());

			// update block by propagating this edge orientation
			if (! theIsInit)
			{
				initBlockEOs(thePtEOs, fromKey, intoKey, ori2w1);
				theIsInit = true;
			}
			else
			{
				updateBlockEOs(thePtEOs, fromKey, intoKey, ori2w1);
			}
		}
	};


	//! Block node orientations by propgating relOri's along graph edges
	std::map<NodeKey, ga::Rigid>
	blockNodeOrientations
		( TopoGraph const & graph
		, std::shared_ptr<RelOriPool> const & ptRoPool
		)
	{
		std::map<NodeKey, ga::Rigid> blkOriMap;

		// use graph B.F.S. traversal to invoke visitor node operations
		using VDes = TopoGraph::vertex_descriptor;
		using CType = boost::default_color_type;

		// functor for block (orientation tree) formation on each node visit
		TreeBuilder visitor(ptRoPool, &blkOriMap);

		// BGL assistance
		boost::queue<VDes> buffer;
		std::map<VDes, CType> colorData;
		boost::associative_property_map<std::map<VDes, CType> >
			colorMap(colorData);

		// perform block formation behind the scenes during BFS traversal
		boost::breadth_first_search
			( graph
			, boost::vertex(0u, graph)
			, buffer
			, visitor
			, colorMap
			);

		return blkOriMap;
	}

	//! True if graph is fully connected
	bool
	isConnected
		( priv::TopoGraph const & graph
		)
	{
		bool isOne{ false };

		// classify nodes into connected subgraphs
		using VDes = boost::graph_traits<priv::TopoGraph>::vertex_descriptor;
		using VNdx = boost::graph_traits<priv::TopoGraph>::vertices_size_type;
		std::map<VDes, VNdx> vNdxMapData;
		boost::associative_property_map<std::map<VDes, VNdx> >
			vNdxMap(vNdxMapData);
		boost::connected_components
			(graph, vNdxMap, boost::vertex_index_map(vNdxMap));

		// check number of connected components
		std::set<VNdx> subNdxs;
		for (std::pair<VDes, VNdx> const & vNdxMapItem : vNdxMapData)
		{
			size_t const & subNdx = vNdxMapItem.second;
			subNdxs.insert(subNdx);
			/*
			VDes const & vdes = vNdxMapItem.first;
			io::out()
				<< "vNdxMapItem[" << vdes << "]" << " in " << subNdx
				<< std::endl;
			*/
		}
		isOne = (1u == subNdxs.size());

		return isOne;
	}

} // priv


std::map<NodeKey, ga::Rigid>
viaSpan
	( std::vector<EdgeOri> const & edgeOris
	)
{
	std::map<NodeKey, ga::Rigid> blkOriMap;

	// order relative measurements for easy retrieval
	std::shared_ptr<RelOriPool> const ptRoPool
		{ std::make_shared<RelOriPool>
			(RelOriPool::from(edgeOris.begin(), edgeOris.end()))
		};

	// use graph structure to organize relative connections
	priv::TopoGraph const fullGraph
		{ priv::fullTopoGraph(ptRoPool->theRelOriMap) };
	// io::out() << priv::infoString(fullGraph, "fullGraph") << std::endl;

	// check if block is fully connected by the RO measurements
	bool isOneBlock{ isConnected(fullGraph) };
	if (isOneBlock)
	{
		// use minimum spanning tree to minimize orientation transfers
		priv::TopoGraph const spanGraph{ priv::minTopoGraph(fullGraph) };
		// io::out() << priv::infoString(spanGraph, "spanGraph") << std::endl;

		// propagate orientations through graph
		blkOriMap = priv::blockNodeOrientations(spanGraph, ptRoPool);
	}

	return blkOriMap;
}

} // form
} // blk

