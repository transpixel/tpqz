//
//
// MIT License
//
// Copyright (c) 2020 Stellacore Corporation.
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
\brief  This file contains unit test for cam::fit
*/


#include "libcam/fit.h"

#include "libgeo/Triangle.h" // move

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
cam_fit_test0
	()
{
	std::ostringstream oss;
	/*
	cam::fit const aNull(dat::nullValue<cam::fit>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

namespace
{
	//! Generalized 3D vertex angle.
	struct Wedge
	{
		geo::Triangle theTriangle;

		//! Value construction.
		explicit
		Wedge
			( ga::Vector const & vert
			, std::pair<ga::Vector, ga::Vector> const & locPair
			)
			: theTriangle(locPair.first, vert, locPair.second)
		{ }

		//! True if this instance is valid
		inline
		bool
		isValid
			() const
		{
			return dat::isValid(theTriangle);
		}

		//! Vector edge from vert to first location
		inline
		ga::Vector
		edge1
			() const
		{
			return { theTriangle[0] - theTriangle[1] };
		}

		//! Vector edge from vert to second location
		inline
		ga::Vector
		edge2
			() const
		{
			return { theTriangle[2] - theTriangle[1] };
		}

		/*! Generalized (G2-subspace) angle second location w.r.t. first.
		 *
		 * For b=edge2(), a=edge1(), then b=vertGangle()*a
		 */
		ga::Spinor
		vertGangle
			() const
		{
			ga::Vector const inv1{ ga::inverse(edge1()) };
			return { ga::logG2(edge2() * inv1) };
		}

		//! Descriptive information about this instance
		std::string
		infoString
			( std::string const & title = {}
			) const
		{
			std::ostringstream oss;
			if (! title.empty())
			{
				oss << title << std::endl;
			}
			if (isValid())
			{
				oss << dat::infoString(theTriangle, "theTriangle");
				oss << std::endl;
				oss << dat::infoString(vertGangle(), "vertGangle");
			}
			else
			{
				oss << " <null>";
			}
			return oss.str();
		}

	}; // Wedge

	//! Generalized 3D vertex angle.
	struct VertGangle
	{
		ga::Spinor const theGangle;

		//! Generalized angle at vertex from first toward second
		explicit
		VertGangle
			( ga::Vector const & vert
			, std::pair<ga::Vector, ga::Vector> const & locPair
			)
			: theGangle{ Wedge(vert, locPair).vertGangle() }
		{
		}

		//! True if this instance is valid
		inline
		bool
		isValid
			() const
		{
			return dat::isValid(theGangle);
		}

		//! Scalar magnitude of scalar part of vertGangle()
		inline
		double
		ratioMag
			() const
		{
			return { ga::magnitude(theGangle.theS) };
		}

		//! Scalar magnitude of bivector part of vertGangle()
		inline
		double
		angleMag
			() const
		{
			return { theGangle.theS.theValue };
		}

		//! Descriptive information about this instance
		std::string
		infoString
			( std::string const & title = {}
			) const
		{
			std::ostringstream oss;
			if (! title.empty())
			{
				oss << title << std::endl;
			}
			if (isValid())
			{
				oss
					<< dat::infoString(theGangle, "theGangle")
					<< " angleMag: " << dat::infoString(angleMag())
					<< " ratioMag: " << dat::infoString(ratioMag())
					;
			}
			else
			{
				oss << " <null>";
			}
			return oss.str();
		}

	}; // VertGangle

	cam::Camera
	fitTo
		( VertGangle const & objVertGangle
		, dat::Extents const & detSize
		)
	{
io::out() << dat::infoString(objVertGangle, "objVertGangle") << std::endl;

		return {};
	}
}

//! Check principal distance calibration
std::string
cam_fit_test1
	()
{
	std::ostringstream oss;

	dat::Extents const detSize{ 2000u, 3000u };
	constexpr double expPD{ 3456. };

	ga::Vector const locRefSta{ 100., 200., 300. };
	ga::Vector const locRefX1{ locRefSta + 100.*ga::e1 };
	ga::Vector const locRefX2{ locRefSta + 100.*(ga::e1 + ga::e2) };
	VertGangle const objVertGangle
		{ locRefSta, std::make_pair(locRefX1, locRefX2) };

	cam::Camera const expCam{ expPD, detSize };
	cam::Camera const gotCam{ fitTo(objVertGangle, detSize) };
	if (! gotCam.nearlyEquals(expCam))
	{
		oss << "Failure of simple calibration test" << std::endl;
		oss << dat::infoString(expCam, "expCam") << std::endl;
		oss << dat::infoString(gotCam, "gotCam") << std::endl;
	}

oss << "Failure: implement this test!" << std::endl;
	return oss.str();
}


}

//! Unit test for cam::fit
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << cam_fit_test0();
	oss << cam_fit_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
