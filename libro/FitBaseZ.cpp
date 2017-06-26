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
\brief Definitions for ro::FitBaseZ
*/


#include "libro/FitBaseZ.h"

#include "libmath/angle.h"
#include "libro/QuadForm.h"

#include "Eigen"

#include <sstream>


namespace ro
{

// explicit
FitBaseZ :: FitBaseZ
	( std::array<PtrPairUV, 5u> const & uvPtrs
	)
	: theUVPtrs(uvPtrs)
{
}

bool
FitBaseZ :: isValid
	() const
{
	return (theUVPtrs[0]);
}

namespace
{
	using CoefMat = Eigen::Matrix<double, 5u, 5u>;

	//! Condition number extracted from svd
	double
	conditionFor
		( Eigen::JacobiSVD<CoefMat> const & svd
		)
	{
		double condNum{ std::numeric_limits<double>::infinity() };
		long const last{ svd.singularValues().size() - 1 };
		double const svMax{ svd.singularValues()(0) };
		double const svMin{ svd.singularValues()(last) };
		assert(! (svMax < 0.));
		assert(! (svMin < 0.));
		if (std::numeric_limits<double>::min() < svMin)
		{
			condNum = svMax / svMin;
		}
		return condNum;
	}

	using ColVec = Eigen::Matrix<double, 5u, 1u>;
	using CoefMat = Eigen::Matrix<double, 5u, 5u>;

	//! Linearized system coefficient matrix and r.h.s. vector
	std::pair<CoefMat, ColVec>
	normMatRhs
		( ro::PairBaseZ const & roAt
		, std::array<ro::FitBaseZ::PtrPairUV, 5u> const & uvPtrs
		)
	{
		// all elements are explicitly set explicitly below
		std::pair<CoefMat, ColVec> pair;

		if (roAt.isValid() && (5u == uvPtrs.size()))
		{
			// access return structure
			CoefMat & matA = pair.first;
			ColVec & f0 = pair.second;

			// fill cofficient matrix and RHS
			ro::QuadForm const quadAt(roAt.pair());
			for (int nMea{0} ; nMea < 5 ; ++nMea)
			{
				ro::FitBaseZ::PairUV const & uvPair = *(uvPtrs[nMea]);

				// Coefficient matrix
				// Jacobian for this measurement
				std::array<double, 5u> const rowJ(roAt.jacobianRow(uvPair));
				for (int nParm{0} ; nParm < 5 ; ++nParm)
				{
					matA(nMea, nParm) = rowJ[nParm];
				}

				// Value at expansion point for this measurement
				// Negative sign from transposing expansion point to RHS
				f0(nMea, 0) = -quadAt.tripleProductGap(uvPair);
			}
		}
		return pair;
	}

	//! Change in parameters: linear solution for expansion point roAt
	std::array<double, 5u>
	deltaParmsFor
		( ro::PairBaseZ const & roAt
		, std::array<ro::FitBaseZ::PtrPairUV, 5u> const & uvPtrs
		, double * const & ptCondNum
		)
	{
		std::array<double, 5u> dParms
			{{ dat::nullValue<double>()
			 , dat::nullValue<double>()
			 , dat::nullValue<double>()
			 , dat::nullValue<double>()
			 , dat::nullValue<double>()
			}};

		using ColVec = Eigen::Matrix<double, 5u, 1u>;
		using CoefMat = Eigen::Matrix<double, 5u, 5u>;

		ColVec xSoln{};
		if (roAt.isValid() && (5u == uvPtrs.size()))
		{
			// get linearized system matrices
			std::pair<CoefMat, ColVec> const matPair
				{ normMatRhs(roAt, uvPtrs) };

			CoefMat const & matA = matPair.first;
			ColVec const & f0 = matPair.second;

			// use singular value decomposition in order to assess condNum
			Eigen::JacobiSVD<CoefMat> const svd
				(matA, Eigen::ComputeFullU | Eigen::ComputeFullV);
			assert(ptCondNum);
			*ptCondNum = conditionFor(svd);

			// cast to c++ return struct
			// TODO - access Eigen data directly - e.g. create la::cast
			xSoln = svd.solve(f0);
			dParms[0] = math::principalAngle(xSoln(0, 0));
			dParms[1] = math::principalAngle(xSoln(1, 0));
			dParms[2] = math::principalAngle(xSoln(2, 0));
			dParms[3] = math::principalAngle(xSoln(3, 0));
			dParms[4] = math::principalAngle(xSoln(4, 0));
		}

		return dParms;
	}
}

ro::PairBaseZ
FitBaseZ :: improvedNear
	( ro::PairBaseZ const & roAt
	, double const & maxCondNum
	, double * const & ptCondNum
	) const
{
	PairBaseZ roNext;

	// get parameter values at expansion point
	std::array<double, 5u> const parmsAt(roAt.parmValues());

	// compute change in parameters that fits measurements
	double condNum{};
	std::array<double, 5u> const dParms
		(deltaParmsFor(roAt, theUVPtrs, &condNum));
	if (ptCondNum)
	{
		*ptCondNum = condNum;
	}
	if (dat::isValid(dParms[0]) && (condNum < maxCondNum))
	{
		// compute updated parameters
		using dat::operator+;
		std::array<double, 5u> const parmsNext(parmsAt + dParms);

		// and use to create improved RO
		roNext = PairBaseZ(ArgsBaseZ(parmsNext), roAt.baseMag());
	}

	return roNext;
}

ro::PairBaseZ
FitBaseZ :: solutionNear
	( ro::PairBaseZ const & roNom
	, double const & tol
	, size_t const & itMax
	, double const & maxCondNum
	, double * const & ptCondNum
	) const
{
	PairBaseZ roSoln;
	if (isValid() && roNom.isValid())
	{
		roSoln = roNom;
		double rmsGap{ rmsGapFor(roSoln) };
		size_t itCount{ 0u };
		while ((tol < rmsGap) && (itCount++ < itMax))
		{
			roSoln = improvedNear(roSoln, maxCondNum, ptCondNum);
			rmsGap = rmsGapFor(roSoln);
		}
	}
	return roSoln;
}

double
FitBaseZ :: rmsGapFor
	( ro::Pair const & roNom
	) const
{
	double rms{ dat::nullValue<double>() };
	if (isValid() && roNom.isValid())
	{
		ro::QuadForm const quadNom(roNom.pair());
		double sumGapSq{ 0. };
		for (PtrPairUV const & uvPtr : theUVPtrs)
		{
		//	sumGapSq += math::sq(roNom.tripleProductGap(*uvPtr));
			sumGapSq += math::sq(quadNom.tripleProductGap(*uvPtr));
		}
		double const aveGapSq{ (1./double(theUVPtrs.size())) * sumGapSq };
		rms = std::sqrt(aveGapSq);
	}
	return rms;
}

std::string
FitBaseZ :: infoString
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
		oss << dat::infoString(theUVPtrs.size(), "theUVPtrs.size");
	}
	else
	{
		oss << " <null>";
	}
	return oss.str();
}


} // ro

