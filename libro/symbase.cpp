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
\brief Declarations for ro::symbase
*/


#include "libro/symbase.h"

#include "libdat/info.h"
#include "libio/sprintf.h"
#include "libio/stream.h"

#include <cassert>
#include <limits>


namespace ro
{
namespace symbase
{

	namespace // pair operations
	{
		//! Pose components of oriPair
		std::pair<ga::Pose, ga::Pose>
		attPairFrom
			( std::pair<ga::Rigid, ga::Rigid> const & oriPair
			)
		{
			return { oriPair.first.pose(), oriPair.second.pose() };
		}

		//! EO pair from attitude pair and separation vector
		std::pair<ga::Rigid, ga::Rigid>
		oriPairFor
			( ga::Vector const & baseVec
			, std::pair<ga::Pose, ga::Pose> const & attPair
			)
		{
			ga::Vector const halfBase{ .5 * baseVec };
			return
				{ ga::Rigid(-halfBase, attPair.first)
				, ga::Rigid( halfBase, attPair.second)
				};
		}

		//! Composition of attitude pair
		template <typename Type>
		std::pair<Type, Type>
		compositeOf
			( Type const & attCwB
			, std::pair<Type, Type> const & apPairBwA
			)
		{
			return { attCwB*apPairBwA.first, attCwB*apPairBwA.second };
		}

		//! Composition of attitude pair
		template <typename Type>
		std::pair<Type, Type>
		compositeOf
			( std::pair<Type, Type> const & apPairCwB
			, Type const & attBwA
			)
		{
			return { apPairCwB.first*attBwA, apPairCwB.second*attBwA };
		}
	}

	namespace
	{
		//! Symmetric model baseline direction
		inline
		ga::Vector
		bInSym
			()
		{
			return ga::e3;
		}

		//! Unit mag direction from A to B
		ga::Vector
		baseVector
			( std::pair<ga::Rigid, ga::Rigid> const & roPair
			)
		{
			return { roPair.second.location() - roPair.first.location() };
		}


		//! Unit mag direction from A to B
		ga::Vector
		baseDir
			( std::pair<ga::Rigid, ga::Rigid> const & roPair
			)
		{
			return ga::unit(baseVector(roPair));
		}

		//! Orientation of intermediate base-plane aligned axial frame 
		ga::Rigid
		oriTmpWrtRefFor
			( ga::Vector const & bInRef
			)
		{
			using namespace ga;
			Vector const bInTmp{ bInSym() }; // direction is same
			Spinor const spinTmpWrtRef{ spin::between(bInRef, bInTmp) };
			return Rigid(ga::vZero, Pose(spinTmpWrtRef));
		}

		//! Transform both orientations into intermediate axial frame
		std::pair<ga::Rigid, ga::Rigid>
		pairTmpWrtRefFor
			( std::pair<ga::Rigid, ga::Rigid> const & roPairWrtRef
			)
		{
			std::pair<ga::Rigid, ga::Rigid> roPairWrtTmp;

			ga::Vector const bInRef{ baseDir(roPairWrtRef) };
			if (bInRef.isValid())
			{
				ga::Rigid const oriTmpWrtRef{ oriTmpWrtRefFor(bInRef) };
				if (oriTmpWrtRef.isValid())
				{
					roPairWrtTmp = compositeOf(oriTmpWrtRef, roPairWrtRef);
				}
				else
				{
					assert(! "implement special case oriTmpWrtRef");
				}
			}
			return roPairWrtTmp;
		}

		//! Attitude of symmetric frame w.r.t. axial frame
		ga::Pose
		attSymWrtTmpFor
			( std::pair<ga::Pose, ga::Pose> const & attPair
			)
		{
			using namespace ga;

			// determine physical rotation angles in axial frame
			BiVector const bivAInTmp{ attPair.first.physicalAngle() };
			BiVector const bivBInTmp{ attPair.second.physicalAngle() };

			// compute mean azimuth angle w.r.t baseplane
			BiVector const basePlane{ -ga::E123 * bInSym() };
			double const magA{ -ga::dot(bivAInTmp, basePlane).theValue };
			double const magB{ -ga::dot(bivBInTmp, basePlane).theValue };
			// NOTE: halve first, then add to avoid phase wrap
			double const magTmpWrtSym{ (.5*magA) + (.5*magB) };
			double const magSymWrtTmp{ -magTmpWrtSym };

			// create spinor to align with mean azimuth
			BiVector const bivSymWrtTmp{ magSymWrtTmp * basePlane };
			return Pose(bivSymWrtTmp);
		}

		//! Attitudes in symmetric base frame (from axial frame)
		std::pair<ga::Pose, ga::Pose>
		attPairSymWrtRefFor
			( ga::Pose const & attSymWrtTmp
			, std::pair<ga::Pose, ga::Pose> const & apTmpWrtRef
			)
		{
			std::pair<ga::Pose, ga::Pose> attPair;
			if (attSymWrtTmp.isValid() && dat::isValid(apTmpWrtRef))
			{
				attPair = compositeOf(attSymWrtTmp, apTmpWrtRef);
			}
			return attPair;
		}

		std::string
		bivInfoString
			( std::pair<ga::Pose, ga::Pose> const & attPair
			, std::string const & title = {}
			)
		{
			std::ostringstream oss;
			ga::BiVector const biv1{ attPair.first.physicalAngle() };
			ga::BiVector const biv2{ attPair.second.physicalAngle() };
			oss
				<< title
				<< " " << dat::infoString(biv1, ".1")
				<< " " << dat::infoString(biv2, ".2")
				;
			return oss.str();
		}
			
		//! Orientation pair in symmetric base frame
		std::pair<ga::Rigid, ga::Rigid>
		roPairInSym
			( ga::Rigid const & oriAwRef
			, ga::Rigid const & oriBwRef
			)
		{
			using OriPair = std::pair<ga::Rigid, ga::Rigid>;
			using AttPair = std::pair<ga::Pose, ga::Pose>;

			OriPair roInSym;

			// bundle orientations into a pair for convenience
			OriPair const roInRef{ oriAwRef, oriBwRef };

			// determine RO ex.in. an intermediate axial frame
			OriPair const roInTmp{ pairTmpWrtRefFor(roInRef) };

			// determine rotation to make angle symmetric (in E12 component)
			AttPair apInSym{ attPairFrom(roInTmp) };
// TODO -- need a mathematical approach to this
//         e.g. 1D curve root finding
//         ... but this should be good enough if speed is not a concern
			constexpr double tolKappa{ std::numeric_limits<double>::epsilon() };
			for (size_t nn{0u} ; nn < 32u ; ++nn)
			{
				ga::Pose const attSymWrtTmp{ attSymWrtTmpFor(apInSym) };
				apInSym = attPairSymWrtRefFor(attSymWrtTmp, apInSym);
				double const kASymWrtTmp{ apInSym.first.spinComponents()[3] };
				double const kBSymWrtTmp{ apInSym.second.spinComponents()[3] };
				if ( (std::abs(kASymWrtTmp) < tolKappa)
				  && (std::abs(kBSymWrtTmp) < tolKappa)
				   )
				{
					break;
				}
			}

			// retain baselength magnitude from reference frame (?)
			ga::Vector const bInRef{ baseVector(roInRef) };
			double const bMag{ ga::magnitude(bInRef) };
			roInSym = oriPairFor(bMag * bInSym(), apInSym);

			return roInSym;
		}

	}

Tweak
tweakFrom
	( ga::Rigid const & oriAwRef
	, ga::Rigid const & oriBwRef
	)
{
	Tweak twk{};

	// convert orientations to symmetric frame
	using OriPair = std::pair<ga::Rigid, ga::Rigid>;
	OriPair const roInSym{ roPairInSym(oriAwRef, oriBwRef) };

	/*
	ga::Rigid const & oriAInSym = roInSym.first;
	ga::Rigid const & oriBInSym = roInSym.second;
	io::out() << std::endl;
	io::out() << dat::infoString(oriAInSym.pose(), "oriAInSym") << '\n';
	io::out() << dat::infoString(oriBInSym.pose(), "oriBInSym") << '\n';
	io::out() << std::endl;
	*/

	if (dat::isValid(roInSym))
	{
		// and construct tweak from symmetric orientations
		twk = Tweak::from(roInSym.first.pose(), roInSym.second.pose());
	}

	return twk;
}

std::pair<ga::Rigid, ga::Rigid>
roPairFor
	( Tweak const & twk
	, ga::Rigid const & oriSymWrtRef
	)
{
	std::pair<ga::Rigid, ga::Rigid> eoPair;

	static ga::Vector const staAInSym{ -.5*ga::e3 };
	static ga::Vector const staBInSym{  .5*ga::e3 };
	ga::Pose const attAwSym{ twk.attPhi() };
	ga::Pose const attBwSym{ twk.attTheta() };

	ga::Rigid const oriAwSym(staAInSym, attAwSym);
	ga::Rigid const oriBwSym(staBInSym, attBwSym);

	ga::Rigid const oriAwRef{ oriAwSym * oriSymWrtRef };
	ga::Rigid const oriBwRef{ oriBwSym * oriSymWrtRef };

	eoPair = { oriAwRef, oriBwRef };

	return eoPair;
}

} // symbase

} // ro

