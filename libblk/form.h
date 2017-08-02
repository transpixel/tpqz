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

#ifndef blk_form_INCL_
#define blk_form_INCL_

/*! \file
\brief Declarations for blk::form
*/


#include "libblk/OriPair.h"
#include "libga/Rigid.h"
#include "libio/sprintf.h"

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

namespace
{
}


namespace blk
{

/*! \brief Block formation algorithms.

\par Example
\dontinclude testblk/uform.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace form
{
	// Define a simple graph structure for topographic connectivity
	using TopoWgt = double;
	using TopoGraph = boost::adjacency_list
		< boost::vecS // OutEdgeList
		, boost::vecS // VertexList
		, boost::undirectedS // Directed
		, boost::no_property // VertexProperties
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

	//! Index pair which can assume (.first < .second)
	using LoHiEdge = std::pair<size_t, size_t>;

	//! Gather relative orientations sorted by directed edge indices
	std::map<LoHiEdge, ga::Rigid>
	lohiEdgeOrientations
		( std::vector<blk::OriPair> const & rops
		)
	{
		std::map<LoHiEdge, ga::Rigid> lohiMap;
		for (blk::OriPair const & rop : rops)
		{
			blk::OriPair const ijRop{ rop.principalPair() };
			LoHiEdge const ijPair{ ijRop.ndxI(), ijRop.ndxJ() };
			lohiMap[ijPair] = ijRop.theOriJwI;
		}
		return lohiMap;
	}

	//! Graph with all edge connections
	TopoGraph
	fullTopoGraph
		( std::map<LoHiEdge, ga::Rigid> const & relOriMap
		)
	{
		TopoGraph graph;
		for (std::pair<LoHiEdge, ga::Rigid> const & relOriItem : relOriMap)
		{
			NodeNdx const & ndxI = relOriItem.first.first;
			NodeNdx const & ndxJ = relOriItem.first.second;
			constexpr TopoWgt weight{ 1. };
			boost::add_edge(ndxI, ndxJ, weight, graph).first;
		}
		return graph;
	}

	//! Graph with MinSpanningTree within rops
	TopoGraph
	minTopoGraph
		( TopoGraph const & fullGraph
		)
	{
		TopoGraph minGraph;

		// compute minimum spanning tree (minimize number of edges involved)
		using VDes = boost::graph_traits<TopoGraph>::vertex_descriptor;
		std::map<VDes, VDes> predMapData;
		boost::associative_property_map<std::map<VDes, VDes> >
			predMap(predMapData);
		boost::prim_minimum_spanning_tree(fullGraph, predMap);

		// populate new/return graph to represent MST
		for (auto const & predMapItem : predMapData)
		{
			add_edge(predMapItem.first, predMapItem.second, minGraph);
		}

		return minGraph;
	}

	//! Report orientations
	std::string
	infoStringShort
		( std::vector<ga::Rigid> const & oris
		, std::string const & title = std::string{}
		)
	{
		std::ostringstream oss;
		if (! title.empty())
		{
			oss << title;
		}
		for (size_t nn{0u} ; nn < oris.size() ; ++nn)
		{
			oss << std::endl;
			oss
				<< dat::infoString(nn)
				<< " " << ":"
				<< " " << oris[nn].infoStringShort()
				;
		}
		return oss.str();
	}

	//! Set *both* referenced EOs based on ori2w1
	void
	initBlockEOs
		( std::vector<ga::Rigid> * const ptEOs
		, size_t const & ndx1
		, size_t const & ndx2
		, ga::Rigid const & ori2w1
		)
	{
		(*ptEOs)[ndx1] = ga::Rigid::identity();
		(*ptEOs)[ndx2] = ori2w1;
	}

	//! Set EO[2] by propagating ori2w1 starting from EO[1]
	void
	updateBlockEOs
		( std::vector<ga::Rigid> * const ptEOs
		, size_t const & ndx1
		, size_t const & ndx2
		, ga::Rigid const & ori2w1
		)
	{
		ga::Rigid const & ori1 = (*ptEOs)[ndx1];
		ga::Rigid const ori2{ ori2w1 * ori1 };
		(*ptEOs)[ndx2] = ori2;
	}

	//! Propagate orentations as each spanning RO is descovered/traversed
	class TreeBuilder : public boost::default_bfs_visitor
	{
		std::map<LoHiEdge, ga::Rigid> const & theRelOriMap;
		bool theIsInit;
		std::vector<ga::Rigid> * const thePtEOs;

	public:

		//! Attach to relOriMap as source for tree build
		explicit
		TreeBuilder
			( std::map<LoHiEdge, ga::Rigid> const & relOriMap
			, std::vector<ga::Rigid> * const & ptEOs
			)
			: boost::default_bfs_visitor{}
			, theRelOriMap(relOriMap)
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
			size_t const fromNdx{ boost::source(edesc, graph) };
			size_t const intoNdx{ boost::target(edesc, graph) };

			assert(thePtEOs);
			assert(fromNdx < thePtEOs->size());
			assert(intoNdx < thePtEOs->size());

// TODO -- need to flip indices if need be
assert(fromNdx < intoNdx);

			// get (properly directed) orientation for this edge
			LoHiEdge const lohiNdxs{ fromNdx, intoNdx };
			std::map<LoHiEdge, ga::Rigid>::const_iterator const itFind
				(theRelOriMap.find(lohiNdxs));
			assert(theRelOriMap.end() != itFind);
			ga::Rigid const & ori2w1 = itFind->second;

			// update block by propagating this edge orientation
			if (! theIsInit)
			{
				initBlockEOs(thePtEOs, fromNdx, intoNdx, ori2w1);
				theIsInit = true;
			}
			else
			{
				updateBlockEOs(thePtEOs, fromNdx, intoNdx, ori2w1);
			}
		}
	};


	//! Block node orientations by propgating relOri's along graph edges
	std::vector<ga::Rigid>
	blockNodeOrientations
		( TopoGraph const & graph
		, std::map<LoHiEdge, ga::Rigid> const & relOriMap
		)
	{
		std::vector<ga::Rigid> blockEOs;

		// use graph B.F.S. traversal to invoke visitor node operations
		using VDes = TopoGraph::vertex_descriptor;
		using CType = boost::default_color_type;

		// functor for block (orientation tree) formation on each node visit
		size_t const numNodes{ boost::num_vertices(graph) };
		blockEOs.resize(numNodes);
		TreeBuilder visitor(relOriMap, &blockEOs);

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

		return blockEOs;
	}

	//! Formation by repeated traversal of orientations
	std::vector<ga::Rigid>
	bruteForce
		( std::vector<blk::OriPair> const & rops
		)
	{
		std::vector<ga::Rigid> blockEOs;

		// order relative measurements for easy retrieval
		std::map<LoHiEdge, ga::Rigid> const relOriMap
			{ lohiEdgeOrientations(rops) };

		// use graph structure to organize relative connections
		TopoGraph const fullGraph{ fullTopoGraph(relOriMap) };
		io::out() << infoString(fullGraph, "fullGraph") << std::endl;

		// TODO verify connectivity ? (or return multiple blocks?)
		bool isConnected{ true };
		if (isConnected)
		{
			// use minimum spanning tree to minimize orientation transfers
			TopoGraph const spanGraph{ minTopoGraph(fullGraph) };
			io::out() << infoString(spanGraph, "spanGraph") << std::endl;

			// propagate orientations through graph
			blockEOs = blockNodeOrientations(spanGraph, relOriMap);
		}

		io::out() << infoStringShort(blockEOs, "blockEOs") << std::endl;

		return blockEOs;
	}

} // form

} // blk

// Inline definitions
// #include "libblk/form.inl"

#endif // blk_form_INCL_

