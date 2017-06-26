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
\brief  This file contains unit test for geo::Plane
*/


#include "libgeo/Plane.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libga/Rigid.h"
#include "libgeo/intersect.h"
#include "libgeo/Ray.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
geo_Plane_test0
	()
{
	std::ostringstream oss;
	geo::Plane const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check basic intersection operations
std::string
geo_Plane_test1
	()
{
	std::ostringstream oss;

	// use simple plane as reference
	ga::Vector const nomOrig(0., 0., 0.);
	ga::BiVector const nomDir(0., 0., 3.);
	ga::Vector const nomStaPos( 3., 0.,  2.);
	ga::Vector const nomStaNeg( 3., 0., -2.);
	ga::Vector const nomPIP(-3., 0., 0.);
	ga::Vector const nomAnyPnt(11., -5., 7.);
	ga::Vector const nomAnyPrj(11., -5., 0.);
	ga::Vector const nomAnyRej(0.,  0., 7.);

	// generate general test case
#define EasyData
#	if defined (EasyData)
		ga::Rigid const & xfm = ga::Rigid::identity();
#	else
		ga::Rigid const xfm
			( ga::Vector(-17., 13., -11)
			, ga::BiVector(1.125, -.25, 2.75);
			);
#	endif
	ga::Vector const expOrig(xfm(nomOrig));
	ga::BiVector const expBvDir(ga::unit(xfm.pose()(nomDir)));
	ga::Vector const expStaPos(xfm(nomStaPos));
	ga::Vector const expStaNeg(xfm(nomStaNeg));
	ga::Vector const expPIP(xfm(nomPIP));
	ga::Vector const expAnyPnt(xfm(nomAnyPnt));
	ga::Vector const expAnyPrj(xfm(nomAnyPrj));
	ga::Vector const expAnyRej(xfm(nomAnyRej));

	// check for valid construction
	geo::Plane const plane(expOrig, expBvDir);
	if (! dat::isValid(plane))
	{
		oss << "Failure of good validity test" << std::endl;
		oss << dat::infoString(plane, "plane") << std::endl;
	}

	//
	// check basic properties
	//

	// origin()
	ga::Vector const gotOrig(plane.origin());
	if (! gotOrig.nearlyEquals(expOrig))
	{
		oss << "Failure of origin test" << std::endl;
	}
	// unitNormal()
	ga::Vector const gotNorm(plane.unitNormal());
	ga::Vector const expNorm(-ga::E123 * expBvDir);
	if (! gotNorm.nearlyEquals(expNorm))
	{
		oss << "Failure of normal vector test" << std::endl;
	}
	// projectionOf()
	ga::Vector const gotAnyPrj(plane.projectionOf(expAnyPnt));
	if (! gotAnyPrj.nearlyEquals(expAnyPrj))
	{
		oss << "Failure of point projection test" << std::endl;
		oss << dat::infoString(expAnyPrj, "expAnyPrj") << std::endl;
		oss << dat::infoString(gotAnyPrj, "gotAnyPrj") << std::endl;
	}
	// rejectionTo()
	ga::Vector const gotAnyRej(plane.rejectionTo(expAnyPnt));
	if (! gotAnyRej.nearlyEquals(expAnyRej))
	{
		oss << "Failure of prj/rej reconstruction test" << std::endl;
		oss << dat::infoString(expAnyRej, "expAnyRej") << std::endl;
		oss << dat::infoString(gotAnyRej, "gotAnyRej") << std::endl;
	}
	// offsetTo()
	double const expAnyOffPos{ ga::magnitude(expAnyRej) };
	double const expAnyOffNeg{ -expAnyOffPos };
	double const gotAnyOffPos(plane.offsetTo( expAnyPnt));
	double const gotAnyOffNeg(plane.offsetTo(-expAnyPnt));
	if (! dat::nearlyEquals(gotAnyOffPos, expAnyOffPos))
	{
		oss << "Failure of pos offset test" << std::endl;
	}
	if (! dat::nearlyEquals(gotAnyOffNeg, expAnyOffNeg))
	{
		oss << "Failure of neg offset test" << std::endl;
	}

	//
	// Ray/Plane operations
	//

	ga::Vector fwdDirPos(expPIP - expStaPos);
	ga::Vector fwdDirNeg(expPIP - expStaNeg);
	ga::Vector revDirPos(-fwdDirPos);
	ga::Vector revDirNeg(-fwdDirNeg);

	geo::Ray const fwdRayPos(expStaPos, fwdDirPos);
	geo::Ray const fwdRayNeg(expStaNeg, fwdDirNeg);
	geo::Ray const revRayPos(expStaPos, revDirPos);
	geo::Ray const revRayNeg(expStaNeg, revDirNeg);

	constexpr bool shouldWork{ true };
	std::vector<std::pair<geo::Ray, bool> > const rayCases
		{ { fwdRayPos,   shouldWork }
		, { fwdRayNeg, ! shouldWork }
		, { revRayPos, ! shouldWork }
		, { revRayNeg, ! shouldWork }
		};

	for (size_t ndx{0u} ; ndx < rayCases.size() ; ++ndx)
	{
		geo::Ray const & ray = rayCases[ndx].first;
		bool const & expOkay = rayCases[ndx].second;

		// check intersection of ray and plane
		ga::Vector const & gotPIP(geo::intersect::point(ray, plane));
		bool const gotOkay(gotPIP.nearlyEquals(expPIP));
		if (! (expOkay == gotOkay))
		{
			oss << "Failure of infront/forward test" << std::endl;
			oss << dat::infoString(expOkay, "expOkay") << std::endl;
			oss << dat::infoString(gotOkay, "gotOkay") << std::endl;
			oss << dat::infoString(expPIP, "expPIP") << std::endl;
			oss << dat::infoString(gotPIP, "gotPIP") << std::endl;
			oss << dat::infoString(ray, "ray") << std::endl;
			oss << dat::infoString(plane, "plane") << std::endl;
		}
	}

	return oss.str();
}


}

//! Unit test for geo::Plane
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << geo_Plane_test0();
	oss << geo_Plane_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
