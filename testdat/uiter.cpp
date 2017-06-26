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
\brief  This file contains unit test for dat::iter
*/


#include "libdat/iter.h"

#include "libdat/info.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_iter_test0
	()
{
	std::ostringstream oss;
	/*
	dat::iter const aNull;
	if (aNull.isValid())
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << aNull.infoString("aNull") << std::endl;
	}
	*/
	return oss.str();
}

	//! Conduct test with sizes and check against expEnd
	bool
	checkEnd
		( std::ostream & oss
		, size_t const & sourceSize
		, size_t const & patchSize
		, size_t const & numLooks
		, size_t const & expEnd
		)
	{
		bool okay{ false };
		size_t const gotEnd
			{ dat::iter::activeEndFor(sourceSize, patchSize, numLooks) };

		// if expecting a bad value
		if (! dat::isValid(expEnd))
		{
			// then result should be bad as well
			okay = (! dat::isValid(gotEnd));
		}
		else
		{
			okay = dat::nearlyEquals(gotEnd, expEnd);
			if (! okay)
			{
				oss << "Failure of end iterator test" << std::endl;
				oss
					<< " " << dat::infoString(sourceSize, "sourceSize")
					<< " " << dat::infoString(patchSize, "patchSize")
					<< " " << dat::infoString(numLooks, "numLooks")
					<< " " << dat::infoString(expEnd, "expEnd")
					<< " " << dat::infoString(gotEnd, "gotEnd")
					<< std::endl;

				/*
				constexpr size_t beg(0u);
				for (size_t n0(beg) ; end != n0 ; ++n0)
				{
					io::out() << "n0: " << n0;
					for (size_t nlook(0u) ; nlook < numLooks ; ++nlook)
					{
						io::out() << " (" << nlook << ":Patch:";
						for (size_t np(0u) ; np < patchSize ; ++np)
						{
							size_t const at{ n0 + nlook + np };
							io::out() << " " << at;
						}
						io::out() << ") ";
					}
					io::out() << std::endl;
				}
				*/

			}
		}
		return okay;
	}


//! Check operations
std::string
dat_iter_test1
	()
{
	std::ostringstream oss;

	constexpr size_t badValue{ dat::nullValue<size_t>() };

	checkEnd(oss, 7,0,0, badValue);
	checkEnd(oss, 7,1,0, badValue);
	checkEnd(oss, 7,0,1, badValue);

	checkEnd(oss, 7,1,1, 7u);

	checkEnd(oss, 7,1,7, 1u);
	checkEnd(oss, 7,7,1, 1u);

	checkEnd(oss, 7,2,3, 4u);
	checkEnd(oss, 7,3,2, 4u);

	checkEnd(oss, 7,7,2, badValue);
	checkEnd(oss, 7,2,7, badValue);
	checkEnd(oss, 7,9,9, badValue);

	return oss.str();
}

	using Iter = std::vector<int>::const_iterator;
	using ItPair = std::pair<Iter, Iter>;
	using ItPairs = std::vector<ItPair>;

	void
	check
		( std::ostream & oss
		, Iter const & beg
		, Iter const & end
		, size_t const & numGroups
		, ItPairs const & exps
		)
	{
		ItPairs gots{ dat::iter::groups(beg, end, numGroups) };

		if (gots.size() == exps.size())
		{
			for (size_t nn{0u} ; nn < gots.size() ; ++nn)
			{
				ItPair const & got = gots[nn];
				ItPair const & exp = exps[nn];
				if (! (got == exp))
				{
					std::pair<int, int> const gotDistPair
						{ std::distance(beg, got.first)
						, std::distance(beg, got.second)
						};
					std::pair<int, int> const expDistPair
						{ std::distance(beg, exp.first)
						, std::distance(beg, exp.second)
						};
					oss << "Failure of pair test: num:" << numGroups << '\n';
					oss << dat::infoString(expDistPair, "exp") << std::endl;
					oss << dat::infoString(gotDistPair, "got") << std::endl;
					break;
				}
			}
		}
		else
		{
			oss << "Failure of size test: num: " << numGroups << '\n';
			oss << dat::infoString(exps.size(), "exp") << std::endl;
			oss << dat::infoString(gots.size(), "got") << std::endl;
		}

	}

//! Check grouping ops
std::string
dat_iter_test2
	()
{
	std::ostringstream oss;

	constexpr size_t numValues{ 7u };
	std::vector<int> values(numValues);
	std::iota(values.begin(), values.end(), 0u);

	Iter const itBeg{ values.begin() };
	Iter const itEnd{ values.end() };

	Iter const it0{ itBeg + 0u };
	Iter const it1{ itBeg + 1u };
	Iter const it2{ itBeg + 2u };
	Iter const it3{ itBeg + 3u };
	Iter const it4{ itBeg + 4u };
	Iter const it5{ itBeg + 5u };
	Iter const it6{ itBeg + 6u };
	Iter const & it7 = itEnd;

	ItPairs const exps0 // null set
		{ 
		};
	ItPairs const exps1 // single group is entire collection
		{ { itBeg, itEnd }
		};

	ItPairs const exps2 // groups: 4, 3
		{ { it0, it4 }
		, { it4, it7 }
		};
	ItPairs const exps3 // groups: 3, 2, 2
		{ { it0, it3 }
		, { it3, it5 }
		, { it5, it7 }
		};
	ItPairs const exps4 // groups: 2, 2, 2, 1
		{ { it0, it2 }
		, { it2, it4 }
		, { it4, it6 }
		, { it6, it7 }
		};
	ItPairs const exps5 // groups: 2, 2, 1, 1, 1
		{ { it0, it2 }
		, { it2, it4 }
		, { it4, it5 }
		, { it5, it6 }
		, { it6, it7 }
		};
	ItPairs const exps6 // groups: 2, 1, 1, 1, 1, 1
		{ { it0, it2 }
		, { it2, it3 }
		, { it3, it4 }
		, { it4, it5 }
		, { it5, it6 }
		, { it6, it7 }
		};

	ItPairs const exps7 // every element is own group
		{ { it0, it1 }
		, { it1, it2 }
		, { it2, it3 }
		, { it3, it4 }
		, { it4, it5 }
		, { it5, it6 }
		, { it6, it7 }
		};
	ItPairs const & exps8 = exps7; // treat overages as 'every element' case
	ItPairs const & exps9 = exps7; // treat overages as 'every element' case

	check(oss, values.begin(), values.end(), 0u, exps0);
	check(oss, values.begin(), values.end(), 1u, exps1);
	check(oss, values.begin(), values.end(), 2u, exps2);
	check(oss, values.begin(), values.end(), 3u, exps3);
	check(oss, values.begin(), values.end(), 4u, exps4);
	check(oss, values.begin(), values.end(), 5u, exps5);
	check(oss, values.begin(), values.end(), 6u, exps6);
	check(oss, values.begin(), values.end(), 7u, exps7);
	check(oss, values.begin(), values.end(), 8u, exps8);
	check(oss, values.begin(), values.end(), 9u, exps9);

	return oss.str();
}


}

//! Unit test for dat::iter
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_iter_test0();
	oss << dat_iter_test1();
	oss << dat_iter_test2();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
