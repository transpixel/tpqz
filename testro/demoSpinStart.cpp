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
\brief  This file contains main application program demoSpinStart
*/


#include "libapp/Usage.h"

#include "build/version.h"
#include "libio/stream.h"

#include "libga/ga.h"
#include "libga/groups.h"

#include <cassert>
#include <iostream>
#include <vector>


namespace
{
}

//! Display spinors proposed by Horn in "Relative Orientation [Revisisted]"
int
main
	( int const argc
	, char const * const * const argv
	)
{
	// check args
	app::Usage usage;
	usage.setSummary
		( "Display spinor sampling values (and bivAngle) proposed by Horn"
		);
//	usage.addArg("arg1", "description");
	// ...
	if (usage.argStatus(argc, argv) != app::Usage::Valid)
	{
		std::string const fname(argv[0]);
		io::err()
			<< std::endl << build::version::buildInfo(argv[0]) << std::endl
			<< usage.infoString(fname) << std::endl;
		return 1;
	}

	// parse input argument
//	int argnum(0);
//	std::string const arg1(argv[++argnum]);

	std::vector<ga::Spinor> const spins{ ga::groups::spreadOfSpinors() };
	std::vector<ga::BiVector> const bivs
		{ ga::groups::physicalAnglesFor(spins) };
	size_t const numAtts{ spins.size() };
	assert(numAtts == bivs.size());
	for (size_t nn{0u} ; nn < numAtts ; ++nn)
	{
		ga::Spinor const & spin = spins[nn];
		ga::BiVector const & bivPos = bivs[nn];
		ga::BiVector const pAng{ ga::spin::physicalAngleFrom(spin) };
		double const pMag{ ga::magnitude(pAng) };
		assert(pAng.nearlyEquals(bivPos));
		io::out()
			<< " " << dat::infoString(spin, "spin")
			<< " " << dat::infoString(bivPos, "bivPos")
		//	<< " " << dat::infoString(pAng, "pAng")
			<< " " << dat::infoString(pMag, "pMag")
			<< std::endl;
	}

	return 0;
}

