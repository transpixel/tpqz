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
\brief  This file contains unit test for map::Proj
*/


#include "libmap/Proj.h"

#include "libio/stream.h"
#include "libmath/Partition.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
map_Proj_test0
	()
{
	std::ostringstream oss;
	map::Proj const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

//! Check base-class operations
std::string
map_Proj_test1
	()
{
	std::ostringstream oss;

	// check default implementation values
	map::Proj proj;

	ga::Vector const anyDir{ ga::unit(ga::Vector{ .2, .5, .2 }) };
	dat::RowCol const gotRowCol(proj.gridRowColFor(anyDir));
	if (dat::isValid(gotRowCol))
	{
		oss << "Failure of default null rowcol test" << std::endl;
		oss << "expRowCol: <null>" << std::endl;
		oss << dat::infoString(gotRowCol) << std::endl;
	}

	dat::RowCol const anyRowCol{{ 17u, 18u }};
	ga::Vector const & gotDir{ proj.directionFor(anyRowCol) };
	if (gotDir.isValid())
	{
		oss << "Failure of default null direction test" << std::endl;
		oss << "expDir: <null>" << std::endl;
		oss << dat::infoString(gotDir) << std::endl;
	}

	return oss.str();
}


constexpr size_t sNumBins{ 1024u };
dat::Range<double> const sUnitRange(-1., 1.+1./double(128u*sNumBins));
math::Partition const sUnitPart(sUnitRange, sNumBins);

//! Simple orthographic projection
struct ProjOrtho : public map::Proj
{
	explicit
	ProjOrtho
		()
		: Proj(dat::Extents{ 2u*sNumBins, sNumBins })
	{ }

	//! just for style
	virtual
	~ProjOrtho
		() = default;

	//! [Virtual] Raster location for a direction
	inline
	virtual
	dat::RowCol
	gridRowColFor
		( ga::Vector const & avec
		) const
	{
		dat::RowCol rowcol(dat::nullValue<dat::RowCol>());
		if (isValid())
		{
			ga::Vector const dir{ ga::unit(avec) };
			size_t row{ sUnitPart.binIndexFor(dir[0]) };
			if (dir[2] < 0.)
			{
				row += sUnitPart.size();
			}
			size_t const col{ sUnitPart.binIndexFor(dir[1]) };
			rowcol = {{ row, col }};
		}
		return rowcol;
	}

	//! [Virtual] Direction recovered from raster location
	inline
	virtual
	ga::Vector
	directionFor
		( dat::RowCol const & mapRC
		) const
	{
		ga::Vector dir;
		if (isValid())
		{
			size_t row{ mapRC[0] };
			double zsgn{ 1. };
			if (sUnitPart.size() < row)
			{
				row -= sUnitPart.size();
				zsgn = -1.;
			}
			size_t const & col = mapRC[1];
			double const xx{ sUnitPart.interpValueFor(row) };
			double const yy{ sUnitPart.interpValueFor(col) };
			double const rSq{ math::sq(xx) + math::sq(yy) };
			double const zmag{ std::sqrt(1. - rSq) };
			dir = ga::Vector{ xx, yy, zsgn*zmag };
		}
		return dir;
	}

};

//! Check Basic operations
std::string
map_Proj_test2
	()
{
	std::ostringstream oss;

	ProjOrtho aProj;
	map::Proj const * const ptProj = &aProj;

	ga::Vector const anyDir{ ga::unit(ga::Vector{ .2, .5, .2 }) };
	dat::Extents const hws(ptProj->hwSize());
	dat::RowCol const midRowCol{{ hws.high()/2u, hws.wide()/2u }};
	dat::RowCol const anyRowCol{{ midRowCol[0]-17u, midRowCol[1]+18u }};

	// check for valid return values

	dat::RowCol const gotRowCol(ptProj->gridRowColFor(anyDir));
	if (! dat::isValid(gotRowCol))
	{
		oss << "Failure of derived rowcol test" << std::endl;
		oss << dat::infoString(gotRowCol) << std::endl;
	}

	ga::Vector const & gotDir{ ptProj->directionFor(anyRowCol) };
	if (! gotDir.isValid())
	{
		oss << "Failure of derived direction test" << std::endl;
		oss << dat::infoString(gotDir) << std::endl;
	}

	// check round trip test -- mostly just fer grins

	dat::RowCol const rtRowCol(ptProj->gridRowColFor(gotDir));
	if (! dat::nearlyEquals(rtRowCol, anyRowCol))
	{
		oss << "Failure of derived roundtrip rowcol test" << std::endl;
		oss << dat::infoString(anyRowCol, "expRowCol") << std::endl;
		oss << dat::infoString(rtRowCol, "gotRowCol") << std::endl;
	}

	ga::Vector const rtDir(ptProj->directionFor(gotRowCol));
	double const tol{ 1./16. }; // cheesy test case
	if (! rtDir.nearlyEquals(anyDir, tol))
	{
		oss << "Failure of derived roundtrip direction test" << std::endl;
		oss << dat::infoString(anyDir, "expDir") << std::endl;
		oss << dat::infoString(rtDir, "gotDir") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for map::Proj
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << map_Proj_test0();
	oss << map_Proj_test1();
	oss << map_Proj_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
