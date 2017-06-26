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
\brief  This file contains unit test for ro::ArgsBaseZ
*/


#include "libro/ArgsBaseZ.h"

#include "libdat/array.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"
#include "libmath/NumDeriv.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
ro_ArgsBaseZ_test0
	()
{
	std::ostringstream oss;
	ro::ArgsBaseZ const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

//! Check consistency of ctors
std::string
ro_ArgsBaseZ_test1
	()
{
	std::ostringstream oss;

	constexpr double a3{ .5 };
	ga::BiVector const expPhi(-2., 3., -a3);
	ga::BiVector const expTheta(.2, -.3, a3);
	std::array<double, 5u> const pptta
		{{ expPhi[0], expPhi[1], expTheta[0], expTheta[1], a3 }};

	ro::ArgsBaseZ const ctor1(expPhi, expTheta);
	ro::ArgsBaseZ const ctor0(pptta);
	if (! ctor1.nearlyEquals(ctor0))
	{
		oss << "Failure of ctor match test: 1,0" << std::endl;
	}

	// copy and test values
	ro::ArgsBaseZ const args{ ctor1 };
	ga::BiVector const gotPhi{ args.phiBiv() };
	ga::BiVector const gotTheta{ args.thetaBiv() };
	if (! gotPhi.nearlyEquals(expPhi))
	{
		oss << "Failure of phi retrieval test" << std::endl;
	}
	if (! gotTheta.nearlyEquals(expTheta))
	{
		oss << "Failure of theta retrieval test" << std::endl;
	}

	return oss.str();
}

//! Check orientation interactions
std::string
ro_ArgsBaseZ_test2
	()
{
	std::ostringstream oss;

	// keep within "small" range to avoid phase wrap in reconstruction
	// e.g. angle mags less than pi/2 or so
	constexpr double a3{ 1. };
	ga::BiVector const expPhi  ( .2,  .3, -a3);
	ga::BiVector const expTheta( .4,  .5,  a3);

	// construct args and get resulting orientations
	constexpr double bMag{ 1. };
	ro::ArgsBaseZ const expArgs(expPhi, expTheta);
	ga::Rigid const gotOri1{ expArgs.rigid1w0(bMag) };
	ga::Rigid const gotOri2{ expArgs.rigid2w0(bMag) };
	if ((! gotOri1.isValid()) || (! gotOri2.isValid()))
	{
		oss << "Failure of rigid[21]w0 test" << std::endl;
	}

	// reconstruct args from orientations
	ro::ArgsBaseZ const gotArgs{ ro::ArgsBaseZ::from(gotOri1, gotOri2) };

	// check consistency
	if (! gotArgs.nearlyEquals(expArgs))
	{
		oss << "Failure of reconstruction test" << std::endl;
		oss << dat::infoString(expArgs, "expArgs") << std::endl;
		oss << dat::infoString(gotArgs, "gotArgs") << std::endl;
	}

	// check anti-symmetry condition
	double const gotSumA3{ (gotArgs.phiBiv() + gotArgs.thetaBiv())[2] };
	if (! dat::nearlyEquals(gotSumA3, 0.))
	{
		oss << "Failure of alpha3 reconstruction symmetry test" << std::endl;
	}

	return oss.str();
}

//! Check derivatives
std::string
ro_ArgsBaseZ_test3
	()
{
	std::ostringstream oss;

	constexpr size_t na{ dat::nullValue<size_t>() };
//	std::array<double, 5u> const vals{{ 0., 0., 0., 0., 0. }};
	std::array<double, 5u> const vals{{ 1.1, 1.2, 2.1, 2.2, 0.3 }};
	std::array<size_t, 5u> const pNdxs{{ 0u, 1u, na, na, 2u }};
	std::array<size_t, 5u> const tNdxs{{ na, na, 0u, 1u, 2u }};

	// perturbation for numerical differentation
	constexpr double dp{ math::eps };
	constexpr double perRun{ 1. / (2. * dp) };
	constexpr double tol{ dp };

	// evaluate args at nominal values
	ro::ArgsBaseZ const args(vals);

	// compute phi perturbations
	for (size_t ndx{0u} ; ndx < 5u ; ++ndx)
	{
		std::array<double, 5u> del{{ 0., 0., 0., 0., 0. }};
		del[ndx] = dp;

		// evaluate args at perturbed values
		using dat::operator-;
		using dat::operator+;
		ro::ArgsBaseZ const nArgs(vals - del);
		ro::ArgsBaseZ const pArgs(vals + del);

		// check dPhi w.r.t. ndx
		size_t const & pNdx = pNdxs[ndx];
		if (dat::isValid(pNdx))
		{
			ga::BiVector const nPhi{ nArgs.phiBiv() };
			ga::BiVector const pPhi{ pArgs.phiBiv() };
			ga::BiVector const expdpx{ perRun * (pPhi - nPhi) };
			ga::BiVector const gotdpx{ args.phiDot(pNdx) };
			if (! gotdpx.nearlyEquals(expdpx, tol))
			{
				oss << "Failure of dpx test: phi pNdx = " << pNdx << '\n';
				oss << "exp dPhi" << dat::infoString(expdpx) << '\n';
				oss << "got dPhi" << dat::infoString(gotdpx) << '\n';
			}
		}

		// check dTheta w.r.t. ndx
		size_t const & tNdx = tNdxs[ndx];
		if (dat::isValid(tNdx))
		{
			ga::BiVector const nTheta{ nArgs.thetaBiv() };
			ga::BiVector const pTheta{ pArgs.thetaBiv() };
			ga::BiVector const expdtx{ perRun * (pTheta - nTheta) };
			ga::BiVector const gotdtx{ args.thetaDot(tNdx) };
			if (! gotdtx.nearlyEquals(expdtx, tol))
			{
				oss << "Failure of dtx test: theta tNdx = " << tNdx << '\n';
				oss << "exp dTheta " << dat::infoString(expdtx) << '\n';
				oss << "got dTheta " << dat::infoString(gotdtx) << '\n';
			}
		}
	}

	return oss.str();
}

}

//! Unit test for ro::ArgsBaseZ
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ro_ArgsBaseZ_test0();
	oss << ro_ArgsBaseZ_test1();
	oss << ro_ArgsBaseZ_test2();
	oss << ro_ArgsBaseZ_test3();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
