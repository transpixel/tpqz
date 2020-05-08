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
\brief Definitions for cam::fit
*/


#include "libcam/fit.h"

#include "libdat/ops.h"
#include "libmath/math.h"
#include "libmath/QuadEq.h"

#include "libio/stream.h"
#include "libdat/info.h"


namespace cam
{

namespace fit
{

namespace normalized
{
	//! Compare solution values for consistency with input data
	struct Checker
	{
		std::pair<dat::Spot, dat::Spot> const & theMeaPair;
		double const & theExpGam;
		dat::Spot const & theDetCenter;

		//! Angle indicator: gamma = 2*cos(beta)
		double
		gammaFor
			( double const & gotPD
			) const
		{
			double gam{ dat::nullValue<double>() };
			if (math::eps < gotPD) // consider only positive values
			{
				using dat::operator-;
				double const radSq1
					{ dat::magSq(theMeaPair.first - theDetCenter) };
				double const radSq2
					{ dat::magSq(theMeaPair.second - theDetCenter) };
				double const pdSq{ math::sq(gotPD) };
				double const muSq1{ pdSq + radSq1 };
				double const muSq2{ pdSq + radSq2 };
				double const delSq
					{ dat::magSq(theMeaPair.second - theMeaPair.first) };
				// delSq = muSq1 + muSq2 - mu1*mu2*(2.*cosBeta)
				// gamma = 2*cosBeta
				// delSq - muSq1 - muSq2 = - mu1*mu2*gamma
				// (-delSq + muSq1 + muSq2)/(mu1*mu2) = gamma
				gam = (muSq1 + muSq2 - delSq) / std::sqrt(muSq1 * muSq2);
			}
			return gam;
		}

		//! True if gotPD produces vertex angle of theExpGam within tol
		bool
		isConsistent
			( double const & gotPD
			, double const & tol = { math::eps }
			) const
		{
			double const & expGam = theExpGam;
			double const gotGam{ gammaFor(gotPD) };
			return dat::nearlyEquals(gotGam, expGam, tol);
		}

	}; // Checker

	//! Perform detailed computations (assuming data are reasonably normalized)
	std::vector<double>
	principalDistanceFor
		( std::pair<dat::Spot, dat::Spot> const & meaPair
		, double const & angleMag
		, dat::Spot const & detCenter
		)
	{
		std::vector<double> fitPDs;
		if ( dat::isValid(detCenter)
		  && dat::isValid(angleMag)
		  && dat::isValid(meaPair.first)
		  && dat::isValid(meaPair.second)
		   )
		{
			// compute various intermediate quantities
			double const gam{ 2. * std::cos(angleMag) };
			double const gamSq{ math::sq(gam) };

			dat::Spot const & m1 = meaPair.first;
			dat::Spot const & m2 = meaPair.second;
			double const m1Sq{ dat::magSq(m1) };
			double const m2Sq{ dat::magSq(m2) };

			using dat::operator-;
			double const rSq{ dat::magSq(m2 - m1) };
			double const r4{ math::sq(rSq) };

			double const & etaRow = detCenter[0];
			double const & etaCol = detCenter[1];
			double const rho1Sq{ m1[0]*etaRow + m1[1]*etaCol };
			double const rho2Sq{ m2[0]*etaRow + m2[1]*etaCol };

			double const nuSq{ .5 * (m1Sq + m2Sq) - (rho1Sq + rho2Sq) };
			double const nu4{ math::sq(nuSq) };

			double const om4{ (m1Sq - 2.*rho1Sq) * (m2Sq - 2.*rho2Sq) };

			// form quadratic equation in h^2
			double const ka{ 4. - gamSq };
			double const kb{ 4.*nuSq - nuSq*gamSq - 2.*rSq };
			double const kc{ 4.*nu4 - 4.*nuSq*rSq + r4 - om4*gamSq };
			math::QuadEq const quad(ka, 2.*kb, kc);

			// get unique roots
			// NOTE: this algorithms is not necessarily numerically stable
			// (no attention given to order of operations)
			static double const tol{ std::sqrt(math::eps) };
			std::vector<double> const hSqs{ quad.realUniqueRoots(tol) };
			if (! hSqs.empty())
			{
				fitPDs.reserve(hSqs.size());
				Checker const config{ meaPair, gam, detCenter };
				for (double const & hSq : hSqs)
				{
					// extract component perpendicular to detector
					double const detRadSq
						{ math::sq(etaRow) + math::sq(etaCol) };
					double const etaSq{ hSq - detRadSq };
					if (0. < etaSq)
					{
						double const gotPD{ std::sqrt(etaSq) };
						if (config.isConsistent(gotPD, tol))
						{
							fitPDs.emplace_back(gotPD);
						}
					}
				}
			}
		}

		return fitPDs;
	}
}

std::vector<double>
principalDistanceFor
	( std::pair<dat::Spot, dat::Spot> const & meaPair
	, double const & angleMag
	, dat::Extents const & detSize
	)
{
	std::vector<double> fitPDs;
	if ( dat::isValid(detSize)
	  && dat::isValid(angleMag)
	  && dat::isValid(meaPair.first)
	  && dat::isValid(meaPair.second)
	   )
	{
		// compute normalization factor
		double const restPerNorm{ dat::diagonalMag(detSize) };
		double const normPerRest{ 1. / restPerNorm };

		// normalize input data
		using dat::operator*;
		dat::Spot const normCenter{ normPerRest * dat::centerOf(detSize) };
		std::pair<dat::Spot, dat::Spot> const normMeaPair
			{ normPerRest * meaPair.first
			, normPerRest * meaPair.second
			};

		// fit principal distance to normalized data
		std::vector<double> const normPDs
			{ normalized::principalDistanceFor
				(normMeaPair, angleMag, normCenter)
			};

		// restore principal distance magnitudes
		if (! normPDs.empty())
		{
			fitPDs.reserve(normPDs.size());
			for (double const & normPD : normPDs)
			{
				fitPDs.emplace_back(restPerNorm * normPD);
			}
		}
	}
	return fitPDs;
}


} // fit

} // cam

