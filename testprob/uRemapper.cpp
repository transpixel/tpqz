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
\brief  This file contains unit test for prob::Remapper
*/


#include "libprob/Remapper.h"

#include "libdat/info.h"
#include "libdat/types.h"
#include "libio/stream.h"
#include "libmath/math.h"
#include "libmath/Partition.h"
#include "libprob/distro.h"
#include "libprob/histo.h"
#include "libprob/SampleGen.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>


namespace
{

//! Check for common functions
std::string
prob_Remapper_test0
	()
{
	std::ostringstream oss;
	prob::Remapper const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	return oss.str();
}

	//! Write data to file
	void
	save
		( std::vector<double> const & values
		, std::string const & fpath
		)
	{
		std::ofstream ofs(fpath);
		ofs
			<< dat::infoString(values.begin(), values.end(), "", 9u, 6u, 1u)
			<< std::endl;
	}

	//! Compare sample sets using ? TODO
	template <typename DataType>
	void
	checkSimilarity
		( std::ostream & oss
		, std::vector<DataType> const & gotSamps
		, std::vector<DataType> const & expSamps
		, dat::Range<double> const & dataRange
		, double const & tol
		)
	{
		constexpr bool showDetails(false);
		constexpr bool saveDetails(false);

		// define partition for histogram
		constexpr size_t Dim{ 64u };
		math::Partition const dataPart(dataRange, Dim);

		// compute probability distribution of each
		prob::CdfForward const gotCdfFwd
			( prob::CdfForward::fromSamps
				(gotSamps.begin(), gotSamps.end(), dataPart)
			);
		prob::CdfForward const expCdfFwd
			( prob::CdfForward::fromSamps
				(expSamps.begin(), expSamps.end(), dataPart)
			);

		//! Evaluate probabilities at various data values
		std::vector<double> expCdf;
		std::vector<double> gotCdf;
		expCdf.reserve(dataPart.size());
		gotCdf.reserve(dataPart.size());
		for (size_t nn(0u) ; nn < dataPart.size() ; ++nn)
		{
			double const dataValue(dataPart.interpValueFor(double(nn)));
			double const expProb(expCdfFwd(dataValue));
			double const gotProb(gotCdfFwd(dataValue));
			expCdf.push_back(expProb);
			gotCdf.push_back(gotProb);
		}

		// evaluate cdf
		bool const same = dat::nearlyEquals
			( gotCdf.begin(), gotCdf.end()
			, expCdf.begin(), expCdf.end()
			, tol
			);
		if (! same)
		{
			std::vector<double> difCdf(Dim);
			std::transform
				( gotCdf.begin(), gotCdf.end(), expCdf.begin()
				, difCdf.begin()
				, std::minus<double>()
				);
			size_t const nUsed(1u); // cdf end point is fixed (?)
			double const rsse
				(math::rms<double>(difCdf.begin(), difCdf.end(), nUsed));
			double const maxMag
				(math::maxAbs<double>(difCdf.begin(), difCdf.end()));

			oss << "Failure of remapper equalize test for"
				<< " DataType: '" << typeid(DataType).name() << "'"
				<< std::endl;
			if (showDetails)
			{
				oss
					<< dataRange.infoString("dataMinMax") << '\n'
					<< '\n'
					<< dat::infoString(gotCdf.begin(), gotCdf.end(), "gotCdf")
					<< "\n\n"
					<< dat::infoString(expCdf.begin(), expCdf.end(), "expCdf")
					<< "\n\n"
					<< dat::infoString(difCdf.begin(), difCdf.end(), "difCdf")
					<< "\n\n"
					;
			}
			oss
				<< "  rsse: " << dat::infoString(rsse, "", 9u, 6u)
				<< std::endl
				<< "maxMag: " << dat::infoString(maxMag, "", 9u, 6u)
				<< std::endl
				<< "   tol: " << dat::infoString(tol, "", 9u, 6u)
				<< std::endl;

			if (saveDetails)
			{
				save(expCdf, "expCdf.dat");
				save(gotCdf, "gotCdf.dat");
				save(difCdf, "difCdf.dat");
			}
		}
	}

	//! Container of generated sample data
	template <typename DataType>
	struct SimTrial
	{
		prob::CdfInverse theCdfInv;
		std::vector<DataType> theSamps;
		prob::CdfForward theCdfFwd;

		explicit
		SimTrial
			( prob::CdfInverse const & cdfInv
			, size_t const & numData
			)
			: theCdfInv(cdfInv)
			, theSamps()
			, theCdfFwd()
		{
			// generate pseudo-random samples
			theSamps.reserve(numData);
			prob::SampleGen const generator(theCdfInv);
			for (size_t nn(0u) ; nn < numData ; ++nn)
			{
				DataType const samp(static_cast<DataType>(generator()));
				theSamps.push_back(samp);
			}

			// use samples to create forward CDF
			std::vector<size_t> const counts
				(prob::histo::countsFromSamps(theSamps, cdfInv.theDataPart));

			theCdfFwd = prob::CdfForward::fromFreqs
				(counts.begin(), counts.end(), cdfInv.dataRange());
		}

	};

	//! Run equalization test with particular data type
	template <typename DataType>
	void
	runEqualizationTest
		( std::ostream & oss
		, size_t const & numData
		, dat::Range<double> const & dataRange
		)
	{
		// configure sample trials
		constexpr size_t lutSize{ 4u * 1024u };

		// generate samples according to "from" distribution
		std::function<double(double const & frac)> const & probFunc
			= prob::distro::unitHumpProb;
		prob::CdfInverse const fromCdfInv
			(prob::CdfInverse::generateFor(probFunc, dataRange, lutSize));
		SimTrial<DataType> const fromTrial(fromCdfInv, numData);

		// generate samples according to "into" distribution
		prob::CdfInverse const intoCdfInv(prob::CdfInverse::uniform(dataRange));
		SimTrial<DataType> const intoTrial(intoCdfInv, numData);

		// Construct a remapper
		prob::Remapper const rem(fromTrial.theCdfFwd, intoTrial.theCdfInv);
		// remap the range data (using remapper defined with same data)
		std::vector<DataType> const & fromSamps = fromTrial.theSamps;
		std::vector<DataType> gotIntoSamps(fromSamps.size());
		std::transform
			( fromSamps.begin(), fromSamps.end()
			, gotIntoSamps.begin()
			, rem
			);

		// check results
		std::vector<DataType> const & expIntoSamps = intoTrial.theSamps;
		double const tol(1./128.); // inversion not well defined for 256 bins
		checkSimilarity(oss, gotIntoSamps, expIntoSamps, dataRange, tol);

	}

//! Check test remapping for data equalization
std::string
prob_Remapper_test1
	()
{
	std::ostringstream oss;

	constexpr size_t numDataD{ 8u * 1024u };
	dat::Range<double> const minmaxD{ -445., -17. };
	runEqualizationTest<double>(oss, numDataD, minmaxD);

	constexpr size_t numDataF{16u * 1024u };
	dat::Range<double> const minmaxF{ 2000., 5000000. };
	runEqualizationTest<float>(oss, numDataF, minmaxF);

	// TODO - needs nullValue which needs is_float...for half
//	constexpr size_t numDataH{ 8u * 1024u };
//	runEqualizationTest<dat::f16_t>(oss, numDataH);


	return oss.str();
}

//! Check extreme value handling
std::string
prob_Remapper_test2
	()
{
	std::ostringstream oss;

	// configure an identity remapping
	std::vector<double> const samples{ 2000., 2100., 2300., 2400. };
	double const eps(samples.back() * dat::smallValue<double>());
	dat::Range<double> const dataRange
		(dat::Range<double>::spanning(samples.begin(), samples.end(), eps));

	// construct remapper
	prob::CdfForward const cdfFwd(prob::CdfForward::uniform(dataRange));
	prob::CdfInverse const cdfInv(prob::CdfInverse::uniform(dataRange));
	prob::Remapper const rem(cdfFwd, cdfInv);

	// test each data sample onto itself
	for (double const & iSamp : samples)
	{
		double const oSamp(rem(iSamp));
		if (! (oSamp == iSamp)) // Exact since for test values here
		{
			oss << "Failure of identity sample mapping test"
				<< " " << dat::infoString(iSamp, "iSamp")
				<< " " << dat::infoString(oSamp, "oSamp")
				<< std::endl;
		}
	}

	// check small end point
	{
		double const maxJustOt(samples.front() * (1. - eps));
		double const maxOnEdge(samples.front());
		double const maxJustIn(samples.front() * (1. + eps));

		if (! dat::isValid(rem(maxJustIn)))
		{
			oss << "Failure of identity mapping front JustIn test\n";
		}
		if (! dat::isValid(rem(maxOnEdge)))
		{
			oss << "Failure of identity mapping front OnEdge test\n";
		}
		double const gotJustOt(rem(maxJustOt));
		if (dat::isValid(rem(maxJustOt)))
		{
			oss << "Failure of identity mapping front JustOt test\n";
			oss << dat::infoString(maxJustOt, "maxJustOt") << std::endl;
			oss << dat::infoString(gotJustOt, "gotJustOt") << std::endl;
		}
	}

	// check large end point
	{
		double const maxJustIn(samples.back() * (1. - eps));
		double const maxOnEdge(samples.back());
		double const maxJustOt(samples.back() * (1. + eps));
		if (! dat::isValid(rem(maxJustIn)))
		{
			oss << "Failure of identity mapping back JustIn test\n";
		}
		if (! dat::isValid(rem(maxOnEdge)))
		{
			oss << "Failure of identity mapping back OnEdge test\n";
		}
		double const gotJustOt(rem(maxJustOt));
		if (dat::isValid(rem(maxJustOt)))
		{
			oss << "Failure of identity mapping back JustOt test\n";
			oss << dat::infoString(maxJustOt, "maxJustOt") << std::endl;
			oss << dat::infoString(gotJustOt, "gotJustOt") << std::endl;
		}
	}

	return oss.str();
}


}

//! Unit test for prob::Remapper
int
main
	( int const /*argc*/
	, char const * const * const /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << prob_Remapper_test0();
	oss << prob_Remapper_test1();
	oss << prob_Remapper_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
