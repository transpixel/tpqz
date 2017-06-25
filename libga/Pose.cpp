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


/*! \file
\brief Definitions for ga::Pose
*/


#include "libga/Pose.h"

#include <sstream>


namespace ga
{
//======================================================================

Vector
Pose :: bodyAxisInRef
	( size_t const & ndx
	) const
{
	Vector dirInRef{};
	if (ndx < 3u)
	{
		static std::array<Vector, 3u> const axes{ e1, e2, e3 };
		dirInRef = inverse()(axes[ndx]);
	}
	return dirInRef;
}

bool
Pose :: nearlyEquals
	( Pose const & other
	, double const & tol
	) const
{
	bool okay(false);
	// for numeric stability and to avoid testing special cases
	// evaluate equivalence by effect on transformation of basis vectors
	Vector const self1((*this)(e1));
	Vector const other1(other(e1));
	okay = ga::nearlyEquals(self1, other1, tol);
	if (okay)
	{
		Vector const self2((*this)(e2));
		Vector const other2(other(e2));
		okay = ga::nearlyEquals(self2, other2, tol);
		if (okay)
		{
			Vector const self3((*this)(e3));
			Vector const other3(other(e3));
			okay = ga::nearlyEquals(self3, other3, tol);
		}
	}
	return okay;
}

std::array<double, 4u>
Pose :: spinComponents
	() const
{
	return spin::components(theSpin);
}

std::string
Pose :: infoString
	( std::string const & title
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << std::endl;
	}
	if (isValid())
	{
		BiVector const pAngle(spin::physicalAngleFrom(theSpin));
		Pose const refWrtThis(inverse());

		static bool const showG(false);
		static bool const showM(false);
		std::array<Vector, 3u> const bodyAxes
			{{ refWrtThis(e1), refWrtThis(e2), refWrtThis(e3) }};
		oss
			<< pAngle.infoString("pAngle", showG, true)
			<< '\n'
			<< theSpin.infoString ("spin", showG, showM)
			<< '\n'
			<< bodyAxes[0].infoString("x-BodyAxis", showG, showM)
			<< '\n'
			<< bodyAxes[1].infoString("y-BodyAxis", showG, showM)
			<< '\n'
			<< bodyAxes[2].infoString("z-BodyAxis", showG, showM)
			;
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}

//======================================================================
}

