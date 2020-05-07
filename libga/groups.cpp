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
\brief Declarations for ga::groups
*/


#include "libga/groups.h"
#include "libga/operators.h"
#include "libga/spin.h"


namespace ga
{
namespace groups
{

std::vector<ga::Spinor>
cubeSpinors
	()
{
	static std::vector<ga::Spinor> const spins
		{ ga::Spinor(0., ga::BiVector(1., 0., 0.))
		, ga::Spinor(0., ga::BiVector(0., 1., 0.))
		, ga::Spinor(0., ga::BiVector(0., 0., 1.))
		};
	return spins;
}

std::vector<ga::Spinor>
tetraSpinors
	()
{
	constexpr double const bb{ .5 };
	static std::vector<ga::Spinor> const spins
		{ ga::Spinor( bb, ga::BiVector( bb,  bb,  bb))
		, ga::Spinor( bb, ga::BiVector(-bb,  bb,  bb))
		, ga::Spinor( bb, ga::BiVector( bb, -bb,  bb))
		, ga::Spinor( bb, ga::BiVector( bb,  bb, -bb))
		, ga::Spinor( bb, ga::BiVector( bb, -bb, -bb))
		, ga::Spinor( bb, ga::BiVector(-bb,  bb, -bb))
		, ga::Spinor( bb, ga::BiVector(-bb, -bb,  bb))
		, ga::Spinor( bb, ga::BiVector(-bb, -bb, -bb))
		};
	return spins;
}

std::vector<ga::Spinor>
octaSpinors
	()
{
	static double const cc{ std::sqrt(.5) };
	static std::vector<ga::Spinor> const spins
		{ ga::Spinor( 0., ga::BiVector( 0.,  cc,  cc))
		, ga::Spinor( 0., ga::BiVector( 0.,  cc, -cc))
		, ga::Spinor( 0., ga::BiVector( cc,  0.,  cc))
		, ga::Spinor( 0., ga::BiVector( cc,  0., -cc))
		, ga::Spinor( 0., ga::BiVector( cc,  cc,  0.))
		, ga::Spinor( 0., ga::BiVector( cc, -cc,  0.))

		, ga::Spinor( cc, ga::BiVector( 0.,  0.,  cc))
		, ga::Spinor( cc, ga::BiVector( 0.,  0., -cc))
		, ga::Spinor( cc, ga::BiVector( 0.,  cc,  0.))
		, ga::Spinor( cc, ga::BiVector( 0., -cc,  0.))
		, ga::Spinor( cc, ga::BiVector( cc,  0.,  0.))
		, ga::Spinor( cc, ga::BiVector(-cc,  0.,  0.))
		};
	return spins;
}

std::vector<ga::Spinor>
spreadOfSpinors
	()
{
	static std::vector<ga::Spinor> spins;
	if (spins.empty())
	{
		static std::vector<ga::Spinor> const identity
			{ ga::Spinor(1., ga::BiVector(0., 0., 0.))
			};

		static std::vector<ga::Spinor> const cube{ cubeSpinors() };
		static std::vector<ga::Spinor> const tetrahedron{ tetraSpinors() };
		static std::vector<ga::Spinor> const octahedron{ octaSpinors() };

		// 1 value
		spins.insert(spins.end(), identity.begin(), identity.end());
		// 4 values
		spins.insert(spins.end(), cube.begin(), cube.end());
		// 12 values
		spins.insert(spins.end(), tetrahedron.begin(), tetrahedron.end());
		// 24 values
		spins.insert(spins.end(), octahedron.begin(), octahedron.end());
	}

	return spins;
}

std::vector<ga::BiVector>
physicalAnglesFor
	( std::vector<ga::Spinor> const & spins
	)
{
	std::vector<ga::BiVector> bivs;
	bivs.reserve(spins.size());
	for (ga::Spinor const & spin : spins)
	{
		bivs.emplace_back(ga::spin::physicalAngleFrom(spin));
	}
	return bivs;
}

} // groups

} // ga

