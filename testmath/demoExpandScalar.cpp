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
\brief  This file contains main application program demoExpandScalar
*/


#include "libapp/Usage.h"

#include "build/version.h"
#include "libdat/MinMax.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/sprintf.h"
#include "libio/stream.h"
#include "libio/string.h"

#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <stack>
#include <vector>


namespace
{
	//! Largest integer for which (exp(k) <= value)
	int
	kBelow
		( double const & value
		)
	{
		int kk{ dat::nullValue<int>() };
		if (std::numeric_limits<double>::min() < value)
		{
			double const lnVal{ std::log(value) };
			double const kDub{ std::floor(lnVal) };
			kk = static_cast<int>(kDub);
		}
		return kk;
	}

	//! Smallest integer for which (value < exp(k))
	int
	kAbove
		( double const & value
		)
	{
		int kk{ dat::nullValue<int>() };
		if (std::numeric_limits<double>::min() < value)
		{
			kk = kBelow(value) + 1;
		}
		return kk;
	}

	//! Geometric mean of (exp(k1), exp(k2))
	double
	geoMeanBetween
		( int const & k1
		, int const & k2
		)
	{
		return std::sqrt(std::exp(double(k1 + k2)));
	}

	//! Interpretation of a term (or series of terms)
	enum Interp
	{
		  Unknown
		, Keep
		, Flip
	};

	//! Convert Interp to string
	static std::vector<std::string> const sInterpString
		{ "Unknown", "Keep", "Flip" };

	/*
	//! Reverse input interpretation
	Interp
	flipped
		( Interp const & iterp
		)
	{
		Interp oterp{ Unknown };
		if (Keep == iterp)
		{
			oterp = Flip;
		}
		else
		if (Flip == iterp)
		{
			oterp = Keep;
		}
		return oterp;
	}
	*/

	/*
	bool
	isValidMag
		( double const & value
		)
	{
		bool isPos{ false };
		if (dat::isValid(value))
		{
			isPos = (std::numeric_limits<double>::min() < value);
		}
		return isPos;
	}
	*/

	/* --- Decision w/o involving other values
	bool
	useHiK
		( double const & posMag
		)
	{
		bool yes{ false };
		if (isValidMag(posMag))
		{
			int const hiK{ kAbove(posMag) };
			int const loK{ kBelow(posMag) };
			double const geoMean{ geoMeanBetween(hiK, loK) };
			yes = (geoMean < posMag);
		}
		return yes;
	}
	*/

	//! Generate context information associated with a magnitude
	struct InfoV
	{
		double const theMag{};
		int const theHiK{};
		int const theLoK{};
		double const theHiMag{};
		double const theLoMag{};

		InfoV
			( double const & value
			)
			: theMag{ std::abs(value) }
			, theHiK{ kAbove(theMag) }
			, theLoK{ kBelow(theMag) }
			, theHiMag{ std::exp(double(theHiK)) }
			, theLoMag{ std::exp(double(theLoK)) }
		{ }

		bool
		isValid
			() const
		{
			return
				(  dat::isValid(theMag)
				&& dat::isValid(theHiK)
				&& dat::isValid(theLoK)
				&& dat::isValid(theHiMag)
				&& dat::isValid(theLoMag)
				);
		}

		double
		hiResid
			() const
		{
			double resid{ dat::nullValue<double>() };
			if (isValid())
			{
				resid = theHiMag - theMag;
			}
			return resid;
		}

		double
		loResid
			() const
		{
			double resid{ dat::nullValue<double>() };
			if (isValid())
			{
				resid = theMag - theLoMag;
			}
			return resid;
		}

		bool
		useHi
			() const
		{
			bool yes{ false };
			if (isValid())
			{
				double const geoMean{ geoMeanBetween(theHiK, theLoK) };
				yes = (geoMean < theMag);
			}
			return yes;
		}

		std::string
		infoString
			( std::string const & title = {}
			) const
		{
			std::ostringstream oss;

			if (! title.empty())
			{
				oss << std::setw(15u) << title << " ";
			}

			if (isValid())
			{
				std::string hiStr{};
				std::string loStr{};
				if (useHi())
				{
					hiStr = "  <==";
					loStr = "     ";
				}
				else
				{
					hiStr = "     ";
					loStr = "  ==>";
				}
				oss
					<< "Hi,vals,Lo:"
					<< " " << dat::infoString(theHiK)
					<< " " << dat::infoString(theHiMag)
					<< " " << hiStr
					<< " " << dat::infoString(hiResid())
					<< " " << dat::infoString(theMag)
					<< " " << dat::infoString(loResid())
					<< " " << loStr
					<< " " << dat::infoString(theLoMag)
					<< " " << dat::infoString(theLoK)
					;
			}
			else
			{
				oss << " <null>";
			}

			return oss.str();
		}

	}; // InfoV

	//! One term of series expansion
	struct Term
	{
		//! Order of this term
		int theK{ dat::nullValue<int>() };

		//! Iterpretation of the remainder of series
		Interp theTailTerp{ Unknown };

		Term
			() = default;

		Term
			( Interp const & terp
			, int const & kk
			)
			: theK{ kk }
			, theTailTerp{ terp }
		{ }


		bool
		isValid
			() const
		{
			return
				(  (Unknown != theTailTerp)
				&& dat::isValid(theK)
				);
		}

		bool
		flipTail
			() const
		{
			return (isValid() && (Flip == theTailTerp));
		}

		double
		value
			() const
		{
			return (std::exp(double(theK)));
		}

		std::string
		infoString
			( std::string const & title = {}
			, std::string const & fmt = { "%9.6f" }
			) const
		{
			std::ostringstream oss;
			oss
				<< std::setw(15) << title
				<< " " << dat::infoString(theK)
				<< " " << io::sprintf(fmt, value())
				<< " " << dat::infoString(sInterpString[theTailTerp])
				;

			return oss.str();
		}

	}; // Term


	using Resid = double;

	//! Term associated with value
	std::pair<Term, Resid>
	nextTermResidFor
		( double const & value
		)
	{
		std::pair<Term, Resid> trPair{ Term{}, dat::nullValue<double>() };
		Term & term = trPair.first;
		double & resid = trPair.second;

		InfoV const info(value);
		int k0{ dat::nullValue<int>() };
		if (info.useHi())
		{
		//	io::out() << "useHi:";
			k0 = info.theHiK;
			resid = -info.hiResid();
		}
		else
		{
		//	io::out() << "useLo:";
			k0 = info.theLoK;
			resid = info.loResid();
		}

		Interp tailTerp{ Keep };
		if (resid < 0.)
		{
			tailTerp = Flip;
		}

		term = Term(tailTerp, k0);

		/*
		io::out()
			<< " " << dat::infoString(info)
			<< " " << dat::infoString(term, "term")
			<< " " << dat::infoString(resid, "resid")
			<< std::endl;
		*/

		return trPair;
	}

	struct KInfo
	{
		std::stack<Term> theStack{};
		dat::MinMax<int> theMinMaxK{};
		std::map<int, int> theKCoMap{};
	};

	//! Analyse source value into a series of diministing terms
	KInfo
	kInfoFor
		( double const & srcVal
		)
	{
		KInfo infoK;

		if (std::numeric_limits<double>::min() < srcVal)
		{
			std::stack<Term> & termStack = infoK.theStack;
			std::map<int, int> & kCoMap = infoK.theKCoMap;

			constexpr double eps{ std::numeric_limits<double>::epsilon() };
			double const resTol{ 16. * eps * std::abs(srcVal) };

			constexpr size_t maxTerms{ 32u }; // avoid infinite loops
			double resid{ srcVal };
			int nextParity{ 1 };
			while (! (std::abs(resid) < resTol)) 
			{
				// get next term associated with remainder of expansion series
				std::pair<Term, Resid> const trPair{ nextTermResidFor(resid) };

				Term const & term = trPair.first;
				int const & kk = term.theK;
				Interp const & terp = term.theTailTerp;
				if (Flip == terp)
				{
					nextParity = -1 * nextParity;
				}
				kCoMap[kk] = nextParity;

				// push onto FILO stack
				termStack.emplace(trPair.first);

				// recurse decomposition on remainder
				resid = trPair.second;

				// safety catch for dev/debug operations
				if (maxTerms < termStack.size())
				{
					assert(! (maxTerms < termStack.size()));
				}
			}
		}

		return infoK;
	}

	//! Sybolic string for coefficients
	std::string
	symbolString
		( std::map<int, int> const & kCoMap
		)
	{
		std::string symVal;

		if (! kCoMap.empty())
		{
			std::pair<int, int> const pairMin{ *(kCoMap.begin()) };
			std::pair<int, int> const pairMax{ *(kCoMap.rbegin()) };
			int const kMax{ std::max(pairMax.first, 0) };
			int const kMin{ std::min(pairMin.first, 0) };
			assert(kMin <= kMax);
			for (int kk{kMax} ; kMin <= kk ; --kk)
			{
				// retrieve coefficient for this term
				int co{ 0 };
				std::map<int, int>::const_iterator
					const itFind(kCoMap.find(kk));
				if (kCoMap.end() != itFind)
				{
					co = itFind->second;
				}

				// encode into character string
			//	char coSym{ '.' };
				char coSym{ '.' };
				if (co < 0)
				{
				//	coSym = '-';
					coSym = 'x';
				}
				else
				if (0 < co)
				{
				//	coSym = '+';
					coSym = 'r';
				}

				// introduce position holder
				if (0 == kk)
				{
					symVal += '(';
					symVal += coSym;
					symVal += ')';
				}
				else
				{
					symVal += coSym;
				}
			}
		}

		return symVal;
	}

	//! Sybolic string for coefficients
	std::string
	symbolString
		( double const & number
		)
	{
		KInfo const infoK{ kInfoFor(number) };
		std::string const symbols{ symbolString(infoK.theKCoMap) };
		return symbols;
	}

	//! Evaluate recurrsive expansion stack
	double
	evaluate
		( std::stack<Term> const & origStack
		)
	{
		double currSum{ 0. };
		std::stack<Term> termStack{ origStack };
		while (! termStack.empty())
		{
			// get smallest term from top of stack
			Term const & term = termStack.top();
			termStack.pop();

			// apply tail to this term with appropriate interpration
			double tailSum{ currSum };
			if (Keep == term.theTailTerp)
			{
				currSum = term.value() + tailSum;
			}
			else
			if (Flip == term.theTailTerp)
			{
				currSum = term.value() - tailSum;
			}
			else
			{
				assert(false);
			}
		}
		return currSum;
	}

}

//! Main program that #####
int
main
	( int const argc
	, char const * const * const argv
	)
{
	// check args
	app::Usage usage;
	usage.setSummary
		( "Compute cofficients for value as sum of exponentials"
		);
	usage.addArg("value", "Floating point number to expand");
	// ...
	if (usage.argStatus(argc, argv) != app::Usage::Valid)
	{
		std::string const fname(argv[0]);
		io::err()
			<< std::endl << build::version::buildInfo(argv[0]) << std::endl
			<< usage.infoString(fname) << std::endl;
		return 1;
	}

	// parse input argument
	int argnum(0);
	std::string const valstr(argv[++argnum]);

	double const srcVal{ io::string::from(valstr, dat::nullValue<double>()) };

	// if (! (std::numeric_limits<double>::min() < std::abs(srcVal)))
	if (! (std::numeric_limits<double>::min() < srcVal))
	{
		io::err()
			<< std::endl
		//	<< "WARNING: Zero (or very small) value not expandable"
			<< "WARNING: value must be signifant positive (eps < value)\n"
			<< "       : got: " << srcVal
			<< std::endl
			<< std::endl;
		return 1;
	}
	assert(std::numeric_limits<double>::min() < std::abs(srcVal));

	// decompose source value into series of exponents
	KInfo const infoK{ kInfoFor(srcVal) };

	// Encode coefficients into a string format
	std::map<int, int> const & kCoMap = infoK.theKCoMap;
	std::string const symVal{ symbolString(kCoMap) };

	// reconstitute
	std::stack<Term> const & termStack{ infoK.theStack };
	double const fitVal{ evaluate(termStack) };


/*
	// fun values
io::out() << "e+3: " << io::sprintf("%25.21f", std::exp( 3.)) << std::endl;
io::out() << "e+2: " << io::sprintf("%25.21f", std::exp( 2.)) << std::endl;
io::out() << "e+1: " << io::sprintf("%25.21f", std::exp( 1.)) << std::endl;
io::out() << "e.0: " << io::sprintf("%25.21f", std::exp( 0.)) << std::endl;
io::out() << "e-1: " << io::sprintf("%25.21f", std::exp(-1.)) << std::endl;
io::out() << "e-2: " << io::sprintf("%25.21f", std::exp(-2.)) << std::endl;
io::out() << "e-3: " << io::sprintf("%25.21f", std::exp(-3.)) << std::endl;
double sum{0.};
for (double arg{-3.} ; arg <= 3. ; arg += 1.)
{
	sum += std::exp(arg);
}
sum -= 1.;
io::out() << "sum: " << io::sprintf("%25.21f", sum) << std::endl;
*/

	// report decomposition
	io::out() << dat::infoString(InfoV(srcVal), "info") << std::endl;
	static std::string const fmt{ "%18.15f" };
	double const err{ fitVal - srcVal };
	io::out() << std::endl;
	io::out() << "input: val: " << io::sprintf(fmt, srcVal) << std::endl;
	io::out() << "string val: " << symVal << std::endl;
	io::out() << "final: sum: " << io::sprintf(fmt, fitVal) << std::endl;
	io::out() << "final: err: " << io::sprintf(fmt, err) << std::endl;
	io::out() << std::endl;

	// decompose source value into series of exponents
	constexpr double eVal{ std::exp(1.) };
	constexpr double piVal{ 4.*std::atan(1.) };
	constexpr double sqrt2{ std::sqrt(2) };
	constexpr double sqrte{ std::sqrt(eVal) };
	std::vector<double> const xxs
		{ 0., 1.
		, eVal, piVal, sqrt2, sqrte
		, 1./eVal, 1./piVal, 1./sqrt2, 1./sqrte
		};
	for (double const & xx : xxs)
	{
		std::string const symVal{ symbolString(xx) };
		io::out()
			<< "xx,eBase3"
			<< " " << dat::infoString(xx)
			<< " " << symVal
			<< std::endl;
	}


	return 0;
}

