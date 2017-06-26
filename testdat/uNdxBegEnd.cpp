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
\brief  This file contains unit test for dat::NdxBegEnd
*/


#include "libdat/NdxBegEnd.h"

#include "libdat/compare.h"
#include "libdat/info.h"
#include "libdat/validity.h"
#include "libio/stream.h"

#include <iostream>
#include <sstream>
#include <string>


namespace
{

//! Check for common functions
std::string
dat_NdxBegEnd_test0
	()
{
	std::ostringstream oss;
	dat::NdxBegEnd const aNull{};
	if (dat::isValid(aNull))
	{
		oss << "Failure of null value test" << std::endl;
		oss << "infoString: " << dat::infoString(aNull) << std::endl;
	}

	dat::NdxBegEnd const badBegEnd(7u, 3u);
	if (dat::isValid(badBegEnd))
	{
		oss << "Failure of bad span test" << std::endl;
	}

	return oss.str();
}

//! Check basic properties
std::string
dat_NdxBegEnd_test1
	()
{
	std::ostringstream oss;

	constexpr size_t expBeg{ 3u };
	constexpr size_t expSize{ 2u };
	constexpr size_t expEnd{ expBeg + expSize };
	dat::NdxBegEnd ival(expBeg, expEnd);

	size_t const gotBeg{ ival.begin() };
	size_t const gotEnd{ ival.end() };
	size_t const gotSize{ ival.size() };

	if (! dat::nearlyEquals(gotBeg, expBeg))
	{
		oss << "Failure of begin value test" << std::endl;
	}
	if (! dat::nearlyEquals(gotEnd, expEnd))
	{
		oss << "Failure of end value test" << std::endl;
	}
	if (! dat::nearlyEquals(gotSize, expSize))
	{
		oss << "Failure of size value test" << std::endl;
		oss << dat::infoString(expSize, "expSize") << std::endl;
		oss << dat::infoString(gotSize, "gotSize") << std::endl;
	}

	return oss.str();
}

//! Check subsets
std::string
dat_NdxBegEnd_test2
	()
{
	std::ostringstream oss;

	// [100, 101, 102, 103, 104), 105
	//           [), 102
	//           [102), 103
	//      [101, 102), 103
	//      [101, 102, 103), 104
	// [100, 101, 102, 103), 104
	// [100, 101, 102, 103, 104), 105
	dat::NdxBegEnd const full(100u, 105u);
	std::vector<dat::NdxBegEnd> const expSubs
		{ dat::NdxBegEnd(102u, 102u)
		, dat::NdxBegEnd(102u, 103u)
		, dat::NdxBegEnd(101u, 103u)
		, dat::NdxBegEnd(101u, 104u)
		, dat::NdxBegEnd(100u, 104u)
		, dat::NdxBegEnd(full)
		};

	for (size_t nn{0u} ; nn < expSubs.size() ; ++nn)
	{
		dat::NdxBegEnd const & expSub = expSubs[nn];
		dat::NdxBegEnd const gotSub{ full.centeredSubOfSize(nn) };
		if (! dat::areSameNoSmall(gotSub, expSub))
		{
			oss << "Failure of centeredSubOfSize test: size: " << nn << '\n';
			oss << dat::infoString(full, "full") << std::endl;
			oss << dat::infoString(expSub, "expSub") << std::endl;
			oss << dat::infoString(gotSub, "gotSub") << std::endl;
		}
	}

	return oss.str();
}

	void
	checkSub
		( std::ostream & oss
		, dat::NdxBegEnd const gotBE
		, dat::NdxBegEnd const expBE
		)
	{
		if (! dat::areSame(gotBE.begin(), expBE.begin()))
		{
			oss << "Failure of begin test" << std::endl;
			oss << dat::infoString(expBE, "expBE") << std::endl;
			oss << dat::infoString(gotBE, "gotBE") << std::endl;
		}
		if (! dat::areSame(gotBE.end(), expBE.end()))
		{
			oss << "Failure of end test" << std::endl;
			oss << dat::infoString(expBE, "expBE") << std::endl;
			oss << dat::infoString(gotBE, "gotBE") << std::endl;
		}
	}

//! Check window operations
std::string
dat_NdxBegEnd_test3
	()
{
	std::ostringstream oss;

	struct MidSize
	{
		size_t theMid;
		size_t theSize;
	};

	dat::NdxBegEnd const full(0u, 4u + 1u);

	using dat::NdxBegEnd;
	std::vector<std::pair<MidSize, NdxBegEnd> > const msbes
		/*
		{ { MidSize{0, 1}, NdxBegEnd{0, 1} }
		};
		*/
		{ { MidSize{0, 0}, NdxBegEnd{} }
		, { MidSize{1, 0}, NdxBegEnd{} }
		, { MidSize{4, 0}, NdxBegEnd{} }
		, { MidSize{5, 0}, NdxBegEnd{} }
		//
		, { MidSize{0, 1}, NdxBegEnd{0, 1} }
		, { MidSize{1, 1}, NdxBegEnd{1, 2} }
		, { MidSize{4, 1}, NdxBegEnd{4, 5} }
		, { MidSize{5, 1}, NdxBegEnd{} }
		//
		, { MidSize{0, 2}, NdxBegEnd{0, 1} }
		, { MidSize{1, 2}, NdxBegEnd{0, 2} }
		, { MidSize{4, 2}, NdxBegEnd{3, 5} }
		, { MidSize{5, 2}, NdxBegEnd{4, 5} }
		//
		, { MidSize{0, 3}, NdxBegEnd{0, 2} }
		, { MidSize{1, 3}, NdxBegEnd{0, 3} }
		, { MidSize{4, 3}, NdxBegEnd{3, 5} }
		, { MidSize{5, 3}, NdxBegEnd{4, 5} }
		};

	for (std::pair<MidSize, NdxBegEnd> const & msbe : msbes)
	{
		size_t const & center = msbe.first.theMid;
		size_t const & reqSize = msbe.first.theSize;
		dat::NdxBegEnd const & expSub = msbe.second;
		dat::NdxBegEnd const gotSub(full.croppedCenterSize(center, reqSize));

		// check resulting size (easier test for)
		size_t const expSize{ expSub.size() };
		if (dat::isValid(expSize))
		{
			size_t const gotSize{ gotSub.size() };
			if (! dat::nearlyEquals(gotSize, expSize))
			{
				oss << "Failure of construction size test" << std::endl;
				oss << dat::infoString(reqSize, "..reqSize") << std::endl;
				oss << dat::infoString(expSize, "expSize") << std::endl;
				oss << dat::infoString(gotSize, "gotSize") << std::endl;
				oss << dat::infoString(expSub, "expSub") << std::endl;
				oss << dat::infoString(gotSub, "gotSub") << std::endl;
				break;
			}
		}

		// check details (comprehensive test)
		checkSub(oss, gotSub, expSub);
		if (! oss.str().empty())
		{
			break;
		}
	}

	return oss.str();
}


}

//! Unit test for dat::NdxBegEnd
int
main
	( int const /*argc*/
	, char const * const * /*argv*/
	)
{
	std::ostringstream oss;

	// run tests
	oss << dat_NdxBegEnd_test0();
	oss << dat_NdxBegEnd_test1();
	oss << dat_NdxBegEnd_test2();
	oss << dat_NdxBegEnd_test3();

	// check/report results
	std::string const errMessages(oss.str());
	if (! errMessages.empty())
	{
		io::err() << errMessages << std::endl;
		return 1;
	}
	return 0;
}
