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
\brief  This file contains unit test for sig::filter
*/


#include "libsig/filter.h"

#include "libdat/ExtentsIterator.h"
#include "libdat/grid.h"
#include "libdat/info.h"
#include "libdat/RowCol.h"
#include "libdat/SubExtents.h"
#include "libimg/convert.h"
#include "libimg/geo.h"
#include "libimg/io.h"
#include "libio/stream.h"
#include "libmath/math.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
sig_filter_test0
	()
{
	std::ostringstream oss;

	// check moving filter size relationships
	struct HMF
	{
		size_t theHunkUse;
		size_t theMoveUse;
		size_t theFullUse;
		size_t theHunkExp;
		size_t theMoveExp;
		size_t theFullExp;
	};

	constexpr size_t XX{ dat::nullValue<size_t>() };
	std::vector<HMF> const trials
		{ HMF{ XX, 1u, 5u,  5u, XX, XX }
		, HMF{ 2u, XX, 5u,  XX, 4u, XX }
		, HMF{ 3u, 1u, XX,  XX, XX, 3u }

		, HMF{ 0u, 0u, 0u,  XX, XX, XX }
		, HMF{ 0u, 0u, 1u,  XX, XX, XX }
		, HMF{ 0u, 1u, 0u,  XX, XX, XX }

		, HMF{ 1u, 1u, 1u,  1u, 1u, 1u }
		, HMF{ 2u, 1u, 2u,  2u, 1u, 2u }
		, HMF{ 3u, 1u, 3u,  3u, 1u, 3u }

		, HMF{ 1u, 2u, 2u,  1u, 2u, 2u }
		, HMF{ 2u, 2u, 3u,  2u, 2u, 3u }
		, HMF{ 3u, 2u, 4u,  3u, 2u, 4u }

		, HMF{ 1u, 3u, 3u,  1u, 3u, 3u }
		, HMF{ 2u, 3u, 4u,  2u, 3u, 4u }
		, HMF{ 3u, 3u, 5u,  3u, 3u, 5u }
		};

	for (HMF const & trial : trials)
	{
		size_t const & useHunk = trial.theHunkUse;
		size_t const & useMove = trial.theMoveUse;
		size_t const & useFull = trial.theFullUse;

		size_t const gotHunk(sig::filter::hunkSizeFor(useMove, useFull));
		size_t const gotMove(sig::filter::moveSizeFor(useFull, useHunk));
		size_t const gotFull(sig::filter::fullSizeFor(useHunk, useMove));

		size_t const & expHunk = trial.theHunkExp;
		size_t const & expMove = trial.theMoveExp;
		size_t const & expFull = trial.theFullExp;

		std::ostringstream msg;
		if (! dat::areSame(gotHunk, expHunk))
		{
			msg << "Failure of Hunk test" << std::endl;
		}
		if (! dat::areSame(gotMove, expMove))
		{
			msg << "Failure of Move test" << std::endl;
		}
		if (! dat::areSame(gotFull, expFull))
		{
			msg << "Failure of Full test" << std::endl;
		}


		if (! msg.str().empty())
		{
			oss << "Failure for test case, HMF:"
				<< '\n'
				<< "use,exp,got:"
				<< '\n'
				<< " " << dat::infoString(useHunk)
				<< " " << dat::infoString(useMove)
				<< " " << dat::infoString(useFull)
				<< '\n'
				<< " " << dat::infoString(expHunk)
				<< " " << dat::infoString(expMove)
				<< " " << dat::infoString(expFull)
				<< '\n'
				<< " " << dat::infoString(gotHunk)
				<< " " << dat::infoString(gotMove)
				<< " " << dat::infoString(gotFull)
				<< std::endl
				;
			oss << msg.str();
			break;
		}
	}

	return oss.str();
}


	dat::grid<short>
	simgrid
		()
	{
		dat::Extents const size(10u, 10u);
		dat::grid<short> grid(size);
		for (size_t row(0u) ; row < size.high() ; ++row)
		{
			for (size_t col(0u) ; col < size.wide() ; ++col)
			{
				grid(row, col) = 10000u + 100u*row + col;
			}
		}
		return grid;
	}

	dat::grid<short>
	unitPatch
		( dat::Extents const & patchSize
		)
	{
		dat::grid<short> patch(patchSize);
		std::fill(patch.begin(), patch.end(), 1u);
		return patch;
	}

	template <typename OutType, typename InType>
	struct PixFunc
	{
		inline
		OutType
		operator()
			( InType const & inA
			, InType const & inB
			) const
		{
		  return (OutType(inA-10000u) * OutType(inB));
		}
	};


//! Check perform basic filter operations
std::string
sig_filter_test1
	()
{
	std::ostringstream oss;

	using OutType = double;
	using InType = short;

	// create some "large" source
	dat::grid<InType> const sourceGrid(simgrid());

	// define a template patch
	dat::Extents const patchSize(3u, 2u);
	dat::grid<InType> const patchGrid(unitPatch(patchSize));

	// establish expected values
	dat::RowCol const atSrcRowCol{{ 1u, 5u }};
	OutType const expSum
		{ OutType(10105u + 10106u - 20000u)
		+ OutType(10205u + 10206u - 20000u)
		+ OutType(10305u + 10306u - 20000u)
		};
	OutType const exp{ expSum };

	/*
	io::out() << sourceGrid.infoStringContents("sourceGrid", "%6d") << '\n';
	io::out() << patchGrid.infoStringContents("patchGrid", "%6d") << '\n';
	*/

	// compute filter response for an individual patch location
	{
		OutType const got
			( sig::filter::hunkResponse<OutType, InType>
				( sourceGrid
				, atSrcRowCol
				, patchGrid
				, PixFunc<OutType, InType>()
				, std::plus<OutType>()
				)
			);
		if (! dat::nearlyEquals(got, exp))
		{
			oss << "Failure of patch response test" << std::endl;
			oss << dat::infoString(exp, "exp") << std::endl;
			oss << dat::infoString(got, "got") << std::endl;
		}
	}

	// compute filter response over a sub extent of the source

	{
		// define a smaller search region
		dat::Extents const & fullExtents = sourceGrid.hwSize();
		dat::Extents const & hunkExtents = patchGrid.hwSize();
		dat::Extents const moveSize
			(sig::filter::moveExtentsFor(fullExtents, hunkExtents));

		// evaluate response function over moveSize
		dat::grid<OutType> const resultGrid
			( sig::filter::moveResponseGrid<OutType, InType>
				( sourceGrid
				, moveSize
				, patchGrid
				, PixFunc<OutType, InType>()
				, std::plus<OutType>()
				)
			);
		if (resultGrid.isValid())
		{
			// move offset is centered on (0,0)
			dat::RowCol const & atLookRowCol = atSrcRowCol;
			OutType const & got = resultGrid(atLookRowCol);
			if (! dat::nearlyEquals(got, exp))
			{
				oss << "Failure of look response test" << std::endl;
				oss << dat::infoString(exp, "exp") << std::endl;
				oss << dat::infoString(got, "got") << std::endl;
			}
		}
		else
		{
			oss << "Failure of valid resultGrid test" << std::endl;
		}
	}

	return oss.str();
}

//! Check sizes debug a particular use-case
std::string
sig_filter_test1a
	()
{
	std::ostringstream oss;
	{
		dat::grid<float> const fullGrid(1032u, 132u);
		dat::RowCol const ulCropInFull{{ 0u, 0u }};
		dat::grid<float> const hunkGrid(1032u, 132u);

		// check that this call does not assert
		(void)sig::filter::hunkResponse<float, float>
			( fullGrid
			, ulCropInFull
			, hunkGrid
			, PixFunc<float, float>()
			, std::plus<float>()
			);
	}
	return oss.str();
}


	//! Synthesize arbitrary signal good for correlation
	dat::grid<float>
	simSignal
		( size_t const & hwDim
		)
	{
		dat::grid<float> grid(hwDim, hwDim);
		double const radMax{ double(hwDim) * std::sqrt(2.) };
		for (dat::ExtentsIterator iter(grid.hwSize()) ; iter ; ++iter)
		{
			dat::RowCol const & rowcol = *iter;
			double const xx{ double(rowcol[0]) };
			double const yy{ double(rowcol[1]) };
			double const rad{ std::hypot(xx, yy) };

			double const freq{ 10. * (rad/radMax) * math::twoPi };
			double const xAmp{ std::cos(freq * xx/radMax * math::twoPi) };
			double const yAmp{ std::sin(freq * yy/radMax * math::twoPi) };
			grid(rowcol) = float(xAmp + yAmp);
		}
		return grid;
	}


	//! Synthesize data signal with underlying sloping bias
	dat::grid<float>
	simSlopped
		( size_t const & hwDim
		, double const & slope
		)
	{
		dat::grid<float> grid(simSignal(hwDim));
		double const radMax{ std::hypot(grid.high(), grid.wide()) };
		for (dat::ExtentsIterator iter(grid.hwSize()) ; iter ; ++iter)
		{
			dat::RowCol const & rowcol = *iter;
			double const rad{ std::hypot(rowcol[0], rowcol[1]) };
			grid(rowcol) += float(slope * rad/radMax);
		}
		return grid;
	}

//! Check filter response in presence of bias
std::string
sig_filter_test2
	()
{
	std::ostringstream oss;

	using InType = float;
	using OutType = float;

	// create grids with different biases
	constexpr size_t srcDim(100u);
	dat::grid<InType> const fullGridA(simSlopped(srcDim,  0.));
	dat::grid<InType> const fullGridB(simSlopped(srcDim,  5.));

	// specify filter parameters
	dat::Extents const patchSize(16u, 16u);

	// define crop from reference image
	dat::RowCol const ulA{{ 40u, 60u }};
	dat::SubExtents const cropA(ulA, patchSize);

	// define crop from target image
	dat::Extents const moveSizeB
		( fullGridA.high() - patchSize.high() - 1u
		, fullGridA.wide() - patchSize.wide() - 1u
		);

	// crop working areas
	dat::grid<InType> const patchA
		(img::geo::croppedGrid<InType>(fullGridA, cropA));

	// evaluate filter response (using biased input)
	dat::grid<OutType> const gotScore
		( sig::filter::ssdResponseGrid<OutType, InType>
			( fullGridB
			, moveSizeB
			, patchA
			)
		);

	// evaluate expected filter response via unbiased input
	dat::grid<OutType> const expScore
		( sig::filter::moveResponseGrid<OutType, InType>
			( fullGridA // same as unbiased B for test case
			, moveSizeB
			, patchA
			, sig::filter::SqDiff<OutType, InType>()
			, std::plus<OutType>()
			)
		);

	// compare results
	bool const sameScores
		( dat::nearlyEquals<OutType>
			( gotScore.begin(), gotScore.end()
			, expScore.begin(), expScore.end()
			)
		);
	if (! sameScores)
	{
		if (gotScore.size() == expScore.size())
		{
			// check if "close" (test expScore is a simplification)
			double expMagSum{ 0. };
			double difMagSum{ 0. };
			for (dat::ExtentsIterator iter(expScore.hwSize()) ; iter ; ++iter)
			{
				dat::RowCol const & rowcol = *iter;
				OutType const & exp = expScore(rowcol);
				OutType const & got = gotScore(rowcol);
				OutType const dif{ exp - got };
				expMagSum += std::abs(exp);
				difMagSum += std::abs(dif);
			}
			assert(0. < expMagSum); // bad test case if score is all zeros
			double const difFrac(difMagSum / expMagSum);
			constexpr double tolFrac{ .02 }; // heuristic
			if (tolFrac < difFrac)
			{
				oss << "Failure of biased ripples size test" << std::endl;
				oss << dat::infoString(tolFrac, "tolFrac") << std::endl;
				oss << dat::infoString(difFrac, "difFrac") << std::endl;
			}
		}
		else
		{
			oss << "Failure of biased ripples size test" << std::endl;
		}
	}

	return oss.str();
}

	/*
	//! grid with bias removed
	dat::grid<double>
	unbiasedFrom
		( dat::grid<double> const & biased
		, double * const & ptBias
		)
	{
		dat::grid<double> unbiased(biased.hwSize());
		double const sum(std::accumulate(biased.begin(), biased.end(), 0.));
		double const bias(sum / double(biased.size()));
		std::transform
			( biased.begin(), biased.end()
			, unbiased.begin()
			, [&bias] (double const & in) { return (in - bias); }
			);
		*ptBias = bias;
		return unbiased;
	}
	*/

//! Check SSD bias compensation
std::string
sig_filter_test3
	()
{
	std::ostringstream oss;

	dat::grid<double> flat(5u, 1u);
	dat::grid<double> ramp( 7u, 1u);
	size_t const maxMoveHigh{ ramp.high() - flat.high() + 1u};
	dat::Extents const moveSize(maxMoveHigh, 1u);

	// create condition of a biased response
	std::fill(flat.begin(), flat.end(), 10u);
	std::iota(ramp.begin(), ramp.end(), 20u-ramp.high()/2u);

	// compute SSD
	dat::grid<double> const responseGrid
		(sig::filter::ssdResponseGrid<double, double>(ramp, moveSize, flat));

	// check if response is valid and proper size
	dat::Extents const & expSize = moveSize;
	dat::Extents const gotSize(responseGrid.hwSize());
	if (! gotSize.nearlyEquals(expSize))
	{
		oss << "Failure of response grid size test" << std::endl;
		oss << dat::infoString(expSize, "exp") << std::endl;
		oss << dat::infoString(gotSize, "got") << std::endl;
	}
	else
	{
		// check if response is unbiased
		assert(0u < moveSize.high());
		size_t const ndxBeg{ 0u };
		size_t const ndxEnd{ maxMoveHigh - 1u };
		double const & got1 = responseGrid(ndxBeg, 0u);
		double const & got2 = responseGrid(ndxEnd, 0u);
		if (! dat::nearlyEquals(got1, got2))
		{
			oss << "Failure of unbaised test" << std::endl;
			oss << dat::infoString(ndxBeg, "ndxBeg") << std::endl;
			oss << dat::infoString(ndxEnd, "ndxEnd") << std::endl;
			oss << dat::infoString(got1, "got1") << std::endl;
			oss << dat::infoString(got2, "got2") << std::endl;
			oss << responseGrid.infoStringContents("responseGrid", "%5.0f")
				<< std::endl;
		}
	}


	// Compute intermediate product (e.g. for evaluation/debuggin)
	/*
	io::out() << ramp.infoStringContents("ramp", "%5.3f") << std::endl;
	size_t const rowHigh{ flat.high() };
	for (size_t moveRow{ 0u } ; moveRow < maxMoveHigh ; ++moveRow)
	{
		io::out() << std::endl;
		io::out() << dat::infoString(moveRow, "moveRow") << std::endl;

		// remove bias from currently offset corresponding window
		dat::grid<double> const & iSamps = flat;
		dat::grid<double> jSamps(rowHigh, 1u);
		for (size_t row{0u} ; row < rowHigh ; ++row)
		{
			jSamps(row, 0u) = ramp(moveRow + row, 0u);
		}

		double iBias{};
		double jBias{};
		dat::grid<double> const xSamps(unbiasedFrom(iSamps, &iBias));
		dat::grid<double> const ySamps(unbiasedFrom(jSamps, &jBias));

		io::out() << dat::infoString(iBias, "iBias") << std::endl;
		io::out() << dat::infoString(jBias, "jBias") << std::endl;
		for (size_t row{0u} ; row < rowHigh ; ++row)
		{
			io::out()
				<< "i,j:x,y:"
				<< " " << dat::infoString(iSamps(row, 0u))
				<< " " << dat::infoString(jSamps(row, 0u))
				<< " " << dat::infoString(xSamps(row, 0u))
				<< " " << dat::infoString(ySamps(row, 0u))
				<< std::endl;
		}


		// compute
		double psi{ 0. };
		double sumDk{ 0. };
		double x1{ 0. };
		double y1{ 0. };
		double d1{ 0. };
		double x2{ 0. };
		double y2{ 0. };
		double d2{ 0. };

		double beta{ 0. };
		double sumBk{ 0. };
		double i1{ 0. };
		double j1{ 0. };
		double b1{ 0. };
		double i2{ 0. };
		double j2{ 0. };
		double b2{ 0. };
		double sumIk{ 0. };
		double sumJk{ 0. };

		for (size_t row{0u} ; row < rowHigh ; ++row)
		{
			double const & Xk = xSamps(row,0u);
			double const & Yk = ySamps(row,0u);
			double const & Ik = iSamps(row,0u);
			double const & Jk = jSamps(row,0u);

			double const Dk{ Xk - Yk };
			double const Bk{ Ik - Jk };

			psi += math::sq(Dk);
			beta += math::sq(Bk);

			sumDk += Dk;
			sumBk += Bk;

			sumIk += Ik;
			sumJk += Jk;

			x1 += Xk;
			y1 += Yk;
			d1 += Dk;

			i1 += Ik;
			j1 += Jk;
			b1 += Bk;

			x2 += math::sq(Xk);
			y2 += math::sq(Yk);
			d2 += math::sq(Dk);

			i2 += math::sq(Ik);
			j2 += math::sq(Jk);
			b2 += math::sq(Bk);

			io::out()
				<< "Ik,Jk,Bk:"
				<< " " << dat::infoString(Ik)
				<< " " << dat::infoString(Jk)
				<< " " << dat::infoString(Bk)
				<< "    "
				<< "Xk,Yk,Dk:"
				<< " " << dat::infoString(Xk)
				<< " " << dat::infoString(Yk)
				<< " " << dat::infoString(Dk)
				<< std::endl;
		}

		io::out() << std::endl;
		io::out()
			<< "Sums:ijb:"
			<< " " << dat::infoString(i1)
			<< " " << dat::infoString(j1)
			<< " " << dat::infoString(b1)
			<< "    "
			<< "Sums:xyd:"
			<< " " << dat::infoString(x1)
			<< " " << dat::infoString(y1)
			<< " " << dat::infoString(d1)
			<< std::endl;
		io::out()
			<< "SSqs:ijb:"
			<< " " << dat::infoString(i2)
			<< " " << dat::infoString(j2)
			<< " " << dat::infoString(b2)
			<< "    "
			<< "SSqs:xyd:"
			<< " " << dat::infoString(x2)
			<< " " << dat::infoString(y2)
			<< " " << dat::infoString(d2)
			<< std::endl;
		io::out()
			<< "----:ijb:"
			<< " " << dat::infoString(dat::nullValue<double>())
			<< " " << dat::infoString(dat::nullValue<double>())
			<< " " << dat::infoString(beta)
			<< "    "
			<< "----:xyd:"
			<< " " << dat::infoString(dat::nullValue<double>())
			<< " " << dat::infoString(dat::nullValue<double>())
			<< " " << dat::infoString(psi)
			<< std::endl;

		double const sumSqBk1{ math::sq(sumBk) };
		double const est1{ beta - sumSqBk1/double(rowHigh) };

		double const sumSqBk2{ math::sq(sumIk - sumJk) };
		double const est2{ beta - sumSqBk2/double(rowHigh) };

		io::out()
			<< "----:---:"
			<< " " << dat::infoString(dat::nullValue<double>())
			<< " " << dat::infoString(dat::nullValue<double>())
			<< " " << dat::infoString(dat::nullValue<double>())
			<< "    "
			<< "----:est1"
			<< " " << dat::infoString(dat::nullValue<double>())
			<< " " << dat::infoString(dat::nullValue<double>())
			<< " " << dat::infoString(est1)
			<< std::endl;
		io::out()
			<< "----:---:"
			<< " " << dat::infoString(dat::nullValue<double>())
			<< " " << dat::infoString(dat::nullValue<double>())
			<< " " << dat::infoString(dat::nullValue<double>())
			<< "    "
			<< "----:est2"
			<< " " << dat::infoString(dat::nullValue<double>())
			<< " " << dat::infoString(dat::nullValue<double>())
			<< " " << dat::infoString(est2)
			<< std::endl;

	}
	io::out() << std::endl;
	*/

	return oss.str();
}

}

//! Unit test for sig::filter
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << sig_filter_test0();
	oss << sig_filter_test1();
	oss << sig_filter_test1a();
	oss << sig_filter_test2();
	oss << sig_filter_test3();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
