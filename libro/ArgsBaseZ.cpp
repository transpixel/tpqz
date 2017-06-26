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
\brief Definitions for ro::ArgsBaseZ
*/


#include "libro/ArgsBaseZ.h"

#include "libio/sprintf.h"
#include "libmath/angle.h"
#include "libmath/Extreme.h"
#include "libmath/root.h"
#include "libro/ops.h"

#include <fstream>
#include <sstream>
#include <vector>

namespace
{
	inline
	bool
	containsRoot
		( std::pair<double, double> const & ab
		)
	{
		// NOTE: include 0 in case one of the members itself is the root
		return { (ab.first*ab.second) <= 0. };
	}

	//! True if start member of aPair is nearly the same as brackets
	bool
	isNewBracket
		( std::pair<double, double> & aPair
		, std::vector<std::pair<double, double> > & brackets
		, double const & delta
		)
	{
		bool isNew{ true };
		double const find{ math::principalAngle(aPair.first) };
		double const tol{ .25 * delta };
		for (std::pair<double, double> const & bracket : brackets)
		{
			double const start{ math::principalAngle(bracket.first) };
			if (math::sameRotation(start, find, tol))
			{
				isNew = false;
				break;
			}
		}
		return isNew;
	}

	//! (likely) Angle values with bracket meritFunc roots
	template <typename Func>
	std::vector<std::pair<double, double> >
	rootBrackets
		( Func const & meritFunc
		)
	{
		std::vector<std::pair<double, double> > brackets;

		constexpr double delA{ math::pi / 16. }; // larger?
		constexpr double maxA{ math::halfPi + delA };
		constexpr double minA{ -maxA };
		std::pair<double, double> aPair{ minA, minA + delA };
		std::pair<double, double> fPair
			{ meritFunc(aPair.first), meritFunc(aPair.second) };
		for (double angle{minA + 2.*delA} ; angle <= maxA ; angle += delA)
		{
			// check if root is bracketed
			if (containsRoot(fPair))
			{
				// check for likely phase wrapping
				double const & f1 = fPair.first;
				double const & f2 = fPair.second;
				double const fDelta{ std::abs(f2 - f1) };
				constexpr double bigJump{ (7./8.)*math::halfPi };
				bool const phaseWrapped( bigJump < fDelta);
				if (! phaseWrapped)
				{
					// enforce legitimate bracket
					assert(dat::isValid(aPair));
					assert(dat::isValid(fPair));
					assert(containsRoot(fPair));

					// avoid duplication due to phase wrap at ends
					if (isNewBracket(aPair, brackets, delA))
					{
						brackets.emplace_back(aPair);
					}
				}
			}

			// slide bracket to next candidate
			double const metric{ meritFunc(angle) };
			aPair = { aPair.second, angle };
			fPair = { fPair.second, metric };
		}

		return brackets;
	}

	using Bracket = std::pair<double, double>;

	//! Omega parm value producing which satisfy (e^I * b * e^-I = -t12)
	template <typename Func>
	std::vector<double>
	angleRootsFor
		( Func const & meritFunc
		, std::vector<Bracket> const & brackets
		)
	{
		std::vector<double> angles{}; // probably should be an array
		angles.reserve(2u);

		// perform rigorous root finding
		for (Bracket const & bracket : brackets)
		{
			double const root{ math::root::viaRidder(meritFunc, bracket) };
			double const angle{ math::principalAngle(root) };
			angles.emplace_back(angle);
		}

		return angles;
	}
}

namespace
{
	//! Optionally negated spinor such that scalar grade component is positive
	inline
	ga::Spinor
	principalSpinor
		( ga::Spinor const anySpin
		)
	{
		ga::Spinor posSpin{ anySpin };
		if (posSpin.isValid() && (posSpin.theS.theValue < 0.))
		{
			posSpin = -1. * anySpin;
		}
		return posSpin;
	}

	struct ModelSpecs
	{
		ga::Vector const theBaseDir;
		ga::BiVector const theBasePlane;

		explicit
		ModelSpecs
			( ga::Vector const & bHatInFit
			)
			: theBaseDir{ bHatInFit }
			, theBasePlane{ ga::E123 * theBaseDir }
		{ }
	};

	//! Functor to evalute baseline rotation anti-symmetry misclosure.
	struct KappaMetric
	{
		ga::Spinor const theExpP;
		ga::Spinor const theExpK;
		ModelSpecs const theSpecs;

		explicit
		KappaMetric
			( ga::Spinor const & eP
			, ga::Spinor const & eK
			, ga::Vector const & bHatInFit
			)
			: theExpP{ eP }
			, theExpK{ eK }
			, theSpecs(bHatInFit)
		{ }

		static
		KappaMetric
		from
			( ro::OriPair const & roPairInRef
			, ga::Vector const & bHatInFit
			)
		{
			// access components of input data
			ga::Rigid const ori2w1{ ro::relative2w1(roPairInRef) };
			ga::Spinor const & eK = ori2w1.pose().theSpin;
			ga::Vector const t12{ ga::unit(ori2w1.location()) };

			ga::Vector const bInRef{ ga::unit(t12) };
			ga::Vector const & bInTmp = bHatInFit;

			ga::Spinor const eP{ ga::sqrt(bInTmp*bInRef) };

			return KappaMetric(eP, eK, bHatInFit);
		}

		ga::Spinor
		spinIk
			( double const & kappa
			) const
		{
			ga::BiVector const & basePlane = theSpecs.theBasePlane;
			ga::Spinor const spin
				{ theExpP.reverse() * ga::exp(-kappa*basePlane) };
			return principalSpinor(spin);
		}

		ga::Spinor
		spinJk
			( double const & kappa
			) const
		{
			ga::Spinor const spin{ theExpK * spinIk(kappa) };
			return principalSpinor(spin);
		}

		ga::BiVector
		angleIk
			( double const & kappa
			) const
		{
			return { ga::spin::argAngleFrom(spinIk(kappa)) };
		}

		ga::BiVector
		angleJk
			( double const & kappa
			) const
		{
			return { ga::spin::argAngleFrom(spinJk(kappa)) };
		}

		double
		rmsAzim
			( double const & kappa
			) const
		{
			ga::BiVector const Ik{ angleIk(kappa) };
			ga::BiVector const Jk{ angleJk(kappa) };
			return math::hypot(Ik[2], Jk[2]);
		}

		double
		operator()
			( double const kappa
			) const
		{
			ga::BiVector const Ik{ angleIk(kappa) };
			ga::BiVector const Jk{ angleJk(kappa) };
			// assert(std::abs(Ik[2]) <= math::halfPi);
			// assert(std::abs(Jk[2]) <= math::halfPi);
			return { Ik[2] + Jk[2] };
		}
	};

	// for debugging
	//! Save a collection of values to data file
	void
	saveMeritValues
		( KappaMetric const & meritFunc
		, std::string const & fpath = "azimMet.dat"
		)
	{
		std::ofstream ofs(fpath);
		constexpr size_t nSamps{ 1u*1024u };
		constexpr double kappaBeg{ -2.*math::halfPi };
		constexpr double kappaEnd{  2.*math::halfPi };
		constexpr double dw{ (kappaEnd - kappaBeg) / double(nSamps) };
		for (double kappa{kappaBeg} ; kappa < kappaEnd ; kappa += dw)
		{
			double const merit{ meritFunc(kappa) };
			ofs
				<< "kappa,merit:"
				<< " " << dat::infoString(kappa)
				<< " " << io::sprintf("%21.18f", merit)
				<< std::endl;
		}
	}

	//! Return the kappa value with smallest kappa-deviation RMS value
	double
	bestKappa
		( std::vector<double> const & kappas
		, KappaMetric const & meritFunc
		)
	{
		//! Functor to compare two values based on rms metric
		struct IsBetter
		{
			KappaMetric const * theMet;

			bool
			operator()
				( double const kappa1
				, double const kappa2
				) const
			{
				return { theMet->rmsAzim(kappa1) < theMet->rmsAzim(kappa2) };
			}
		};

		// find kappa value which produces best rms state
		math::Extreme<std::vector<double>::const_iterator> const extrema2
			(kappas.begin(), kappas.end(), IsBetter{& meritFunc});
		return extrema2.theExVal;
	}

}


namespace ro
{

ga::Rigid
ArgsBaseZ :: baseFrameWrtRef
	( ga::Rigid const & ori1wRef
	, ga::Rigid const & ori2wRef
	) const
{
	ga::Rigid oriFitWrtRef;

	double const bMag
		{ ga::magnitude(ori2wRef.location() - ori1wRef.location()) };
	if (dat::isValid(bMag) && (math::eps < bMag))
	{
		// get current (scaled) orientations w.r.t. base frame
		ga::Rigid const ori1wFit{ rigid1w0(bMag) };
		ga::Rigid const ori2wFit{ rigid2w0(bMag) };

		// compute answer starting from first frame
		ga::Rigid const oriFitWrtRef_1
			{ ro::rigidModelWrtRef(ori1wFit, ori1wRef) };

		// verify self consistency by checking computation from second frame
		constexpr bool verifyConsistency{ false }; // e.g. for dev work
		if (verifyConsistency)
		{
			ga::Rigid const chkFitWrtRef
				{ ro::rigidModelWrtRef(ori2wFit, ori2wRef) };
			if (chkFitWrtRef.nearlyEquals(oriFitWrtRef))
			{
				oriFitWrtRef = oriFitWrtRef_1;
			}
		}
		else
		{
			oriFitWrtRef = oriFitWrtRef_1;
		}
	}

	return oriFitWrtRef;
}

// static
ArgsBaseZ
ArgsBaseZ :: from
	( ga::Rigid const & ori1wRef
	, ga::Rigid const & ori2wRef
	)
{
	ArgsBaseZ parms;

	// check if there's anything meaningful to do (both valid, legit base)
	OriPair const roPairInRef{ ori1wRef, ori2wRef };
	double const bMag{ gapBetween(roPairInRef) };
	if (dat::isValid(bMag) && (math::eps < bMag))
	{
		ga::Vector const bHat{ ArgsBaseZ::baseVectorDir() };
		// configure merit function for evaluating anti-symmetric quality
		KappaMetric const meritFunc
			{ KappaMetric::from(roPairInRef, bHat) };
		// saveMeritValues(meritFunc);

		// find free parameter (kappa) value for which satisfies merit func
		std::vector<Bracket> const brackets{ rootBrackets(meritFunc) };
		std::vector<double> const kappas{ angleRootsFor(meritFunc, brackets) };

		// set angle attitudes which are anti-symmetric in azimuth
		double const k0{ bestKappa(kappas, meritFunc) };
		ga::BiVector const phi{ 2. * meritFunc.angleIk(k0)};
		ga::BiVector const theta{ 2. * meritFunc.angleJk(k0)};

		parms = ArgsBaseZ(phi, theta);
	}

	return parms;
}

bool
ArgsBaseZ :: nearlyEquals
	( ArgsBaseZ const & other
	, double const & tol
	) const
{
	return
		(  dat::nearlyEquals(thePhi12, other.thePhi12, tol)
		&& dat::nearlyEquals(theTheta12, other.theTheta12, tol)
		&& dat::nearlyEquals(theAlpha3, other.theAlpha3, tol)
		);
}

std::string
ArgsBaseZ :: infoString
	( std::string const & title
	, std::string const & fmt
	) const
{
	std::ostringstream oss;
	if (! title.empty())
	{
		oss << title << " ";
	}
	if (isValid())
	{
		oss 
			<< " " << io::sprintf(fmt, thePhi12[0])
			<< " " << io::sprintf(fmt, thePhi12[1])
			<< " " << io::sprintf(fmt, theTheta12[0])
			<< " " << io::sprintf(fmt, theTheta12[1])
			<< " " << io::sprintf(fmt, theAlpha3)
			;
	}
	return oss.str();
}

} // ro

