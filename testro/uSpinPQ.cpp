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
\brief  This file contains unit test for ro::SpinPQ
*/


#include "libro/SpinPQ.h"

#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libro/cast.h"
#include "libro/PairBaseZ.h"

#include <iostream>
#include <set>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
ro_SpinPQ_test0
	()
{
	std::ostringstream oss;
	ro::SpinPQ const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}
	return oss.str();
}

	//! Generate a spectrum of corresponding direction pairs
	std::vector<ro::PairUV>
	simPairUVs
		( ro::PairBaseZ const & ro
		)
	{
		std::vector<ro::PairUV> uvPairs;
		uvPairs.reserve(3u * 3u * 3u);
		for (int n1{-1} ; n1 < 2 ; ++n1)
		for (int n2{-1} ; n2 < 2 ; ++n2)
		for (int n3{-1} ; n3 < 2 ; ++n3)
		{
			constexpr double dist{ 1.25 }; // stay away from stations
			ga::Vector const pnt
				{ dist*double(n1)*ga::e1
				+ dist*double(n2)*ga::e2
				+ dist*double(n3)*ga::e3
				};
			ro::PairUV const uvPair{ ro.uvDirectionsFor(pnt) };
			uvPairs.emplace_back(uvPair);
		}
		return uvPairs;
	}

	// True if tripleProductGap disappears agrees with all uvPairs
	template <typename Model>
	bool
	checkTriple
		( Model const & model
		, std::vector<ro::PairUV> const & uvPairs
		)
	{
		bool okay{ false };
		if (model.isValid() && (! uvPairs.empty()))
		{
			okay = true; // unless contradicted
			for (ro::PairUV const & uvPair : uvPairs)
			{
				double const gap{ model.tripleProductGap(uvPair) };
				if (! dat::nearlyEquals(gap, 0.))
				{
io::out() << dat::infoString(gap, "gap") << std::endl;
					okay = false;
					break;
				}
			}
		}
		return okay;
	}

//! Check check basic operations
std::string
ro_SpinPQ_test1
	()
{
	std::ostringstream oss;

	// create an arbitrary configuration
//#define EasyData
#	if defined (EasyData)
	std::array<double, 5u> const vals
		{{ 0., 0.   // phi1,2
		 , 1., 0.   // theta1,2
		 , .00        // alpha3
		}};
#	else
	std::array<double, 5u> const vals
		{{  1.2, -1.3   // phi1,2
		 , -1.5,  2.3   // theta1,2
		 , .50        // alpha3
		}};
#	endif

	// get ref angles
	ro::ArgsBaseZ const nomArgs(vals);
	ga::BiVector const p0{ nomArgs.phiBiv() };
	ga::BiVector const t0{ nomArgs.thetaBiv() };

	// get reference directions
	ro::ArgsBaseZ const args0(p0, t0);
	ro::PairBaseZ const ro0(args0);
	std::vector<ro::PairUV> const uvPairs{ simPairUVs(ro0) };

	// original solution
	ga::BiVector const B0{ ga::E12 };
	ro::SpinPQ const expPQ{ ro::SpinPQ::from(p0, t0, B0) };

	// debug output
	constexpr bool showGaps{ false };
	if (showGaps)
	{
		// check coplanarity for this solution
		for (ro::PairUV const & uvPair : uvPairs)
		{
			std::ostringstream line;
			line << "gaps: ";
			for (size_t ndx{0u} ; ndx < expPQ.theNumPerms ; ++ndx)
			{
				double const gap{ expPQ[ndx].tripleProductGap(uvPair) };
				line << " " << io::sprintf("%9.6f", gap);
			}
			io::out() << line.str() << std::endl;
		}
	}


	// check all combinatorial solutions
	std::set<std::string> solnStrs;
	for (size_t ndx{0u} ; ndx < expPQ.theNumPerms ; ++ndx)
	{
		// check if this purported solution is consistent with measurements
		bool const okay{ checkTriple(expPQ[ndx], uvPairs) };
		if (! okay)
		{
			oss << "Failure of gap test: ndx = " << ndx << std::endl;
			break;
		}

		// get ndx-th permutation solution
		ro::OriPair const roPair{ expPQ[ndx].pair() };
		if (dat::isValid(roPair))
		{
			// check OriPair construction
			ro::SpinPQ const gotPQ{ ro::SpinPQ::from(roPair) };

			// check if triple product is satisified
			if (! checkTriple(gotPQ, uvPairs))
			{
				oss << "Failure of reconstruction test: ndx = "
					<< ndx << std::endl;
			}

			// check if triple product is satisified
			ro::PairRel const gotRel(roPair);
			if (! checkTriple(gotRel, uvPairs))
			{
				oss << "Failure of PairRel gap test: ndx = " << ndx << '\n';
			}

			// construct (anti-symmetric pair)
			ro::PairBaseZ const gotBaseZ(roPair);
			assert(gotBaseZ.isValid());

			if (! checkTriple(gotBaseZ, uvPairs))
			{
				oss << "Failure of pairBaseZ gap test: ndx = " << ndx << '\n';
			}

			// use ascii encoding of parms to check uniqueness
			std::string const solnStr{ gotBaseZ.parmString() };
			solnStrs.insert(solnStr);

			/*
			double const rmsParms{ gotBaseZ.parmRMS() };
			io::out() << solnStr << " " << dat::infoString(rmsParms) << '\n';
			*/
		}
	}

	// check if there are as many unique solutions as expected
	size_t const expNumSolns{ expPQ.theNumPerms };
	size_t const gotNumSolns{ solnStrs.size() };
	if (! dat::nearlyEquals(gotNumSolns, expNumSolns))
	{
		oss << "Failure of number unique solutions test" << std::endl;
		oss << dat::infoString(expNumSolns, "expNumSolns") << std::endl;
		oss << dat::infoString(gotNumSolns, "gotNumSolns") << std::endl;
	}

	return oss.str();
}


}

//! Unit test for ro::SpinPQ
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << ro_SpinPQ_test0();
	oss << ro_SpinPQ_test1();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
