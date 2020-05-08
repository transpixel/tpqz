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
\brief Definitions for geo::cube
*/


#include "libgeo/cube.h"

#include "libga/Rigid.h"

#include <algorithm>


namespace geo
{
namespace cube
{
//======================================================================

std::vector<ga::Vector>
points :: onVertices
	()
{
	using namespace ga;
	return std::vector<ga::Vector>
		{ (    vZero   )
		, (e1          )
		, (     e2     )
		, (          e3)
		, (e1 + e2     )
		, (     e2 + e3)
		, (e1      + e3)
		, (e1 + e2 + e3)
		};
}

std::vector<ga::Vector>
points :: onEdge
	( size_t const & numBetween
	)
{
	std::vector<ga::Vector> pnts;
	pnts.reserve(numBetween);
	ga::Vector const dir(ga::e1);
	double const delta(1. / static_cast<double>(numBetween + 1u));
	for (size_t nn(0u) ; nn < numBetween ; ++nn)
	{
		double const dist(double(nn+1u)*delta);
		pnts.push_back(dist*dir);
	}
	return pnts;
}

std::vector<ga::Vector>
points :: onTriad
	( std::vector<ga::Vector> const & pnts1d
	)
{
	std::vector<ga::Vector> pnts;
	if (! pnts1d.empty())
	{
		size_t const numPnts(pnts1d.size());
		pnts.reserve(3u * numPnts);

		using namespace ga;

		// configure 3 (one-sided) rotations to form a dextral triad
		std::array<Spinor, 3u> const planeSpinors
			{{ Spinor(one)
			 , Spinor(exp( math::halfPi * E12))
			 , Spinor(exp(-math::halfPi * E31))
			}};

		// transform input points to triad locations
		for (size_t kk(0u) ; kk < 3u ; ++kk)
		{
			for (size_t nn(0u) ; nn < numPnts ; ++nn)
			{
				Vector const & p1 = pnts1d[nn];
				Vector const pnta((p1 * planeSpinors[kk]).theV);
				pnts.push_back(pnta);
			}
		}
	}

	return pnts;
}

std::vector<ga::Vector>
points :: onSkeleton
	( std::vector<ga::Vector> const & triad
	)
{
	std::vector<ga::Vector> pnts;
	size_t const numTri(triad.size());
	if (0u < numTri)
	{
		pnts.reserve(4u * numTri);

		using namespace ga;

		// define triads at the 4 key points
		std::array<Rigid, 4u> const xforms
			{{ Rigid::identity()
			 , Rigid(Vector(e1 + e2), Pose(math::pi * E12))
			 , Rigid(Vector(e2 + e3), Pose(math::pi * E23))
			 , Rigid(Vector(e3 + e1), Pose(math::pi * E31))
			}};

		// transform each point into all four corner frames
		for (size_t kk(0u) ; kk < 4u ; ++kk)
		{
			for (size_t nn(0u) ; nn < numTri ; ++nn)
			{
				Vector const & pntIn = triad[nn];
				pnts.push_back(xforms[kk](pntIn));
			}
		}
	}

	return pnts;
}

std::vector<ga::Vector>
points :: onFrame
	( size_t const & numBetween
	)
{
	std::vector<ga::Vector> pnts;

	// generate points on the edges
	if (0u < numBetween)
	{
		// generate points on one edge
		std::vector<ga::Vector> const edge(points::onEdge(numBetween));;

		// generate points configured in a triad
		std::vector<ga::Vector> const triad(points::onTriad(edge));

		// fill the skeleton with four triads
		pnts = points::onSkeleton(triad);
	}

	// add corners
	std::vector<ga::Vector> const verts(points::onVertices());
	pnts.insert(pnts.end(), verts.begin(), verts.end());

	return pnts;
}

ga::Vector
center
	()
{
	static ga::Vector const center(.5, .5, .5);
	return center;
}

std::vector<ga::Vector>
centralDirections
	( size_t const & numBetween
	)
{
	std::vector<ga::Vector> dirs;
	std::vector<ga::Vector> const pnts(points::onFrame(numBetween));
	dirs.resize(pnts.size());
	static ga::Vector const orig(center());
	std::transform
		( pnts.begin(), pnts.end()
		, dirs.begin()
		, []
			( ga::Vector const & pnt )
			{ return ga::unit(pnt - orig); }
		);
	return dirs;
}

std::array<ga::Vector, 6u>
faceCenterDirections
	()
{
	using namespace ga;
	return std::array<ga::Vector, 6u>{{ -e3, e1, e2, -e1, -e2, e3 }};
}

//======================================================================
}
}

