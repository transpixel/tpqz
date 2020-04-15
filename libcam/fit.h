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

#ifndef cam_fit_INCL_
#define cam_fit_INCL_

/*! \file
\brief Declarations for cam::fit
*/


#include "libcam/Camera.h"
#include "libdat/Extents.h"
#include "libdat/Spot.h"
#include "libmath/math.h"
#include "libmath/QuadEq.h"

#include "libio/stream.h"
#include "libdat/info.h"

#include <utility>
#include <vector>


namespace cam
{

/*! \brief Utilities/functions for fitting camera models to measurement data.

\par Example
\dontinclude testcam/ufit.cpp
\skip ExampleStart
\until ExampleEnd
*/

namespace fit
{

	std::vector<double>
	principalDistanceFor
		( std::pair<dat::Spot, dat::Spot> const & meaPair
		, double const & beta
		, dat::Extents const & detSize
		)
	{
		std::vector<double> fitPDs;
		fitPDs.reserve(2u);
		double const gam{ std::cos(beta) };
		double const gamSq{ math::sq(gam) };

		dat::Spot const & m1 = meaPair.first;
		dat::Spot const & m2 = meaPair.second;
		double const m1Sq{ dat::magSq(m1) };
		double const m2Sq{ dat::magSq(m2) };

		using dat::operator-;
		double const rSq{ dat::magSq(m2 - m1) };
		double const r4{ math::sq(rSq) };

		double const etaRow{ .5 * double(detSize.high()) };
		double const etaCol{ .5 * double(detSize.wide()) };
		double const rho1Sq{ m1[0]*etaRow + m1[1]*etaCol };
		double const rho2Sq{ m2[0]*etaRow + m2[1]*etaCol };

//ok io::out() << dat::infoString(r4, "r4") << std::endl;
//ok io::out() << dat::infoString(rho1Sq, "rho1Sq") << std::endl;
//ok io::out() << dat::infoString(rho2Sq, "rho2Sq") << std::endl;

		double const nuSq{ .5 * (m1Sq + m2Sq) - (rho1Sq + rho2Sq) };
		double const nu4{ math::sq(nuSq) };

		double const om4{ (m1Sq - 2.*rho1Sq) * (m2Sq - 2.*rho2Sq) };

//ok io::out() << dat::infoString(nuSq, "nuSq") << std::endl;
//ok io::out() << dat::infoString(nu4, "nu4") << std::endl;
//ok io::out() << dat::infoString(om4, "om4") << std::endl;

		double const ka{ 4. - gamSq };
		double const kb{ 4.*nuSq - nuSq*gamSq - 2.*rSq };
		double const kc{ 4.*nu4 - 4.*nuSq*rSq + r4 - om4*gamSq };

io::out() << dat::infoString(ka, "ka") << std::endl;
io::out() << dat::infoString(kb, "kb") << std::endl;
io::out() << dat::infoString(kc, "kc") << std::endl;


		math::QuadEq const quad(ka, 2.*kb, kc);
io::out() << dat::infoString(quad, "quad") << std::endl;

		double const hSq1{ quad.realRootMax() };
		double const hSq2{ quad.realRootMin() };

double const chkEqQuad1{ ka*math::sq(hSq1) + 2*kb*hSq1 + kc };
io::out() << dat::infoString(chkEqQuad1, "chkEqQuad1") << std::endl;
double const chkEqQuad2{ ka*math::sq(hSq2) + 2*kb*hSq2 + kc };
io::out() << dat::infoString(chkEqQuad2, "chkEqQuad2") << std::endl;

		double const eta1
			{ std::sqrt(hSq1 - math::sq(etaRow) - math::sq(etaCol)) };
		double const eta2
			{ std::sqrt(hSq2 - math::sq(etaRow) - math::sq(etaCol)) };

//io::out() << dat::infoString(quad, "quad") << std::endl;
io::out() << dat::infoString(eta1, "eta1") << std::endl;
io::out() << dat::infoString(eta2, "eta2") << std::endl;


		if (dat::isValid(hSq1) && (0. < hSq1))
		{
			fitPDs.emplace_back(std::sqrt(eta1));
		}
		if (dat::isValid(hSq2) && (0. < hSq2))
		{
			fitPDs.emplace_back(std::sqrt(eta2));
		}

		return fitPDs;
	}


} // fit

} // cam

// Inline definitions
// #include "libcam/fit.inl"

#endif // cam_fit_INCL_

