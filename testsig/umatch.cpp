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
\brief  This file contains unit test for sig::match
*/


#include "libsig/match.h"

#include "libdat/compare.h"
#include "libdat/ExtentsIterator.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace
{

//! Check for common functions
std::string
sig_match_test0
	()
{
	std::ostringstream oss;
	/*
	sig::match const aNull(dat::nullValue<sig::match>());
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	*/
	return oss.str();
}

	constexpr int sWhatever{ 4 }; // whatever (legit filter reponse)
	constexpr int sPartnull{ 5 }; // includes one or more nan

	template <typename Type>
	inline
	std::string
	infoString
		( dat::grid<Type> const & grid
		, std::string const & title
		, Type const & whatever = static_cast<Type>(sWhatever)
		, Type const & partnull = static_cast<Type>(sPartnull)
		)
	{
		std::ostringstream oss;

		if (!title.empty())
		{
			oss
				<< title
			//	<< " " << dat::infoString(grid.hwSize(), "hwSize")
				<< std::endl;
		}
		constexpr size_t bufSize{ 1024 };
		char buf[bufSize];

		std::string const fmt{ "%5.1f" };
		//                          12345"67890
		std::string const whatStr{ "   * " };
		std::string const nullStr{ "  nan" };
		std::string const partStr{ "   Q " };
		std::string const zeroStr{ "   . " };

		for (size_t row{0u} ; row < grid.high() ; ++row)
		{
			for (size_t col{0u} ; col < grid.wide() ; ++col)
			{
				Type const & elem = grid(row, col);
				if (dat::isValid(elem))
				{
					constexpr Type zero{ 0 };
					if (! dat::nearlyEquals(elem, zero))
					{
						if (dat::nearlyEquals(elem, whatever))
						{
							oss << ' ' << whatStr;
						}
						else
						if (dat::nearlyEquals(elem, partnull))
						{
							oss << ' ' << partStr;
						}
						else
						{
							snprintf(buf, bufSize, fmt.c_str(), elem);
							oss << ' ' << buf;
						}
					}
					else
					{
						oss << ' ' << zeroStr;
					}
				}
				else
				{
					oss << ' ' << nullStr;
				}
			}
			oss << '\n';
		}

		return oss.str();
	}

	bool
	isWhatever
		( double const & val
		)
	{
		return dat::nearlyEquals(val, static_cast<double>(sWhatever));
	}

	bool
	isPartnull
		( double const & val
		)
	{
		return dat::nearlyEquals(val, static_cast<double>(sPartnull));
	}

	int
	compare
		( double const & exp
		, double const & // got
		)
	{
		int same{ -8 };
		if (isWhatever(exp))
		{
			same = -1;
		}
		return same;
	}

	dat::grid<int>
	scoreDiff
		( dat::grid<double> const & gridExp
		, dat::grid<double> const & gridGot
		)
	{
		dat::Extents const hwSize{ gridExp.hwSize() };
		dat::grid<int> sameGrid(hwSize);
		assert(hwSize == gridGot.hwSize());
		for (dat::ExtentsIterator iter(hwSize) ; iter ; ++iter)
		{
			dat::RowCol const & rowcol = *iter;
			double const & elemExp = gridExp(rowcol);
			double const & elemGot = gridGot(rowcol);
			if (dat::isValid(elemExp))
			{
				if (dat::isValid(elemGot))
				{
					sameGrid(rowcol) = true; // both okay
				}
				else
				{
					sameGrid(rowcol) = compare(elemExp, elemGot);
				}
			}
			else
			{
				if (dat::isValid(elemGot))
				{
					sameGrid(rowcol) = compare(elemExp, elemGot);
				}
				else
				{
					sameGrid(rowcol) = true; // both null
				}
			}
		}
		return sameGrid;
	}


//! Check check scoring grid computations
std::string
sig_match_test1
	()
{
	std::ostringstream oss;

	constexpr size_t numField{ 15u };
	constexpr size_t numHunk{ 3u };
	constexpr size_t numScore{ numField - numHunk + 1u};
	constexpr float XX{ dat::nullValue<float>() };

	std::vector<float> const hunkValues
		{  1., 1., 1.
		,  1., 1., 1.
		,  1., 1., 1.
		};

	// determine expected responses
	double zz{};
	double zo{};
	double oo{};
	{
		dat::grid<float> const zero(numHunk, numHunk, 0.f);
		dat::grid<float> const ones(numHunk, numHunk, 1.f);
		dat::Extents const moveSize(1u, 1u);
		dat::grid<double> zzScore
			(sig::match::scoringGridFor(zero, moveSize, zero));
		dat::grid<double> zoScore
			(sig::match::scoringGridFor(zero, moveSize, ones));
		dat::grid<double> ooScore
			(sig::match::scoringGridFor(ones, moveSize, ones));
		zz = zzScore(0, 0);
		zo = zoScore(0, 0);
		oo = ooScore(0, 0);
	}
	io::out() << dat::infoString(zz, "zzScore") << std::endl;
	io::out() << dat::infoString(zo, "zoScore") << std::endl;
	io::out() << dat::infoString(oo, "ooScore") << std::endl;

	// signal environment to explore
	std::vector<float> const fieldValues
		{  XX, XX, XX, XX, XX , 0., 0., 0., 0., 0. , 0., 0., 0., 0., 0.
		,  XX, XX, XX, XX, XX , 0., 0., 0., 0., 0. , 0., 0., 0., 0., 0.
		,  XX, XX, XX, XX, XX , 0., 0., 0., 0., 0. , 0., 0., 0., 0., 0.
		,  XX, XX, XX, XX, XX , 0., 0., 0., 0., 0. , 0., 0., 0., 0., 0.
		,  XX, XX, XX, XX, XX , 0., 0., 0., 0., 0. , 0., 0., 0., 0., 0.

		,  0., 0., 0., 0., 0. , 1., 1., 1., 1., 1. , XX, XX, XX, XX, XX
		,  0., 0., 0., 0., 0. , 1., 1., 1., 1., 1. , XX, XX, XX, XX, XX
		,  0., 0., 0., 0., 0. , 1., 1., 1., 1., 1. , XX, XX, XX, XX, XX
		,  0., 0., 0., 0., 0. , 1., 1., 1., 1., 1. , 0., 0., 0., 0., 0.
		,  0., 0., 0., 0., 0. , 1., 1., 1., 1., 1. , 0., 0., 0., 0., 0.

		,  0., 0., 0., 0., 0. , XX, XX, XX, XX, XX , XX, XX, 0., 0., 0.
		,  0., 0., 0., 0., 0. , XX, XX, XX, XX, XX , XX, XX, 0., 0., 0.
		,  0., 0., 0., 0., 0. , XX, XX, XX, XX, XX , XX, XX, 0., 0., 0.
		,  0., 0., 0., 0., 0. , XX, XX, XX, XX, XX , XX, XX, 0., 0., 0.
		,  0., 0., 0., 0., 0. , XX, XX, XX, XX, XX , XX, XX, 0., 0., 0.
		};

	constexpr double ww{ static_cast<double>(sWhatever) };
	constexpr double qq{ static_cast<double>(sPartnull) };
	std::vector<double> const scores
		{  XX, XX, XX, qq, qq , zo, zo, zo, zo, zo , zo, zo, zo
		,  XX, XX, XX, qq, qq , zo, zo, zo, zo, zo , zo, zo, zo
		,  XX, XX, XX, qq, qq , zo, zo, zo, zo, zo , zo, zo, zo
		,  qq, qq, qq, qq, qq , ww, ww, ww, qq, qq , qq, qq, qq
		,  qq, qq, qq, qq, qq , ww, ww, ww, qq, qq , qq, qq, qq

		,  zo, zo, zo, ww, ww , 0., 0., 0., qq, qq , XX, XX, XX
		,  zo, zo, zo, ww, ww , 0., 0., 0., qq, qq , qq, qq, qq
		,  zo, zo, zo, ww, ww , 0., 0., 0., qq, qq , qq, qq, qq
		,  zo, zo, zo, qq, qq , ww, ww, ww, ww, ww , ww, ww, zo
		,  zo, zo, zo, qq, qq , ww, ww, ww, ww, ww , ww, ww, zo

		,  zo, zo, zo, qq, qq , XX, XX, XX, XX, XX , qq, qq, zo
		,  zo, zo, zo, qq, qq , XX, XX, XX, XX, XX , qq, qq, zo
		,  zo, zo, zo, qq, qq , XX, XX, XX, XX, XX , qq, qq, zo
		//
		//
		};
	dat::grid<float> field(numField, numField);
	assert(fieldValues.size() == field.size());
	std::copy(fieldValues.begin(), fieldValues.end(), field.begin());

	dat::grid<float> hunk(numHunk, numHunk);
	assert(hunkValues.size() == hunk.size());
	std::copy(hunkValues.begin(), hunkValues.end(), hunk.begin());

	dat::grid<double> expScore(numScore, numScore);
	assert(scores.size() == expScore.size());
	std::copy(scores.begin(), scores.end(), expScore.begin());

	// evaluate function
	dat::Extents const moveSize(numScore, numScore);
	size_t const reqMin{ 0u };
	dat::grid<double> gotScoreMin
		(sig::match::scoringGridFor(field, moveSize, hunk, reqMin));
	size_t const reqMax{ numHunk * numHunk - 1u };
	dat::grid<double> gotScoreMax
		(sig::match::scoringGridFor(field, moveSize, hunk, reqMax));

	dat::grid<int> const sameEnough{ scoreDiff(gotScoreMin, gotScoreMax) };

io::out() << infoString<float>(field, "field") << std::endl;
io::out() << infoString<double>(gotScoreMin, "gotScoreMin") << '\n';
io::out() << infoString<double>(expScore, "expScore") << '\n';
io::out() << infoString<double>(gotScoreMax, "gotScoreMax") << '\n';
//io::out() << sameEnough.infoStringContents("sameEnough", "%5d") << '\n';

oss << "Failure: implement this test!" << std::endl;
	return oss.str();
}


}

//! Unit test for sig::match
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << sig_match_test0();
//	oss << sig_match_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
