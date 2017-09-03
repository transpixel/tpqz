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
\brief  This file contains unit test for ro::ROLinear
*/


//#include "libro/ROLinear.h"

#include "libcam/PinHole.h"
#include "libcloud/Panel.h"
#include "libdat/array.h"
#include "libdat/ExtentsIterator.h"
#include "libdat/grid.h"
#include "libdat/info.h"
#include "libdat/MinMax.h"
#include "libdat/validity.h"
#include "libga/Rigid.h"
#include "libgeo/Ray.h"
#include "libgeo/xform.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libmath/MapSizeArea.h"
#include "libmath/math.h"
#include "libro/PairBaseZ.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
ro_ROLinear_test0
	()
{
	std::ostringstream oss;
	/*
	ro::ROLinear const aNull(dat::nullValue<ro::ROLinear>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}


//
// Test configuration values and functions
//


//#define SaveData

static dat::Range<double> const sRange(-1., 1.);
static dat::Area<double> const sPointArea{ sRange, sRange };
constexpr size_t sPntPerSide{ 2u + 1u };
static dat::Extents const sPointSize(sPntPerSide, sPntPerSide);

#if defined(SaveData)
static dat::Extents const sSumCountSize(200u, 200u);
static size_t const sNumTweaks{ 4u * sSumCountSize.size() };
#else
static size_t const sNumTweaks{ 8u * 1024u };
#endif

//constexpr double sMaxTweakAngleMag{ (1./2.) * math::qtrTurn };
constexpr double sMaxTweakAngleMag{ (.050/.500) };

constexpr double sBaseMag{ 1. };
constexpr double sDistFromGrid{ 10. };
constexpr double sCamPD{ 1. };


//
// Pseudo-random value funcs
//

static std::default_random_engine engine{};
//
inline
double
randValue
	( double const & maxMag
	)
{
	std::uniform_real_distribution<> distro(-maxMag, maxMag);
	return distro(engine);
}
//
template <size_t Dim>
inline
std::array<double, Dim>
randValue
	( double const & maxMag
	)
{
	std::array<double, Dim> values;
	for (size_t nn{0u} ; nn < Dim ; ++nn)
	{
		values[nn] = randValue(maxMag);
	}
	return values;
}

template <size_t Dim>
std::array<double, Dim>
randArray
	( double const & maxMag
	, size_t * const & ptNumTrys
	)
{
	std::array<double, Dim> values{{}};
	double mag{ std::numeric_limits<double>::max() };
	bool nonzero{ true };
	while ((maxMag < mag) && nonzero)
	{
		// sample until within magnitude of
		values = randValue<Dim>(maxMag);
		mag = { dat::magnitude(values) };
		nonzero = (std::numeric_limits<double>::epsilon() < std::abs(mag));
		if (! nonzero)
		{
			io::out() << "hit near-zero array" << std::endl;
		}
		++(*ptNumTrys);
	}
	return values;
}

ga::BiVector
randBiVector
	( double const & maxMag
	, size_t * const & ptNumTrys
	)
{
	// get arbitrary values for direction
	std::array<double, 3u> const rvs(randArray<3u>(maxMag, ptNumTrys));
	double const rvMag{ dat::magnitude(rvs) };

	// get arbitrary value for magnitude
	double const scale{ randValue(maxMag) / rvMag };

	// use direction and set magnitude with uniform distribution
	return ga::BiVector
		( (scale * rvs[0])
		, (scale * rvs[1])
		, (scale * rvs[2])
		);
}

//
// Misc funcs defining data configuration
//

ga::BiVector
basePlane
	()
{
	// Def: Base direction
	return ga::E12;
}

ga::Rigid
oriPanelWrtXY
	()
{
	// Def: Point locations
	return ga::Rigid( sDistFromGrid*ga::e1, ga::Pose(-math::halfPi*ga::E31));
}

ga::Pose
attNominal
	()
{
	// Def: Camera pointing
	ga::Pose const attTmpWrtRef(ga::BiVector(0., 0., math::pi));
	ga::Pose const attNomWrtTmp(ga::BiVector(0., math::halfPi, 0.));
	ga::Pose attNomWrtRef{ attNomWrtTmp * attTmpWrtRef };
	return attNomWrtRef;
}

ga::Vector
baseDir
	()
{
	return (-ga::E123 * basePlane());
}

std::vector<ga::Vector>
pointsInRef
	()
{
	cloud::Panel const panel(sPointArea, sPointSize, oriPanelWrtXY());
	return panel.thePoints;
}


//
// Sensor with (pseudo)EO (Pse frame) nominally aligned with Ref frame
//

class PseudoSensor
{
	cam::PinHole const theCam{};
	ga::Rigid const theOriSenWrtPse{};
	ga::Pose const theAttPseWrtSen{};

public:

	//! Provide passthrough sensing to sensor with some nominal fixed attitude
	PseudoSensor
		( ga::Pose const & attSenWrtPse
		)
		: theCam{ sCamPD }
		, theOriSenWrtPse{ ga::vZero, attSenWrtPse }
		, theAttPseWrtSen{ theOriSenWrtPse.pose().inverse() }
	{ }

	//! Sensed measurement for point in pseudo frame
	ga::Vector
	directionTo
		( ga::Vector const & pntInPse
		) const
	{
		// transform point into true-sensor exterior frame
		ga::Vector const pntInSen{ theOriSenWrtPse(pntInPse) };

		// project through optics
		ga::Vector const dirInSen{ theCam.directionTo(pntInSen) };
		double const & zVal = dirInSen[2];
		if (-.1 < zVal)
		{
			io::out() << "small dirInSen zVal: " << zVal << std::endl;
		}

		// transform direction from true-sensor into pseudo-sensor expression
		ga::Vector const dirInPse{ theAttPseWrtSen(dirInSen) };

		return dirInPse;
	}

	//! Sensed measurement for point in pseudo frame
	dat::Spot
	imageSpotFor
		( ga::Vector const & pntInPse
		) const
	{
		// transform point to PSE and project through optics
		return theCam.imageSpotFor(theOriSenWrtPse(pntInPse));
	}

};


//! Geometry for a stereo pair
struct Model
{
	std::vector<ga::Vector> const thePntInRefs;
	ga::Vector const theSta1;
	ga::Vector const theSta2;
	ga::Pose const theAttSenWrtPse;

	Model // Model
		()
		: thePntInRefs{ pointsInRef() }
		, theSta1{ -.5*sBaseMag*baseDir() }
		, theSta2{ -theSta1 }
		, theAttSenWrtPse{ attNominal() }
	{ }

	ro::PairBaseZ
	roPairFor
		( ga::BiVector const & biv1w0
		, ga::BiVector const & biv2w0
		) const
	{
		ga::BiVector const & phi = biv1w0;
		ga::BiVector const theta(biv2w0[0], biv2w0[1], -phi[2]);
		ro::ArgsBaseZ const args(phi, theta);
		return ro::PairBaseZ(args);

		/* TODO - make this faster (?)
		ga::Rigid const ori1w0(theSta1, ga::Pose(biv1w0));
		ga::Rigid const ori2w0(theSta2, ga::Pose(biv2w0));
		return ro::PairBaseZ(ori1w0, ori2w0);
		*/
	}

	using UVPair = std::pair<ga::Vector, ga::Vector>;

	std::vector<UVPair>
	uvPairsFor // Model
		( ro::PairBaseZ const & roPair
		) const
	{
		std::vector<UVPair> uvs;

		// get sensor orientations for this pair
		ga::Rigid const oriPse1WrtRef{ roPair.rigid1w0() };
		ga::Rigid const oriPse2WrtRef{ roPair.rigid2w0() };

		PseudoSensor const camera(theAttSenWrtPse);

		uvs.reserve(thePntInRefs.size());
		for (ga::Vector const & pntInRef : thePntInRefs)
		{
			ga::Vector const pntInPse1{ oriPse1WrtRef(pntInRef) };
			ga::Vector const uDirInPse1{ camera.directionTo(pntInPse1) };

			ga::Vector const pntInPse2{ oriPse2WrtRef(pntInRef) };
			ga::Vector const vDirInPse2{ camera.directionTo(pntInPse2) };

			uvs.emplace_back(std::make_pair(uDirInPse1, vDirInPse2));
		}

		return uvs;
	}

};


//! Generate a bunch of sample orientations
std::vector<ro::PairBaseZ>
roPairPerturbations
	( Model const & world
	, ro::ArgsBaseZ const & args0 // expansion point
	)
{
	// phi1, phi2, theta1, theta2, alpha3
	std::vector<ro::PairBaseZ> roPairs;
	double const aMaxMag{ sMaxTweakAngleMag };
	if (aMaxMag < 1.e-6)
	{
		ro::PairBaseZ const roPair(args0);
		roPairs.emplace_back(roPair);
	}
	else
	{
		ga::BiVector const nom1w0{ args0.phiBiv() };
		ga::BiVector const nom2w0{ args0.thetaBiv() };

		// for (size_t nn{0u} ; nn < sNumTweaks ; ++nn)
		size_t numTrys{0u};
		while (roPairs.size() < sNumTweaks)
		{
			// generate arbitrary, limited-mag rotations for each image
			ga::BiVector const del1w0{ randBiVector(aMaxMag, &numTrys) };
			ga::BiVector const del2w0{ randBiVector(aMaxMag, &numTrys) };
			ga::BiVector const biv1w0{ nom1w0 + del1w0 };
			ga::BiVector const biv2w0{ nom2w0 + del2w0 };

			// and save for use
			roPairs.emplace_back(world.roPairFor(biv1w0, biv2w0));

		} // while

#		if defined(SaveData)
		size_t const numGots{ roPairs.size() };
		// .5 since numTrys is called 2x
		double const fracTry{ .5 * double(numTrys) / double(numGots) };
		double const fracExp{ 8. / (4.*math::pi/3.) };
		io::out() << dat::infoString(numTrys, "numTrys") << std::endl;
		io::out() << dat::infoString(numGots, "numGots") << std::endl;
		io::out() << dat::infoString(fracTry, "fracTry") << std::endl;
		io::out() << dat::infoString(fracExp, "fracExp") << std::endl;
#		endif
	}
	return roPairs;
}

	struct TrackStat
	{
		dat::MinMax<double> theMM{};

		bool
		isValid
			() const
		{
			return theMM.isValid();
		}

		void
		operator()
			( double const & value
			)
		{
			theMM = theMM.expandedWith(value);
		}

		double
		maxAbs
			() const
		{
			return std::max(std::abs(theMM.min()), std::abs(theMM.max()));
		}
	};

	void
	updateTracking
		( dat::grid<TrackStat> * const & ptGrid
		, dat::RowCol const & rowcol
		, double const & value
		)
	{
		(*ptGrid)(rowcol)(value);
	}

	void
	saveTracking
		( dat::grid<TrackStat> const & grid
		, std::string const & fpath
		, math::MapSizeArea const & saMap
		)
	{
		std::ofstream ofs(fpath);
		ofs << "#\n";
		ofs << "# <magPhi magTheta value>\n";
		ofs << "#\n";
		for (dat::ExtentsIterator iter(grid.hwSize()) ; iter ; ++iter)
		{
			dat::RowCol const & rowcol = *iter;
			dat::Spot const ptMag
				(saMap.xyAreaSpotFor(dat::cast::Spot(rowcol)));
			double const & magPhi = ptMag[0];
			double const & magTheta = ptMag[1];

			TrackStat const & stat = grid(rowcol);
			if (dat::isValid(stat))
			{
				static std::string const fmt{ "%9.6f" };
				ofs
						<< " " << io::sprintf(fmt, magPhi)
						<< " " << io::sprintf(fmt, magTheta)
						<< " " << io::sprintf(fmt, stat.maxAbs())
						<< '\n';
			}
		}
	}

	//! emperically determined tolerance for 45-deg maxTweak
	double
	tolFunc
		( ro::PairBaseZ const & roAt
		, ro::PairBaseZ const & roNom
		)
	{
		ga::BiVector const phiAt{ roAt.angle1w0() };
		ga::BiVector const phiNom{ roNom.angle1w0() };
		ga::BiVector const thetaAt{ roAt.angle2w0() };
		ga::BiVector const thetaNom{ roNom.angle2w0() };
		double const magPhi{ ga::magnitude(phiAt - phiNom) };
		double const magTheta{ ga::magnitude(thetaAt - thetaNom) };
		double const xx{ math::hypot(magPhi, magTheta) };
		constexpr double tol0{ 0. }; // perhaps eps?
		return { tol0 + .35*math::sq(xx) };
	}

	//! Estimate RO misclosure value using linear expansion
	struct LinearApprox
	{
		// expansion point values
		ro::PairBaseZ const ro0;
		std::array<double, 5u> const p0;

		explicit
		LinearApprox
			( ro::PairBaseZ const & roNom
			)
			: ro0{ roNom }
			, p0(ro0.parmValues())
		{ }

		double
		gap
			( ro::PairBaseZ const & roAt
			, std::pair<ga::Vector, ga::Vector> const & uv
			) const
		{
			using A5 = std::array<double, 5u>;
			using dat::operator-;

			// fetch parameter and partial deriviative values
			A5 const pAt(roAt.parmValues());
			A5 const rowJ(ro0.jacobianRow(uv));

			// evaluate function at expansion point
			double const f0{ ro0.tripleProductGap(uv) };

			// estimate change
			A5 const pDelta(pAt - p0);
			double const df{ dat::dot(rowJ, pDelta) };
			assert(dat::isValid(f0));

			// return linearly estimation
			double const fAt{ f0 + df };
			assert(dat::isValid(fAt));
			return fAt;
		}
	};


//! Check evaluate RO function error
std::string
ro_ROLinear_test1
	()
{
	std::ostringstream oss;

	// simulate a stereo model (in symmetric-RO reference frame)
	Model const world{};

	// define test evaluation config
	// std::array<double, 5u> const nomPPTTA{{ 0., 0., 0., 0., 0. }};
	std::array<double, 5u> const nomPPTTA{{ .1, .2, .3, .4, .5 }};
	ro::ArgsBaseZ const args0(nomPPTTA);
	ro::PairBaseZ const roPair0(args0);

	LinearApprox const linApprox(roPair0);

	// perturb the model geometry in various ways
	std::vector<ro::PairBaseZ> const roPairs
		{ roPairPerturbations(world, args0) };

	// Simulate ray data with a perfect model (using first RO sample)
	using DirPair = std::pair<ga::Vector, ga::Vector>;
	std::vector<DirPair> const uvs{ world.uvPairsFor(roPair0) };

	// confirm test config
	for (DirPair const & uv: uvs)
	{
		// check that rigorous relationship is satisfied
		double const roFunc{ roPair0.tripleProductGap(uv) };
		if (! dat::nearlyEquals(roFunc, 0.))
		{
			oss << "Failure of test case rigorous expansion" << std::endl;
			assert(! "fix in pair test");
		}

		// check that linearizing function is also satisfied at expansion point
		double const linFunc{ linApprox.gap(roPair0, uv) };
		if (! dat::nearlyEquals(linFunc, 0.))
		{
			oss << "Failure of test case linear expansion" << std::endl;
			assert(! "fix in linear expansion");
		}
	}

#	if defined(SaveData)
	dat::Extents const & hwSize = sSumCountSize;
	dat::grid<TrackStat> gridRoFunc(hwSize);
	dat::grid<TrackStat> gridLinFunc(hwSize);
	dat::grid<TrackStat> gridDifErr(hwSize);
	static TrackStat const nullStat{};
	std::fill(gridRoFunc.begin(), gridRoFunc.end(), nullStat);
	std::fill(gridLinFunc.begin(), gridLinFunc.end(), nullStat);
	std::fill(gridDifErr.begin(), gridDifErr.end(), nullStat);

	constexpr double delPad{ 1.15 }; // pad for way 'alpha' is synthesized
	dat::Range<double> const delRange(0., delPad*sMaxTweakAngleMag);
	dat::Area<double> const delArea{ delRange, delRange };
	math::MapSizeArea const saMap(hwSize, delArea);
#	endif

	for (ro::PairBaseZ const & roPair : roPairs)
	{
#		if defined(SaveData)
		ga::BiVector const del1{ roPair.angle1w0() - roPair0.angle1w0() };
		ga::BiVector const del2{ roPair.angle2w0() - roPair0.angle2w0() };
		double const magDelPhi{ ga::magnitude(del1) };
		double const magDelTheta{ ga::magnitude(del2) };
		assert(magDelPhi < std::sqrt(3.)*sMaxTweakAngleMag);
		assert(magDelTheta < std::sqrt(3.)*sMaxTweakAngleMag);
#		endif

		for (DirPair const & uv: uvs)
		{
			// rigorous gap metric for reference
			double const roFunc{ roPair.tripleProductGap(uv) };

			// approximate the gap function via first order computation
			double const linFunc{ linApprox.gap(roPair, uv) };

			// error in linear approximation
			double const linErr{ linFunc - roFunc };
			assert(dat::isValid(linErr));

			// rms for error metric
#			if defined(SaveData)
			double const tolErr{ 1. }; // e.g. be more generous for debugging
#			else
			double const tolErr{ tolFunc(roPair, roPair0) };
#			endif

			// check that linear relationship is approximately satisfied
			if (! dat::nearlyEquals(linErr, 0., tolErr))
			{
				oss << "Failure of linearFuncGap test (exp==0.)" << std::endl;
				oss << dat::infoString(roFunc, "expFunc") << std::endl;
				oss << dat::infoString(linFunc, "linFunc") << std::endl;
				oss << "linErr: " << io::sprintf("%12.5e", linErr) << std::endl;
				oss << "tolErr: " << io::sprintf("%12.5e", tolErr) << std::endl;
				goto EndTest;
			}

#			if defined(SaveData)
			{
				assert(dat::isValid(roFunc));
				assert(dat::isValid(linFunc));
				assert(dat::isValid(linErr));

				// should be constrained by unit volume of triple product
				assert(std::abs(roFunc) <= 1.);

				dat::Spot const areaSpot{{ magDelPhi, magDelTheta }};
				dat::Spot const sizeSpot(saMap.hwSizeSpotFor(areaSpot));
				if (dat::isValid(sizeSpot))
				{
					dat::RowCol const rowcol(dat::cast::RowCol(sizeSpot));

					updateTracking(&gridRoFunc, rowcol, roFunc);
					updateTracking(&gridLinFunc, rowcol, linFunc);
					updateTracking(&gridDifErr, rowcol, linErr);
				}
			}
#			endif

		}
	}
EndTest:

#	if defined(SaveData)
	saveTracking(gridRoFunc, "uRO_RoFunc.dat", saMap);
	saveTracking(gridLinFunc, "uRO_LinFunc.dat", saMap);
	saveTracking(gridDifErr, "uRO_DifErr.dat", saMap);
#	endif

	return oss.str();
}


}

//! Unit test for ro::ROLinear
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ro_ROLinear_test0();
	oss << ro_ROLinear_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
