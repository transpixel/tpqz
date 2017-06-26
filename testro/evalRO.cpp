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
\brief  This file contains main application program evalRO
*/


#include "build/version.h"
#include "libio/stream.h"

#include "libga/Rigid.h"
#include "libga/spin.h"
#include "libdat/info.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>


namespace
{
	std::string
	infoString
		( std::vector<ga::Vector> const & vecs
		, std::string const & title
		)
	{
		std::ostringstream oss;
		oss << title << std::endl;
		for (ga::Vector const & vec: vecs)
		{
			oss << dat::infoString(vec) << std::endl;
		}
		return oss.str();
	}

	ga::BiVector
	spinAngleFrom
		( ga::Rigid const & ori
		)
	{
		return { .5 * ori.pose().physicalAngle() };
	}

	struct SpinIt
	{
		ga::Spinor theK1;
		ga::BiVector theB2;
		ga::Spinor theK2;

		explicit
		SpinIt
			( ga::Vector const & vOut
			, ga::Vector const & vIn
			)
			: theK1{ ga::sqrt(vOut * vIn) }
			, theB2{ ga::unit(vOut + vIn) * ga::E123 }
			, theK2(0., theB2)
		{ }

		ga::Spinor
		operator()
			( double const & xi
			) const
		{
			return { std::cos(xi) * theK1 + std::sin(xi) * theK2 };
		}
	};
}

//! Evaluate RO formulae
int
main
	( int const // argc
	, char const * const * const // argv
	)
{
	ga::Vector const vd{ ga::e3 };
#define EasyData
#	if defined(EasyData)
	double const beta{ 1.00 };
	double const alpha{ .50 };
	ga::BiVector const bivPhi( .1, .0, alpha);
	ga::BiVector const bivTheta( .0, .1, -alpha);
#	else
	double const beta{ 1.25 };
	double const alpha{ .07 };
	ga::BiVector const bivPhi( .4, .5, alpha);
	ga::BiVector const bivTheta( .1, .3, -alpha);
#	endif

	ga::Spinor const ePosPhi{ ga::exp(bivPhi) };
	ga::Spinor const ePosTheta{ ga::exp(bivTheta) };
	ga::Spinor const eNegPhi{ ePosPhi.reverse() };
	ga::Spinor const eNegTheta{ ePosTheta.reverse() };

	ga::BiVector const angPhi{ 2. * bivPhi};
	ga::BiVector const angTheta{ 2. * bivTheta};

	ga::Rigid const x1wB(-beta*vd, ga::Pose(angPhi));
	ga::Rigid const x2wB( beta*vd, ga::Pose(angTheta));
	ga::Rigid const xBw1{ x1wB.inverse() };
	ga::Rigid const x2w1{ x2wB * xBw1 };

	ga::BiVector const bivPsi{ spinAngleFrom(x2w1) };
	ga::Spinor const ePosPsi{ ga::exp(bivPsi) };
	ga::Spinor const eNegPsi{ ePosPsi.reverse() };
	ga::Vector const b1{ .5 * x2w1.location() };
	ga::Vector const b2{ x2w1.pose()(b1) };


	std::vector<ga::Vector> const xbs
		{ ga::vZero, ga::e1, ga::e2, ga::e3 };

	std::vector<ga::Vector> const x1s
		{ ((ePosPhi * (xbs[0] + beta*vd) * eNegPhi).theV)
		, ((ePosPhi * (xbs[1] + beta*vd) * eNegPhi).theV)
		, ((ePosPhi * (xbs[2] + beta*vd) * eNegPhi).theV)
		, ((ePosPhi * (xbs[3] + beta*vd) * eNegPhi).theV)
		};
	std::vector<ga::Vector> const x2s
		{ ((ePosTheta * (xbs[0] - beta*vd) * eNegTheta).theV)
		, ((ePosTheta * (xbs[1] - beta*vd) * eNegTheta).theV)
		, ((ePosTheta * (xbs[2] - beta*vd) * eNegTheta).theV)
		, ((ePosTheta * (xbs[3] - beta*vd) * eNegTheta).theV)
		};

	for (size_t nn{0u} ; nn < 4u ; ++nn)
	{
		ga::Vector const & x1 = x1s[nn];
		ga::Vector const & xb = xbs[nn];
		ga::Vector const & x2 = x2s[nn];

		ga::Vector const funcA
			{ (eNegPhi * x1 * ePosPhi).theV - beta*vd - xb };

		ga::Vector const tmpB1
			{ x1 - (2. * beta * ePosPhi*vd*eNegPhi).theV };
		ga::Vector const funcB
			{ (ePosTheta*eNegPhi * (tmpB1) * ePosPhi*eNegTheta).theV - x2};

		/*
		ga::Vector const funcC
			{ vd - (eNegTheta*ePosPsi*b1*eNegPsi*ePosTheta).theV };
		*/

		ga::Vector const funcC
			{ vd - (eNegTheta*b2*ePosTheta).theV };

		ga::Vector const funcD
			{ vd - (eNegPhi*b1*ePosPhi).theV };

		double const err
			{ ga::magnitude(funcA)
			+ ga::magnitude(funcB)
			+ ga::magnitude(funcC)
			+ ga::magnitude(funcD)
			};

		io::out() << std::endl;
		io::out() << nn << std::endl;
		io::out() << dat::infoString(funcA, "funcA") << std::endl;
		io::out() << dat::infoString(funcB, "funcB") << std::endl;
		io::out() << dat::infoString(funcC, "funcC") << std::endl;
		io::out() << dat::infoString(funcD, "funcD") << std::endl;
		io::out() << dat::infoString(err, "err") << std::endl;
	}

	SpinIt const spinPhi(vd, b1);

	constexpr double delta{ math::pi/1024. };
	std::ofstream ofs("xialphasum.dat");
	for (double xi{0.} ; xi <= math::halfPi ; xi += delta)
	{
		ga::BiVector const bivXiPhi{ ga::spin::physicalAngleFrom(spinPhi(xi)) };

		ga::Vector const x1(1., 1., 1.);
		ga::Vector const vOut{ (ePosPsi*(b1-x1)*eNegPsi).theV };
		ga::Vector const vIn{ vd - (eNegPhi*x1*ePosPhi).theV };
		SpinIt const spinTheta(vOut, vIn);

	//	io::out() << dat::infoString(xi, "xi") << std::endl;
		for (double alpha{0.} ; alpha <= math::halfPi ; alpha += delta)
		{
			ga::BiVector const bivXiTheta
				{ ga::spin::physicalAngleFrom(spinTheta(alpha)) };

			double const sum3{ bivXiPhi[2] + bivXiTheta[2] };
			if (std::abs(sum3) < 1./128.)
			{
				ofs
					<< "xi,alpha:"
					<< " " << dat::infoString(xi)
					<< " " << dat::infoString(alpha)
					<< " " << dat::infoString(bivXiPhi, "bivXiPhi")
					<< " " << dat::infoString(bivXiTheta, "bivXiTheta")
					<< " " << dat::infoString(sum3, "sum3")
					<< std::endl;
			}
		}
	}

	// cross check generated data

	/*
	std::vector<ga::Vector> const r1s
		{ x1wB(xbs[0]), x1wB(xbs[1]), x1wB(xbs[2]), x1wB(xbs[3]) };
	std::vector<ga::Vector> const r2s
		{ x2wB(xbs[0]), x2wB(xbs[1]), x2wB(xbs[2]), x2wB(xbs[3]) };
	std::vector<ga::Vector> const z2s
		{ x2w1(r1s[0]), x2w1(r1s[1]), x2w1(r1s[2]), x2w1(r1s[3]) };
	std::vector<ga::Vector> const w2s
		{ ((ePosPsi * (x1s[0] - 2.*beta*b1) * eNegPsi).theV)
		, ((ePosPsi * (x1s[1] - 2.*beta*b1) * eNegPsi).theV)
		, ((ePosPsi * (x1s[2] - 2.*beta*b1) * eNegPsi).theV)
		, ((ePosPsi * (x1s[3] - 2.*beta*b1) * eNegPsi).theV)
		};


	io::out() << infoString(xbs, "xbs") << std::endl;
	io::out() << infoString(r1s, "r1s") << std::endl;
	io::out() << infoString(r2s, "r2s") << std::endl;
	io::out() << infoString(x1s, "x1s") << std::endl;
	io::out() << infoString(x2s, "x2s") << std::endl;
	io::out() << infoString(z2s, "z2s") << std::endl;
	io::out() << infoString(w2s, "w2s") << std::endl;
	*/

	return 0;
}
